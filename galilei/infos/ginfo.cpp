/*

	GALILEI Research Project

	GInfo.cpp

	Basic Information - Implementation.

	Copyright 2001 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).

	Version $Revision$

	Last Modify: $Date$

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
#include <infos/ginfo.h>
using namespace GALILEI;
using namespace RStd;



//-----------------------------------------------------------------------------
//
// GInfo
//
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
GALILEI::GInfo::GInfo(void)
{
}


//-----------------------------------------------------------------------------
int GALILEI::GInfo::Compare(const GInfo* i) const
{
	return(this!=i);
}


//-----------------------------------------------------------------------------
float GALILEI::GInfo::Similarity(const GInfo* i) const
{
	return(this==i);
}


//-----------------------------------------------------------------------------
float GALILEI::GInfo::DisSimilarity(const GInfo* i) const
{
	return(this!=i);
}


//-----------------------------------------------------------------------------
const RString GALILEI::GInfo::ClassName(void) const
{
	return("GInfo");
}


//-----------------------------------------------------------------------------
const GALILEI::GInfo::GInfoType GALILEI::GInfo::InfoType(void) const
{
	return(infoNothing);
}


//-----------------------------------------------------------------------------
bool GALILEI::GInfo::SameClass(const GInfo* i) const
{
	return(InfoType()==i->InfoType());
}


//-----------------------------------------------------------------------------
GALILEI::GInfo::~GInfo(void)
{
}
