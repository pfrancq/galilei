/*

	GALILEI Research Project

	GSession.cpp

	Generic GALILEI Session - Implementation.

	(C) 2001 by P. Francq.

	Version $Revision$

	Last Modify: $Date$

*/



//-----------------------------------------------------------------------------
// include files for R Project
#include <rstd/rcontainercursor.h>
using namespace RStd;


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <gsessions/gsession.h>
#include <glangs/glangen.h>
#include <glangs/glangfr.h>
using namespace GALILEI;
using namespace RStd;



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
void GALILEI::GSessionSignalsReceiver::receiveNextDoc(const GDoc* doc)
{
}


//-----------------------------------------------------------------------------
void GALILEI::GSessionSignalsReceiver::receiveNextProfile(const GProfile* prof)
{
}



//-----------------------------------------------------------------------------
//
// GSession
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GALILEI::GSession::GSession(unsigned int d,unsigned int u,unsigned int f,GURLManager* mng) throw(bad_alloc,GException)
	: Langs(2),Stops(2),Dics(2),Users(u),Docs(d,this),Groups(2,5), Fdbks(f+f/2,f/2), Mng(mng),
	  bDics(false), bDocs(false), bUsers(false), bGroups(false),
	  bGroupsMember(false), bFdbks(false)
	
{
	GLang* l;

	Langs.InsertPtr(l=new GLangEN());
	Groups.InsertPtr(new GGroups(l));
	Langs.InsertPtr(l=new GLangFR());
	Groups.InsertPtr(new GGroups(l));
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
GDocXML* GALILEI::GSession::CreateDocXML(const GDoc* doc) throw(GException)
{
	return(Mng->CreateDocXML(doc));
}


//-----------------------------------------------------------------------------
void GALILEI::GSession::AnalyseDocs(GSessionSignalsReceiver* rec) throw(GException)
{
	tObjState s;
	GDocXML* xml;

	for(Docs.Start();!Docs.End();Docs.Next())
	{
		s=Docs()->GetState();
		if((s==osUpToDate)||(s==osUpdated)) continue;
		rec->receiveNextDoc(Docs());
		try
		{
			xml=0;
			xml=Mng->CreateDocXML(Docs());
			Docs()->Analyse(xml,this);
			delete xml;
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
	if(bUsers) return;

	// Load the users
	LoadUsers();
	bUsers=true;
}


//-----------------------------------------------------------------------------
GUser* GALILEI::GSession::CreateUser(const char* usr,const char* pwd,const char* name,const char* email,
	                  const char* title,const char* org,const char* addr1,
	                  const char* addr2,const char* city,const char* country) throw(bad_alloc)
{
	return(0);
}


//-----------------------------------------------------------------------------
void GALILEI::GSession::CalcProfiles(GSessionSignalsReceiver* rec,GProfileCalc* method) throw(GException)
{
	tObjState s;
	GProfile* prof;

	for(Users.Start();!Users.End();Users.Next())
	{
		for(Users()->Start();!Users()->End();Users()->Next())
		{
			prof=(*Users())();
			s=prof->GetState();
			if((s==osUpToDate)||(s==osUpdated)) continue;
			rec->receiveNextProfile(prof);
			try
			{
				method->Compute(prof);
			}
			catch(GException& e)
			{
			}
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
void GALILEI::GSession::InitGroupsMember(void) throw(bad_alloc,GException)
{
	// If users' feedback already loaded, do nothing.
	if(bGroupsMember) return;

	// Verify that users and groups are loaded
	if(!bUsers)
		InitUsers();
	if(!bGroups)
		InitGroups();

	// Load the groups' member.
	for(Groups.Start();!Groups.End();Groups.Next())
		for((Groups)()->Start();!(Groups)()->End();(Groups)()->Next())
			LoadGroupsMember((*(Groups)())());
	bGroupsMember=true;
}


//-----------------------------------------------------------------------------
GGroups* GALILEI::GSession::GetGroups(const GLang* lang) const
{
	return(Groups.GetPtr<const GLang*>(lang));
}


//-----------------------------------------------------------------------------
GGroup* GALILEI::GSession::NewGroup( GLang* lang)
{
	return(new GGroup(lang));
}


//-----------------------------------------------------------------------------
GALILEI::GSession::~GSession(void) throw(GException)
{
}
