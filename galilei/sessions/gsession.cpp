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
#include <gpluginmanagers.h>
using namespace GALILEI;



//------------------------------------------------------------------------------
//
// Static Variables
//
//------------------------------------------------------------------------------
R::RContainer<GSignalHandler,false,false> GSession::Handlers(30,20);
GPluginManagers GPluginManagers::PluginManagers;



//------------------------------------------------------------------------------
//
// General functions
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
R::RString GetObjType(tObjType objtype)
{
	switch(objtype)
	{
		case otNoClass:
			return(RString("unknow"));
			break;
		case otSession:
			return(RString("session"));
			break;
		case otDoc:
			return(RString("document"));
			break;
		case otDocs:
			return(RString("documents"));
			break;
		case otUsers:
			return(RString("users"));
			break;
		case otUser:
			return(RString("user"));
			break;
		case otProfile:
			return(RString("profile"));
			break;
		case otSubProfile:
			return(RString("subProfile"));
			break;
		case otGroups:
			return(RString("groups"));
			break;
		case otGroup:
			return(RString("group"));
			break;
		case otDocSubProfile:
			return(RString("document or subprofile"));
			break;
		case otDocGroup:
			return(RString("document or group"));
			break;
		case otSubProfileGroup:
			return(RString("subprofile or group"));
			break;
		case otFdbk:
			return(RString("assessment"));
			break;
	}
	return(RString("unknow"));
}


//------------------------------------------------------------------------------
R::RString GetState(tObjState state)
{
	switch(state)
	{
		case osUnknow:
			return(RString("unknow"));
			break;
		case osCreated:
			return(RString("created"));
			break;
		case osUpToDate:
			return(RString("up to date"));
			break;
		case osModified:
			return(RString("modified"));
			break;
		case osUpdated:
			return(RString("updated"));
			break;
		case osSaved:
			return(RString("saved"));
			break;
		case osDelete:
			return(RString("deleted"));
			break;
		case osNotNeeded:
			return(RString("not needed"));
			break;
		case osOnDemand:
			return(RString("demands information"));
			break;
		case osNeedLoad:
			return(RString("must load information"));
			break;
	}
	return(RString("unknow"));
}


//------------------------------------------------------------------------------
R::RString GetEvent(tEvent event)
{
	switch(event)
	{
		case eUnknow:
			return(RString("Unknow"));
			break;
		case eObjCreated:
			return(RString("object created"));
			break;
		case eObjModified:
			return(RString("object modified"));
			break;
		case eObjDeleted:
			return(RString("object deleted"));
			break;
	}
	return(RString("unknow"));
}


//------------------------------------------------------------------------------
R::RString GetAssessment(tDocAssessment assessment)
{
	switch(assessment)
	{
		case djUnknow:
			return(RString("unknow"));
			break;
		case djOK:
			return(RString("relevant"));
			break;
		case djKO:
			return(RString("fuzzy relevant"));
			break;
		case djOutScope:
			return(RString("irrelevant"));
			break;
		case djHub:
			return(RString("hub"));
			break;
		case djAutority:
			return(RString("autority"));
			break;
		case djMaskJudg:
			return(RString("mask for assessments"));
			break;
		case djMaskHubAuto:
			return(RString("mask for hub/autority"));
			break;
	}
	return(RString("unknow"));
}


//------------------------------------------------------------------------------
R::RString GetInfoType(tInfoType infotype)
{
	switch(infotype)
	{
		case infoNothing:
			return(RString("unknow"));
			break;
		case infoWord:
			return(RString("word"));
			break;
		case infoWordList:
			return(RString("list of words"));
			break;
		case infoWordOccurs:
			return(RString("list of occurences of a word"));
			break;
		case infoDoc:
			return(RString("document"));
			break;
	}
	return(RString("unknow"));
}



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
GSession::GSession(GStorage* str,bool savedocs,bool modifieddocs,bool savesubprofiles,bool modifiedsubprofiles,bool savegroups,bool modifiedgroups)
	: GDocs(str->GetNbSaved(otDoc)), GUsers(str->GetNbSaved(otUser),str->GetNbSaved(otProfile)),
	  GGroups(str->GetNbSaved(otGroup)), Subjects(0), ProfilesSims(0), ProfilesDocsSims(0),
	  GroupsDocsSims(0), Random(0), Storage(str),  SaveDocs(savedocs),
	  SaveSubProfiles(savesubprofiles), SaveGroups(savegroups), ComputeModifiedDocs(modifieddocs),
	  ComputeModifiedSubProfiles(modifiedsubprofiles), ComputeModifiedGroups(modifiedgroups), Slot(0)
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
bool GSession::MustSave(tObjType objtype) const
{
	switch(objtype)
	{
		case otDoc:
			return(SaveDocs);
			break;
		case otSubProfile:
			return(SaveSubProfiles);
			break;
		case otGroup:
			return(SaveGroups);
			break;
		default:
			throw GException(itou(objtype)+": Invalid object type for session parameters.");
			return(false);
	}
}


//------------------------------------------------------------------------------
bool GSession::ComputeModified(tObjType objtype) const
{
	switch(objtype)
	{
		case otDoc:
			return(ComputeModifiedDocs);
			break;
		case otSubProfile:
			return(ComputeModifiedSubProfiles);
			break;
		case otGroup:
			return(ComputeModifiedGroups);
			break;
		default:
			throw GException(itou(objtype)+": Invalid object type for session parameters.");
			return(false);
	}
}


//------------------------------------------------------------------------------
void GSession::SetSave(tObjType objtype,bool save)
{
	switch(objtype)
	{
		case otDoc:
			SaveDocs=save;
			break;
		case otSubProfile:
			SaveSubProfiles=save;
			break;
		case otGroup:
			SaveGroups=save;
			break;
		default:
			throw GException(itou(objtype)+": Invalid object type for session parameters.");
			break;
	}
}


//------------------------------------------------------------------------------
void GSession::SetComputeModified(tObjType objtype,bool modified)
{
	switch(objtype)
	{
		case otDoc:
			ComputeModifiedDocs=modified;
			break;
		case otSubProfile:
			ComputeModifiedSubProfiles=modified;
			break;
		case otGroup:
			ComputeModifiedGroups=modified;
			break;
		default:
			throw GException(itou(objtype)+": Invalid object type for session parameters.");
			break;
	}
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
GSubjects* GSession::GetSubjects(void)
{
	if(!Subjects)
	{
		Subjects=new GSubjects(this);
		if(Storage)
			Storage->LoadSubjectTree(Session);
	}
	return(Subjects);
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
void GSession::SetSims(GProfilesGroupsSimsManager* sims)
{
	ProfilesGroupsSims=sims;
}


//------------------------------------------------------------------------------
void GSession::SetSlot(GSlot* slot)
{
	Slot=slot;
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
void GSession::AnalyseDocs(GSlot* rec)
{
	bool undefLang;
	GDocXML* xml=0;
	R::RCursor<GDoc> Docs=GetDocs();
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
			if(ComputeModifiedDocs&&(!Docs()->MustCompute())) continue;
			if(rec)
			{
				rec->Interact();
				rec->NextDoc(Docs());
			}
			if(ExternBreak) return;
			undefLang=false;
			try
			{
				if((!ComputeModifiedDocs)||(Docs()->MustCompute()))
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
				if(SaveDocs)
				{
					Storage->SaveDoc(Docs());
					Docs()->SetState(osSaved);
				}
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
void GSession::AnalyseNewDocs(GSlot* rec)
{
	bool undefLang;
	GDocXML* xml=0;
	R::RCursor<GDoc> Docs=GetDocs();
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
			if(ComputeModifiedDocs&&(!Docs()->MustCompute())) continue;
			if(rec)
			{
				rec->Interact();
				rec->NextDoc(Docs());
			}
			if(ExternBreak) return;
			undefLang=false;
			try
			{
				if((!ComputeModifiedDocs)||(Docs()->MustCompute()))
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
				if(SaveDocs)
				{
					Storage->SaveDoc(Docs());
					Docs()->SetState(osSaved);
				}
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
			if(rec)
			{
				sprintf(tmp, "PostDoc : Running %s",PostDocs()->GetName().Latin1());
				rec->WriteStr(tmp);
			}
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
void GSession::CalcProfiles(GSlot* rec)
{
	RCursor<GSubProfile> Subs;
	R::RCursor<GProfile> Prof=GetProfiles();
	GProfileCalc* Profiling=(dynamic_cast<GProfileCalcManager*>(GPluginManagers::PluginManagers.GetManager("ProfileCalc")))->GetCurrentMethod();
	GLinkCalc* LinkCalc=(dynamic_cast<GLinkCalcManager*>(GPluginManagers::PluginManagers.GetManager("LinkCalc")))->GetCurrentMethod();

	//runs the pre profiling methods;
	ComputePreProfile(rec);


	if(!Profiling)
		throw GException("No computing method chosen.");

	for(Prof.Start();!Prof.End();Prof.Next())
	{
		if(rec)
			rec->NextProfile(Prof());
		Prof()->Update();
		//Calc Links on the profile description
		if(LinkCalc)
			LinkCalc->Compute(Prof());
		Subs=Prof()->GetSubProfiles();
		for(Subs.Start();!Subs.End();Subs.Next())
		{
			if(rec)
				rec->Interact();

			if(ExternBreak) return;
			if(ComputeModifiedSubProfiles&&(!Subs()->MustCompute())) continue;
			try
			{
				if((!ComputeModifiedSubProfiles)||(Subs()->MustCompute()))
				{
					Profiling->Compute(Subs());

					if(SaveSubProfiles)
					{
						Storage->SaveSubProfile(Subs());
						Subs()->SetState(osSaved);
					}
				}
			}
			catch(GException& e)
			{
			}
		}
	}

	// Save the best computed Links (As Hub and Authority)
	if((SaveSubProfiles)&&(LinkCalc))
		Storage->SaveLinks(this);

	//runs the post profiling methds;
	ComputePostProfile(rec);
}


//------------------------------------------------------------------------------
void GSession::CalcProfile(GSlot* rec,GProfile* profile)
{
	GProfileCalc* Profiling=(dynamic_cast<GProfileCalcManager*>(GPluginManagers::PluginManagers.GetManager("ProfileCalc")))->GetCurrentMethod();
	GLinkCalc* LinkCalc=(dynamic_cast<GLinkCalcManager*>(GPluginManagers::PluginManagers.GetManager("LinkCalc")))->GetCurrentMethod();

	//runs the pre profiling methods;
//	ComputePreProfile(rec);


	if(!Profiling)
		throw GException("No computing method chosen.");
	if(rec)
		rec->NextProfile(profile);
	profile->Update();
	//Calc Links on the profile description
	if(LinkCalc)
		LinkCalc->Compute(profile);
	RCursor<GSubProfile> Subs(profile->GetSubProfiles());
	for(Subs.Start();!Subs.End();Subs.Next())
	{
		if(rec)
			rec->Interact();

		if(ExternBreak) return;
		if(ComputeModifiedSubProfiles&&(!Subs()->MustCompute())) continue;
		try
		{
			if((!ComputeModifiedSubProfiles)||(Subs()->MustCompute()))
			{
				Profiling->Compute(Subs());

				if(SaveSubProfiles)
				{
					Storage->SaveSubProfile(Subs());
					Subs()->SetState(osSaved);
				}
			}
		}
		catch(GException& e)
		{
		}
	}

	// Save the best computed Links (As Hub and Authority)
	if((SaveSubProfiles)&&(LinkCalc))
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
			if(rec)
			{
				sprintf(tmp, "PostProfile : Running %s",PreProfile()->GetName().Latin1());
				rec->WriteStr(tmp);
			}
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
			if(rec)
			{
				sprintf(tmp, "PostProfile : Running %s",PostProfile()->GetName().Latin1());
				rec->WriteStr(tmp);
			}
			PostProfile()->GetPlugin()->Run();
		}
	}
}


//------------------------------------------------------------------------------
void GSession::GroupingProfiles(GSlot* rec)  throw(GException)
{
	GGrouping* Grouping=(dynamic_cast<GGroupingManager*>(GPluginManagers::PluginManagers.GetManager("Grouping")))->GetCurrentMethod();

	// Verify that there is a method to cluster the subprofile
	if(!Grouping)
		throw GException("No grouping method chosen.");

    // Group the subprofiles
	Grouping->Grouping(rec,ComputeModifiedGroups,SaveGroups);

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
			if(rec)
			{
				sprintf(tmp, "PostGroup : Running %s",PostGroups()->GetName().Latin1());
				rec->WriteStr(tmp);
			}
			PostGroups()->GetPlugin()->Run();
		}
	}
}


//------------------------------------------------------------------------------
void GSession::InsertFdbk(unsigned int p,unsigned int d,tDocAssessment assess,R::RDate date,R::RDate update)
{
	GProfile* prof=GetProfile(p);
	if(prof)
		prof->InsertFdbk(d,assess,date,update);
	GDoc* doc=GetDoc(d);
	if(doc)
		doc->InsertFdbk(p);
}


//------------------------------------------------------------------------------
void GSession::ClearFdbks(void)
{
	RCursor<GDoc> Docs(GetDocs());
	for(Docs.Start();!Docs.End();Docs.Next())
	{
		Docs()->ClearFdbks();
	}
	RCursor<GProfile> Profiles=GetProfiles();
	for(Profiles.Start();!Profiles.End();Profiles.Next())
	{
		Profiles()->ClearFdbks();
	}
}


//------------------------------------------------------------------------------
void GSession::CopyIdealGroups(void)
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
	Grps=Subjects->GetIdealGroups()->GetGroups();
	for(Grps.Start();!Grps.End();Grps.Next())
	{
		// Create a new group in groups
		grp=new GGroup(cNoRef,Grps()->GetLang(),true,RDate(""),RDate(""));
		AssignId(grp);
		InsertGroup(grp);

		// Go through each subprofile
		Sub=Grps()->GetSubProfiles();
		for(Sub.Start();!Sub.End();Sub.Next())
		{
			grp->InsertSubProfile(Sub());
		}

		// Compute Description
		CalcDesc->Compute(grp);
	}

	if(SaveGroups)
	{
		Storage->SaveGroups(this);
		RCursor<GGroup> Groups(Session->GetGroups());
		for(Groups.Start();!Groups.End();Groups.Next())
			Groups()->SetState(osSaved);
	}
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
	R::RCursor<GDoc> DocCursorTemp =GetDocs();
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
	R::RCursor<GDoc> DocCursor=GetDocs();
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
void GSession::ReInit(void)
{
	// Clean groups, feedbacks and users.
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
GSession::~GSession(void)
{
	try
	{
		GPluginManagers::PluginManagers.Disconnect(this);

		// Delete stuctures
		delete Random;
		delete Subjects;
		delete GroupsHistoryMng;
	}
	catch(...)
	{
	}
	Session=0;
	ExternBreak=false;
}
