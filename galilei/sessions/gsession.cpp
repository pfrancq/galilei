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
#include <infos/glang.h>
#include <infos/glangs.h>
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
#include <docs/gdocslang.h>
#include <docs/gdocprofsim.h>
#include <docs/gdocprofsims.h>
#include <docs/glinkcalc.h>
#include <docs/glinkcalcmanager.h>
#include <docs/glink.h>
#include <docs/gpostdoc.h>
#include <docs/gpostdocmanager.h>
#include <profiles/guser.h>
#include <profiles/gprofile.h>
#include <profiles/gsubprofile.h>
#include <profiles/gprofilessim.h>
#include <profiles/gprofilessims.h>
#include <profiles/gprofilesbehaviour.h>
#include <profiles/gprofilesbehaviours.h>
#include <profiles/gsubprofiledesc.h>
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
#include <groups/gsubjecttree.h>
#include <filters/gurlmanager.h>
#include <filters/gfilter.h>
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
	: GDocs(d), GUsers(u,p), GGroupsMng(g),
	  Subjects(0), Fdbks(f+f/2,f/2),
	  Langs(0), URLMng(0), ProfilingMng(0), GroupingMng(0), GroupCalcMng(0),
	  StatsCalcMng(0), LinkCalcMng(0), PostGroupMng(0), PostDocMng(0),
	  bGroups(false),bFdbks(false), Random(0),
	  SessParams(sessparams)
{
	// Init Part
	SubProfileDescs=new RContainer<GSubProfileDesc,unsigned int,true,true>(3,3);
	CurrentRandom=0;
	Random = new RRandomGood(CurrentRandom);

	// Only Vector Space
	SubProfileDesc=new GSubProfileDesc("Vector space",GSubProfileVector::NewSubProfile);
	SubProfileDescs->InsertPtr(SubProfileDesc);

	// Create SubjectTree
	if(tests)
		Subjects=new GSubjectTree(this);
}


//-----------------------------------------------------------------------------
void GSession::Connect(GLangs* langs,GURLManager* umng, GDocAnalyseManager* dmng, GProfileCalcManager* pmng,
	GGroupingManager* gmng, GGroupCalcManager* gcmng,GStatsCalcManager* smng,
	GPostDocManager* pdmng,GPostGroupManager* pgmng) throw(bad_alloc,GException)

{
	GLang* lang;

	Langs=langs;
	if(Langs)
	{
		Langs->Connect(this);
		GFactoryLangCursor Cur;
		Cur=Langs->GetLangsCursor();
		for(Cur.Start();!Cur.End();Cur.Next())
		{
			lang=Cur()->GetPlugin();
			if(!lang) continue;
			Groups.InsertPtr(new GGroups(lang));
		}
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
void GSession::AnalyseAssociation(bool save)
{
	GPostDoc* PostDoc=PostDocMng->GetCurrentMethod();
	if(!PostDoc)
		throw GException("No computing method chosen.");
	else
		PostDoc->Compute(save);
}


//-----------------------------------------------------------------------------
void GSession::RemoveAssociation()
{
	GDocCursor docs=GetDocsCursor();
	GDocVector* doc;
	unsigned int i,n;
	docs.Start();
	GDict* dic=docs()->GetLang()->GetDict();
	dic->GroupsList.Start();
	n=(dic->GroupsList)()->GetId();

	for(i=docs.GetNb(),docs.Start();--i;docs.Next())
	{
		doc=dynamic_cast<GDocVector*>(docs());
		for(dic->GroupsList.Start();!dic->GroupsList.End();dic->GroupsList.Next())
		{
			if(doc->IsIn((dic->GroupsList)()->GetId()))
				doc->DeletePtr((dic->GroupsList)()->GetId());
		}
		doc->UpdateRefs();
		SaveUpDatedDoc(doc,n);/*n=First id to save.*/
	}
	DeleteWordsGroups(dic);
}


//-----------------------------------------------------------------------------
void GSession::AnalyseDocs(GSlot* rec,bool modified) throw(GException)
{
	bool undefLang;
	GDocXML* xml=0;
	GDocCursor Docs=GetDocsCursor();
	RContainer<GDoc,unsigned int,false,true>* tmpDocs = new RContainer<GDoc,unsigned int,false,true>(5,2);
	GDocAnalyse* Analyse;

	Analyse=DocAnalyseMng->GetCurrentMethod();
	if(!Analyse)
		throw GException("No document analysis method chosen.");
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
void GSession::InitUsers(bool wg,bool w) throw(bad_alloc,GException)
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
void GSession::InitDocProfSims(void)
{
	GFactoryLangCursor langs;
	GLang* lang;
	RContainer<GSubProfile,unsigned int,false,true>* subProf;
	GDocProfSim* docProfSim;

	DocProfSims = new GDocProfSims(100);

	langs= Langs->GetLangsCursor();
	for(langs.Start();!langs.End(); langs.Next())
	{
		lang=langs()->GetPlugin();
		if(!lang) continue;
		subProf = new RContainer<GSubProfile,unsigned int, false,true>(100,50);
		GSubProfileCursor subProfCur = GetSubProfilesCursor(lang);

		for(subProfCur.Start();!subProfCur.End();subProfCur.Next())
		{
			subProf->InsertPtr( subProfCur());
		}
		docProfSim= new GDocProfSim(this,subProf, false,lang);
		DocProfSims->InsertPtr(docProfSim);
	}
}


//-----------------------------------------------------------------------------
void GSession::ChangeDocProfState(bool global,GLang* lang)throw(bad_alloc)
{
	GDocProfSim* docProfSim = DocProfSims->GetPtr<GLang*>(lang);
	docProfSim->UpdateDocProfSim(this,this, global);
}


//-----------------------------------------------------------------------------
void GSession::ChangeAllDocProfState(bool global)throw(bad_alloc)
{
	GFactoryLangCursor langs;
	GLang* lang;

	langs = Langs->GetLangsCursor();
	for(langs.Start();!langs.End();langs.Next())
	{
		lang=langs()->GetPlugin();
		if(!lang) continue;
		ChangeDocProfState(global,lang);
	}
}


//-----------------------------------------------------------------------------
double GSession::GetSimDocProf(GLang* l,unsigned int id_doc, unsigned int id_sub)
{
	GDocProfSim* docProfSim = DocProfSims->GetPtr<GLang*>(l);
	return docProfSim->GetSim(this,this,id_doc,id_sub);
}


//-----------------------------------------------------------------------------
double GSession::GetSimDocProf(const GDoc* doc,const GSubProfile* sub)
{
	GDocProfSim* docProfSim = DocProfSims->GetPtr<GLang*>(doc->GetLang());
	return docProfSim->GetSim(doc,sub);
}


//-----------------------------------------------------------------------------
void GSession::InitProfilesSims(void) 
{
	GProfilesSim* profSim;
	RContainer<GSubProfile,unsigned int,false,true>* subProfs;
	GFactoryLangCursor langs = Langs->GetLangsCursor();
	GLang* lang;

	//init a profilsims contianing a profilesim for each langs
	ProfilesSims = new GProfilesSims(langs.GetNb());

	//for each languages
	for(langs.Start();!langs.End(); langs.Next())
	{
		lang=langs()->GetPlugin();
		if(!lang) continue;
		subProfs = new RContainer<GSubProfile,unsigned int, false,true>(100,50);
		GSubProfileCursor subProfCur = GetSubProfilesCursor(lang);

		for(subProfCur.Start();!subProfCur.End();subProfCur.Next())
		{
			subProfs->InsertPtr( subProfCur());
		}
		//init the profsim with a global=false (can be 'true').
		profSim= new GProfilesSim(subProfs, false,lang);
		// insert the profsim in the container of profsims
		ProfilesSims->InsertPtr(profSim);
	}
}


//-----------------------------------------------------------------------------
void GSession::ChangeProfilesSimState(bool global,GLang* lang)throw(bad_alloc)
{
	GProfilesSim* profSim;

 	profSim= ProfilesSims->GetPtr<GLang*>(lang);
	profSim->UpdateProfSim(global);

}


//-----------------------------------------------------------------------------
void GSession::ChangeAllProfilesSimState(bool global)throw(bad_alloc)
{
	GFactoryLangCursor langs = Langs->GetLangsCursor();
	GLang* lang;

	for( langs.Start(); !langs.End(); langs.Next())
	{
		lang=langs()->GetPlugin();
		if(!lang) continue;
		ChangeProfilesSimState(global,lang);
	}
}


//-----------------------------------------------------------------------------
double GSession::GetSimProf(const GSubProfile* sub1,const GSubProfile* sub2)
{
	GProfilesSim* profSim;

	profSim= ProfilesSims->GetPtr<const GLang*>(sub1->GetLang());
	if((!profSim)||(sub1->GetLang()!=sub2->GetLang()))
		return(0.0);
	return(profSim->GetSim(sub1,sub2));
}


//-----------------------------------------------------------------------------
void GSession::InitProfilesBehaviours(void)
{
	GFactoryLangCursor langs = Langs->GetLangsCursor();
	GLang* lang;

	RContainer<GSubProfile,unsigned int,false,true>* subProfs;
	GProfilesBehaviour* profBehaviour;

	ProfilesBehaviours = new GProfilesBehaviours(5);

	for(langs.Start();!langs.End(); langs.Next())
	{
		lang=langs()->GetPlugin();
		if(!lang) continue;
		subProfs = new RContainer<GSubProfile,unsigned int, false,true>(100,50);
		GSubProfileCursor subProfCur = GetSubProfilesCursor(lang);

		for(subProfCur.Start();!subProfCur.End();subProfCur.Next())
		{
			subProfs->InsertPtr(subProfCur());
		}
		profBehaviour= new GProfilesBehaviour(subProfs,lang,SessParams->GetUInt("SameBehaviourMinDocs"), SessParams->GetUInt("DiffBehaviourMinDocs"));
		ProfilesBehaviours->InsertPtr(profBehaviour);
	}
}


//-----------------------------------------------------------------------------
void GSession::ChangeProfilesBehaviourState(GLang* lang)throw(bad_alloc)
{
	GProfilesBehaviour* profBehaviour = ProfilesBehaviours->GetPtr<const GLang*>(lang);
	profBehaviour->UpdateProfBehaviour();

}


//-----------------------------------------------------------------------------
void GSession::ChangeAllProfilesBehaviourState(void) throw(bad_alloc)
{
	GFactoryLangCursor langs = Langs->GetLangsCursor();
	GLang* lang;

	for( langs.Start(); !langs.End(); langs.Next())
	{
		lang=langs()->GetPlugin();
		if(!lang) continue;
		ChangeProfilesBehaviourState(lang);
	}
}


//-----------------------------------------------------------------------------
double GSession::GetAgreementRatio(GSubProfile* sub1,GSubProfile* sub2)
{
	GProfilesBehaviour* profBehaviour = ProfilesBehaviours->GetPtr<const GLang*>(sub1->GetLang());
	return(profBehaviour->GetAgreementRatio(sub1,sub2));
}


//-----------------------------------------------------------------------------
double GSession::GetDisagreementRatio(GSubProfile* sub1,GSubProfile* sub2)
{
	GProfilesBehaviour* profBehaviour = ProfilesBehaviours->GetPtr<const GLang*>(sub1->GetLang());
	return(profBehaviour->GetDisAgreementRatio(sub1,sub2));
}


//-----------------------------------------------------------------------------
double GSession::GetMinimumOfSimilarity(R::RContainer<GSubProfile,unsigned int,false,true>* subprofiles, double deviationrate)
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
	GProfilesSim* profSim;
	GProfilesBehaviour* profBehaviour;
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
			sub=(Subs)();
			if(modified&&(Subs()->GetState()==osUpToDate)) continue;
			try
			{
				if((!modified)||(Subs()->GetState()!=osUpdated))
				{
					if(LinkCalc)
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
void GSession::CalcProfile(GProfile* prof) throw(GException)
{
	GSubProfileCursor Subs;
	GProfileCalc* Profiling=ProfilingMng->GetCurrentMethod();
	GLinkCalc* LinkCalc=LinkCalcMng->GetCurrentMethod();

	if(!Profiling)
		throw GException("No computing method chosen.");

	Subs=prof->GetSubProfilesCursor();
	for (Subs.Start(); !Subs.End(); Subs.Next())
		if(LinkCalc)
		{
			LinkCalc->Compute(Subs());
		}
		Profiling->Compute(Subs());
}


//-----------------------------------------------------------------------------
void GSession::CalcPostGroup() throw(GException)
{
	GPostGroup* PostGrouping=PostGroupMng->GetCurrentMethod();
	if(!PostGrouping)
		throw GException("No postgroup method chosen.");
	cout << "postgroup method found "<<endl;
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
void GSession::InitFdbks(void) throw(bad_alloc,GException)
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
void GSession::InsertFdbk(GProfile* p,GDoc* d,tDocJudgement j,R::RDate date) throw(bad_alloc)
{
	GProfDoc* f;

	Fdbks.InsertPtr(f=new GProfDoc(d,p,j,date));
	p->AddJudgement(f,this);
	d->AddJudgement(f);
}


//-----------------------------------------------------------------------------
void GSession::InitGroups(bool wg,bool w) throw(bad_alloc,GException)
{
	// If groups already loaded, do nothing.
	if(bGroups) return;

	// Load the users
	LoadGroups(wg,w);
	bGroups=true;
}


//-----------------------------------------------------------------------------
void GSession::CopyIdealGroups(void) throw(bad_alloc,GException)
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
	Grps=Subjects->GetIdealGroupsCursor();
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
			Sub=Ideal()->GetSubProfilesCursor();
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
	// cout<<compt<<" nbre de mots restants"<<endl;

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
GSession::~GSession(void) throw(GException)
{
	// Disconnect from the different managers
	if(ProfilingMng) ProfilingMng->Disconnect(this);
	if(GroupingMng) GroupingMng->Disconnect(this);
	if(GroupCalcMng) GroupCalcMng->Disconnect(this);
	if(StatsCalcMng) StatsCalcMng->Disconnect(this);
	if(LinkCalcMng) LinkCalcMng->Disconnect(this);
	if(PostGroupMng) PostGroupMng->Disconnect(this);
	if(Langs) Langs->Disconnect(this);
	if(DocAnalyseMng) DocAnalyseMng->Disconnect(this);

	// Delete stuctures
	if(SubProfileDescs) delete SubProfileDescs;
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
	InsertPtr(p=new GParamUInt("SameBehaviourMinDocs"));
	InsertPtr(p=new GParamUInt("DiffBehaviourMinDocs"));
}

