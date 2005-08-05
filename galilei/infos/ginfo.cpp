/*

	GALILEI Research Project

	GInfo.cpp

	Information entity - Implementation.

	Copyright 2001-2003 by the Universit�Libre de Bruxelles.

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



//------------------------------------------------------------------------------
// include files for GALILEI
#include <ginfo.h>
using namespace GALILEI;
using namespace R;



//------------------------------------------------------------------------------
//
// GInfo
//
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
GInfo::GInfo(unsigned int id,tInfoType type)
	: Id(id), Type(type)
{
}


//------------------------------------------------------------------------------
GInfo::GInfo(const GInfo& i)
	: Id(i.Id), Type(i.Type)
{
}


//------------------------------------------------------------------------------
int GInfo::Compare(const GInfo& i) const
{
	return(Id-i.Id);
}


//------------------------------------------------------------------------------
GInfo& GInfo::operator=(const GInfo& i)
{
	Id=i.Id;
	Type=i.Type;
	return(*this);
}


//------------------------------------------------------------------------------
float GInfo::Similarity(const GInfo& i) const
{
	return(Id-i.Id==0);
}


//------------------------------------------------------------------------------
float GInfo::Similarity(const GInfo* i) const
{
	return(Id-i->Id==0);
}


//------------------------------------------------------------------------------
float GInfo::DisSimilarity(const GInfo& i) const
{
	return(Id-i.Id!=0);
}


//------------------------------------------------------------------------------
float GInfo::DisSimilarity(const GInfo* i) const
{
	return(Id-i->Id!=0);
}


//------------------------------------------------------------------------------
GInfo::~GInfo(void)
{
}
