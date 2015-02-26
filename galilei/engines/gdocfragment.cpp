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
GDocFragment::GDocFragment(GDoc* doc,size_t pos,size_t begin,size_t end,double ranking,const R::RString& info,const R::RDate& proposed)
	: Doc(doc), Pos(pos), Begin(begin), End(end), Ranking(ranking), Proposed(proposed), Info(info), WholeDoc(false)
{
	if(!Doc)
		mThrowGException("Cannot have a null document reference");
	if(Begin>End)
		mThrowGException("Invalid window positions");
}


//------------------------------------------------------------------------------
GDocFragment::GDocFragment(GDoc* doc,double ranking,const R::RString& info,const R::RDate& proposed)
	: Doc(Doc), Pos(0), Begin(0), End(0), Ranking(ranking), Proposed(proposed), Info(info), WholeDoc(true)
{
	if(!Doc)
		mThrowGException("Cannot have a null document reference");
	End=Doc->GetTree()->GetMaxPos();
	if(End>100)
		End=100;
}


//------------------------------------------------------------------------------
int GDocFragment::Compare(const GDocFragment& d) const
{
	int i(CompareIds(Doc->GetId(),d.Doc->GetId()));
	if(!i)
		return(CompareIds(Pos,d.Pos));
	return(i);
}


//------------------------------------------------------------------------------
int GDocFragment::Compare(const GDoc* doc) const
{
	int i(CompareIds(Doc->GetId(),doc->GetId()));
	if(!i)
	{
		if(WholeDoc)
			return(0);
		return(-1);
	}
	else
		return(i);
}


//------------------------------------------------------------------------------
int GDocFragment::Compare(const Search& search) const
{
	int i(CompareIds(Doc->GetId(),search.DocId));
	if(!i)
		return(CompareIds(Pos,search.Pos));
	return(i);
}


//------------------------------------------------------------------------------
R::RString GDocFragment::GetFragment(void)
{
	if(Fragment.IsEmpty())
	{
		GFilter* Filter(GALILEIApp->FindMIMEType(Doc));
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
int GDocFragment::SortOrderRanking(const void* a,const void* b)
{
	double af=(*((GDocFragment**)(a)))->Ranking;
	double bf=(*((GDocFragment**)(b)))->Ranking;

	if(af==bf) return(0);
	if(af>bf)
		return(-1);
	else
		return(1);
}


//------------------------------------------------------------------------------
GDocFragment::~GDocFragment(void)
{
}
