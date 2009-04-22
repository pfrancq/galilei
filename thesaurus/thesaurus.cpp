 /*

	GALILEI Research Project

	Thesaurus.cpp

	Thesaurus Creation - Implementation

	Copyright 2008-2009 by Pascal Francq (pascal@francq.info).

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
//include files for R/GALILEI
#include <gsession.h>
#include <gtopic.h>


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
Thesaurus::Thesaurus(GFactoryPostTopic* fac)
		: GPostTopic(fac), Objs(100), Words(20000)
{
}


//------------------------------------------------------------------------------
void Thesaurus::ApplyConfig(void)
{
	GPostTopic::ApplyConfig();
	Heuristic=Factory->Get("GAHeuristic");
	MaxGen=Factory->GetUInt("MaxGen");
	PopSize=Factory->GetUInt("PopSize");
	Verify=Factory->GetBool("Verify");
}


//------------------------------------------------------------------------------
void Thesaurus::Connect(GSession* session)
{
	GPostTopic::Connect(session);
}


//------------------------------------------------------------------------------
void Thesaurus::Disconnect(GSession* session)
{
	GPostTopic::Disconnect(session);
}


//-----------------------------------------------------------------------------
void Thesaurus::PrintObj(RObjH* obj,char depth)
{
	RString Deb;
	for(int i=0;i<depth;i++)
		Deb+="\t";
	cout<<Deb<<"Obj "<<obj->GetId()<<" (";
	RAttrList List(obj->GetAttr());
	int i;
	for(List.Start(),i=0;!List.End();List.Next(),i++)
	{
		if(i)
			cout<<",";
		cout<<Words[List()]->Concept->GetName();
	}
	cout<<endl;
}


//-----------------------------------------------------------------------------
void Thesaurus::PrintNode(GNodeInfos* node,char depth)
{
	RString Deb;
	for(int i=0;i<depth;i++)
		Deb+="\t";

	// Print Node attributes
	cout<<Deb<<"Node (";
	RAttrList List(node->GetAttr());
	int i;
	for(List.Start(),i=0;!List.End();List.Next(),i++)
	{
		if(i)
			cout<<",";
		cout<<Words[List()]->Concept->GetName();
	}
	cout<<endl;

	// Print objects
	Deb+="\t";
	RCursor<RObjH> Objs(node->GetObjs());
	for(Objs.Start();!Objs.End();Objs.Next())
		cout<<Deb<<Objs()->GetName()<<endl;

	// Print sub-nodes
	RCursor<GNodeInfos> Cur(node->GetNodes());
	for(Cur.Start();!Cur.End();Cur.Next())
		PrintNode(Cur(),depth+1);
}


//-----------------------------------------------------------------------------
void Thesaurus::ConstructResults(RCursor<GNodeInfos> Cur)
{
	for(Cur.Start();!Cur.End();Cur.Next())
		PrintNode(Cur(),0);
}


//------------------------------------------------------------------------------
void Thesaurus::Run(void)
{
	try
	{
		// If no element to group -> skip it
		RCursor<GTopic> Cur(Session->GetTopics());
		if(!Cur.GetNb())
			return;
		Objs.Clear(Cur.GetNb());
		Words.Clear();

		// Create objects
		cout<<"Create objects...";
		size_t i(0);
		RObjH* obj;
		for(Cur.Start();!Cur.End();Cur.Next(),i++)
		{
			RCursor<GWeightInfo> Infos(Cur()->GetVector()->GetInfos());
			Objs.InsertPtr(obj=new RObjH(i,Cur()->GetName(),Infos.GetNb()));
			for(Infos.Start();!Infos.End();Infos.Next())
			{
				Word* w(Words.GetInsertPtr(Infos()->GetConcept()));
				if(w->Id==cNoRef)
					w->Id=Words.GetNb()-1;
				obj->AddAttribute(w->Id);
			}
		}

		cout<<" "<<i<<" created ("<<Words.GetNb()<<" different concepts)"<<endl;
//		RCursor<RObjH> Test(Objs);
//		for(Test.Start();!Test.End();Test.Next())
//			PrintObj(Test(),1);

		// Init the GCA
		cout<<"New HCA"<<endl;
		GInstH Instance(MaxGen,PopSize,Objs,Heuristic,Session->GetDebug());
		InsertObserver(reinterpret_cast<tNotificationHandler>(&Thesaurus::Gen),"RInst::Generation",&Instance);
		cout<<"Init HCA"<<endl;
		Instance.SetVerify(Verify);
		Instance.Init();
/*		Instance.SetLocalOptimisation(LocalOptimisation);
		Instance.SetOptimisation(Optimisation);
		if(!InternalRandom)
			Instance.SetSeed(Seed);*/

		// Run
		cout<<"Run HCA"<<endl;
		Instance.Run();
		cout<<"Build solutions"<<endl;
		ConstructResults(Instance.BestChromosome->GetNodes(Instance.BestChromosome->GetTop()));
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
void Thesaurus::CreateParams(RConfig* params)
{
	params->InsertParam(new RParamValue("GAHeuristic","FirstFit"));
	params->InsertParam(new RParamValue("MaxGen",50));
	params->InsertParam(new RParamValue("PopSize",16));
	params->InsertParam(new RParamValue("Verify",false));
}


//------------------------------------------------------------------------------
Thesaurus::~Thesaurus(void)
{
}


//------------------------------------------------------------------------------
CREATE_POSTTOPIC_FACTORY("Thesaurus Creation",Thesaurus)
