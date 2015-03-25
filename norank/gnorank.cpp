/*

	GALILEI Research Project

	GNoRank.cpp

	No Ranking Method - Implementation.

   Copyright 2015-2015 by Pascal Francq.

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
#include <gnorank.h>


//-----------------------------------------------------------------------------
// DEBUG Mode
const bool Debug=false;



//------------------------------------------------------------------------------
//
// class GNoRank
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GNoRank::GNoRank(GSession* session,GPlugInFactory* fac)
	: GComputeRank(session,fac)
{
}


//------------------------------------------------------------------------------
void GNoRank::Rank(GEngine*,GSearchQuery*)
{

}


//------------------------------------------------------------------------------
void GNoRank::CreateConfig(void)
{
//	InsertParam(new RParamValue("UseWeight",false));
}


//-----------------------------------------------------------------------------
void GNoRank::ApplyConfig(void)
{
//	PercentValue=FindParam<RParamValue>("PercentValue")->GetBool();
}


//------------------------------------------------------------------------------
CREATE_COMPUTERANK_FACTORY("NoRanking","No Ranking Method",GNoRank)
