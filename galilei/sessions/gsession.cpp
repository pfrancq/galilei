/*

	GALILEI Research Project

	GSession.cpp

	Generic GALILEI Session - Implementation.

	Copyright 2001-2002 by the Université Libre de Bruxelles.

	Authors:

	Version $Revision$

	Last Modify: $Date$

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

//-----------------------------------------------------------------------------
// include files for R Project
#include <fstream>

//-----------------------------------------------------------------------------
// include files for R Project
#include <rstd/random.h>
#include <rstd/rcursor.h>
#include <rstd/rtextfile.h>
using namespace R;


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <infos/glang.h>
#include <infos/glangmanager.h>
#include <infos/gdict.h>
#include <infos/gwordlist.h>
#include <sessions/gsession.h>
#include <sessions/gslot.h>
#include <sessions/gsessionprg.h>
#include <sessions/gstatscalc.h>
#include <sessions/gstatscalcmanager.h>
#include <docs/gdocvector.h>
#include <docs/gdocanalyse.h>
#include <docs/gdocanalysemanager.h>
#include <docs/gdocxml.h>
#include <docs/gdocprofsims.h>
#include <docs/glinkcalc.h>
#include <docs/glinkcalcmanager.h>
#include <docs/glink.h>
#include <docs/gpostdoc.h>
#include <docs/gpostdocmanager.h>
#include <profiles/guser.h>
#include <profiles/gprofile.h>
#include <profiles/gsubprofile.h>
#include <profiles/gprofilessims.h>
#include <profiles/gprofilesbehaviours.h>
#include <profiles/gsubprofilevector.h>
#include <profiles/gprofdoc.h>
#include <profiles/gprofilecalc.h>
#include <profiles/gprofilecalcmanager.h>
#include <groups/ggroups.h>
#include <groups/ggroup.h>
#include <groups/ggroupvector.h>
#include <groups/ggrouping.h>
#include <groups/ggroupingmanager.h>
#include <groups/ggroupcalc.h>
#include <groups/ggroupcalcmanager.h>
#include <groups/gsubjects.h>
#include <docs/gfiltermanager.h>
#include <docs/gfilter.h>
#include <infos/gweightinfo.h>
#include <historic/ggroupshistory.h>
#include <groups/gpostgroup.h>
#include <groups/gpostgroupmanager.h>

using namespace GALILEI;



//-----------------------------------------------------------------------------
//
// GSession
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GSession::GSession(unsigned int d,unsigned int u,unsigned int p,unsigned int f,unsigned int g,
 GSessionParams* sessparams,bool tests) throw(bad_alloc,GException)
	: GDocs(d), GUsers(u,p), GGroups(g),
	  Subjects(0), Fdbks(f+f/2,f/2),
	  Langs(0), URLMng(0), ProfilingMng(0), GroupingMng(0), GroupCalcMng(0),
	  StatsCalcMng(0), LinkCalcMng(0), PostGroupMng(0), PostDocMng(0),
	  ProfilesBehaviours(0), DocProfSims(0), Random(0),  SessParams(sessparams)
{
	// Init Part
	CurrentRandom=0;
	Random = new RRandomGood(CurrentRandom);

	// Create SubjectTree
	if(tests)
		Subjects=new GSubjects(this);
}


//-----------------------------------------------------------------------------
void GSession::Connect(GLangManager* langs,GFilterManager* umng, GDocAnalyseManager* dmng, GProfileCalcManager* pmng,
	GGroupingManager* gmng, GGroupCalcManager* gcmng,GStatsCalcManager* smng,
	GPostDocManager* pdmng,GPostGroupManager* pgmng) throw(bad_alloc,GException)

{
//	GLang* lang;

	Langs=langs;
	if(Langs)
	{
		Langs->Connect(this);
/*		GFactoryLangCursor Cur;
		Cur=Langs->GetLangsCursor();
		for(Cur.Start();!Cur.End();Cur.Next())
		{
			lang=Cur()->GetPlugin();
			if(!lang) continue;
			Groups.InsertPtr(new GGroups(lang));
		}*/
	}
	URLMng=umng;
	DocAnalyseMng=dmng;
	if(DocAnalyseMng)
		DocAnalyseMng->Connect(this);
	ProfilingMng=pmng;
	if(ProfilingMng)
		ProfilingMng->Connect(this);
	GroupingMng=gmng;
	if(GroupingMng)
		GroupingMng->Connect(this);
	GroupCalcMng=gcmng;
	if(GroupCalcMng)
		GroupCalcMng->Connect(this);
	StatsCalcMng=smng;
	if(StatsCalcMng)
		StatsCalcMng->Connect(this);
	PostGroupMng=pgmng;
	if(PostGroupMng)
		PostGroupMng->Connect(this);

	PostDocMng=pdmng;
	if(PostDocMng)
		PostDocMng->Connect(this);
}


//-----------------------------------------------------------------------------
void GSession::PostConnect(GLinkCalcManager* lmng) throw(bad_alloc,GException)
{
	LinkCalcMng=lmng;
	if(LinkCalcMng)
		LinkCalcMng->Connect(this);

	// Create Similarities Managers (IFF used by default)
	ProfilesSims = new GProfilesSims(this,true, true);
	ProfilesBehaviours = new GProfilesBehaviours(this,true);
	DocProfSims = new GDocProfSims(this,true,false);
}

//-----------------------------------------------------------------------------
GFactoryLinkCalcCursor& GSession::GetLinkCalcsCursor(void)
{
	GFactoryLinkCalcCursor *cur=GFactoryLinkCalcCursor::GetTmpCursor();
	cur->Set(LinkCalcMng);
	return(*cur);
}


//------------------------------------------------------------------------------
GFactoryPostDocCursor& GSession::GetPostDocsCursor(void)
{
	GFactoryPostDocCursor *cur=GFactoryPostDocCursor::GetTmpCursor();
	cur->Set(PostDocMng);
	return(*cur);
}


//------------------------------------------------------------------------------
GDocXML* GSession::CreateDocXML(GDoc* doc) throw(GException)
{
	return(URLMng->CreateDocXML(doc));
}


//-----------------------------------------------------------------------------
void GSession::AnalyseAssociation(void) throw(GException)
{
	GPostDoc* PostDoc=PostDocMng->GetCurrentMethod();
	if(!PostDoc)
		throw GException("No computing method chosen.");
	else
		PostDoc->Run();
}


//-----------------------------------------------------------------------------
void GSession::RemoveAssociation(void) throw(GException)
{
	unsigned i;
	GDocCursor docs=GetDocsCursor();
	GDocVector* doc;
	GDataCursor Datas;
	docs.Start();
	GDict* dic=docs()->GetLang()->GetDict();

	Datas=dic->GetDataCursor(infoWordList);

	for(i=docs.GetNb(),docs.Start();--i;docs.Next())
	{
		doc=dynamic_cast<GDocVector*>(docs());
		for(Datas.Start();!Datas.End();Datas.Next())
		{
			if(doc->IsIn(Datas()->GetId()))
				doc->DeletePtr(Datas()->GetId());
		}
		doc->UpdateRefs();
	}

	DeleteWordList(dic);
}


//-----------------------------------------------------------------------------
void GSession::AssignId(GGroup* data)
{
	data->SetId(GGroups::GetMaxId()+1);
}


//-----------------------------------------------------------------------------
void GSession::AnalyseDocs(GSlot* rec,bool modified) throw(GException)
{
	bool undefLang;
	GDocXML* xml=0;
	GDocCursor Docs=GetDocsCursor();
	RContainer<GDoc,unsigned int,false,true>* tmpDocs = new RContainer<GDoc,unsigned int,false,true>(5,2);
	GDocAnalyse* Analyse;
	RString err;

	// verify that the textanalyse method is selected
	Analyse=DocAnalyseMng->GetCurrentMethod();
	if(!Analyse)
		throw GException("No document analysis method chosen.");


	// opens and appends the Log File for all errors occuring in the filter or analyse phase.
	if(rec)
	{
		err= "Documents Filtering and Analysis on Data Set : "+GetDbName()+ " on : " +itou(RDate::GetToday().GetDay())+"/"+ itou(RDate::GetToday().GetMonth())+"/"+itou(RDate::GetToday().GetYear());
		rec->WriteStr(err.Latin1());
	}

	for(Docs.Start();!Docs.End();Docs.Next())
	{
		if(modified&&(Docs()->GetState()==osUpToDate)) continue;
		if(rec)
			rec->receiveNextDoc(Docs());
		undefLang=false;
		try
		{
			if(((!modified)||(Docs()->GetState()!=osUpdated))||((Docs()->GetState()!=osNotNeeded)))
			{
				if (!Docs()->GetLang()) undefLang=true;
				xml=URLMng->CreateDocXML(Docs());
				if(xml)
				{
					Docs()->InitFailed();
					Analyse->Analyse(xml,Docs(),tmpDocs);
					delete xml;
					xml=0;
					if ((undefLang)&&(Docs()->GetLang()))
					{
						MoveDoc(Docs());
					}
				}
				else
					Docs()->IncFailed();
			}
			SaveDoc(Docs());
			if(Docs()->GetState()==osUpdated)
				Docs()->SetState(osUpToDate);
		}
		catch(GException& e)
		{
			if(xml)
				delete xml;

			cout<< "error "<<e.GetMsg()<<endl;
			// write error message to the log file handled by the GSlot.
			if(rec)
				rec->WriteStr(e.GetMsg());
		}
	}

	// add new doc in the container of documents.
	RCursor<GDoc,unsigned int> Cur(tmpDocs);
	for (Cur.Start();!Cur.End();Cur.Next())
	{
		InsertDoc(Cur());
	}
	tmpDocs->Clear();

}


//-----------------------------------------------------------------------------
void GSession::UseIFFDocProf(bool iff)
{
	DocProfSims->UseIFF(iff);
}


//-----------------------------------------------------------------------------
double GSession::GetSimDocProf(const GDoc* doc,const GSubProfile* sub) throw(GException)
{
	return(DocProfSims->GetSim(doc,sub));
}


//-----------------------------------------------------------------------------
void GSession::UseIFFProfs(bool iff)
{
	ProfilesSims->UseIFF(iff);
}


//-----------------------------------------------------------------------------
double GSession::GetSimProf(const GSubProfile* sub1,const GSubProfile* sub2) throw(GException)
{
	return(ProfilesSims->GetSim(sub1,sub2));
}


//-----------------------------------------------------------------------------
double GSession::GetAgreementRatio(GSubProfile* sub1,GSubProfile* sub2) throw(GException)
{
	return(ProfilesBehaviours->GetAgreementRatio(sub1,sub2));
}


//-----------------------------------------------------------------------------
double GSession::GetDisagreementRatio(GSubProfile* sub1,GSubProfile* sub2) throw(GException)
{
	return(ProfilesBehaviours->GetDisagreementRatio(sub1,sub2));
}


//-----------------------------------------------------------------------------
double GSession::GetMinimumOfSimilarity(GLang* lang, double deviationrate) throw(GException)
{
	return(ProfilesSims->GetMinimumOfSimilarity(lang,deviationrate));
}



//-----------------------------------------------------------------------------
GUser* GSession::NewUser(const char* /*usr*/,const char* /*pwd*/,const char* /*name*/,const char* /*email*/,
	                  const char* /*title*/,const char* /*org*/,const char* /*addr1*/,
	                  const char* /*addr2*/,const char* /*city*/,const char* /*country*/) throw(bad_alloc)
{
	return(0);
}


//-----------------------------------------------------------------------------
void GSession::CalcProfiles(GSlot* rec,bool modified,bool save) throw(GException)
{
	GSubProfileCursor Subs;
	GSubProfile* sub;
	GProfileCursor Prof=GetProfilesCursor();
	GProfileCalc* Profiling=ProfilingMng->GetCurrentMethod();
	GLinkCalc* LinkCalc=LinkCalcMng->GetCurrentMethod();

	if(!Profiling)
		throw GException("No computing method chosen.");

	for(Prof.Start();!Prof.End();Prof.Next())
	{
		if(rec)
			rec->receiveNextProfile(Prof());
		Subs=Prof()->GetSubProfilesCursor();
		for (Subs.Start(); !Subs.End(); Subs.Next())
		{
			sub=Subs();
			if(modified&&(Subs()->GetState()==osUpToDate)) continue;
			try
			{
				if((!modified)||(Subs()->GetState()!=osUpdated))
				{
					if(LinkCalc)
						LinkCalc->Compute(Subs());
					Profiling->Compute(Subs());

					// add the mofified profile to the list of modified profiles
					ProfilesSims->AddModifiedProfile(Subs());
					ProfilesBehaviours->AddModifiedProfile(Subs());
				}
			}
			catch(GException& e)
			{
			}
		}
	}

	// update the state of all the sims.
	ProfilesSims->Update();

	//update the state of the behaviours
	ProfilesBehaviours->Update();

	//   save profiles if necessary and set their state to UpToDate.
	for(Prof.Start();!Prof.End();Prof.Next())
	{
		if(save)
		{
			try
			{
				SaveProfile(Prof());
			}
			catch(GException& e)
			{
			}
		}
	}
}


//-----------------------------------------------------------------------------
void GSession::CalcProfile(GProfile* prof) throw(GException)
{
	GSubProfileCursor Subs;
	GProfileCalc* Profiling=ProfilingMng->GetCurrentMethod();
	GLinkCalc* LinkCalc=LinkCalcMng->GetCurrentMethod();

	if(!Profiling)
		throw GException("No computing method chosen.");

	Subs=prof->GetSubProfilesCursor();
	for (Subs.Start(); !Subs.End(); Subs.Next())
	{
		if(LinkCalc)
		{
			LinkCalc->Compute(Subs());
		}
		Profiling->Compute(Subs());
	}
}


//-----------------------------------------------------------------------------
void GSession::CalcPostGroup() throw(GException)
{
	GPostGroup* PostGrouping=PostGroupMng->GetCurrentMethod();
	if(!PostGrouping)
		throw GException("No postgroup method chosen.");
	PostGrouping->Compute();
}


//-----------------------------------------------------------------------------
void GSession::GroupingProfiles(GSlot* rec,bool modified,bool save)  throw(GException)
{
	GGrouping* Grouping=GroupingMng->GetCurrentMethod();

	if(!Grouping)
		throw GException("No grouping method chosen.");
	Grouping->Grouping(rec,modified,save);
}


//-----------------------------------------------------------------------------
GProfDocCursor& GSession::GetProfDocCursor(void)
{
	GProfDocCursor *cur=GProfDocCursor::GetTmpCursor();
	cur->Set(Fdbks);
	return(*cur);
}


//-----------------------------------------------------------------------------
void GSession::ClearFdbks(void)
{
	GDocCursor cur=GetDocsCursor();
	for(cur.Start();!cur.End();cur.Next())
	{
		cur()->ClearFdbks();
	}
	GProfileCursor cur2=GetProfilesCursor();
	for(cur2.Start();!cur2.End(); cur2.Next())
	{
		cur2()->ClearFdbks();
	}
	Fdbks.Clear();
}


//-----------------------------------------------------------------------------
void GSession::InsertFdbk(GProfile* p,GDoc* d,tDocAssessment j,R::RDate& date) throw(bad_alloc)
{
	GProfDoc* f;

	Fdbks.InsertPtr(f=new GProfDoc(d,p,j,date));
	p->AddAssessment(f,this);
	d->AddAssessment(f);
}


//-----------------------------------------------------------------------------
void GSession::InsertFdbk(GProfile* p,GDoc* d,tDocAssessment j,const char* date) throw(bad_alloc)
{
	GProfDoc* f;

	Fdbks.InsertPtr(f=new GProfDoc(d,p,j,date));
	p->AddAssessment(f,this);
	d->AddAssessment(f);
}


//-----------------------------------------------------------------------------
void GSession::CopyIdealGroups(void) throw(bad_alloc,GException)
{
	GGroupCursor Grps;
//	GGroupCursor Ideal;
	GSubProfileCursor Sub;
//	GGroups* grps;
	GGroup* grp;
	GGroupCalc* CalcDesc;

	// Get current grouping description method
	CalcDesc=GroupCalcMng->GetCurrentMethod();

	// Clear current group
	GGroups::Clear();

	// Go through each languages
	Grps=Subjects->GetIdealGroups()->GetGroupsCursor();
	for(Grps.Start();!Grps.End();Grps.Next())
	{
		// Create a new group in groups
		grp=new GGroupVector(cNoRef,Grps()->GetLang());
		grp->SetId(GGroups::GetMaxId()+1);
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

/*	if(save)
		Session->SaveGroups();*/
}


//-----------------------------------------------------------------------------
void GSession::Save(GGroup* grp) throw(GException)
{
	if(grp->GetState()==osUpToDate) return;
	for(grp->Start();!grp->End();grp->Next())
		SaveSubProfile((*grp)());
	if(grp->GetState()==osUpdated)
		grp->SetState(osUpToDate);
}


//-----------------------------------------------------------------------------
GFactoryFilterCursor& GSession::GetFiltersCursor(void)
{
	return(URLMng->GetFiltersCursor());
}


//-----------------------------------------------------------------------------
const char* GSession::GetMIMEType(const char* mime) const
{
	return(URLMng->GetMIMEType(mime));
}


//-----------------------------------------------------------------------------
void GSession::RunPrg(GSlot* rec,const char* filename) throw(GException)
{
	GSessionPrg Prg(filename,this,rec);
	Prg.Exec();
}


//-----------------------------------------------------------------------------
void GSession::DocsFilter(int nbdocs,int nboccurs) throw(GException)
{
	//The number of word in current lang.
	GDocCursor DocCursorTemp =GetDocsCursor();
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
	GDocCursor DocCursor=GetDocsCursor();
	for(DocCursor.Start();!DocCursor.End();DocCursor.Next())
	{
		GDocVector* Doc=dynamic_cast<GDocVector*>(DocCursor());
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
		GDocVector* Doc=dynamic_cast<GDocVector*>(DocCursor());
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


//-----------------------------------------------------------------------------
void GSession::SetCurrentRandom(int rand)
{
	CurrentRandom=rand;
	Random->Reset(CurrentRandom);
}


//-----------------------------------------------------------------------------
int GSession::GetCurrentRandomValue(unsigned int max)
{
	return(int(Random->Value(max)));

}


//-----------------------------------------------------------------------------
RContainer<GGroupsHistory, unsigned int, false,true>* GSession::LoadHistoricGroups (RContainer<GSubProfile, unsigned int, false,true>* subprofiles,GLang* lang,unsigned int mingen, unsigned int maxgen)
{
	unsigned int i;
	GGroupsHistory* hgrps;
	RContainer<GGroupsHistory, unsigned int, false,true>* historicalgroups;

	// fill the container
	historicalgroups=new RContainer<GGroupsHistory, unsigned int, false,true>(maxgen-mingen+1);
	for (i=mingen; i<maxgen+1; i++)
	{
		hgrps=LoadAnHistoricGroups(subprofiles, lang,i);
		historicalgroups->InsertPtr(hgrps);
	}

	return(historicalgroups);
}


//-----------------------------------------------------------------------------
void GSession::ReInit(bool)
{
	//clean subprofiles = clear the feedbacles & the content of the profiles.
	ClearFdbks();

	// set group to 0 for subprofiles.
	ClearSubProfilesGroups();

	// Re-Init the sims and behaviorsbetween documents and subprofiles
	ProfilesSims->ReInit();
	ProfilesBehaviours->ReInit();
	DocProfSims->ReInit();
}


//-----------------------------------------------------------------------------
GSession::~GSession(void) throw(GException)
{
	// Delete Similarities Managers
	if(DocProfSims) delete DocProfSims;
	if(ProfilesBehaviours) delete ProfilesBehaviours;
	if(ProfilesSims) delete ProfilesSims;

	// Clear all entities
	GGroups::Clear();
	GUsers::Clear();
	GDocs::Clear();

	// Disconnect from the different managers
	if(ProfilingMng) ProfilingMng->Disconnect(this);
	if(GroupingMng) GroupingMng->Disconnect(this);
	if(GroupCalcMng) GroupCalcMng->Disconnect(this);
	if(StatsCalcMng) StatsCalcMng->Disconnect(this);
	if(LinkCalcMng) LinkCalcMng->Disconnect(this);
	if(PostGroupMng) PostGroupMng->Disconnect(this);
	if(DocAnalyseMng) DocAnalyseMng->Disconnect(this);
	if(Langs) Langs->Disconnect(this);

	// Delete stuctures
	if(Random) delete Random;
	if(Subjects) delete Subjects;
}


//-----------------------------------------------------------------------------
// class GSessionParams
//-----------------------------------------------------------------------------
GSessionParams::GSessionParams(void)
	: GParams("Session Parameters")
{
	GParam* p;
	InsertPtr(p=new GParamUInt("SameBehaviourMinDocs",7));
	InsertPtr(p=new GParamUInt("DiffBehaviourMinDocs",4));
	InsertPtr(p=new GParamDouble("NullSimLevel",0.00001));
}

