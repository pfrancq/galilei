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
#include <rmath/random.h>
using namespace RMath;
#include <rstd/rcontainercursor.h>
using namespace RStd;
#include <rio/rtextfile.h>
using namespace RIO;


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <langs/glang.h>
#include <langs/gdict.h>
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
#include <profiles/guser.h>
#include <profiles/gprofile.h>
#include <profiles/gsubprofile.h>
#include <profiles/gprofilessim.h>
#include <profiles/gprofilessims.h>
#include <profiles/gsubprofiledesc.h>
#include <profiles/gprofdoc.h>
#include <groups/ggroups.h>
#include <groups/ggroup.h>
#include <groups/ggroupvector.h>
#include <groups/ggrouping.h>
#include <groups/ggroupcalc.h>
#include <profiles/gprofilecalc.h>
#include <urlmanagers/gurlmanager.h>
#include <filters/gfilter.h>
#include <filters/gmimefilter.h>
#include <infos/giwordweight.h>
#include <historic/ggroupshistory.h>
using namespace GALILEI;



//-----------------------------------------------------------------------------
//
// GSession
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GALILEI::GSession::GSession(unsigned int d,unsigned int u,unsigned int p,unsigned int f,unsigned int g,GURLManager* mng) throw(bad_alloc,GException)
	: GLangs(2), GDocs(d), GUsers(u,p), GGroupsMng(g), 
	  Subjects(), Fdbks(f+f/2,f/2),
	  ProfileCalcs(0), ProfileCalc(0), Groupings(0), Grouping(0), Mng(mng), DocAnalyse(0),
	  bGroups(false),bFdbks(false), DocOptions(0)
	
{
	GLangCursor Langs;
	IdealGroups= new RStd::RContainer<GALILEI::GGroups, unsigned int, true, true> (g+g/2,g/2);
	IdealDocs=new RStd::RContainer<GALILEI::GGroupsEvaluate, unsigned int, false, false> (2,1);
	Langs=GetLangsCursor();
	for(Langs.Start();!Langs.End();Langs.Next())
		Groups.InsertPtr(new GGroups(Langs()));
	ProfileCalcs=new RContainer<GProfileCalc,unsigned int,true,true>(3,3);
	SubProfileDescs=new RContainer<GSubProfileDesc,unsigned int,true,true>(3,3);
	Groupings=new RContainer<GGrouping,RStd::tId,true,true>(3,3);
	GroupCalcs=new RContainer<GGroupCalc,RStd::tId,true,true>(2,3);
	LinkCalcs=new RContainer<GLinkCalc,unsigned int,true,true>(3,2);
	DocOptions=new GDocOptions();
	DocAnalyse=new GDocAnalyse(this,DocOptions);
	CurrentRandom=0;
	Random = new RRandomGood(CurrentRandom);
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
RStd::RContainer<GGroups,unsigned int,true,true>* GALILEI::GSession::GetIdealGroups(void)
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
RStd::RContainer<GGroupsEvaluate,unsigned int,false,false>* GALILEI::GSession::GetIdealDocs(void)
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
void GALILEI::GSession::RegisterComputingMethod(GProfileCalc* grp) throw(bad_alloc)
{
	ProfileCalcs->InsertPtr(grp);
}


//-----------------------------------------------------------------------------
void GALILEI::GSession::SetCurrentComputingMethod(const char* name) throw(GException)
{
	GProfileCalc* tmp;

	tmp=ProfileCalcs->GetPtr<const char*>(name);
	if(!tmp)
		throw GException(RString("Computing method '")+name+"' doesn't exists.");
	ProfileCalc=tmp;
}


//-----------------------------------------------------------------------------
void GALILEI::GSession::SetCurrentComputingMethodSettings(const char* s) throw(GException)
{
	if((!ProfileCalc)||(!(*s))) return;
	ProfileCalc->SetSettings(s);
}


//-----------------------------------------------------------------------------
GProfileCalcCursor& GALILEI::GSession::GetComputingsCursor(void)
{
	GProfileCalcCursor *cur=GProfileCalcCursor::GetTmpCursor();
	cur->Set(ProfileCalcs);
	return(*cur);
}


//-----------------------------------------------------------------------------
void GALILEI::GSession::RegisterGroupingMethod(GGrouping* grp) throw(bad_alloc)
{
	Groupings->InsertPtr(grp);
}


//-----------------------------------------------------------------------------
void GALILEI::GSession::SetCurrentGroupingMethod(const char* name) throw(GException)
{
	GGrouping* tmp;

	tmp=Groupings->GetPtr<const char*>(name);
	if(!tmp)
		throw GException(RString("Grouping method '")+name+"' doesn't exists.");
	Grouping=tmp;
}


//-----------------------------------------------------------------------------
void GALILEI::GSession::SetCurrentGroupingMethodSettings(const char* s) throw(GException)
{
	if((!Grouping)||(!(*s))) return;
	Grouping->SetSettings(s);
}


//-----------------------------------------------------------------------------
const char* GALILEI::GSession::GetGroupingMethodSettings(const char* n) throw(GException)
{
	GGrouping* tmp;

	tmp=Groupings->GetPtr<const char*>(n);
	if(!tmp)
		return(0);
	return(tmp->GetSettings());
}


//-----------------------------------------------------------------------------
GGroupingCursor& GALILEI::GSession::GetGroupingsCursor(void)
{
	GGroupingCursor *cur=GGroupingCursor::GetTmpCursor();
	cur->Set(Groupings);
	return(*cur);
}


//-----------------------------------------------------------------------------
void GALILEI::GSession::RegisterGroupCalcMethod(GGroupCalc* grp) throw(bad_alloc)
{
	GroupCalcs->InsertPtr(grp);
}


//-----------------------------------------------------------------------------
void GALILEI::GSession::SetCurrentGroupCalcMethod(const char* name) throw(GException)
{
	GGroupCalc* tmp;

	tmp=GroupCalcs->GetPtr<const char*>(name);
	if(!tmp)
		throw GException(RString("Group Description method '")+name+"' doesn't exists.");
	GroupCalc=tmp;
}


//-----------------------------------------------------------------------------
void GALILEI::GSession::SetCurrentGroupCalcMethodSettings(const char* s) throw(GException)
{
	if((!GroupCalc)||(!(*s))) return;
	GroupCalc->SetSettings(s);
}


//-----------------------------------------------------------------------------
const char* GALILEI::GSession::GetGroupCalcMethodSettings(const char* n) throw(GException)
{
	GGroupCalc* tmp;

	tmp=GroupCalcs->GetPtr<const char*>(n);
	if(!tmp)
		return(0);
	return(tmp->GetSettings());
}


//-----------------------------------------------------------------------------
GGroupCalcCursor& GALILEI::GSession::GetGroupCalcsCursor(void)
{
	GGroupCalcCursor *cur=GGroupCalcCursor::GetTmpCursor();
	cur->Set(GroupCalcs);
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
	return(Mng->CreateDocXML(doc));
}

//-----------------------------------------------------------------------------
void GALILEI::GSession::AnalyseAssociation(bool save)
{
	GDocCursor Docs=GetDocsCursor();
	GWordsClustering* test;
	unsigned int i,n=1;
	bool end=false;
	Docs.Start();
	test= new GWordsClustering(GetDic(Docs()->GetLang()),DocOptions->MinDocs,DocOptions->MaxDocs,DocOptions->MinOccurCluster);

	for(i=Docs.GetNb(),Docs.Start();i--;Docs.Next())
	{
		test->AddDoc(dynamic_cast<GDocVector*>(Docs()));
	}
	test->CleanWords();
	Docs.Start();
  for(i=0;i<DocOptions->NbIteration;i++)
  {
	  end=test->OrdreByDocs(i);
	  test->SaveAssociations(i,save);
  }
  test->View();
  if(save) SaveWordsGroups(GetDic(Docs()->GetLang()));
	for(i=Docs.GetNb(),Docs.Start();--i;Docs.Next())
	{
		test->UpdateDoc(dynamic_cast<GDocVector*>(Docs()));
		if(save) SaveUpDatedDoc(Docs(),n);/*n=id du premier mot a sauver.*/
	}
	delete(test);
}


//-----------------------------------------------------------------------------
void GALILEI::GSession::AnalyseDocs(GSlot* rec,bool modified) throw(GException)
{
	bool undefLang;
	GDocXML* xml=0;
	GDocCursor Docs=GetDocsCursor();
	RContainer<GDoc,unsigned int,false,true>* tmpDocs = new RContainer<GDoc,unsigned int,false,true>(5,2);

//	cout <<"------------------------------"<<endl;
//  for(Docs.Start();!Docs.End();Docs.Next())
//	{
//		cout<< "id du doc : "<< Docs()->GetId()<<" id docRef "<<GetDoc(Docs()->GetURL())->GetId() <<" url : "<<Docs()->GetURL()<<endl;
//		GSubjectCursor Scur=Docs()->GetSubjectCursor();
//		cout << "Subjects : " << endl;
//		for (Scur.Start();!Scur.End();Scur.Next())
//		{
//			cout<< Scur()->GetName() <<endl;
//		}
//	}
//	cout <<"------------------------------"<<endl;
//	
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
				xml=Mng->CreateDocXML(Docs());
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

	
//	cout <<"------------------------------"<<endl;
//	Docs=GetDocsCursor();
//  for(Docs.Start();!Docs.End();Docs.Next())
//	{
//		cout<< "id du doc : "<< Docs()->GetId()<<" id docRef "<<GetDoc(Docs()->GetURL())->GetId() <<" url : "<<Docs()->GetURL()<<endl;
//		GSubjectCursor Scur=Docs()->GetSubjectCursor();
//		cout << "Subjects : " << endl;
//		for (Scur.Start();!Scur.End();Scur.Next())
//		{
//			cout<< Scur()->GetName() <<endl;
//		}
//	}
//	cout <<"------------------------------"<<endl;
	
}


void GALILEI::GSession::ComputeLinks(GSlot* rec)
{
  LinkCalc->Compute(rec);
}


//-----------------------------------------------------------------------------
void GALILEI::GSession::InitUsers(void) throw(bad_alloc,GException)
{
	// If users already loaded, do nothing.
	if(IsUsersLoad()) return;

	// Initialise groups.
	if(!bGroups)
		InitGroups();

	// Load the users
	LoadUsers();
  bUsers=true;

  // Initialise the profiles sims  
	InitProfilesSims();

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
	GLangCursor langs = GetLangsCursor();

	RContainer<GSubProfile,unsigned int,false,true>* subProf;
	GProfilesSim* profSim;

	ProfilesSims = new GProfilesSims(100);
	
	for(langs.Start();!langs.End(); langs.Next())
	{
		subProf = new RContainer<GSubProfile,unsigned int, false,true>(100,50);
		GSubProfileCursor subProfCur = GetSubProfilesCursor(langs());

		for(subProfCur.Start();!subProfCur.End();subProfCur.Next())
		{
			subProf->InsertPtr( subProfCur());      
		}
    profSim= new GProfilesSim(subProf, false,langs());
		ProfilesSims->InsertPtr(profSim);
	}
}


//-----------------------------------------------------------------------------
void GALILEI::GSession::ChangeProfilesSimState(bool global,GLang* lang)throw(bad_alloc)
{
	GProfilesSim* profSim = ProfilesSims->GetPtr<GLang*>(lang);
	profSim->UpdateProfSim(this, global,lang);

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
double GALILEI::GSession::GetSimProf(GLang* l,unsigned int id1, unsigned int id2)
{
	GProfilesSim* profSim = ProfilesSims->GetPtr<GLang*>(l);
	return profSim->GetSim(this,id1,id2);
}


//-----------------------------------------------------------------------------
double GALILEI::GSession::GetSimProf(const GSubProfile* sub1,const GSubProfile* sub2)
{

	GProfilesSim* profSim = ProfilesSims->GetPtr<GLang*>(sub1->GetLang());
	return profSim->GetSim(sub1,sub2);
}


//-----------------------------------------------------------------------------
GUser* GALILEI::GSession::NewUser(const char* /*usr*/,const char* /*pwd*/,const char* /*name*/,const char* /*email*/,
	                  const char* /*title*/,const char* /*org*/,const char* /*addr1*/,
	                  const char* /*addr2*/,const char* /*city*/,const char* /*country*/) throw(bad_alloc)
{
	return(0);
}


//-----------------------------------------------------------------------------
void GALILEI::GSession::CalcProfiles(GSlot* rec,bool modified,bool save) throw(GException)
{
	GProfileCursor Prof=GetProfilesCursor();

	if(!ProfileCalc)
		throw GException("No computing method chosen.");
	for(Prof.Start();!Prof.End();Prof.Next())
	{
		if(modified&&(Prof()->GetState()==osUpToDate)) continue;
		rec->receiveNextProfile(Prof());
		try
		{
			if((!modified)||(Prof()->GetState()!=osUpdated))
				ProfileCalc->Compute(Prof());
			if(save)
				SaveProfile(Prof());
			if(Prof()->GetState()==osUpdated)
				Prof()->SetState(osUpToDate);
		}
		catch(GException& e)
		{
		}
	}


	ChangeAllProfilesSimState(true);   /// !!!!
}


//-----------------------------------------------------------------------------
void GALILEI::GSession::CalcProfile(GProfile* prof) throw(GException)
{
	ProfileCalc->Compute(prof);
}


//-----------------------------------------------------------------------------
void GALILEI::GSession::GroupingProfiles(GSlot* rec,bool modified,bool save)  throw(GException)
{
	Grouping->Grouping(rec,modified,save);
}


//-----------------------------------------------------------------------------
void GALILEI::GSession::InitFdbks(void) throw(bad_alloc,GException)
{
	// If users' feedback already loaded, do nothing.
	if(bFdbks) return;

	// Verify that users and docs are loaded
	if(!IsUsersLoad())
		InitUsers();
	InitDocs();

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
void GALILEI::GSession::InsertFdbk(GProfile* p,GDoc* d,tDocJudgement j,const char* date) throw(bad_alloc)
{
	GProfDoc* f;

	Fdbks.InsertPtr(f=new GProfDoc(d,p,j,date));
	p->AddJudgement(f);
	d->AddJudgement(f);
	p->SetState(osModified);
}


//-----------------------------------------------------------------------------
void GALILEI::GSession::InitGroups(void) throw(bad_alloc,GException)
{
	// If groups already loaded, do nothing.
	if(bGroups) return;

	// Load the users
	LoadGroups();
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
	CalcDesc=GetCurrentGroupCalcMethod();

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
GFilterCursor& GALILEI::GSession::GetFiltersCursor(void)
{
	return(Mng->GetFiltersCursor());
}


//-----------------------------------------------------------------------------
GMIMEFilter* GALILEI::GSession::GetMIMEType(const char* mime) const
{
	return(Mng->GetMIMEType(mime));
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
RStd::RContainer<GGroupsHistory, unsigned int, false,true>* GALILEI::GSession::LoadHistoricGroups (RContainer<GSubProfile, unsigned int, false,true>* subprofiles,GLang* lang,unsigned int mingen, unsigned int maxgen)
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
GALILEI::GSession::~GSession(void) throw(GException)
{
	if(DocAnalyse) delete DocAnalyse;
	if(DocOptions) delete DocOptions;
	if(Groupings) delete Groupings;
	if(GroupCalcs) delete GroupCalcs;
	if(SubProfileDescs) delete SubProfileDescs;
	if(ProfileCalcs) delete ProfileCalcs;
	if(IdealGroups) delete IdealGroups;
	if(IdealDocs) delete IdealDocs;
}
