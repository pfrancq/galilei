/*

	GALILEI Research Project

	GMetaEngine.cpp

	Meta Engine for extraction of results from different search engines - Implementation.

   Copyright 2003-2012 by Pascal Francq.
	Copyright 2003-2004 by Valery Vandaele.
	Copyright 2003-2008 Université Libre de Bruxelles (ULB).

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
#include <stdio.h>
#include <iostream>
#include <cstdlib>


//------------------------------------------------------------------------------
// include files for GALILEI
#include <gengine.h>
#include <gdoc.h>
#include <gsession.h>
#include <gmetaengine.h>
using namespace GALILEI;
using namespace R;
using namespace std;



//------------------------------------------------------------------------------
//
// class GMetaEngine
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GMetaEngine::GMetaEngine(GSession* session,GPlugInFactory* fac)
	: GPlugIn(session,fac), Results(200)
{
}


//------------------------------------------------------------------------------
void GMetaEngine::AddResult(size_t docid,const RString desc,double ranking,const GEngine* engine)
{
	if((ranking<0)||(ranking>1))
		ThrowGException("Ranking must be included in [0,1]");

	GDoc* Doc(Session->GetObj(pDoc,docid));
	RString URI(Doc->GetURI()());

	// Test if URI is already there
	bool Find;
	size_t idx(Results.GetIndex(URI,Find));

	if(Find)
		Results[idx]->AddRanking(ranking,engine->GetName());
	else
		Results.InsertPtrAt(new GDocRetrieved(docid,URI,Doc->GetName(),desc,ranking,engine->GetName()),idx,false);
}


//------------------------------------------------------------------------------
void GMetaEngine::AddResult(const RString& uri,const RString& title,const RString desc,double ranking,const GEngine* engine)
{
	if((ranking<0)||(ranking>1))
		ThrowGException("Ranking must be included in [0,1]");

	// Test if URI is already there
	bool Find;
	size_t idx(Results.GetIndex(uri,Find));

	if(Find)
		Results[idx]->AddRanking(ranking,engine->GetName());
	else
		Results.InsertPtrAt(new GDocRetrieved(0,uri,title,desc,ranking,engine->GetName()),idx,false);
}


//------------------------------------------------------------------------------
void GMetaEngine::SetRanking(GDocRetrieved* doc,double ranking)
{
	doc->Ranking=ranking;
}


//------------------------------------------------------------------------------
void GMetaEngine::SetRanking(size_t docid,double ranking)
{
	GDoc* TheDoc(Session->GetObj(pDoc,docid));
	GDocRetrieved* Doc(Results.GetPtr(TheDoc->GetURI()()));
	if(!Doc)
		ThrowGException("Document with identifier '"+RString::Number(docid)+"' was not retrieved.");
	Doc->Ranking=ranking;
}


//------------------------------------------------------------------------------
void GMetaEngine::SetRanking(const RString& uri,double ranking)
{
	GDocRetrieved* Doc(Results.GetPtr(uri));
	if(!Doc)
		ThrowGException("Document '"+uri+"' was not retrieved.");
	Doc->Ranking=ranking;
}


//-----------------------------------------------------------------------------
R::RCursor<GDocRetrieved> GMetaEngine::GetDocs(void)
{
	return(R::RCursor<GDocRetrieved>(Results));
}


//------------------------------------------------------------------------------
GMetaEngine::~GMetaEngine(void)
{
}
