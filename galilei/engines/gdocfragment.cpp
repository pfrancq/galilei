/*

	GALILEI Research Project

	GDocFragment.cpp

	Relevant Document for a Search - Implementation.

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

#include "gmetaengine.h"




//------------------------------------------------------------------------------
// include files for ANSI C/C++
#include <stdio.h>
#include <iostream>
#include <cstdlib>


//------------------------------------------------------------------------------
// include files for GALILEI
#include <gdocfragment.h>
#include <gdoc.h>
#include <gsession.h>
using namespace GALILEI;
using namespace R;
using namespace std;



//------------------------------------------------------------------------------
//
// class GDocFragmentGDocFragment
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GDocFragment::Search::Search(const R::RURI& uri,size_t pos,size_t first,size_t last)
	: URI(uri), Pos(pos), First(first), Last(last)
{
}



//------------------------------------------------------------------------------
//
// class GDocFragment
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GDocFragment::GDocFragment(GMetaEngine* owner,GDoc* doc,size_t pos,size_t first,size_t last,double ranking,const R::RString& engine)
	: Owner(owner), Doc(doc), URI(doc->GetURI()()), Title(doc->GetName()),
	  Pos(pos), First(first), Last(last), Ranking(0), Rankings(10,5)
{
	if(!Owner)
		mThrowGException("No meta-engine assigned to document fragment");
	if(!Doc)
		mThrowGException("Invalid document passed");
	Rankings.InsertPtr(new GDocRanking(Doc->GetId(),ranking,engine));
}


//------------------------------------------------------------------------------
GDocFragment::GDocFragment(GMetaEngine* owner,const R::RString& uri,const R::RString& title,const RString& fragment,double ranking,const RString& engine)
	: Owner(owner), Doc(0), URI(uri), Title(title), Fragment(fragment), Pos(0),
	  First(0), Last(cNoRef), Ranking(0), Rankings(10,5)
{
	if(!Owner)
		mThrowGException("No meta-engine assigned to document fragment");
	Rankings.InsertPtr(new GDocRanking(0,ranking,engine));
}


//------------------------------------------------------------------------------
int GDocFragment::Compare(const GDocFragment& d) const
{
	return(Compare(Search(d.URI,d.Pos,d.First,d.Last)));
}


//------------------------------------------------------------------------------
int GDocFragment::Compare(const Search& search) const
{
	// Compare first the URI
	int i(URI.Compare(search.URI));
	if(i)
		return(i);

	// Then compare the position
	return(CompareIds(Pos,search.Pos));
}


//------------------------------------------------------------------------------
R::RString GDocFragment::GetFragment(void)
{
	if(Fragment.IsEmpty()&&Doc)
		Fragment=Owner->GetTextFragment(this);
	return(Fragment);
}


//------------------------------------------------------------------------------
void GDocFragment::AddRanking(double ranking,const R::RString engine)
{
	if(Doc)
		Rankings.InsertPtr(new GDocRanking(Doc->GetId(),ranking,engine));
	else
		Rankings.InsertPtr(new GDocRanking(0,ranking,engine));
}


//-----------------------------------------------------------------------------
R::RCursor<GDocRanking> GDocFragment::GetRankings(void) const
{
	return(R::RCursor<GDocRanking>(Rankings));
}


//------------------------------------------------------------------------------
GDocFragment::~GDocFragment(void)
{
}
