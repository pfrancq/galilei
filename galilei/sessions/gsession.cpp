/*

	GALILEI Research Project

	gwordref.h

	Basic Information - Implementation.

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

//---------------------------------------------------------------------------
// include files for Galilei
#include "gsession.h"




using namespace GALILEI;
namespace GALILEI{


//---------------------------------------------------------------------------
//
// GSession
//
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
GSession::GSession(void) throw(bad_alloc,GException)
  : AllDocs(true),Langs(0),Stops(0),Dics(0),Users(0),Docs(0),GroupsLangs(0)
{
}



//---------------------------------------------------------------------------
void GSession::LoadDics(void) throw(bad_alloc,GException)
{
 cout << "ok";
  if(!Dics) throw(GException("Error in ""GSession::LoadDics"": Dictionnaries not created"));
  Dics->Load(true,true);
}


//---------------------------------------------------------------------------
void GSession::LoadStops(void) throw(bad_alloc,GException)
{
  if(!Stops) throw(GException("Error in ""GSession::LoadStops"": StopLists not created"));
  Stops->Load(false,true);
}

//---------------------------------------------------------------------------
GDict* GSession::GetDic(GLang *lang) throw(GException)
{
  if(!lang) throw(GException("Error in ""GSession::GetDic"": Language not defined"));
  if(!Dics) throw(GException("Error in ""GSession::GetDic"": Dictionnaries not created"));
  return(Dics->GetPtr<GLang*>(lang,false));
}


//---------------------------------------------------------------------------
GDict* GSession::GetStop(GLang *lang) throw(GException)
{
  if(!lang) throw(GException("Error in ""GSession::GetStop"": Language not defined"));
  if(!Stops) throw(GException("Error in ""GSession::GetStop"": StopLists not created"));
  return(Stops->GetPtr<GLang*>(lang,false));
}


//---------------------------------------------------------------------------
void GSession::ClearDics(void) throw(GException)
{
  if(!Dics) throw(GException("Error in ""GSession::ClearDics"": Dictionnaries not created"));
  Dics->Clear();
}


//---------------------------------------------------------------------------
void GSession::ClearStops(void) throw(GException)
{
  if(!Stops) throw(GException("Error in ""GSession::ClearStops"": StopLists not created"));
  Stops->Clear();
}


//---------------------------------------------------------------------------
void GSession::PutAllDocs(bool alldocs)
{
  AllDocs=alldocs;
  if(Docs) Docs->AllDocs=AllDocs;
}


//---------------------------------------------------------------------------
unsigned GSession::GetNbDocs(void)
{
	return(Docs->NbPtr);
}


//---------------------------------------------------------------------------
void GSession::AnalyseDocs(URLFunc *urlfunc,InfoFunc *infofunc) throw(bad_alloc,GException)
{
  Docs->Analyse(urlfunc,infofunc);
}


//---------------------------------------------------------------------------
void GSession::ClearDocs(void) throw(GException)
{
  Docs->Clear();
}

//---------------------------------------------------------------------------

GSession::~GSession(void) throw(GException)

{
	if(Langs) delete Langs;
	if(Stops) delete Stops;
	if(Dics) delete Dics;
	if(Docs) delete Docs;
	if(Users) delete Users;
	if(GroupsLangs) delete GroupsLangs;
}
}

