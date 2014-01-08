/*

	GALILEI Research Project

	GMetaEngine.cpp

	Meta Engine for extraction of results from different search engines - Implementation.

   Copyright 2003-2014 by Pascal Francq.
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
// include files for GALILEI
#include <gengine.h>
#include <gdoc.h>
#include <gsession.h>
#include <gmetaengine.h>
#include <gfilter.h>
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
	: GPlugIn(session,fac), RDownload(), Results(200)
{
}


//------------------------------------------------------------------------------
RString GMetaEngine::GetTextFragment(GDocFragment* fragment)
{
	if(!fragment->GetDoc())
		mThrowGException("Invalid document passed");

	// Find the filter for this document
	R::RSmartTempFile TmpFile;
	RURI File;
	GFilter* Filter(GALILEIApp->FindMIMEType(fragment->GetDoc()));

	// If it is not a local	file -> Download it
	if(fragment->GetURI().GetScheme()!="file")
	{
		File=TmpFile.GetName();
		DownloadFile(fragment->GetDoc()->GetURI(),File);
	}
	else
		File=fragment->GetDoc()->GetURI();

	return(Filter->GetTextFragment(fragment));
}


//------------------------------------------------------------------------------
void GMetaEngine::AddResult(size_t docid,size_t pos,size_t first,size_t last,double ranking,const GEngine* engine)
{
	if((ranking<0)||(ranking>1))
		mThrowGException("Ranking must be included in [0,1]");

	GDoc* Doc(Session->GetObj(pDoc,docid));
	RURI URI(Doc->GetURI());

	// Test if URI is already there
	bool Find;
	size_t idx(Results.GetIndex(GDocFragment::Search(URI,pos,first,last),Find));

	if(Find)
		Results[idx]->AddRanking(ranking,engine->GetName());
	else
		Results.InsertPtrAt(new GDocFragment(this,Doc,pos,first,last,ranking,engine->GetName()),idx,false);
}


//------------------------------------------------------------------------------
void GMetaEngine::AddResult(const RString& uri,const RString& title,const RString fragment,double ranking,const GEngine* engine)
{
	if((ranking<0)||(ranking>1))
		mThrowGException("Ranking must be included in [0,1]");

	// Test if URI is already there
	bool Find;
	size_t idx(Results.GetIndex(GDocFragment::Search(uri,0,0,cNoRef),Find));

	if(Find)
	{
		Results[idx]->AddRanking(ranking,engine->GetName());
	}
	else
		Results.InsertPtrAt(new GDocFragment(this,uri,title,fragment,ranking,engine->GetName()),idx,false);
}


//------------------------------------------------------------------------------
void GMetaEngine::SetRanking(GDocFragment* doc,double ranking)
{
	doc->Ranking=ranking;
}


//-----------------------------------------------------------------------------
R::RCursor<GDocFragment> GMetaEngine::GetResults(void)
{
	return(R::RCursor<GDocFragment>(Results));
}


//-----------------------------------------------------------------------------
size_t GMetaEngine::GetNbResults(void) const
{
	return(Results.GetNb());
}


//------------------------------------------------------------------------------
GMetaEngine::~GMetaEngine(void)
{
}
