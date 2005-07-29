/*

	GALILEI Research Project

	GSession.cpp

	Generic GALILEI Session - Implementation.

	Copyright 2001-2005 by the Université libre de Bruxelles.

	Authors:

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
// include files for ANSI C/C++
#include <fstream>
#include <math.h>


//------------------------------------------------------------------------------
// include files for R Project
#include <random.h>
#include <rcursor.h>
#include <rtextfile.h>
using namespace R;


//------------------------------------------------------------------------------
// include files for GALILEI
#include <glang.h>
#include <gdict.h>
#include <gwordlist.h>
#include <gsession.h>
#include <gstorage.h>
#include <gslot.h>
#include <gsessionprg.h>
#include <gstatscalc.h>
#include <gdoc.h>
#include <gdocanalyse.h>
#include <gdocxml.h>
#include <glinkcalc.h>
#include <glink.h>
#include <gpostdoc.h>
#include <gengine.h>
#include <gmetaengine.h>
#include <genginedoc.h>
#include <guser.h>
#include <gprofile.h>
#include <gsubprofile.h>
#include <gprofilessims.h>
#include <gprofilesdocssims.h>
#include <ggroupsdocssims.h>
#include <gsubprofile.h>
#include <gprofilecalc.h>
#include <gpreprofile.h>
#include <gpostprofile.h>
#include <ggroups.h>
#include <ggroup.h>
#include <ggrouping.h>
#include <ggroupcalc.h>
#include <gsubjects.h>
#include <gfilter.h>
#include <gpostdoc.h>
#include <gweightinfo.h>
#include <ggroupshistory.h>
#include <gpostgroup.h>
#include <gdocproxymem.h>
#include <gsubprofileproxymem.h>
#include <gprofileproxymem.h>
#include <gpluginmanagers.h>
using namespace GALILEI;



//------------------------------------------------------------------------------
// Static variables
R::RContainer<GSignalHandler,false,false> GSession::Handlers(30,20);
GPluginManagers GPluginManagers::PluginManagers;



//------------------------------------------------------------------------------
//
// GSession
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Global variables
GSession* GSession::Session=0;
bool GSession::ExternBreak=false;


//------------------------------------------------------------------------------
GSession::GSession(GStorage* str)
	: GDocs(str->GetNbSaved(otDoc)), GUsers(0,0),
	  GGroups(0), Subjects(0), ProfilesSims(0), ProfilesDocsSims(0), GroupsDocsSims(0), Random(0),
	  SessParams(0), Storage(str)
{
	// Init Part
	if(!Session)
		Session=this;
	CurrentRandom=0;
	Random = new RRandomGood(CurrentRandom);

	// create the groups history manager.
	GroupsHistoryMng=new GGroupsHistoryManager(this,2);
}


//------------------------------------------------------------------------------
GSession::GSession(GStorage* str,GSessionParams* sessparams,bool tests)
	: GDocs(str->GetNbSaved(otDoc)), GUsers(str->GetNbSaved(otUser),str->GetNbSaved(otProfile)),
	  GGroups(str->GetNbSaved(otGroup)), Subjects(0),
	  ProfilesSims(0), ProfilesDocsSims(0), GroupsDocsSims(0), Random(0),
	  SessParams(sessparams), Storage(str)
{
	// Init Part
	if(!Session)
		Session=this;
	CurrentRandom=0;
	Random = new RRandomGood(CurrentRandom);

	// Create SubjectTree
	if(tests)
		Subjects=new GSubjects(this);

	// create the groups history manager.
	GroupsHistoryMng=new GGroupsHistoryManager(this,2);
}


//------------------------------------------------------------------------------
void GSession::Connect(void)
{
	GPluginManagers::PluginManagers.Connect(this);
}


//------------------------------------------------------------------------------
GDocXML* GSession::CreateDocXML(GDoc* doc)
{
	return((dynamic_cast<GFilterManager*>(GPluginManagers::PluginManagers.GetManager("Filter")))->CreateDocXML(doc));
}


//------------------------------------------------------------------------------
void GSession::SetSims(GProfilesSimsManager* sims)
{
	ProfilesSims=sims;
}


//------------------------------------------------------------------------------
void GSession::SetSims(GProfilesDocsSimsManager* sims)
{
	ProfilesDocsSims=sims;
}


//------------------------------------------------------------------------------
void GSession::SetSims(GGroupsDocsSimsManager* sims)
{
	GroupsDocsSims=sims;
}


//------------------------------------------------------------------------------
void GSession::AssignId(GData* data,const GDict* dict)
{
	Storage->AssignId(data,dict);
}


//------------------------------------------------------------------------------
void GSession::AssignId(GGroup* grp)
{
	grp->SetId(GGroups::GetNewId());
}


//------------------------------------------------------------------------------
void GSession::AssignId(GDoc* doc)
{
	doc->SetId(GDocs::GetNewId());
}


//------------------------------------------------------------------------------
void GSession::AssignId(GSubProfile* sub)
{
	sub->SetId(GUsers::GetNewId(otSubProfile));
}


//------------------------------------------------------------------------------
void GSession::AnalyseDocs(GSlot* rec,bool modified,bool save)
{
	bool undefLang;
	GDocXML* xml=0;
	R::RCursor<GDoc> Docs=GetDocsCursor();
	RContainer<GDoc,false,true> tmpDocs(5,2);
	GDocAnalyse* Analyse;
	RString err;
	bool Cont;               // Continue the analysuis

	// Verify that the textanalyse method is selected
	Analyse=(dynamic_cast<GDocAnalyseManager*>(GPluginManagers::PluginManagers.GetManager("DocAnalyse")))->GetCurrentMethod();
	if(!Analyse)
		throw GException("No document analysis method chosen.");

	// Opens and appends the Log File for all errors occuring in the filter or analyse phase.
	if(rec)
	{
		err= "Documents Filtering and Analysis on Data Set : "+Storage->GetName()+ " on : " +itou(RDate::GetToday().GetDay())+"/"+ itou(RDate::GetToday().GetMonth())+"/"+itou(RDate::GetToday().GetYear());
		rec->WriteStr(err.Latin1());
	}

	// Analyse the documents
	do
	{
		// Go through the existing documents
		for(Docs.Start();!Docs.End();Docs.Next())
		{
			if(modified&&(Docs()->GetState()==osUpToDate)) continue;
			if(rec)
			{
				rec->Interact();
				rec->receiveNextDoc(Docs());
			}
			if(ExternBreak) return;
			undefLang=false;
			try
			{
				if(((!modified)||(Docs()->GetState()!=osUpdated))||((Docs()->GetState()!=osNotNeeded)))
				{
					if (!Docs()->GetLang()) undefLang=true;
					xml=(dynamic_cast<GFilterManager*>(GPluginManagers::PluginManagers.GetManager("Filter")))->CreateDocXML(Docs());
					if(xml)
					{
						Docs()->InitFailed();
						Analyse->Analyze(xml,Docs(),&tmpDocs);
						delete xml;
						xml=0;
						if((undefLang)&&(Docs()->GetLang()))
						{
							MoveDoc(Docs());
						}
					}
					else
						Docs()->IncFailed();
				}
				if(save)
					Storage->SaveDoc(Docs());

				if(Docs()->GetState()==osUpdated)
					Docs()->SetState(osUpToDate);
			}
			catch(GException& e)
			{
				if(xml)
					delete xml;

				// Write error message to the log file handled by the GSlot.
				if(rec)
					rec->WriteStr(e.GetMsg());
				else
					throw GException(e.GetMsg());
			}
		}

		// Add the new documents.
		// Continue the analysis if documents were added.
		RCursor<GDoc> Cur(tmpDocs);
		Cont=tmpDocs.GetNb();
		for(Cur.Start();!Cur.End();Cur.Next())
		{
			InsertDoc(Cur());
			//#warning add the document with the Cur()->GetId()
		}
		tmpDocs.Clear();
	}
	while(Cont);

	// Run all post-doc methods that are enabled
	ComputePostDoc(rec);
}


//------------------------------------------------------------------------------
void GSession::AnalyseNewDocs(GSlot* rec,bool modified,bool save)
{
	bool undefLang;
	GDocXML* xml=0;
	R::RCursor<GDoc> Docs=GetDocsCursor();
	RContainer<GDoc,false,true> tmpDocs(5,2);
	GDocAnalyse* Analyse;
	RString err;
	bool Cont;               // Continue the analysuis

	// Verify that the textanalyse method is selected
	Analyse=(dynamic_cast<GDocAnalyseManager*>(GPluginManagers::PluginManagers.GetManager("DocAnalyse")))->GetCurrentMethod();
	if(!Analyse)
		throw GException("No document analysis method chosen.");

	// Opens and appends the Log File for all errors occuring in the filter or analyse phase.
	if(rec)
	{
		err= "Documents Filtering and Analysis on Data Set : "+Storage->GetName()+ " on : " +itou(RDate::GetToday().GetDay())+"/"+ itou(RDate::GetToday().GetMonth())+"/"+itou(RDate::GetToday().GetYear());
		rec->WriteStr(err.Latin1());
	}

	// Analyse the documents
	do
	{
		// Go through the existing documents
		for(Docs.Start();!Docs.End();Docs.Next())
		{
			if(modified&&(Docs()->GetState()==osUpToDate)) continue;
			if(rec)
			{
				rec->Interact();
				rec->receiveNextDoc(Docs());
			}
			if(ExternBreak) return;
			undefLang=false;
			try
			{
				if(((!modified)||(Docs()->GetState()!=osUpdated))||((Docs()->GetState()!=osNotNeeded)))
				{
					if (!Docs()->GetLang()) undefLang=true;
					xml=(dynamic_cast<GFilterManager*>(GPluginManagers::PluginManagers.GetManager("Filter")))->CreateDocXML(Docs());
					if(xml)
					{
						Docs()->InitFailed();
						Analyse->Analyze(xml,Docs(),&tmpDocs);
						delete xml;
						xml=0;
						if((undefLang)&&(Docs()->GetLang()))
						{
							MoveDoc(Docs());
						}
					}
					else
						Docs()->IncFailed();
				}
				if(save)
					Storage->SaveDoc(Docs());

				if(Docs()->GetState()==osUpdated)
					Docs()->SetState(osUpToDate);
			}
			catch(GException& e)
			{
				if(xml)
					delete xml;

				// Write error message to the log file handled by the GSlot.
				if(rec)
					rec->WriteStr(e.GetMsg());
				else
					throw GException(e.GetMsg());
			}
		}

		// Add the new documents.
		// Continue the analysis if documents were added.
		RCursor<GDoc> Cur(tmpDocs);
		Cont=tmpDocs.GetNb();
		for(Cur.Start();!Cur.End();Cur.Next())
			InsertDoc(Cur());
		tmpDocs.Clear();
	}
	while(Cont);

	// Run all post-doc methods that are enabled
	//ComputePostDoc(rec);
}


//------------------------------------------------------------------------------
void GSession::ComputePostDoc(GSlot* rec)
{
	char tmp[100];

	// Run all post-group methods that are enabled
	R::RCursor<GFactoryPostDoc> PostDocs=(dynamic_cast<GPostDocManager*>(GPluginManagers::PluginManagers.GetManager("PostDoc")))->GetFactories();

	if(rec)
		rec->Interact();
	if(ExternBreak) return;

	for(PostDocs.Start();!PostDocs.End();PostDocs.Next())
	{
		if(rec)
			rec->Interact();
		if(ExternBreak) return;
		if(PostDocs()->GetPlugin())
		{
			sprintf(tmp, "PostDoc : Running %s",PostDocs()->GetName().Latin1());
			rec->WriteStr(tmp);
			PostDocs()->GetPlugin()->Run();
		}
	}
}


//------------------------------------------------------------------------------
void GSession::QueryMetaEngine(RContainer<RString,true,false> &keyWords)
{
	GMetaEngine* metaEngine;
	// Verify that a meta engine is selected
	metaEngine=(dynamic_cast<GMetaEngineManager*>(GPluginManagers::PluginManagers.GetManager("MetaEngine")))->GetCurrentMethod();
	if(!metaEngine)
		throw GException("No meta engine method chosen.");
	metaEngine->Query(keyWords,true); //true ->Use all keywords passed to the meta engine
	metaEngine->Process();
}


//------------------------------------------------------------------------------
void GSession::CalcProfiles(GSlot* rec,bool modified,bool save,bool saveLinks)
{
	RCursor<GSubProfile> Subs;
	R::RCursor<GProfile> Prof=GetProfilesCursor();
	GProfileCalc* Profiling=(dynamic_cast<GProfileCalcManager*>(GPluginManagers::PluginManagers.GetManager("ProfileCalc")))->GetCurrentMethod();
	GLinkCalc* LinkCalc=(dynamic_cast<GLinkCalcManager*>(GPluginManagers::PluginManagers.GetManager("LinkCalc")))->GetCurrentMethod();

	//runs the pre profiling methods;
	ComputePreProfile(rec);


	if(!Profiling)
		throw GException("No computing method chosen.");

	for(Prof.Start();!Prof.End();Prof.Next())
	{
		if(rec)
			rec->receiveNextProfile(Prof());
		Prof()->Update();
		//Calc Links on the profile description
		if(LinkCalc)
			LinkCalc->Compute(Prof());
		Subs=Prof()->GetSubProfilesCursor();
		for(Subs.Start();!Subs.End();Subs.Next())
		{
			if(rec)
				rec->Interact();

			if(ExternBreak) return;
			if(modified&&(Subs()->GetState()==osUpToDate)) continue;
			try
			{
				if((!modified)||(Subs()->GetState()!=osUpdated))
				{
					Profiling->Compute(Subs());

					if(save)
					{
						Storage->SaveSubProfile(Subs());
					}
				}
			}
			catch(GException& e)
			{
			}
		}
	}

	//Save the best computed Links (As Hub and Authority)
	if((saveLinks) &&(LinkCalc))
		Storage->SaveLinks(this);

	//runs the post profiling methds;
	ComputePostProfile(rec);
}


//------------------------------------------------------------------------------
void GSession::CalcProfile(GSlot* rec,GProfile* profile,bool modified,bool save,bool saveLinks)
{
	GProfileCalc* Profiling=(dynamic_cast<GProfileCalcManager*>(GPluginManagers::PluginManagers.GetManager("ProfileCalc")))->GetCurrentMethod();
	GLinkCalc* LinkCalc=(dynamic_cast<GLinkCalcManager*>(GPluginManagers::PluginManagers.GetManager("LinkCalc")))->GetCurrentMethod();

	//runs the pre profiling methods;
//	ComputePreProfile(rec);


	if(!Profiling)
		throw GException("No computing method chosen.");
	if(rec)
		rec->receiveNextProfile(profile);
	profile->Update();
	//Calc Links on the profile description
	if(LinkCalc)
		LinkCalc->Compute(profile);
	RCursor<GSubProfile> Subs(profile->GetSubProfilesCursor());
	for(Subs.Start();!Subs.End();Subs.Next())
	{
		if(rec)
			rec->Interact();

		if(ExternBreak) return;
		if(modified&&(Subs()->GetState()==osUpToDate)) continue;
		try
		{
			if((!modified)||(Subs()->GetState()!=osUpdated))
			{
				Profiling->Compute(Subs());

				if(save)
				{
					Storage->SaveSubProfile(Subs());
				}
			}
		}
		catch(GException& e)
		{
		}
	}

	//Save the best computed Links (As Hub and Authority)
	if((saveLinks) &&(LinkCalc))
		Storage->SaveLinks(this);

	//runs the post profiling methds;
//	ComputePostProfile(rec);
}


//------------------------------------------------------------------------------
void GSession::ComputePreProfile(GSlot* rec)
{
	char tmp[100];

	// Run all post-group methods that are enabled
	R::RCursor<GFactoryPreProfile> PreProfile=(dynamic_cast<GPreProfileManager*>(GPluginManagers::PluginManagers.GetManager("PreProfile")))->GetFactories();

	if(rec)
		rec->Interact();
	if(ExternBreak) return;

	for(PreProfile.Start();!PreProfile.End();PreProfile.Next())
	{
		if(rec)
			rec->Interact();
		if(ExternBreak) return;
		if(PreProfile()->GetPlugin())
		{
			sprintf(tmp, "PostProfile : Running %s",PreProfile()->GetName().Latin1());
			rec->WriteStr(tmp);
			PreProfile()->GetPlugin()->Run();
		}
	}
}


//------------------------------------------------------------------------------
void GSession::ComputePostProfile(GSlot* rec)
{
	char tmp[100];

	// Run all post-group methods that are enabled
	R::RCursor<GFactoryPostProfile> PostProfile=(dynamic_cast<GPostProfileManager*>(GPluginManagers::PluginManagers.GetManager("PostProfile")))->GetFactories();

	if(rec)
		rec->Interact();
	if(ExternBreak) return;

	for(PostProfile.Start();!PostProfile.End();PostProfile.Next())
	{
		if(rec)
			rec->Interact();
		if(ExternBreak) return;
		if(PostProfile()->GetPlugin())
		{
			sprintf(tmp, "PostProfile : Running %s",PostProfile()->GetName().Latin1());
			rec->WriteStr(tmp);
			PostProfile()->GetPlugin()->Run();
		}
	}
}


//------------------------------------------------------------------------------
void GSession::GroupingProfiles(GSlot* rec,bool modified,bool save)  throw(GException)
{
	GGrouping* Grouping=(dynamic_cast<GGroupingManager*>(GPluginManagers::PluginManagers.GetManager("Grouping")))->GetCurrentMethod();

	// Verify that there is a method to cluster the subprofile
	if(!Grouping)
		throw GException("No grouping method chosen.");

    // Group the subprofiles
	Grouping->Grouping(rec,modified,save);

	// Run all post-group methods that are enabled
	ComputePostGroup(rec);
}


//------------------------------------------------------------------------------
void GSession::ComputePostGroup(GSlot* rec)
{
	char tmp[100];

	// Run all post-group methods that are enabled
	R::RCursor<GFactoryPostGroup> PostGroups=(dynamic_cast<GPostGroupManager*>(GPluginManagers::PluginManagers.GetManager("PostGroup")))->GetFactories();

	if(rec)
		rec->Interact();
	if(ExternBreak) return;

	for(PostGroups.Start();!PostGroups.End();PostGroups.Next())
	{
		if(rec)
			rec->Interact();
		if(ExternBreak) return;
		if(PostGroups()->GetPlugin())
		{
			sprintf(tmp, "PostGroup : Running %s",PostGroups()->GetName().Latin1());
			rec->WriteStr(tmp);
			PostGroups()->GetPlugin()->Run();
		}
	}
}


//------------------------------------------------------------------------------
void GSession::InsertFdbk(unsigned int p,unsigned int d,tDocAssessment assess,R::RDate date)
{
	GProfile* prof=GetProfile(p);
	GDoc* doc=GetDoc(d);

	if((!doc)||(!prof))
		return;
	prof->InsertFdbk(d,assess,date);
	doc->InsertFdbk(p);
}


//------------------------------------------------------------------------------
void GSession::ClearFdbks(void)
{
	RCursor<GDoc> Docs(*this);
	for(Docs.Start();!Docs.End();Docs.Next())
	{
		Docs()->ClearFdbks();
	}
	RCursor<GProfile> Profiles=GetProfilesCursor();
	for(Profiles.Start();!Profiles.End();Profiles.Next())
	{
		Profiles()->ClearFdbks();
	}
}


//------------------------------------------------------------------------------
void GSession::CopyIdealGroups(bool save)
{
	R::RCursor<GGroup> Grps;
//	GGroupCursor Ideal;
	RCursor<GSubProfile> Sub;
//	GGroups* grps;
	GGroup* grp;
	GGroupCalc* CalcDesc;

	// Get current grouping description method
	CalcDesc=(dynamic_cast<GGroupCalcManager*>(GPluginManagers::PluginManagers.GetManager("GroupCalc")))->GetCurrentMethod();

	// Clear current group
	ClearGroups();

	// Go through each languages
	Grps=Subjects->GetIdealGroups()->GetGroupsCursor();
	for(Grps.Start();!Grps.End();Grps.Next())
	{
		// Create a new group in groups
		grp=new GGroup(cNoRef,Grps()->GetLang(),true);
		AssignId(grp);
		InsertGroup(grp);

		// Go through each subprofile
		Sub=Grps()->GetSubProfilesCursor();
		for(Sub.Start();!Sub.End();Sub.Next())
		{
			grp->InsertSubProfile(Sub());
		}

		// Compute Description
		CalcDesc->Compute(grp);
	}

	if(save)
		Storage->SaveGroups(this);
}


//------------------------------------------------------------------------------
void GSession::RunPrg(GSlot* rec,const char* filename)
{
	GSessionPrg Prg(filename,this,rec);
	Prg.Exec();
}


//------------------------------------------------------------------------------
void GSession::DocsFilter(int nbdocs,int nboccurs)
{
	//The number of word in current lang.
	R::RCursor<GDoc> DocCursorTemp =GetDocsCursor();
	DocCursorTemp.Start();

	int NbKwd;
	GDict* CurDic=DocCursorTemp()->GetLang()->GetDict();
	NbKwd=CurDic->GetDataMaxId();

	int* j;
	int* k;
	bool* test;
	j=new int [NbKwd];
	k=new int [NbKwd];
	test=new bool[NbKwd];
	for(int i=0;i<NbKwd;i++)
	{
		j[i]=0;
		k[i]=0;
	}
	R::RCursor<GDoc> DocCursor=GetDocsCursor();
	for(DocCursor.Start();!DocCursor.End();DocCursor.Next())
	{
		GDoc* Doc=DocCursor();
		RCursor<GWeightInfo> Cur(*Doc);
		for(Cur.Start();!Cur.End();Cur.Next())
		{
			GWeightInfo* WW=Cur();
			if(WW->GetWeight()>j[WW->GetId()])
			{
				j[WW->GetId()]=int(WW->GetWeight());
			}
			k[WW->GetId()]+=1;
		}
	}
	int compt=0;
	for(int i=0;i<NbKwd;i++)
	{
		if((j[i]>=nboccurs)&&k[i]>=nbdocs)
			{
				test[i]=true;
				compt++;
			}
		else
			{
				test[i]=false;
			}
	}

	for(DocCursor.Start();!DocCursor.End();DocCursor.Next())
	{
		GDoc* Doc=DocCursor();
		for(int i=0;i<NbKwd;i++)
		{
			if(!test[i])
			{
				GWeightInfo* WW=Doc->GetPtr<const unsigned int>(i);
				if(WW) Doc->DeletePtr(*WW);
			}
		}
	}
	delete[] j;
	delete[] k;
	delete[] test;
}


//------------------------------------------------------------------------------
void GSession::SetCurrentRandom(int rand)
{
	CurrentRandom=rand;
	Random->Reset(CurrentRandom);
}


//------------------------------------------------------------------------------
int GSession::GetCurrentRandomValue(unsigned int max)
{
	return(int(Random->Value(max)));
}


//------------------------------------------------------------------------------
void GSession::LoadHistoricGroupsById(unsigned int mingen, unsigned int maxgen)
{
	unsigned int i;

	// fill the container
	for (i=mingen; i<maxgen+1; i++)
		GroupsHistoryMng->InsertGroupsHistory(Storage->LoadAnHistoricGroups(this, i));
}


//------------------------------------------------------------------------------
void GSession::LoadHistoricGroupsByDate(RString mindate,RString maxdate)
{
	Storage->LoadHistoricGroupsByDate(this,mindate,maxdate);
}


//------------------------------------------------------------------------------
void GSession::ReInit(bool)
{
	// Clean subprofiles and feedbacks
	ClearGroups();
	ClearFdbks();
	ClearUsers();
}


//------------------------------------------------------------------------------
void GSession::ExportMatrix(GSlot* rec, const char* type, const char* filename, GLang* lang, bool label)
{
	Storage->ExportMatrix(this, rec, type, filename, lang, label);
}


//------------------------------------------------------------------------------
void GSession::New(GDocProxy* &ptr,unsigned int id)
{
	GDoc* doc=GetDoc(id);
	if(!doc)
		ptr=0;
	else
		ptr=new GDocProxyMem(doc);
}


//------------------------------------------------------------------------------
void GSession::New(GSubProfileProxy* &ptr,unsigned int id)
{
	GSubProfile* sub=GetSubProfile(id);
	if(!sub)
		ptr=0;
	else
		ptr=new GSubProfileProxyMem(sub);
}


//------------------------------------------------------------------------------
void GSession::New(GProfileProxy* &ptr,unsigned int id)
{
	GProfile* prof=GetProfile(id);
	if(!prof)
		ptr=0;
	else
		ptr=new GProfileProxyMem(prof);
}


//------------------------------------------------------------------------------
GSession::~GSession(void)
{
	try
	{
		// Clear all entities
		ClearGroups();
		GUsers::Clear();
		GDocs::Clear();

		GPluginManagers::PluginManagers.Disconnect(this);

		// Delete stuctures
		if(Random) delete Random;
		if(Subjects) delete Subjects;
		if(GroupsHistoryMng) delete GroupsHistoryMng;
	}
	catch(...)
	{
	}
	Session=0;
	ExternBreak=false;
}



//------------------------------------------------------------------------------
//
// class GSessionParams
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GSessionParams::GSessionParams(void)
	: GParams("Session Parameters")
{
	GParam* p;
	InsertPtr(p=new GParamUInt("SameBehaviourMinDocs",7));
	InsertPtr(p=new GParamUInt("DiffBehaviourMinDocs",4));
	InsertPtr(p=new GParamDouble("NullSimLevel",0.00001));
	InsertPtr(p=new GParamBool("SaveGroupsHistory",false));
	InsertPtr(p=new GParamBool("SaveProfilesHistory",false));
	InsertPtr(p=new GParamBool("DebugSim",false));
	InsertPtr(p=new GParamBool("DebugBehaviour",false));
	InsertPtr(p=new GParamBool("DebugMinSim",false));
	InsertPtr(p=new GParamDouble("MinSim",0.05));
	InsertPtr(p=new GParamBool("AutomaticMinSim",true));
	InsertPtr(p=new GParamString("PathtoBinary","/var/galilei/bin/"));
}
