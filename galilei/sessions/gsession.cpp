/*

	GALILEI Research Project

	GSession.cpp

	Generic GALILEI Session - Implementation.

	Copyright 2001-2004 by the Université libre de Bruxelles.

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
#include <rstd/random.h>
#include <rstd/rcursor.h>
#include <rstd/rtextfile.h>
using namespace R;


//------------------------------------------------------------------------------
// include files for GALILEI
#include <infos/glang.h>
#include <infos/glangmanager.h>
#include <infos/gdict.h>
#include <infos/gwordlist.h>
#include <sessions/gsession.h>
#include <sessions/gstorage.h>
#include <sessions/gslot.h>
#include <sessions/gsessionprg.h>
#include <sessions/gstatscalc.h>
#include <sessions/gstatscalcmanager.h>
#include <docs/gdoc.h>
#include <docs/gdocanalyse.h>
#include <docs/gdocanalysemanager.h>
#include <docs/gdocxml.h>
#include <docs/gdocprofsims.h>
#include <docs/glinkcalc.h>
#include <docs/glinkcalcmanager.h>
#include <docs/glink.h>
#include <docs/gpostdoc.h>
#include <docs/gpostdocmanager.h>
#include <engines/gengine.h>
#include <engines/gmetaengine.h>
#include <engines/genginemanager.h>
#include <engines/gmetaenginemanager.h>
#include <engines/genginedoc.h>
#include <profiles/guser.h>
#include <profiles/gprofile.h>
#include <profiles/gsubprofile.h>
#include <profiles/gprofilessims.h>
#include <profiles/gprofilesbehaviours.h>
#include <profiles/gsubprofile.h>
#include <profiles/gprofilecalc.h>
#include <profiles/gprofilecalcmanager.h>
#include <profiles/gpreprofile.h>
#include <profiles/gpostprofile.h>
#include <profiles/gpreprofilemanager.h>
#include <profiles/gpostprofilemanager.h>
#include <groups/ggroups.h>
#include <groups/ggroup.h>
#include <groups/ggrouping.h>
#include <groups/ggroupingmanager.h>
#include <groups/ggroupcalc.h>
#include <groups/ggroupcalcmanager.h>
#include <groups/gsubjects.h>
#include <docs/gfiltermanager.h>
#include <docs/gfilter.h>
#include <docs/gpostdoc.h>
#include <infos/gweightinfo.h>
#include <historic/ggroupshistory.h>
#include <groups/gpostgroup.h>
#include <groups/gpostgroupmanager.h>
#include <docs/gdocproxymem.h>
#include <profiles/gsubprofileproxymem.h>
#include <profiles/gprofileproxymem.h>
using namespace GALILEI;



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
	  GGroups(0), Subjects(0),ProfilesSims(0), ProfilesBehaviours(0), DocProfSims(0), Random(0),
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
//	: GDocs(2000), GUsers(2000,2000),
//	  GGroups(200), Subjects(0), Fdbks(100,50),
	ProfilesSims(0), ProfilesBehaviours(0), DocProfSims(0), Random(0),
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
	RCursor<GPluginManager> cur = GPluginManager::GetCursor();

	for(cur.Start();!cur.End();cur.Next())
	{
		cur()->Connect(this);
	}

	// Create Similarities Managers (IFF used by default)
	if(SessParams)
	{
		if(!SessParams->GetBool("DebugSim"))
			ProfilesSims = new GProfilesSims(this,true, true);
		if(!SessParams->GetBool("DebugBehaviour"))
			ProfilesBehaviours = new GProfilesBehaviours(this,true);
	}
	DocProfSims = new GDocProfSims(this,true,false);
}


//------------------------------------------------------------------------------
GDocXML* GSession::CreateDocXML(GDoc* doc)
{
	return((dynamic_cast<GFilterManager*>(GPluginManager::GetManager("Filter")))->CreateDocXML(doc));
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
	Analyse=(dynamic_cast<GDocAnalyseManager*>(GPluginManager::GetManager("DocAnalyse")))->GetCurrentMethod();
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
					xml=(dynamic_cast<GFilterManager*>(GPluginManager::GetManager("Filter")))->CreateDocXML(Docs());
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
		Cont=tmpDocs.NbPtr;
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
	Analyse=(dynamic_cast<GDocAnalyseManager*>(GPluginManager::GetManager("DocAnalyse")))->GetCurrentMethod();
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
					xml=(dynamic_cast<GFilterManager*>(GPluginManager::GetManager("Filter")))->CreateDocXML(Docs());
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
		Cont=tmpDocs.NbPtr;
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
	R::RCursor<GFactoryPostDoc> PostDocs=(dynamic_cast<GPostDocManager*>(GPluginManager::GetManager("PostDoc")))->GetPostDocsCursor();

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
	metaEngine=(dynamic_cast<GMetaEngineManager*>(GPluginManager::GetManager("MetaEngine")))->GetCurrentMethod();
	if(!metaEngine)
		throw GException("No meta engine method chosen.");
	metaEngine->Query(keyWords,true); //true ->Use all keywords passed to the meta engine
	metaEngine->Process();
}


//------------------------------------------------------------------------------
void GSession::UpdateBehaviours(void)
{
	if (!SessParams->GetBool("DebugBehaviour"))
		ProfilesBehaviours->Update();
}


//------------------------------------------------------------------------------
void GSession::UpdateProfilesSims(void)
{
	if (!SessParams->GetBool("DebugSim"))
		ProfilesSims->Update();
}


//------------------------------------------------------------------------------
void GSession::AddModifiedProfile(GSubProfile* sub)
{
	if(ProfilesSims)
		ProfilesSims->AddModifiedProfile(sub);
	if(ProfilesBehaviours)
		ProfilesBehaviours->AddModifiedProfile(sub);
}


//------------------------------------------------------------------------------
void GSession::UseIFFDocProf(bool iff)
{
	DocProfSims->UseIFF(iff);
}


//------------------------------------------------------------------------------
double GSession::GetSimDocProf(const GDoc* doc,const GSubProfile* sub)
{
	return(DocProfSims->GetSim(doc,sub));
}


//------------------------------------------------------------------------------
double GSession::GetSimDocProf(unsigned int doc,unsigned int sub)
{
	return(DocProfSims->GetSim(GetDoc(doc),GetSubProfile(sub)));
}


//------------------------------------------------------------------------------
void GSession::UseIFFProfs(bool iff)
{
	ProfilesSims->UseIFF(iff);
}


//------------------------------------------------------------------------------
double GSession::GetSimProf(const GSubProfile* sub1,const GSubProfile* sub2)
{
	if (SessParams->GetBool("DebugSim"))
		return(sub1->SimilarityIFF(sub2));
	return(ProfilesSims->GetSim(sub1,sub2));

}


//------------------------------------------------------------------------------
double GSession::GetAgreementRatio(GSubProfile* sub1,GSubProfile* sub2)
{
	double nbcommon, okratio;

	if (SessParams->GetBool("DebugBehaviour"))
	{
		nbcommon=sub1->GetCommonDocs(sub2);
		if (nbcommon&&nbcommon>=SessParams->GetUInt("SameBehaviourMinDocs"))
			okratio=sub1->GetCommonOKDocs(sub2)/nbcommon;
 		else okratio=0.0;
		return okratio;
	}
	else
		return(ProfilesBehaviours->GetAgreementRatio(sub1,sub2));
}


//------------------------------------------------------------------------------
double GSession::GetDisagreementRatio(GSubProfile* sub1,GSubProfile* sub2)
{
	double nbcommon, diffratio;

	if (SessParams->GetBool("DebugBehaviour"))
	{
		nbcommon=sub1->GetCommonDocs(sub2);
		if (nbcommon&&nbcommon>=SessParams->GetUInt("DiffBehaviourMinDocs"))
			diffratio=sub1->GetCommonDiffDocs(sub2)/nbcommon;
 		else diffratio=0.0;
		return diffratio;
	}
	else
		return(ProfilesBehaviours->GetDisagreementRatio(sub1,sub2));
}


//------------------------------------------------------------------------------
double GSession::GetMinimumOfSimilarity(GLang* lang, double deviationrate)
{
	double tmpsim, simssum, deviation, MeanSim;
	unsigned int nbcomp, i, j;
	RCursor<GSubProfile> s, s2;

	//if min sim is not automatic, returnthe fixed value
	if (!SessParams->GetBool("AutomaticMinSim"))
	{
		return(SessParams->GetDouble("MinSim"));
	}

	//else return the computed min sim
	s=this->GetSubProfilesCursor(lang);
	s2=this->GetSubProfilesCursor(lang);

	//if debug mode, force min sim recomputing
	if (SessParams->GetBool("DebugMinSim"))
	{
		simssum=deviation=0.0;
		nbcomp=0;

		for(s.Start(),i=0,j=s.GetNb();--j;s.Next(),i++)
		{
			if (!s()->IsDefined()) continue;
			for(s2.GoTo(i+1);!s2.End();s2.Next())
			{
				if (!s2()->IsDefined()) continue;
				tmpsim=s()->SimilarityIFF(s2());
				simssum+=tmpsim;
				deviation+=tmpsim*tmpsim;
				nbcomp++;
			}
		}
		if (nbcomp)
		{
			MeanSim=simssum/double(nbcomp);
			deviation/=double(nbcomp);
			deviation-=MeanSim*MeanSim;
		}
		return(MeanSim+deviationrate*sqrt(deviation));
	}
	//else return the stored min sim
	else
	{
		return(ProfilesSims->GetMinimumOfSimilarity(lang,deviationrate));
	}
}


//------------------------------------------------------------------------------
void GSession::CalcProfiles(GSlot* rec,bool modified,bool save,bool saveLinks)
{
	RCursor<GSubProfile> Subs;
	R::RCursor<GProfile> Prof=GetProfilesCursor();
	GProfileCalc* Profiling=(dynamic_cast<GProfileCalcManager*>(GPluginManager::GetManager("ProfileCalc")))->GetCurrentMethod();
	GLinkCalc* LinkCalc=(dynamic_cast<GLinkCalcManager*>(GPluginManager::GetManager("LinkCalc")))->GetCurrentMethod();

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

					// add the mofified profile to the list of modified profiles (if it is defined!)
					if (Subs()->IsDefined())
					{
						if (!SessParams->GetBool("DebugSim"))
							ProfilesSims->AddModifiedProfile(Subs());
						if (!SessParams->GetBool("DebugBehaviour"))
							ProfilesBehaviours->AddModifiedProfile(Subs());
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

	// update the state of all the sims.
	if (!SessParams->GetBool("DebugSim"))
		ProfilesSims->Update();

	//update the state of the behaviours
	if (!SessParams->GetBool("DebugBehaviour"))
		ProfilesBehaviours->Update();

	//runs the post profiling methds;
	ComputePostProfile(rec);
}


//------------------------------------------------------------------------------
void GSession::ComputePreProfile(GSlot* rec)
{
	char tmp[100];

	// Run all post-group methods that are enabled
	R::RCursor<GFactoryPreProfile> PreProfile=(dynamic_cast<GPreProfileManager*>(GPluginManager::GetManager("PreProfile")))->GetPreProfileCursor();

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
	R::RCursor<GFactoryPostProfile> PostProfile=(dynamic_cast<GPostProfileManager*>(GPluginManager::GetManager("PostProfile")))->GetPostProfileCursor();

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
	GGrouping* Grouping=(dynamic_cast<GGroupingManager*>(GPluginManager::GetManager("Grouping")))->GetCurrentMethod();

	// Verify that there is a method to cluster the subprofile
	if(!Grouping)
		throw GException("No grouping method chosen.");

	// Update the similarities and the behaviors of the subprofiles
	UpdateBehaviours();
	UpdateProfilesSims();

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
	R::RCursor<GFactoryPostGroup> PostGroups=(dynamic_cast<GPostGroupManager*>(GPluginManager::GetManager("PostGroup")))->GetPostGroupsCursor();

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
	RCursor<GDoc> Docs(this);
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
	CalcDesc=(dynamic_cast<GGroupCalcManager*>(GPluginManager::GetManager("GroupCalc")))->GetCurrentMethod();

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
		for(Doc->Start();!Doc->End();Doc->Next())
		{
			GWeightInfo* WW=(*Doc)();
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
				if(WW) Doc->DeletePtr(WW);
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
	ClearSubProfiles();

	// Re-Init the sims and behaviorsbetween documents and subprofiles
	if (!SessParams->GetBool("DebugSim"))
		ProfilesSims->ReInit();
	if (!SessParams->GetBool("DebugBehaviour"))
		ProfilesBehaviours->ReInit();
	DocProfSims->ReInit();
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
		// Delete Similarities Managers
		if(DocProfSims) delete DocProfSims;
		if(ProfilesBehaviours) delete ProfilesBehaviours;
		if(ProfilesSims) delete ProfilesSims;

		// Clear all entities
		ClearGroups();
		GUsers::Clear();
		GDocs::Clear();

		RCursor<GPluginManager> cur = GPluginManager::GetCursor();
		for(cur.Start();!cur.End();cur.Next())
		{
			cur()->Disconnect(this);
		}

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
