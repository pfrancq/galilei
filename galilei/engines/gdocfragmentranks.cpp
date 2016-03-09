/*

	GALILEI Research Project

	GDocFragmentRanks.cpp

	Document Fragment's Rankings - Header.

	Copyright 2008-2015 by Pascal Francq (pascal@francq.info).

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).

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



//-----------------------------------------------------------------------------
// include files for GALILEI
#include <gdocfragmentranks.h>
#include <gdocfragmentrank.h>
#include <gdocfragment.h>
#include <gsession.h>
#include <gdoc.h>
using namespace std;
using namespace R;
using namespace GALILEI;



//-----------------------------------------------------------------------------
//
// class GDocFragmentRanks
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GDocFragmentRanks::GDocFragmentRanks(GDocFragment* fragment)
	: R::RContainer<GDocFragmentRank,true,true>(10,10), Fragment(fragment), Ranking(0.0)
{
 	if(!Fragment)
		mThrowGException("Cannot have a null document fragment reference");
}


//------------------------------------------------------------------------------
int GDocFragmentRanks::Compare(const GDocFragmentRanks& fragment) const
{
	return(Fragment->Compare(*fragment.Fragment));
}


//------------------------------------------------------------------------------
int GDocFragmentRanks::Compare(const GDocFragment& fragment) const
{
	return(Fragment->Compare(fragment));
}


//------------------------------------------------------------------------------
GDocFragmentRank* GDocFragmentRanks::AddRanking(double ranking,const R::RString info)
{
	// Verify if the ranking exist for the information
	GDocFragmentRank* Rank;
	bool Find;
	size_t Pos(GetIndex(info,Find));
	if(Find)
	{
		Rank=(*this)[Pos];
		Rank->SetRanking(ranking);
	}
	else
		InsertPtrAt(Rank=new GDocFragmentRank(Fragment,ranking,info),Pos,false);

	return(Rank);
}


//-----------------------------------------------------------------------------
R::RCursor<GDocFragmentRank> GDocFragmentRanks::GetRankings(void) const
{
	return(R::RCursor<GDocFragmentRank>(*this));
}


//------------------------------------------------------------------------------
int GDocFragmentRanks::SortOrderRanking(const void* a,const void* b)
{
	double af=(*((GDocFragmentRanks**)(a)))->Ranking;
	double bf=(*((GDocFragmentRanks**)(b)))->Ranking;

	if(af==bf) return(0);
	if(af>bf)
		return(-1);
	else
		return(1);
}
