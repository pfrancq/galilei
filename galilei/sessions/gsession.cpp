/*

	GALILEI Research Project

	GSession.cpp

	Generic GALILEI Session - Implementation.

	(C) 2001 by P. Francq.

	Version $Revision$

	Last Modify: $Date$

*/



//---------------------------------------------------------------------------
// include files for GALILEI
#include <gsessions/gsession.h>
#include <glangs/glangen.h>
#include <glangs/glangfr.h>
using namespace GALILEI;
using namespace RStd;



//---------------------------------------------------------------------------
//
// GSession
//
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
GALILEI::GSession::GSession(void) throw(bad_alloc,GException)
  : Langs(2),Stops(0),Dics(0),Users(0),Docs(0),Groups(2,5), MIMETypes(0)
{
	GLang* l;

	MIMETypes=new RContainer<GMIMEType,unsigned int,true,true>(20,10);
	Langs.InsertPtr(l=new GLangEN());
	Groups.InsertPtr(new GGroups(l));
	Langs.InsertPtr(l=new GLangFR());
	Groups.InsertPtr(new GGroups(l));
}


//-----------------------------------------------------------------------------
GLang* GALILEI::GSession::GetLang(const char* code)
{
	RReturnValIfFail(code,0);
	return(Langs.GetPtr<const char*>(code));
}


//---------------------------------------------------------------------------
GDict* GALILEI::GSession::GetDic(const GLang *lang) throw(GException)
{
  if(!lang) throw(GException("Error in ""GSession::GetDic"": Language not defined"));
  if(!Dics) throw(GException("Error in ""GSession::GetDic"": Dictionnaries not created"));
  return(Dics->GetPtr<const GLang*>(lang,false));
}


//---------------------------------------------------------------------------
GDict* GALILEI::GSession::GetStop(const GLang *lang) throw(GException)
{
  if(!lang) throw(GException("Error in ""GSession::GetStop"": Language not defined"));
  if(!Stops) throw(GException("Error in ""GSession::GetStop"": StopLists not created"));
  return(Stops->GetPtr<const GLang*>(lang,false));
}


//---------------------------------------------------------------------------
void GALILEI::GSession::ClearDics(void) throw(GException)
{
  if(!Dics) throw(GException("Error in ""GSession::ClearDics"": Dictionnaries not created"));
  Dics->Clear();
}


//---------------------------------------------------------------------------
void GALILEI::GSession::ClearStops(void) throw(GException)
{
  if(!Stops) throw(GException("Error in ""GSession::ClearStops"": StopLists not created"));
  Stops->Clear();
}


////---------------------------------------------------------------------------
//void GSession::PutAllDocs(bool alldocs)
//{
//  AllDocs=alldocs;
//  if(Docs) Docs->AllDocs=AllDocs;
//}


//---------------------------------------------------------------------------
unsigned GALILEI::GSession::GetNbDocs(void)
{
	return(Docs->NbPtr);
}


//---------------------------------------------------------------------------
//void GSession::AnalyseDocs(URLFunc *urlfunc,InfoFunc *infofunc) throw(bad_alloc,GException)
//{
//  Docs->Analyse(urlfunc,infofunc);
//}


//---------------------------------------------------------------------------
void GALILEI::GSession::ClearDocs(void) throw(GException)
{
  Docs->Clear();
}


//---------------------------------------------------------------------------
GUser* GALILEI::GSession::CreateUser(const char* usr,const char* pwd,const char* name,const char* email,
	                  const char* title,const char* org,const char* addr1,
	                  const char* addr2,const char* city,const char* country) throw(bad_alloc)
{
	return(0);
}


//---------------------------------------------------------------------------
GGroups* GALILEI::GSession::GetGroups(const GLang* lang) const
{
	return(Groups.GetPtr<const GLang*>(lang));
}


//---------------------------------------------------------------------------
GGroup* GALILEI::GSession::NewGroup(void)
{
	return(new GGroup());
}


//---------------------------------------------------------------------------
GALILEI::GSession::~GSession(void) throw(GException)
{
	if(Stops) delete Stops;
	if(Dics) delete Dics;
	if(Docs) delete Docs;
	if(Users) delete Users;
//	if(GroupsLangs) delete GroupsLangs;
	if(MIMETypes) delete MIMETypes;
}

