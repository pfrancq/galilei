/*

	GALILEI Research Project

	GMetaEngine.cpp

	Meta-Engine - Implementation.

   Copyright 2003-2015 by Pascal Francq (pascal@francq.info).
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
#include <gdocfragment.h>
#include <gdocfragmentrank.h>
#include <gdocfragmentranks.h>
#include <gdocref.h>
#include <gsearchquery.h>
using namespace GALILEI;
using namespace R;
using namespace std;



//------------------------------------------------------------------------------
//
// class GMetaEngine::Internal
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GMetaEngine::Internal::Internal(void)
	: ResultsByDocs(200), Results(200), Rankings(200)
{
}



//------------------------------------------------------------------------------
//
// class GMetaEngine
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GMetaEngine::GMetaEngine(GSession* session,GPlugInFactory* fac)
	: GPlugIn(session,fac), RDownloadFile()//, ResultsByDocs(200), Results(200), Rankings(200)
{
}


//------------------------------------------------------------------------------
GDocFragment* GMetaEngine::AddResult(GDoc* doc,const GConceptRecord* rec,size_t pos,size_t spos,size_t first,size_t last,double ranking,GEngine* engine,size_t caller)
{
	if(!doc)
		mThrowGException("Unknown document");
	 if((ranking<0)||(ranking>1))
		mThrowGException("Ranking must be included in [0,1]");

	// Insert the fragment
	bool Exist;
	GDocRef* Ref(Data.Get(caller)->ResultsByDocs.GetInsertPtr(doc));
	GDocFragment* Fragment(Ref->AddFragment(rec,pos,spos,first,last,Exist));
	if(!Exist)
		engine->Results.InsertPtr(Fragment);

	// Insert a ranking for that fragment
	GDocFragmentRanks* Ranks(Data.Get(caller)->Rankings.GetInsertPtr(Fragment));
	GDocFragmentRank* Rank(Ranks->AddRanking(ranking,engine->GetName()));
	FragmentRankAdded(Rank,engine,caller);
	return(Fragment);
}


//------------------------------------------------------------------------------
GDocFragment* GMetaEngine::AddResult(size_t docid,const GConceptRecord* rec,size_t pos,size_t spos,size_t first,size_t last,double ranking,GEngine* engine,size_t caller)
{
	// Find the document reference
	GDoc* Doc(Session->GetObj(pDoc,docid));
	return(AddResult(Doc,rec,pos,spos,first,last,ranking,engine,caller));
}


//------------------------------------------------------------------------------
void GMetaEngine::FragmentRankAdded(GDocFragmentRank*s,GEngine*,size_t caller)
{
}


//------------------------------------------------------------------------------
void GMetaEngine::PrepareRequest(GSearchQuery* query,size_t caller)
{
	// Clear the previous results
	Data.Get(caller)->ResultsByDocs.Clear();
	Data.Get(caller)->Results.Clear();
	Data.Get(caller)->Rankings.Clear();
	RCastCursor<GPlugIn,GEngine> Cur(GALILEIApp->GetPlugIns<GEngine>("Engine"));
	for(Cur.Start();!Cur.End();Cur.Next())
		Cur()->Clear(this,query,caller);
}


//------------------------------------------------------------------------------
void GMetaEngine::RequestEngines(GSearchQuery* query,size_t caller)
{
	RCastCursor<GPlugIn,GEngine> Cur(GALILEIApp->GetPlugIns<GEngine>("Engine"));
	for(Cur.Start();!Cur.End();Cur.Next())
		Cur()->Request(query,caller);
}


//------------------------------------------------------------------------------
void GMetaEngine::ComputeGlobalRanking(size_t caller)
{

}


//------------------------------------------------------------------------------
void GMetaEngine::PostRequest(size_t caller)
{
}


//------------------------------------------------------------------------------
GSearchQuery* GMetaEngine::BuildQuery(const R::RString query,size_t caller)
{
	GSearchQuery* Query=new GSearchQuery(Session,caller);
	Query->Build(query);
	return(Query);
}


//------------------------------------------------------------------------------
void GMetaEngine::Request(GSearchQuery* query,size_t caller)
{
	PrepareRequest(query,caller);

	// Perform the request
	RequestEngines(query,caller);

	// Compute the global rankings and Sort the results by rankings
	ComputeGlobalRanking(caller);
	RCursor<GDocRef> Refs(GetResultsByDocs(caller));
	for(Refs.Start();!Refs.End();Refs.Next())
	{
		RCursor<GDocFragment> Fragment(Refs()->GetFragments());
		for(Fragment.Start();!Fragment.End();Fragment.Next())
		Data.Get(caller)->Results.InsertPtr(Fragment());
	}
	Data.Get(caller)->Rankings.ReOrder(GDocFragment::SortOrderRanking);

	PostRequest(caller);
}


//-----------------------------------------------------------------------------
R::RCursor<GDocRef> GMetaEngine::GetResultsByDocs(size_t caller)
{
	return(R::RCursor<GDocRef>(Data.Get(caller)->ResultsByDocs));
}


//-----------------------------------------------------------------------------
R::RCursor<GDocFragment> GMetaEngine::GetResults(size_t caller)
{
	return(R::RCursor<GDocFragment>(Data.Get(caller)->Results));
}


//-----------------------------------------------------------------------------
R::RCursor<GDocFragmentRanks> GMetaEngine::GetRankings(size_t caller)
{
	return(R::RCursor<GDocFragmentRanks>(Data.Get(caller)->Rankings));
}


//-----------------------------------------------------------------------------
size_t GMetaEngine::GetNbResults(size_t caller)
{
	return(Data.Get(caller)->Results.GetNb());
}


//------------------------------------------------------------------------------
GMetaEngine::~GMetaEngine(void)
{
}
