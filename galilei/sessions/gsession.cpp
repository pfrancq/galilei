/*

	GALILEI Research Project

	GSession.cpp

	Generic GALILEI Session - Implementation.

	(C) 2001 by P. Francq.

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
#include <rstd/rcontainercursor.h>
using namespace RStd;


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <sessions/gsession.h>
#include <langs/glangen.h>
#include <langs/glangfr.h>
#include <langs/gdict.h>
#include <docs/gdoc.h>
#include <docs/gdocanalyse.h>
#include <docs/gdocxml.h>
#include <docs/gdocoptions.h>
#include <profiles/gprofoptions.h>
#include <groups/ggroupingoptions.h>
#include <profiles/guser.h>
#include <profiles/gprofile.h>
#include <profiles/gsubprofile.h>
#include <profiles/gprofdoc.h>
#include <groups/ggroups.h>
#include <groups/ggroup.h>
#include <profiles/gprofilecalc.h>
#include <profiles/gprofilecalcvector.h>
#include <urlmanagers/gurlmanager.h>
#include <filters/gfilter.h>
#include <filters/gmimefilter.h>
using namespace GALILEI;



//-----------------------------------------------------------------------------
//
//  class GSessionSignalsReceiver
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GALILEI::GSessionSignalsReceiver::GSessionSignalsReceiver(void)
{
}


//-----------------------------------------------------------------------------
void GALILEI::GSessionSignalsReceiver::receiveNextDoc(const GDoc* /*doc*/)
{
}


//-----------------------------------------------------------------------------
void GALILEI::GSessionSignalsReceiver::receiveNextProfile(const GProfile* /*prof*/)
{
}



//-----------------------------------------------------------------------------
//
// GSession
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GALILEI::GSession::GSession(unsigned int d,unsigned int u,unsigned int p,unsigned int f,unsigned int g,GURLManager* mng) throw(bad_alloc,GException)
	: Langs(2),Stops(2),Dics(2),Users(u),Docs(d,this),Groups(g+g/2,g/2), Fdbks(f+f/2,f/2),
	  Profiles(0), SubProfiles(0), ProfileCalcs(0), Mng(mng), DocAnalyse(0), bDics(false),
	  bDocs(false), bUsers(false), bGroups(false),bFdbks(false), DocOptions(0),
	  ProfOptions(0)
	
{
	GLang* l;

	Langs.InsertPtr(l=new GLangEN());
	Groups.InsertPtr(new GGroups(l));
	Langs.InsertPtr(l=new GLangFR());
	Groups.InsertPtr(new GGroups(l));
	Profiles=new RContainer<GProfile,unsigned int,true,true>(p+p/2,p/2);
	SubProfiles=new RContainer<GSubProfile,unsigned int,true,true>(p*2+p,p);
	ProfileCalcs=new RContainer<GProfileCalc,unsigned int,true,true>(3,3);
//	ProfileCalcs->InsertPtr(new GProfileCalcVector(this));
	DocAnalyse=new GDocAnalyse(this,DocOptions);
	DocOptions=new GDocOptions();
	ProfOptions=new GProfOptions();
	GroupingOptions=new GGroupingOptions();
}


//-----------------------------------------------------------------------------
GLang* GALILEI::GSession::GetLang(const char* code) const
{
	if(!code) return(0);
	return(Langs.GetPtr<const char*>(code));
}


//-----------------------------------------------------------------------------
void GALILEI::GSession::InitDics(void) throw(bad_alloc,GException)
{
	// If dictionnary already loaded, do nothing.
	if(bDics) return;

	// For each Lang, create a dictionnary and a stop list
	RContainerCursor<GLang,unsigned int,true,true> CurLang(Langs);
	for(CurLang.Start();!CurLang.End();CurLang.Next())
	{
		LoadDic(CurLang()->GetCode(),true) ;
		LoadDic(CurLang()->GetCode(),false) ;
	}
	bDics=true;
}


//-----------------------------------------------------------------------------
GDict* GALILEI::GSession::GetDic(const GLang *lang) const throw(GException)
{
	return(Dics.GetPtr<const GLang*>(lang,false));
}


//-----------------------------------------------------------------------------
GDict* GALILEI::GSession::GetStop(const GLang *lang) const throw(GException)
{
	return(Stops.GetPtr<const GLang*>(lang,false));
}


//-----------------------------------------------------------------------------
const char* GALILEI::GSession::GetWord(const unsigned int id,const GLang* lang)
{
	if(bDics)
		return(Dics.GetPtr<const GLang*>(lang,false)->GetWord(id));
	return(LoadWord(id,lang->GetCode()));
}


//-----------------------------------------------------------------------------
const char* GALILEI::GSession::GetWord(const unsigned int id,const GDict* dict) const
{
	if(bDics&&dict)
		return(dict->GetWord(id));
	return(0);
}


//-----------------------------------------------------------------------------
void GALILEI::GSession::InitDocs(void) throw(bad_alloc,GException)
{
	// If documents already loaded, do nothing.
	if(bDocs) return;

	// Load the documents
	LoadDocs();
	bDocs=true;
}


//-----------------------------------------------------------------------------
GDocXML* GALILEI::GSession::CreateDocXML(GDoc* doc) throw(GException)
{
	return(Mng->CreateDocXML(doc));
}


//-----------------------------------------------------------------------------
void GALILEI::GSession::AnalyseDocs(GSessionSignalsReceiver* rec,bool modified) throw(GException)
{
	GDocXML* xml=0;

	for(Docs.Start();!Docs.End();Docs.Next())
	{
		if(modified&&(Docs()->GetState()==osUpToDate)) continue;
		rec->receiveNextDoc(Docs());
		try
		{
			if((!modified)||(Docs()->GetState()!=osUpdated))
			{
				xml=Mng->CreateDocXML(Docs());
				if(xml)
				{
					Docs()->InitFailed();
					DocAnalyse->Analyse(xml,Docs());
					delete xml;
					xml=0;
				}
				else
					Docs()->IncFailed();
			}
			Save(Docs());
			if(Docs()->GetState()==osUpdated)
				Docs()->SetState(osUpToDate);
		}
		catch(GException& e)
		{
			if(xml)
				delete xml;
		}
	}
}


//-----------------------------------------------------------------------------
GProfile* GALILEI::GSession::GetProfile(const unsigned int id) const
{
	return(Profiles->GetPtr<unsigned int>(id));
}


//-----------------------------------------------------------------------------
GProfileCursor& GALILEI::GSession::GetProfilesCursor(void)
{
	GProfileCursor *cur=GProfileCursor::GetTmpCursor();
	cur->Set(Profiles);
	return(*cur);
}


//-----------------------------------------------------------------------------
GSubProfileCursor& GALILEI::GSession::GetSubProfilesCursor(void)
{
	GSubProfileCursor *cur=GSubProfileCursor::GetTmpCursor();
	cur->Set(SubProfiles);
	return(*cur);
}


//-----------------------------------------------------------------------------
void GALILEI::GSession::InitUsers(void) throw(bad_alloc,GException)
{
	// If users already loaded, do nothing.
	if(bUsers) return;

	// Initialise groups.
	if(!bGroups)
		InitGroups();

	// Load the users
	LoadUsers();
	bUsers=true;
}


//-----------------------------------------------------------------------------
GUser* GALILEI::GSession::NewUser(const char* /*usr*/,const char* /*pwd*/,const char* /*name*/,const char* /*email*/,
	                  const char* /*title*/,const char* /*org*/,const char* /*addr1*/,
	                  const char* /*addr2*/,const char* /*city*/,const char* /*country*/) throw(bad_alloc)
{
	return(0);
}


//-----------------------------------------------------------------------------
void GALILEI::GSession::CalcProfiles(GSessionSignalsReceiver* rec,GProfileCalc* method,bool modified) throw(GException)
{
	GProfile* prof;

	for(Profiles->Start();!Profiles->End();Profiles->Next())
	{
		prof=(*Profiles)();
		if(modified&&(prof->GetState()==osUpToDate)) continue;
		rec->receiveNextProfile(prof);
		try
		{
			if((!modified)||(prof->GetState()!=osUpdated))
				method->Compute(prof);
			Save(prof);
			if(prof->GetState()==osUpdated)
				prof->SetState(osUpToDate);
		}
		catch(GException& e)
		{
		}
	}
}


//-----------------------------------------------------------------------------
void GALILEI::GSession::InitFdbks(void) throw(bad_alloc,GException)
{
	// If users' feedback already loaded, do nothing.
	if(bFdbks) return;

	// Verify that users and docs are loaded
	if(!bUsers)
		InitUsers();
	if(!bDocs)
		InitDocs();

	// Load the users
	LoadFdbks();
	bFdbks=true;
}


//-----------------------------------------------------------------------------
void GALILEI::GSession::InsertFdbk(GProfile* p,GDoc* d,char j,const char* date) throw(bad_alloc)
{
	GProfDoc* f;

	Fdbks.InsertPtr(f=new GProfDoc(d,p,j,date));
	p->AddJudgement(f);
	d->AddJudgement(f);
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
GGroupsCursor& GALILEI::GSession::GetGroupsCursor(void)
{
	GGroupsCursor *cur=GGroupsCursor::GetTmpCursor();
	cur->Set(Groups);
	return(*cur);
}


//-----------------------------------------------------------------------------
GGroups* GALILEI::GSession::GetGroups(const GLang* lang) const
{
	return(Groups.GetPtr<const GLang*>(lang));
}


//-----------------------------------------------------------------------------
void GALILEI::GSession::Save(GGroup* grp) throw(GException)
{
	if(grp->GetState()==osUpToDate) return;
	for(grp->Start();!grp->End();grp->Next())
		Save((*grp)());
	if(grp->GetState()==osUpdated)
		grp->SetState(osUpToDate);
}


//-----------------------------------------------------------------------------
void GALILEI::GSession::FiltersStart(void)
{
	Mng->GetFilters()->Start();
}


//-----------------------------------------------------------------------------
bool GALILEI::GSession::FiltersEnd(void) const
{
	return(Mng->GetFilters()->End());
}


//-----------------------------------------------------------------------------
void GALILEI::GSession::FiltersNext(void)
{
	Mng->GetFilters()->Next();
}


//-----------------------------------------------------------------------------
GFilter* GALILEI::GSession::GetCurFilters(void)
{
	return((*Mng->GetFilters())());
}


//-----------------------------------------------------------------------------
GMIMEFilter* GALILEI::GSession::GetMIMEType(const char* mime) const
{
	return(Mng->GetMIMEType(mime));
}


//-----------------------------------------------------------------------------
GALILEI::GSession::~GSession(void) throw(GException)
{
	if(SubProfiles) delete SubProfiles;
	if(Profiles) delete Profiles;
	if(DocAnalyse) delete DocAnalyse;
	if(DocOptions) delete DocOptions;
	if(ProfOptions) delete ProfOptions;
	if(GroupingOptions) delete GroupingOptions;
	if(ProfileCalcs) delete ProfileCalcs;
}
