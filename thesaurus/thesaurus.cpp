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
void Thesaurus::ConstructResults(RCursor<GNodeInfos>)
{
/*	for(Cur.Start();!Cur.End();Cur.Next())
		constNode(0,cur,Cur());*/

/*	size_t* tab;
	size_t* ptr;

	session->ClearGroups(GroupType);
	for(Sol.Start();!Sol.End();Sol.Next())
	{
		cGroup* g=static_cast<cGroup*>(session->NewGroup(GroupType));
		session->AssignId(g);
		ptr=tab=Sol()->GetObjectsId();
		while((*ptr)!=cNoRef)
			g->InsertObj(static_cast<cObj*>(Objs[*(ptr++)]->GetElement()));
		delete[] tab;
		session->InsertGroup(g,GroupType);
	}*/
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
			RCursor<GWeightInfo> Infos(Cur()->GetInfos());
			Objs.InsertPtr(obj=new RObjH(i,"Topic "+RString::Number(Cur()->GetId()),Infos.GetNb()));
			for(Infos.Start();!Infos.End();Infos.Next())
			{
				Word* w(Words.GetInsertPtr(Infos()->GetConcept()));
				if(w->Id==cNoRef)
					w->Id=Words.GetNb()-1;
				obj->AddAttribute(w->Id);
			}
		}

		cout<<" "<<i<<" created"<<endl;

		// Init the GCA
		cout<<"New HCA"<<endl;
		GInstH Instance(MaxGen,PopSize,Objs,Heuristic,Session->GetDebug());
		InsertObserver(reinterpret_cast<tNotificationHandler>(&Thesaurus::Gen),"RInst::Generation",&Instance);
		cout<<"Init HCA"<<endl;
	//	Instance.SetVerify(true);
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
