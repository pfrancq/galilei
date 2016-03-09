/*

	GALILEI Research Project

	GDocFragmentRanks.cpp

	Document Fragment's Ranking - Header.

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
// include files for ANSI C++
#include <math.h>


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <gdocfragment.h>
#include <gdocfragmentrank.h>
#include <gsession.h>
#include <gdoc.h>
using namespace std;
using namespace R;
using namespace GALILEI;



//-----------------------------------------------------------------------------
//
// class GDocFragmentRank
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GDocFragmentRank::GDocFragmentRank(GDocFragment* fragment,double ranking,const R::RString info,bool delfragment)
	: Fragment(fragment), DelFragment(delfragment), Ranking(ranking), Info(info)
{
 	if(!Fragment)
		mThrowGException("Cannot have a null document fragment reference");
}


//------------------------------------------------------------------------------
int GDocFragmentRank::Compare(const GDocFragmentRank& ranking) const
{
	int Comp(Fragment->Compare(*ranking.Fragment));
	if(!Comp)
		return(Comp);
	return(Info.Compare(ranking.Info));
}


//------------------------------------------------------------------------------
int GDocFragmentRank::Compare(const GDocFragment::Search& search) const
{
	int Comp(Fragment->Compare(search));
	return(Comp);
}


//------------------------------------------------------------------------------
int GDocFragmentRank::Compare(const R::RString& info) const
{
	return(Info.Compare(info));
}


//------------------------------------------------------------------------------
int GDocFragmentRank::SortOrderRanking(const void* a,const void* b)
{
	double af=(*((GDocFragmentRank**)(a)))->Ranking;
	double bf=(*((GDocFragmentRank**)(b)))->Ranking;

	if(af==bf) return(0);
	if(af>bf)
		return(-1);
	else
		return(1);
}


//------------------------------------------------------------------------------
GDocFragmentRank::~GDocFragmentRank(void)
{
	if(DelFragment)
	{
		delete Fragment;
		Fragment=0;
	}
}