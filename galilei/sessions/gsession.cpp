/*

	GALILEI Research Project

	GSession.cpp

	Generic GALILEI Session - Implementation.

	(C) 2001-2002 by P. Francq.

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
#include <rio/rtextfile.h>
using namespace RIO;


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <langs/glang.h>
#include <sessions/gsession.h>
#include <sessions/gslot.h>
#include <sessions/gsessionprg.h>
#include <docs/gdoc.h>
#include <docs/gdocanalyse.h>
#include <docs/gdocxml.h>
#include <docs/gdocoptions.h>
#include <profiles/guser.h>
#include <profiles/gprofile.h>
#include <profiles/gsubprofile.h>
#include <profiles/gsubprofiledesc.h>
#include <profiles/gprofdoc.h>
#include <groups/ggroups.h>
#include <groups/ggroup.h>
#include <groups/ggrouping.h>
#include <profiles/gprofilecalc.h>
#include <urlmanagers/gurlmanager.h>
#include <filters/gfilter.h>
#include <filters/gmimefilter.h>
using namespace GALILEI;



//-----------------------------------------------------------------------------
//
// GSession
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GALILEI::GSession::GSession(unsigned int d,unsigned int u,unsigned int p,unsigned int f,unsigned int g,GURLManager* mng) throw(bad_alloc,GException)
	: GLangs(2),GDocs(d),GUsers(u,p,2),Groups(g+g/2,g/2), Fdbks(f+f/2,f/2),
	  ProfileCalcs(0), ProfileCalc(0), Groupings(0), Grouping(0), Mng(mng), DocAnalyse(0),
	  bGroups(false),bFdbks(false), DocOptions(0)
	
{
	GLangCursor Langs;

	Langs=GetLangsCursor();
	for(Langs.Start();!Langs.End();Langs.Next())
		Groups.InsertPtr(new GGroups(Langs()));
	ProfileCalcs=new RContainer<GProfileCalc,unsigned int,true,true>(3,3);
	SubProfileDescs=new RContainer<GSubProfileDesc,unsigned int,true,true>(3,3);
	Groupings=new RContainer<GGrouping,RStd::tId,true,true>(3,3);
	DocOptions=new GDocOptions();
	DocAnalyse=new GDocAnalyse(this,DocOptions);
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
GDocXML* GALILEI::GSession::CreateDocXML(GDoc* doc) throw(GException)
{
	return(Mng->CreateDocXML(doc));
}


//-----------------------------------------------------------------------------
void GALILEI::GSession::AnalyseDocs(GSlot* rec,bool modified) throw(GException)
{
	GDocXML* xml=0;
	GDocCursor Docs=GetDocsCursor();

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
}


//-----------------------------------------------------------------------------
GUser* GALILEI::GSession::NewUser(const char* /*usr*/,const char* /*pwd*/,const char* /*name*/,const char* /*email*/,
	                  const char* /*title*/,const char* /*org*/,const char* /*addr1*/,
	                  const char* /*addr2*/,const char* /*city*/,const char* /*country*/) throw(bad_alloc)
{
	return(0);
}


//-----------------------------------------------------------------------------
void GALILEI::GSession::CalcProfiles(GSlot* rec,bool modified) throw(GException)
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
			SaveProfile(Prof());
			if(Prof()->GetState()==osUpdated)
				Prof()->SetState(osUpToDate);
		}
		catch(GException& e)
		{
		}
	}
}


//-----------------------------------------------------------------------------
void GALILEI::GSession::CalcProfile(GProfile* prof) throw(GException)
{
	ProfileCalc->Compute(prof);
}


//-----------------------------------------------------------------------------
void GALILEI::GSession::GroupingProfiles(GSlot* rec,bool modified)  throw(GException)
{
	Grouping->Grouping(rec,modified);
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
		SaveSubProfile((*grp)());
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
void GALILEI::GSession::RunPrg(GSlot* rec,const char* filename) throw(GException)
{
	GSessionPrg Prg(filename,this,rec);
	Prg.Exec();
}


//-----------------------------------------------------------------------------
GALILEI::GSession::~GSession(void) throw(GException)
{
	if(DocAnalyse) delete DocAnalyse;
	if(DocOptions) delete DocOptions;
	if(ProfileCalcs) delete ProfileCalcs;
	if(Groupings) delete Groupings;
	if(SubProfileDescs) delete SubProfileDescs;
}
