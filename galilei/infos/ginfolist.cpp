/*

	GALILEI Research Project

	GInfoList.cpp

	List of information entities - Implementation.

	Copyright 2001-2003 by the Université Libre de Bruxelles.

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
#include <infos/ginfolist.h>
using namespace GALILEI;
using namespace R;



//------------------------------------------------------------------------------
//
// class GInfoList
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GInfoList::GInfoList(unsigned int id,GInfoType type) throw(std::bad_alloc)
	: GInfo(id,type), RContainer<GInfo,unsigned,true,true>(30,10)
{
}


//------------------------------------------------------------------------------
GInfoList::GInfoList(const GInfoList& i) throw(std::bad_alloc)
	: GInfo(i), RContainer<GInfo,unsigned,true,true>(i)
{
}


//------------------------------------------------------------------------------
GInfoList::GInfoList(const GInfoList* i) throw(std::bad_alloc)
	: GInfo(i), RContainer<GInfo,unsigned,true,true>(i)
{
}


//------------------------------------------------------------------------------
int GInfoList::Compare(const unsigned int& id) const
{
	return(Id-id);
}


//------------------------------------------------------------------------------
int GInfoList::Compare(const GInfoList* i) const
{
	return(Id-i->Id);
}


//------------------------------------------------------------------------------
int GInfoList::Compare(const GInfoList& i) const
{
	return(Id-i.Id);
}


//------------------------------------------------------------------------------
GInfoList& GInfoList::operator=(const GInfoList& i) throw(std::bad_alloc)
{
	RContainer<GInfo,unsigned,true,true>::operator=(i);
	GInfo::operator=(i);
	return(*this);
}


//------------------------------------------------------------------------------
bool GInfoList::IsSame(const GInfoList& l) const
{
	const GInfo** ptr=const_cast<const GInfo**>(Tab);
	const GInfo** ptr2=const_cast<const GInfo**>(l.Tab);
	unsigned int i=NbPtr+1;
	unsigned int j=l.NbPtr+1;

	while((--i)&&(--j))
	{
		if((*(ptr++))!=(*(ptr2++)))
			return(false);
	}
	return(true);
}


//------------------------------------------------------------------------------
GInfoList::~GInfoList(void)
{
}
