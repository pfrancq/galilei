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
// class GMetaEngine
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GMetaEngine::GMetaEngine(GSession* session,GPlugInFactory* fac)
	: GPlugIn(session,fac), RDownloadFile(), ResultsByDocs(200), Results(200), Rankings(200)
{
}


//------------------------------------------------------------------------------
GDocFragment* GMetaEngine::AddResult(GDoc* doc,const GConceptRecord* rec,size_t pos,size_t spos,size_t first,size_t last,double ranking,GEngine* engine)
{
	if(!doc)
		mThrowGException("Unknown document");
	 if((ranking<0)||(ranking>1))
		mThrowGException("Ranking must be included in [0,1]");

	// Insert the fragment
	bool Exist;
	GDocRef* Ref(ResultsByDocs.GetInsertPtr(doc));
	GDocFragment* Fragment(Ref->AddFragment(rec,pos,spos,first,last,Exist));
	if(!Exist)
		engine->Results.InsertPtr(Fragment);

	// Insert a ranking for that fragment
	GDocFragmentRanks* Ranks(Rankings.GetInsertPtr(Fragment));
	GDocFragmentRank* Rank(Ranks->AddRanking(ranking,engine->GetName()));
	FragmentRankAdded(Rank,engine);
	return(Fragment);
}


//------------------------------------------------------------------------------
GDocFragment* GMetaEngine::AddResult(size_t docid,const GConceptRecord* rec,size_t pos,size_t spos,size_t first,size_t last,double ranking,GEngine* engine)
{
	// Find the document reference
	GDoc* Doc(Session->GetObj(pDoc,docid));
	return(AddResult(Doc,rec,pos,spos,first,last,ranking,engine));
}


//------------------------------------------------------------------------------
void GMetaEngine::FragmentRankAdded(GDocFragmentRank*s,GEngine*)
{
}


//------------------------------------------------------------------------------
void GMetaEngine::PrepareRequest(GSearchQuery* query)
{
	// Clear the previous results
	ResultsByDocs.Clear();
	Results.Clear();
	Rankings.Clear();
	RCastCursor<GPlugIn,GEngine> Cur(GALILEIApp->GetPlugIns<GEngine>("Engine"));
	for(Cur.Start();!Cur.End();Cur.Next())
		Cur()->Clear(this,query);
}


//------------------------------------------------------------------------------
void GMetaEngine::RequestEngines(GSearchQuery* query)
{
	RCastCursor<GPlugIn,GEngine> Cur(GALILEIApp->GetPlugIns<GEngine>("Engine"));
	for(Cur.Start();!Cur.End();Cur.Next())
		Cur()->Request(query);
}


//------------------------------------------------------------------------------
void GMetaEngine::ComputeGlobalRanking(void)
{

}


//------------------------------------------------------------------------------
void GMetaEngine::PostRequest(void)
{
}


//------------------------------------------------------------------------------
GSearchQuery* GMetaEngine::BuildQuery(const R::RString query)
{
	GSearchQuery* Query=new GSearchQuery(Session,true);
	Query->Build(query);
	return(Query);
}


//------------------------------------------------------------------------------
void GMetaEngine::Request(GSearchQuery* query)
{
	PrepareRequest(query);

	// Perform the request
	RequestEngines(query);

	// Compute the global rankings and Sort the results by rankings
	ComputeGlobalRanking();
	RCursor<GDocRef> Refs(ResultsByDocs);
	for(Refs.Start();!Refs.End();Refs.Next())
	{
		RCursor<GDocFragment> Fragment(Refs()->GetFragments());
		for(Fragment.Start();!Fragment.End();Fragment.Next())
		Results.InsertPtr(Fragment());
	}
	Rankings.ReOrder(GDocFragment::SortOrderRanking);

	PostRequest();
}


//-----------------------------------------------------------------------------
R::RCursor<GDocRef> GMetaEngine::GetResultsByDocs(void)
{
	return(R::RCursor<GDocRef>(ResultsByDocs));
}


//-----------------------------------------------------------------------------
R::RCursor<GDocFragment> GMetaEngine::GetResults(void)
{
	return(R::RCursor<GDocFragment>(Results));
}


//-----------------------------------------------------------------------------
R::RCursor<GDocFragmentRanks> GMetaEngine::GetRankings(void)
{
	return(R::RCursor<GDocFragmentRanks>(Rankings));
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
