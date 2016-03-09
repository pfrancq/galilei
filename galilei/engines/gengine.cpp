/*

	GALILEI Research Project

	GEngine.cpp

	Search Engine - Implementation.

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
// include files for ANSI C/C++
#include <stdio.h>
#include <iostream>
#include <cstdlib>


//------------------------------------------------------------------------------
// include files for GALILEI
#include <gengine.h>
#include <gmetaengine.h>
#include <ggalileiapp.h>
#include <gcomputerank.h>
#include <gdocfragmentrank.h>
using namespace GALILEI;
using namespace R;
using namespace std;



//------------------------------------------------------------------------------
//
// class GEngine
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GEngine::GEngine(GSession* session,GPlugInFactory* fac)
	: GPlugIn(session,fac), Weight(1.0), Ranking(0), Results(200), MetaEngine(0)
{
}


//------------------------------------------------------------------------------
void GEngine::ApplyConfig(void)
{
	RankingMethod=FindParam<RParamValue>("RankingMethod")->Get();
	if(!RankingMethod.IsEmpty())
	{
		Ranking=GALILEIApp->GetPlugIn<GComputeRank>("ComputeRank",RankingMethod,0);
	}
}


//------------------------------------------------------------------------------
void GEngine::Clear(GMetaEngine* metaengine,GSearchQuery*,size_t caller)
{
	Results.Clear();
	MetaEngine=metaengine;
}


//------------------------------------------------------------------------------
GDocFragmentRank* GEngine::AddResult(GDoc* doc,const GConceptRecord* rec,size_t pos,size_t spos,size_t first,size_t last,double ranking,size_t caller)
{
	if(!MetaEngine)
		mThrowGException("Cannot call this method because no meta-engine valid");
	return(MetaEngine->AddResult(doc,rec,pos,spos,first,last,ranking,this,caller));
}


//------------------------------------------------------------------------------
GDocFragmentRank* GEngine::AddResult(size_t docid,const GConceptRecord* rec,size_t pos,size_t spos,size_t first,size_t last,double ranking,size_t caller)
{
	if(!MetaEngine)
		mThrowGException("Cannot call this method because no meta-engine valid");
 	return(MetaEngine->AddResult(docid,rec,pos,spos,first,last,ranking,this,caller));
}


//------------------------------------------------------------------------------
void GEngine::Request(GSearchQuery* query,size_t caller)
{
	PerformRequest(query,caller);
	if(Ranking)
		Ranking->Rank(this,query,caller);
}


//-----------------------------------------------------------------------------
R::RCursor<GDocFragmentRank> GEngine::GetResults(size_t caller)
{
	return(R::RCursor<GDocFragmentRank>(Results));
}


//-----------------------------------------------------------------------------
size_t GEngine::GetNbResults(size_t caller) const
{
	return(Results.GetNb());
}


//-----------------------------------------------------------------------------
size_t GEngine::GetTabResults(const GDocFragmentRank** tab,size_t caller) const
{
	return(Results.GetTab(tab));
}


//-----------------------------------------------------------------------------
size_t GEngine::GetTabResults(GDocFragmentRank** tab,size_t caller)
{
	return(Results.GetTab(tab));
}


//------------------------------------------------------------------------------
void GEngine::CreateConfig(void)
{
	InsertParam(new RParamValue("RankingMethod","None","Name of the ranking method."));
}


//------------------------------------------------------------------------------
GEngine::~GEngine(void)
{
}
