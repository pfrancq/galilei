 /*

	GALILEI Research Project

	Thesaurus.cpp

	Thesaurus Creation - Implementation

	Copyright 2008-2010 by Pascal Francq (pascal@francq.info).

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
		: GTool(session,fac), Objs(100), Words(20000), WordsByIds(20000), Concepts(200)
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
	Session->Insert(parent,Class);

	// Build the vector representing its concepts
	RNumCursor<size_t> List(node->GetAttr());
	GWeightInfos Infos(List.GetNb());
	for(List.Start();!List.End();List.Next())
		Infos.InsertInfo(new GWeightInfo(WordsByIds[List()]->Concept,1.0));
	Session->AssignInfos(Class,Infos);

	// Create sub-classes
	RCursor<GNodeInfos> Cur(node->GetNodes());
	for(Cur.Start();!Cur.End();Cur.Next())
		BuildNode(Cur(),Class);
}


//-----------------------------------------------------------------------------
void Thesaurus::ConstructResults(RCursor<GNodeInfos> Cur)
{
	// Clear the classes.
	Session->ForceReCompute(otClass);
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
		cout<<"    "<<List()<<" - "<<WordsByIds[List()]->Concept->GetName()<<endl;
	}
}


//------------------------------------------------------------------------------
void Thesaurus::Run(GSlot*)
{
	try
	{
		// If no element to group -> skip it
		RCursor<GTopic> Topics(Session->GetTopics());
		if(!Topics.GetNb())
			return;
		Objs.Clear(Topics.GetNb());
		Words.Clear();

		// Create objects
		cout<<"Create objects...";
		size_t id(0);
		RObjH* obj;
		for(Topics.Start();!Topics.End();Topics.Next())
		{
			// Get the vector of the current topic -> if null, treat next object
			GTopic* Topic(Topics());
			if(!Topic->GetVector().GetNb())
			{
				cout<<"'"<<Topic->GetName()<<"' ("<<Topic->GetId()<<") skipped"<<endl;
				continue;
			}

			// Order the vector by weight:
			// 1. Multiply by the tf-idf factors of the topic
			// 2. Order it.
			const GWeightInfos& Desc(Topic->GetVector());
			if(Desc.GetNb()>Concepts.GetNb())
			{
				for(size_t nb=Desc.GetNb()-Concepts.GetNb()+20;--nb;)
					Concepts.InsertPtr(new GWeightInfo(0));
			}
			RCursor<GWeightInfo> Infos(Desc.GetInfos());
			RCursor<GWeightInfo> Cur(Concepts);
			for(Infos.Start(),Cur.Start();!Infos.End();Infos.Next(),Cur.Next())
			{
				GWeightInfo Info(Infos()->GetConcept(),Infos()->GetWeight()*Infos()->GetConcept()->GetIF(otTopic));
				(*Cur())=Info;
			}
			Concepts.ReOrder(GWeightInfos::SortOrder);

			// Create the object
			Objs.InsertPtr(obj=new RObjH(id,Topic->GetName(),NumInfos));
			size_t j,k;
			for(Cur.Start(),j=0,k=0;(k<NumInfos)&&(j<Desc.GetNb());Cur.Next(),j++)
			{
				// Verify that it a word
				GConcept* Concept(Cur()->GetConcept());
				if(!Concept->GetType()->GetLang())
					continue;
				k++;
				Word* w(Words.GetInsertPtr(Concept));
				if(w->Id==cNoRef)
				{
					w->Id=Words.GetNb()-1;
					WordsByIds.InsertPtrAt(w,w->Id,false);
				}
				obj->AddAttribute(w->Id);
			}
			id++; // Increment the identifier
			//PrintObj(obj);
		}
		cout<<" "<<id<<" created ("<<Words.GetNb()<<" different concepts)"<<endl;

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
CREATE_TOOL_FACTORY("Thesaurus Creation","Thesaurus Creation",Thesaurus)
