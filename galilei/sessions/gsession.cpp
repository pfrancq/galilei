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
#include <rstd/random.h>
#include <rstd/rcursor.h>
#include <rstd/rtextfile.h>
using namespace R;


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <langs/glang.h>
#include <langs/gdict.h>
#include <langs/gwordlist.h>
#include <sessions/gsession.h>
#include <sessions/gslot.h>
#include <sessions/gsessionprg.h>
#include <docs/gdocvector.h>
#include <docs/gdocanalyse.h>
#include <docs/gdocxml.h>
#include <docs/gdocoptions.h>
#include <docs/gdocprofsim.h>
#include <docs/gdocprofsims.h>
#include <docs/gwordsclustering.h>
#include <docs/glinkcalc.h>
#include <docs/glink.h>
#include <profiles/guser.h>
#include <profiles/gprofile.h>
#include <profiles/gsubprofile.h>
#include <profiles/gprofilessim.h>
#include <profiles/gprofilessims.h>
#include <profiles/gprofilesbehaviour.h>
#include <profiles/gprofilesbehaviours.h>
#include <profiles/gsubprofiledesc.h>
#include <profiles/gprofdoc.h>
#include <groups/ggroups.h>
#include <groups/ggroup.h>
#include <groups/ggroupvector.h>
#include <groups/ggrouping.h>
#include <groups/ggroupingmanager.h>
#include <groups/ggroupcalc.h>
#include <profiles/gprofilecalc.h>
#include <filters/gurlmanager.h>
#include <filters/gfilter.h>
#include <infos/giwordweight.h>
#include <historic/ggroupshistory.h>
using namespace GALILEI;



//-----------------------------------------------------------------------------
//
// GSession
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GALILEI::GSession::GSession(unsigned int d,unsigned int u,unsigned int p,unsigned int f,unsigned int g,
	GURLManager* umng, GProfileCalcManager* pmng, GGroupingManager* gmng, GGroupCalcManager* gcmng,
	GDocOptions* opt, GSessionParams* sessparams) throw(bad_alloc,GException)
	: GLangs(2), GDocs(d), GUsers(u,p), GGroupsMng(g),
	  Subjects(), Fdbks(f+f/2,f/2),
	  URLMng(umng), ProfilingMng(pmng), GroupingMng(gmng), GroupCalcMng(gcmng),
	  DocAnalyse(0), bGroups(false),bFdbks(false),
	  DocOptions(opt),SessParams(sessparams)

{
	GLangCursor Langs;
	IdealGroups= new RContainer<GALILEI::GGroups, unsigned int, true, true> (g+g/2,g/2);
	IdealDocs=new RContainer<GALILEI::GGroupsEvaluate, unsigned int, false, false> (2,1);
	Langs=GetLangsCursor();
	for(Langs.Start();!Langs.End();Langs.Next())
		Groups.InsertPtr(new GGroups(Langs()));
	SubProfileDescs=new RContainer<GSubProfileDesc,unsigned int,true,true>(3,3);
	LinkCalcs=new RContainer<GLinkCalc,unsigned int,true,true>(3,2);
	DocOptions=new GDocOptions(opt);
	DocAnalyse=new GDocAnalyse(this,DocOptions);
	CurrentRandom=0;
	Random = new RRandomGood(CurrentRandom);
	ProfilingMng->Connect(this);
	GroupingMng->Connect(this);
}


//-----------------------------------------------------------------------------
void GALILEI::GSession::RegisterProfileDesc(GSubProfileDesc* grp) throw(bad_alloc)
{
	SubProfileDescs->InsertPtr(grp);
}


//-----------------------------------------------------------------------------
void GALILEI::GSession::SetCurrentProfileDesc(const char* name) throw(GException)
{
	GSubProfileDesc* tmp;

	tmp=SubProfileDescs->GetPtr<const char*>(name);
	if(!tmp)
		throw GException(RString("Description method '")+name+"' doesn't exists.");
	SubProfileDesc=tmp;
}


//-----------------------------------------------------------------------------
GSubProfileDescCursor& GALILEI::GSession::GetProfileDescsCursor(void)
{
	GSubProfileDescCursor *cur=GSubProfileDescCursor::GetTmpCursor();
	cur->Set(SubProfileDescs);
	return(*cur);
}

//-----------------------------------------------------------------------------
R::RContainer<GGroups,unsigned int,true,true>* GALILEI::GSession::GetIdealGroups(void)
{
	return(IdealGroups);
}


//-----------------------------------------------------------------------------
GGroupsCursor& GALILEI::GSession::GetIdealGroupsCursor(void)
{
	GGroupsCursor *cur=GGroupsCursor::GetTmpCursor();
	cur->Set(IdealGroups);
	return(*cur);
}


//-----------------------------------------------------------------------------
R::RContainer<GGroupsEvaluate,unsigned int,false,false>* GALILEI::GSession::GetIdealDocs(void)
{
	return(IdealDocs);
}


//-----------------------------------------------------------------------------
GGroupsEvaluateCursor& GALILEI::GSession::GetIdealDocsCursor(void)
{
	GGroupsEvaluateCursor *cur=GGroupsEvaluateCursor::GetTmpCursor();
	cur->Set(IdealDocs);
	return(*cur);
}


//-----------------------------------------------------------------------------
void GALILEI::GSession::RegisterLinkCalcMethod(GLinkCalc* lnk) throw(bad_alloc)
{
	LinkCalcs->InsertPtr(lnk);
}


//-----------------------------------------------------------------------------
void GALILEI::GSession::SetCurrentLinkCalcMethod(const char* name) throw(GException)
{
	GLinkCalc* tmp;

	tmp=LinkCalcs->GetPtr<const char*>(name);
	if(!tmp)
		throw GException(RString("Link Description method '")+name+"' doesn't exists.");
	LinkCalc=tmp;
}


//-----------------------------------------------------------------------------
void GALILEI::GSession::SetCurrentLinkCalcMethodSettings(const char* s) throw(GException)
{
	if((!LinkCalc)||(!(*s))) return;
	LinkCalc->SetSettings(s);
}


//-----------------------------------------------------------------------------
const char* GALILEI::GSession::GetLinkCalcMethodSettings(const char* n) throw(GException)
{
	GLinkCalc* tmp;

	tmp=LinkCalcs->GetPtr<const char*>(n);
	if(!tmp)
		return(0);
	return(tmp->GetSettings());
}


//-----------------------------------------------------------------------------
GLinkCalcCursor& GALILEI::GSession::GetLinkCalcsCursor(void)
{
	GLinkCalcCursor *cur=GLinkCalcCursor::GetTmpCursor();
	cur->Set(LinkCalcs);
	return(*cur);
}


//-----------------------------------------------------------------------------
GDocXML* GALILEI::GSession::CreateDocXML(GDoc* doc) throw(GException)
{
	return(URLMng->CreateDocXML(doc));
}


//-----------------------------------------------------------------------------
void GALILEI::GSession::SetCurrentWordsClusteringMethodSettings(unsigned minocc,unsigned s,double c,unsigned ndm)
{
	DocOptions->MinOccurCluster=minocc;
	DocOptions->WindowSize=s;
	DocOptions->MinConfidence=c;
	DocOptions->NbDocsMin=ndm;
}


//-----------------------------------------------------------------------------
void GALILEI::GSession::AnalyseAssociation(bool save)
{
	GDocCursor Docs=GetDocsCursor();
	GWordsClustering* test;
	unsigned int i,n=1;
	bool end=false;
	Docs.Start();
	test= new GWordsClustering(GetDic(Docs()->GetLang()),DocOptions->MinDocs,DocOptions->MaxDocs,DocOptions->MinOccurCluster,DocOptions->WindowSize,DocOptions->MinConfidence,DocOptions->NbDocsMin,GetDbName());
	for(i=Docs.GetNb(),Docs.Start();i--;Docs.Next())
	{
		test->AddDoc(dynamic_cast<GDocVector*>(Docs()));
	}
	test->CleanWords();

	// Teste de la nouvelle mthode
	if(DocOptions->Distance)
	{
		cout<<"warning : Use of distance in words clustering"<<endl;
		test->InsertNeighbor();
		test->ConceptSpace();
		test->SaveConceptSpace(save);
	}
	else
	{
		for(i=0;i<DocOptions->NbIteration;i++)
		{
		  end=test->OrdreByDocs(i);
		}
		test->SaveAssociations(save);
	}
	test->View();
	Docs.Start();
	if(save)
	{
		SaveWordsGroups(GetDic(Docs()->GetLang()));
	}
	GetDic(Docs()->GetLang())->GroupsList.Start();
	if (GetDic(Docs()->GetLang())->GroupsList.NbPtr!=0)
	{
		n=(GetDic(Docs()->GetLang())->GroupsList)()->GetId();
		for(i=Docs.GetNb(),Docs.Start();--i;Docs.Next())
		{
			test->UpdateDocbis(dynamic_cast<GDocVector*>(Docs()));
			if(save) SaveUpDatedDoc(Docs(),n);/*n=id du premier mot a sauver.*/
		}
	}


/*	if(DocOptions->Distance)
	{
		cout<<"warning : Use of distance in words clustering"<<endl;
		test->InsertNeighbor();
		test->OrderByNeighbor();
	}
	else
	{
		for(i=0;i<DocOptions->NbIteration;i++)
		{
		  end=test->OrdreByDocs(i);
		}
	}
	Docs.Start();
	test->SaveAssociations(save);
	if(save) SaveWordsGroups(GetDic(Docs()->GetLang()));
	GetDic(Docs()->GetLang())->GroupsList.Start();
	if (GetDic(Docs()->GetLang())->GroupsList.NbPtr!=0)
	{
		n=(GetDic(Docs()->GetLang())->GroupsList)()->GetId();
		for(i=Docs.GetNb(),Docs.Start();--i;Docs.Next())
		{
			test->UpdateDoc(dynamic_cast<GDocVector*>(Docs()));
			if(save) SaveUpDatedDoc(Docs(),n);/*n=id du premier mot a sauver.*//*
		}
	}
	test->View();*/
	delete(test);
}


//-----------------------------------------------------------------------------
void GALILEI::GSession::RemoveAssociation()
{
	GDocCursor Docs=GetDocsCursor();
	GWordsClustering* test;
	unsigned int i,n=1;
	Docs.Start();
	test= new GWordsClustering(GetDic(Docs()->GetLang()),DocOptions->MinDocs,DocOptions->MaxDocs,DocOptions->MinOccurCluster,DocOptions->WindowSize,DocOptions->MinConfidence,DocOptions->NbDocsMin,GetDbName());
	for(i=Docs.GetNb(),Docs.Start();i--;Docs.Next())
	{
		test->AddDoc(dynamic_cast<GDocVector*>(Docs()));
	}
	Docs.Start();
	GetDic(Docs()->GetLang())->GroupsList.Start();
	if (GetDic(Docs()->GetLang())->GroupsList.NbPtr!=0)
  {
  	n=(GetDic(Docs()->GetLang())->GroupsList)()->GetId();
  	for(i=Docs.GetNb(),Docs.Start();--i;Docs.Next())
  	{
  		test->ReverseUpdateDoc(dynamic_cast<GDocVector*>(Docs()));
  		SaveUpDatedDoc(Docs(),n);/*n=id du premier mot a sauver.*/
  	}
  }
	DeleteWordsGroups(GetDic(Docs()->GetLang()));
	delete(test);
}


//-----------------------------------------------------------------------------
void GALILEI::GSession::Test()
{
}


//-----------------------------------------------------------------------------
void GALILEI::GSession::AnalyseDocs(GSlot* rec,bool modified) throw(GException)
{
	bool undefLang;
	GDocXML* xml=0;
	GDocCursor Docs=GetDocsCursor();
	RContainer<GDoc,unsigned int,false,true>* tmpDocs = new RContainer<GDoc,unsigned int,false,true>(5,2);

	for(Docs.Start();!Docs.End();Docs.Next())
	{
		if(modified&&(Docs()->GetState()==osUpToDate)) continue;
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
					DocAnalyse->Analyse(xml,Docs(),tmpDocs);
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
void GALILEI::GSession::InitUsers(bool wg,bool w) throw(bad_alloc,GException)
{
	// If users already loaded, do nothing.
	if(IsUsersLoad()) return;

	// Initialise groups.
	if(!bGroups)
		InitGroups(false,true);

	// Load the users
	LoadUsers(wg,w);
	bUsers=true;

	// Initialise the profiles sims
	InitProfilesSims();

	// Initilaise the profiles behaviour.
	InitProfilesBehaviours();

	// Initialise the sims between documents and subprofiles
	InitDocProfSims();  
}


//-----------------------------------------------------------------------------
void GALILEI::GSession::InitDocProfSims(void)
{
	GLangCursor langs = GetLangsCursor();
	RContainer<GSubProfile,unsigned int,false,true>* subProf;
	GDocProfSim* docProfSim;

	DocProfSims = new GDocProfSims(100);

	for(langs.Start();!langs.End(); langs.Next())                                                     
	{
		subProf = new RContainer<GSubProfile,unsigned int, false,true>(100,50);
		GSubProfileCursor subProfCur = GetSubProfilesCursor(langs());

		for(subProfCur.Start();!subProfCur.End();subProfCur.Next())
		{
			subProf->InsertPtr( subProfCur());
		}
		docProfSim= new GDocProfSim(this,subProf, false,langs());
		DocProfSims->InsertPtr(docProfSim);
	}
}


//-----------------------------------------------------------------------------
void GALILEI::GSession::ChangeDocProfState(bool global,GLang* lang)throw(bad_alloc)
{
	GDocProfSim* docProfSim = DocProfSims->GetPtr<GLang*>(lang);
	docProfSim->UpdateDocProfSim(this,this, global);
}


//-----------------------------------------------------------------------------
void GALILEI::GSession::ChangeAllDocProfState(bool global)throw(bad_alloc)
{
	GLangCursor langs = GetLangsCursor();
	for (langs.Start();!langs.End();langs.Next())
	{
		ChangeDocProfState(global,langs());
	}
}


//-----------------------------------------------------------------------------
double GALILEI::GSession::GetSimDocProf(GLang* l,unsigned int id_doc, unsigned int id_sub)
{
	GDocProfSim* docProfSim = DocProfSims->GetPtr<GLang*>(l);
	return docProfSim->GetSim(this,this,id_doc,id_sub);
}


//-----------------------------------------------------------------------------
double GALILEI::GSession::GetSimDocProf(const GDoc* doc,const GSubProfile* sub)
{
	GDocProfSim* docProfSim = DocProfSims->GetPtr<GLang*>(doc->GetLang());
	return docProfSim->GetSim(doc,sub);
}


//-----------------------------------------------------------------------------
void GALILEI::GSession::InitProfilesSims(void) 
{
	GProfilesSim* profSim;
	RContainer<GSubProfile,unsigned int,false,true>* subProfs;
	GLangCursor langs = GetLangsCursor();

	//init a profilsims contianing a profilesim for each langs
	ProfilesSims = new GProfilesSims(langs.GetNb());

	//for each languages
	for(langs.Start();!langs.End(); langs.Next())
	{
		subProfs = new RContainer<GSubProfile,unsigned int, false,true>(100,50);
		GSubProfileCursor subProfCur = GetSubProfilesCursor(langs());

		for(subProfCur.Start();!subProfCur.End();subProfCur.Next())
		{
			subProfs->InsertPtr( subProfCur());
		}
		//init the profsim with a global=false (can be 'true').
		profSim= new GProfilesSim(subProfs, false,langs());
		// insert the profsim in the container of profsims
		ProfilesSims->InsertPtr(profSim);
	}
}


//-----------------------------------------------------------------------------
void GALILEI::GSession::ChangeProfilesSimState(bool global,GLang* lang)throw(bad_alloc)
{
	GProfilesSim* profSim;

 	profSim= ProfilesSims->GetPtr<GLang*>(lang);
	profSim->UpdateProfSim(global);

}


//-----------------------------------------------------------------------------
void GALILEI::GSession::ChangeAllProfilesSimState(bool global)throw(bad_alloc)
{
	GLangCursor langs = GetLangsCursor();
	for( langs.Start(); !langs.End(); langs.Next())
	{
		ChangeProfilesSimState(global,langs());
	}
}


//-----------------------------------------------------------------------------
double GALILEI::GSession::GetSimProf(const GSubProfile* sub1,const GSubProfile* sub2)
{
	GProfilesSim* profSim;

	profSim= ProfilesSims->GetPtr<const GLang*>(sub1->GetLang());
	if((!profSim)||(sub1->GetLang()!=sub2->GetLang()))
		return(0.0);
	return(profSim->GetSim(sub1,sub2));
}


//-----------------------------------------------------------------------------
void GALILEI::GSession::InitProfilesBehaviours(void)
{
	GLangCursor langs = GetLangsCursor();

	RContainer<GSubProfile,unsigned int,false,true>* subProfs;
	GProfilesBehaviour* profBehaviour;

	ProfilesBehaviours = new GProfilesBehaviours(5);

	for(langs.Start();!langs.End(); langs.Next())
	{
		subProfs = new RContainer<GSubProfile,unsigned int, false,true>(100,50);
		GSubProfileCursor subProfCur = GetSubProfilesCursor(langs());

		for(subProfCur.Start();!subProfCur.End();subProfCur.Next())
		{
			subProfs->InsertPtr(subProfCur());
		}
		profBehaviour= new GProfilesBehaviour(subProfs,langs(),SessParams->GetUInt("SameBehaviourMinDocs"), SessParams->GetUInt("DiffBehaviourMinDocs"));
		ProfilesBehaviours->InsertPtr(profBehaviour);
	}
}


//-----------------------------------------------------------------------------
void GALILEI::GSession::ChangeProfilesBehaviourState(GLang* lang)throw(bad_alloc)
{
	GProfilesBehaviour* profBehaviour = ProfilesBehaviours->GetPtr<const GLang*>(lang);
	profBehaviour->UpdateProfBehaviour();

}


//-----------------------------------------------------------------------------
void GALILEI::GSession::ChangeAllProfilesBehaviourState(void) throw(bad_alloc)
{
	GLangCursor langs = GetLangsCursor();
	for( langs.Start(); !langs.End(); langs.Next())
	{
		ChangeProfilesBehaviourState(langs());
	}
}


//-----------------------------------------------------------------------------
double GALILEI::GSession::GetAgreementRatio(GSubProfile* sub1,GSubProfile* sub2)
{
	GProfilesBehaviour* profBehaviour = ProfilesBehaviours->GetPtr<const GLang*>(sub1->GetLang());
	return(profBehaviour->GetAgreementRatio(sub1,sub2));
}


//-----------------------------------------------------------------------------
double GALILEI::GSession::GetDisagreementRatio(GSubProfile* sub1,GSubProfile* sub2)
{
	GProfilesBehaviour* profBehaviour = ProfilesBehaviours->GetPtr<const GLang*>(sub1->GetLang());
	return(profBehaviour->GetDisAgreementRatio(sub1,sub2));
}


//-----------------------------------------------------------------------------
double GALILEI::GSession::GetMinimumOfSimilarity(R::RContainer<GSubProfile,unsigned int,false,true>* subprofiles, double deviationrate)
{
	double meanSim;
	double deviation;
	double minSim;
        
	subprofiles->Start();
	GProfilesSim* profSim = ProfilesSims->GetPtr<const GLang*>((*subprofiles)()->GetLang());
	profSim->UpdateDeviationAndMeanSim(subprofiles);
	meanSim=profSim->GetMeanSim();
	deviation=profSim->GetDeviation();
	minSim=meanSim+deviationrate*sqrt(deviation);

	return(minSim);
}



//-----------------------------------------------------------------------------
GUser* GALILEI::GSession::NewUser(const char* /*usr*/,const char* /*pwd*/,const char* /*name*/,const char* /*email*/,
	                  const char* /*title*/,const char* /*org*/,const char* /*addr1*/,
	                  const char* /*addr2*/,const char* /*city*/,const char* /*country*/) throw(bad_alloc)
{
	return(0);
}


//-----------------------------------------------------------------------------
void GALILEI::GSession::InitLinks()
{
	if(!LinkCalc)
		throw GException("No Link computing method chosen.");

	LinkCalc->InitGraph();
}

//-----------------------------------------------------------------------------
void GALILEI::GSession::CalcProfiles(GSlot* rec,bool modified,bool save) throw(GException)
{
	GSubProfileCursor Subs;
	GSubProfile* sub;
	GProfileCursor Prof=GetProfilesCursor();
	GProfilesSim* profSim;
	GProfilesBehaviour* profBehaviour;
	GProfileCalc* Profiling=ProfilingMng->GetCurrentMethod();

	if(!Profiling)
		throw GException("No computing method chosen.");
	if(!LinkCalc)
		throw GException("No Link computing method chosen.");

	for(Prof.Start();!Prof.End();Prof.Next())
	{
		rec->receiveNextProfile(Prof());
		Subs=Prof()->GetSubProfilesCursor();
		for (Subs.Start(); !Subs.End(); Subs.Next())
		{
			sub=(Subs)();
			if(modified&&(Subs()->GetState()==osUpToDate)) continue;
			try
			{
				if((!modified)||(Subs()->GetState()!=osUpdated))
				{
					if (DocOptions->UseLink)
						LinkCalc->Compute(Subs());
					Profiling->Compute(Subs());
					// add the mofified profile to the list of modified profiles  for similarities
					profSim = ProfilesSims->GetPtr<GLang*>(Subs()->GetLang());
					profSim->AddModifiedProfile(Subs()->GetProfile()->GetId());
					// add the mofified profile to the list of modified profiles  for behaviours
					profBehaviour = ProfilesBehaviours->GetPtr<GLang*>(Subs()->GetLang());
					profBehaviour->AddModifiedProfile(Subs()->GetProfile()->GetId());	
				}
			}
			catch(GException& e)
			{
			}
		}
	}

	// update the state of all the sims. 
	ChangeAllProfilesSimState(true);   /// !!!!  check if 'true' is the correct value !?!

	//updtae the state of the behaviours
	ChangeAllProfilesBehaviourState();

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
void GALILEI::GSession::CalcProfile(GProfile* prof) throw(GException)
{
	GSubProfileCursor Subs;
	GProfileCalc* Profiling=ProfilingMng->GetCurrentMethod();

	if(!Profiling)
		throw GException("No computing method chosen.");
	if(!LinkCalc)
		throw GException("No Link computing method chosen.");

	Subs=prof->GetSubProfilesCursor();
	for (Subs.Start(); !Subs.End(); Subs.Next())
		if (DocOptions->UseLink)
		{
			LinkCalc->Compute(Subs());
		}
		Profiling->Compute(Subs());
}


//-----------------------------------------------------------------------------
void GALILEI::GSession::GroupingProfiles(GSlot* rec,bool modified,bool save)  throw(GException)
{
	GGrouping* Grouping=GroupingMng->GetCurrentMethod();

	if(!Grouping)
		throw GException("No grouping method chosen.");
	Grouping->Grouping(rec,modified,save);
}


//-----------------------------------------------------------------------------
void GALILEI::GSession::InitFdbks(void) throw(bad_alloc,GException)
{
	// If users' feedback already loaded, do nothing.
	if(bFdbks) return;

	// Verify that users and docs are loaded
	if(!IsUsersLoad())
		InitUsers(false,true);
	InitDocs(false,true);

	// Load the users
	LoadFdbks();
	bFdbks=true;
}


//-----------------------------------------------------------------------------
GProfDocCursor& GALILEI::GSession::GetProfDocCursor(void)
{
	GProfDocCursor *cur=GProfDocCursor::GetTmpCursor();
	cur->Set(Fdbks);
	return(*cur);
}


//-----------------------------------------------------------------------------
void GALILEI::GSession::ClearFdbks(void)
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
void GALILEI::GSession::InsertFdbk(GProfile* p,GDoc* d,tDocJudgement j,R::RDate date) throw(bad_alloc)
{
	GProfDoc* f;

	Fdbks.InsertPtr(f=new GProfDoc(d,p,j,date));
	p->AddJudgement(f,this);
	d->AddJudgement(f);
}


//-----------------------------------------------------------------------------
void GALILEI::GSession::InitGroups(bool wg,bool w) throw(bad_alloc,GException)
{
	// If groups already loaded, do nothing.
	if(bGroups) return;

	// Load the users
	LoadGroups(wg,w);
	bGroups=true;
}


//-----------------------------------------------------------------------------
void GALILEI::GSession::CopyIdealGroups(void) throw(bad_alloc,GException)
{
	GGroupsCursor Grps;
	GGroupCursor Ideal;
	GSubProfileCursor Sub;
	GGroups* grps;
	GGroup* grp;
	GGroupCalc* CalcDesc;

	// Get current grouping description method
	CalcDesc=GroupCalcMng->GetCurrentMethod();

	// Go through each languages
	Grps=GetIdealGroupsCursor();
	for(Grps.Start();!Grps.End();Grps.Next())
	{
		// Find corresponding groups and clear it
		grps=Groups.GetPtr<const GLang*>(Grps()->GetLang());
		grps->Clear();

		// Go trough each group
		Ideal=Grps()->GetGroupCursor();
		for(Ideal.Start();!Ideal.End();Ideal.Next())
		{
			// Create a new group in groups
			grp=new GGroupVector(cNoRef,Grps()->GetLang());
			NewGroup(Grps()->GetLang(),grp);
			grps->InsertPtr(grp);

			// Go through each subprofile
			Sub=Ideal()->GetSubProfileCursor();
			for(Sub.Start();!Sub.End();Sub.Next())
			{
				grp->InsertSubProfile(Sub());
			}

			// Compute Description
			CalcDesc->Compute(grp);
		}
	}
}


//-----------------------------------------------------------------------------
void GALILEI::GSession::Save(GGroup* grp) throw(GException)
{
	if(grp->GetState()==osUpToDate) return;
	for(grp->Start();!grp->End();grp->Next())
		SaveSubProfile((*grp)());
	if(grp->GetState()==osUpdated)
		grp->SetState(osUpToDate);
}


//-----------------------------------------------------------------------------
GFactoryFilterCursor& GALILEI::GSession::GetFiltersCursor(void)
{
	return(URLMng->GetFiltersCursor());
}


//-----------------------------------------------------------------------------
const char* GALILEI::GSession::GetMIMEType(const char* mime) const
{
	return(URLMng->GetMIMEType(mime));
}


//-----------------------------------------------------------------------------
void GALILEI::GSession::RunPrg(GSlot* rec,const char* filename) throw(GException)
{
	GSessionPrg Prg(filename,this,rec);
	Prg.Exec();
}


//-----------------------------------------------------------------------------
void GALILEI::GSession::DocsFilter(int nbdocs,int nboccurs) throw(GException)
{
	//The number of word in current lang.
	GDocCursor DocCursorTemp =GetDocsCursor();
	DocCursorTemp.Start();
	
	int NbKwd;
	GDict* CurDic=GetDic(DocCursorTemp()->GetLang());
	NbKwd=CurDic->GetMaxId();

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
			GIWordWeight* WW=(*Doc)();
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
	// cout<<compt<<" nbre de mots restants"<<endl;

	for(DocCursor.Start();!DocCursor.End();DocCursor.Next())
	{
		GDocVector* Doc=dynamic_cast<GDocVector*>(DocCursor());
		for(int i=0;i<NbKwd;i++)
		{
			if(!test[i])
			{
				GIWordWeight* WW=Doc->GetPtr<const unsigned int>(i);
				if(WW) Doc->DeletePtr(WW);
			}
		}
	}
	delete[] j;
	delete[] k;
	delete[] test;
}


//-----------------------------------------------------------------------------
void GALILEI::GSession::SetCurrentRandom(int rand)
{
	CurrentRandom=rand;
	Random->Reset(CurrentRandom);
}


//-----------------------------------------------------------------------------
int GALILEI::GSession::GetCurrentRandomValue(unsigned int max)
{
	return(int(Random->Value(max)));

}


//-----------------------------------------------------------------------------
RContainer<GGroupsHistory, unsigned int, false,true>* GALILEI::GSession::LoadHistoricGroups (RContainer<GSubProfile, unsigned int, false,true>* subprofiles,GLang* lang,unsigned int mingen, unsigned int maxgen)
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
void GALILEI::GSession::ReInit(bool)
{
	//clean subprofiles = clear the feedbacles & the content of the profiles.
	ClearFdbks();

	// clean similarities between profiles
	delete(ProfilesSims);

	// clean similarities between documents and subprofiles
	delete(DocProfSims);

	// set group to 0 for subprofiles.
	ClearSubProfilesGroups();

	//re-init similarities between profiles;
	InitProfilesSims();

	//re-init the behaviours inter-profiles.
	InitProfilesBehaviours();

	// re-Init the sims between documents and subprofiles
	InitDocProfSims();  
}


//-----------------------------------------------------------------------------
GALILEI::GSession::~GSession(void) throw(GException)
{
	ProfilingMng->Disconnect(this);
	GroupingMng->Disconnect(this);
	if(DocAnalyse) delete DocAnalyse;
	if(DocOptions) delete DocOptions;
	if(SubProfileDescs) delete SubProfileDescs;
	if(IdealGroups) delete IdealGroups;
	if(IdealDocs) delete IdealDocs;
}


//-----------------------------------------------------------------------------
// class GSessionParams
//-----------------------------------------------------------------------------
GALILEI::GSessionParams::GSessionParams(void)
	: GParams("Session Parameters")
{
	GParam* p;
	InsertPtr(p=new GParamUInt("SameBehaviourMinDocs"));
	InsertPtr(p=new GParamUInt("DiffBehaviourMinDocs"));
}

