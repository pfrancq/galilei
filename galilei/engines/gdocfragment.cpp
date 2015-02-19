/*

	GALILEI Research Project

	GDocFragment.cpp

	Relevant Document for a Search - Implementation.

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
#include <gmetaengine.h>
#include <gdocfragment.h>
#include <gdoc.h>
#include <gdocref.h>
#include <gsession.h>
#include <gfilter.h>
using namespace GALILEI;
using namespace R;
using namespace std;



//------------------------------------------------------------------------------
//
// class GDocFragment::Search
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GDocFragment::Search::Search(size_t docid,size_t pos)
	: DocId(docid), Pos(pos)
{
}



//------------------------------------------------------------------------------
//
// class GDocFragment
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GDocFragment::GDocFragment(GDocRef* doc,size_t pos,size_t begin,size_t end,double ranking,const R::RString& engine)
	: Doc(doc), Pos(pos), Begin(begin), End(end), Ranking(0), Rankings(10,5)
{
	if(!Doc)
		mThrowGException("Cannot have a null document reference");
	if(Begin>End)
		mThrowGException("Invalid window positions");
	Rankings.InsertPtr(new GDocFragmentRank(Doc->GetDoc()->GetId(),ranking,engine));
}


//------------------------------------------------------------------------------
int GDocFragment::Compare(const GDocFragment& d) const
{
	int i(CompareIds(Doc->GetDoc()->GetId(),d.Doc->GetDoc()->GetId()));
	if(!i)
		return(CompareIds(Pos,d.Pos));
	return(i);
}


//------------------------------------------------------------------------------
int GDocFragment::Compare(const Search& search) const
{
	int i(CompareIds(Doc->GetDoc()->GetId(),search.DocId));
	if(!i)
		return(CompareIds(Pos,search.Pos));
	return(i);
}


//------------------------------------------------------------------------------
R::RString GDocFragment::GetFragment(void)
{
	if(Fragment.IsEmpty())
	{
		GFilter* Filter(GALILEIApp->FindMIMEType(Doc->GetDoc()));
		if(Filter)
			Fragment=Filter->GetTextFragment(this);
	}
	return(Fragment);
}


//------------------------------------------------------------------------------
void GDocFragment::SetRanking(double ranking)
{
	Ranking=ranking;
}


//------------------------------------------------------------------------------
void GDocFragment::AddRanking(double ranking,const R::RString engine)
{
	if(Doc)
		Rankings.InsertPtr(new GDocFragmentRank(Doc->GetDoc()->GetId(),ranking,engine));
	else
		Rankings.InsertPtr(new GDocFragmentRank(0,ranking,engine));
}


//-----------------------------------------------------------------------------
R::RCursor<GDocFragmentRank> GDocFragment::GetRankings(void) const
{
	return(R::RCursor<GDocFragmentRank>(Rankings));
}


//------------------------------------------------------------------------------
GDocFragment::~GDocFragment(void)
{
}
