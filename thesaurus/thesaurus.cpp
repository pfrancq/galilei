 /*

	GALILEI Research Project

	Thesaurus.cpp

	Thesaurus Creation - Implementation

	Copyright 2008-2011 by Pascal Francq (pascal@francq.info).

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU Library General Public
	License as published by the Free Software Foundation; either
	version 2.0 of the License, or (at your option) any later version.

	This library is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
	Library General Public License for more details.

	You should have received a copy of the GNU Library General Public
	License along with this library, as a file COPYING.LIB; if not, write
	to the Free Software Foundation, Inc., 59 Temple Place, Suite 330,
	Boston, MA  02111-1307  USA

*/



//------------------------------------------------------------------------------
// include files for R/GALILEI
#include <gsession.h>
#include <gtopic.h>
#include <gclass.h>


//------------------------------------------------------------------------------
//include files for current plug-in
#include <thesaurus.h>
#include <ginsth.h>
#include <gchromoh.h>



//------------------------------------------------------------------------------
//
//  Thesaurus
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Thesaurus::Thesaurus(GSession* session,GPlugInFactory* fac)
		: GTool(session,fac), Objs(100), Attributes(20000), AttributesByIds(20000), Tmp(200)
{
}


//------------------------------------------------------------------------------
void Thesaurus::ApplyConfig(void)
{
	Heuristic=FindParam<RParamValue>("GAHeuristic")->Get();
	MaxGen=FindParam<RParamValue>("MaxGen")->GetUInt();
	PopSize=FindParam<RParamValue>("PopSize")->GetUInt();
	Verify=FindParam<RParamValue>("Verify")->GetBool();
	NumInfos=FindParam<RParamValue>("NumInfos")->GetUInt();
}


//-----------------------------------------------------------------------------
void Thesaurus::BuildNode(GNodeInfos* node,GClass* parent)
{
	// Build Name
	RString Name("Node "+RString::Number(node->GetId()));
	if(!node->GetNbNodes())
	{
		if(node->GetNbObjs()>1)
			cout<<"Warning: Node "<<node->GetId()<<" has "<<node->GetNbObjs()<<" objects attached"<<endl;
		RCursor<RObjH> Objs(node->GetObjs());
		bool Plus=false;
		for(Objs.Start();!Objs.End();Objs.Next())
		{
			if(Plus)
				Name+="+";
			else
			{
				Name+=" (";
				Plus=true;
			}
			Name+=Objs()->GetName();
		}
		if(Plus)
			Name+=")";
	}

	// Create the class
	GClass* Class(new GClass(Session,Name));
	Session->InsertObj(parent,Class);

	// Build the vector representing its concepts
	RNumCursor<size_t> List(node->GetAttr());
	RContainer<GVector,true,true> Vectors(10);
	for(List.Start();!List.End();List.Next())
	{
		GVector* Vector(Vectors.GetInsertPtr(AttributesByIds[List()]->Vector));
		Vector->InsertRef(new GConceptRef(AttributesByIds[List()]->Concept,1.0));
	}
	Session->AssignVectors(Class,Vectors);

	// Create sub-classes
	RCursor<GNodeInfos> Cur(node->GetNodes());
	for(Cur.Start();!Cur.End();Cur.Next())
		BuildNode(Cur(),Class);
}


//-----------------------------------------------------------------------------
void Thesaurus::ConstructResults(RCursor<GNodeInfos> Cur)
{
	// Clear the classes.
	Session->Reset(otClass);
	for(Cur.Start();!Cur.End();Cur.Next())
		BuildNode(Cur(),0);
}


//------------------------------------------------------------------------------
void Thesaurus::PrintObj(RObjH* obj)
{
	cout<<obj->GetName()<<endl;
	RNumCursor<size_t> List(obj->GetAttr());
	for(List.Start();!List.End();List.Next())
	{
		cout<<"    "<<List()<<" - "<<AttributesByIds[List()]->Concept->GetName()<<endl;
	}
}


//------------------------------------------------------------------------------
void Thesaurus::Run(GSlot*)
{
	try
	{
		// If no element to group -> skip it
		RCursor<GTopic> Topics(Session->GetObjs(pTopic));
		if(!Topics.GetNb())
			return;
		Objs.Clear(Topics.GetNb());
		Attributes.Clear();
		AttributesByIds.Clear();

		// Create objects
		cout<<"Create objects...";
		size_t id(0);
		RObjH* obj;
		for(Topics.Start();!Topics.End();Topics.Next())
		{
			// Get the vector of the current topic -> if null, treat next object
			GTopic* Topic(Topics());
			if(!Topic->IsDefined())
			{
				cout<<"'"<<Topic->GetName()<<"' ("<<Topic->GetId()<<") skipped"<<endl;
				continue;
			}

			// Order the vector by weight:
			// 1. Multiply by the tf-idf factors of the topic
			// 2. Order it.
			const GDescription& Desc(*Topic);
			size_t Total(0); // Total number of concepts
			RCursor<GVector> Vector(Desc.GetVectors());
			for(Vector.Start();!Vector.End();Vector.Next())
			{
				Total+=Vector()->GetNb();
				if(Total>Tmp.GetNb())
				{
					for(size_t nb=Total-Tmp.GetNb()+100;--nb;)
						Tmp.InsertPtr(new Attribute());
				}
				RCursor<GConceptRef> Refs(Vector()->GetRefs());
				RCursor<Attribute> Cur(Tmp);
				for(Refs.Start(),Cur.Start();!Refs.End();Refs.Next(),Cur.Next())
				{
					Cur()->Concept=Refs()->GetConcept();
					Cur()->Vector=Vector()->GetConcept();
					Cur()->Weight=Refs()->GetWeight()*Refs()->GetConcept()->GetIF(otTopic);
				}
			}
			Tmp.ReOrder(Attribute::SortOrder,0,Total);

			// Create the object
			Objs.InsertPtr(obj=new RObjH(id,Topic->GetName(),NumInfos));
			size_t j,k;
			RCursor<Attribute> Cur(Tmp);
			for(Cur.Start(),j=0,k=0;(k<NumInfos)&&(j<Total);Cur.Next(),j++)
			{
				Attribute Search(Cur()->Vector,Cur()->Concept);
				k++;
				Attribute* w(Attributes.GetInsertPtr(Search));
				if(w->Id==cNoRef)
				{
					w->Id=Attributes.GetNb()-1;
					AttributesByIds.InsertPtrAt(w,w->Id,false);
				}
				obj->AddAttribute(w->Id);
			}
			id++; // Increment the identifier
			//PrintObj(obj);
		}
		cout<<" "<<id<<" created ("<<Attributes.GetNb()<<" different concepts)"<<endl;

		// Init the GCA
		cout<<"New HCA"<<endl;
		GInstH Instance(MaxGen,PopSize,Objs,Heuristic,0);
		InsertObserver(reinterpret_cast<tNotificationHandler>(&Thesaurus::Gen),"RInst::Generation",&Instance);
		cout<<"Init HCA"<<endl;
		Instance.SetVerify(Verify);
		Instance.Init();

		// Run
		cout<<"Run HCA"<<endl;
		Instance.Run();
		cout<<"Build solutions"<<endl;
		ConstructResults(Instance.BestChromosome->GetTopNodes());
	}
	catch(RGAException& e)
	{
		throw GException(e.GetMsg());
	}
	catch(GException& e)
	{
		throw GException(e.GetMsg());
	}
	catch(RException& e)
	{
		throw GException(e.GetMsg());
	}
	catch(exception& e)
	{
		throw GException(e.what());
	}
	catch(...)
	{
		throw GException("Unknown Problem");
	}
}

//------------------------------------------------------------------------------
void Thesaurus::Gen(const R::RNotification& notification)
{
	cout<<"Gen "<<GetData<size_t>(notification)<<endl;
}



//------------------------------------------------------------------------------
void Thesaurus::CreateConfig(void)
{
	InsertParam(new RParamValue("GAHeuristic","FirstFit"));
	InsertParam(new RParamValue("MaxGen",50));
	InsertParam(new RParamValue("PopSize",16));
	InsertParam(new RParamValue("Verify",false));
	InsertParam(new RParamValue("NumInfos",20));
}


//------------------------------------------------------------------------------
Thesaurus::~Thesaurus(void)
{
}


//------------------------------------------------------------------------------
CREATE_TOOL_FACTORY("Thesaurus Creation","HGA","Hierarchical Genetic Algorithm",Thesaurus)
