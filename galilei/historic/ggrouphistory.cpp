/*

	GALILEI Research Project

	GGroupHistory.cpp

	History of a given group  - Implementation.

	Copyright 2003 by the Universit� Libre de Bruxelles.

	Authors:
		David Wartel (dwartel@ulb.ac.be).

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
#include <historic/ggrouphistory.h>
#include <historic/gweightinfoshistory.h>
using namespace GALILEI;
using namespace R;



//------------------------------------------------------------------------------
//
//  GGroupHistory
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GGroupHistory::GGroupHistory(const unsigned int id,GLang* lang, GGroupsHistory* grps) throw(std::bad_alloc)
	: RContainer<GWeightInfosHistory,unsigned int,false,true>(20,10), Id(id), Lang(lang),
	  Modified(false), Parent(grps), Childrens(2,2)
{
}


//------------------------------------------------------------------------------
void GGroupHistory::AddSubProfile(GWeightInfosHistory* giwwh)
{
	InsertPtr(giwwh);
	giwwh->SetParent(this);
}


//------------------------------------------------------------------------------
void GGroupHistory::SetModified(bool b)
{
	Modified=b;
}


//------------------------------------------------------------------------------
void GGroupHistory::SetSubject(GSubject* sub)
{
	Subject=sub;
}


//------------------------------------------------------------------------------
void GGroupHistory::InsertChildren(GGroupHistory* grp)
{
	Childrens.InsertPtr(grp);
}


//------------------------------------------------------------------------------
int GGroupHistory::Compare(const GGroupHistory& grouph) const
{
	return(Id-grouph.Id);
}


//------------------------------------------------------------------------------
int GGroupHistory::Compare(const GGroupHistory* grouph) const
{
	return(Id-grouph->Id);
}


//------------------------------------------------------------------------------
int GGroupHistory::Compare(const unsigned int id) const
{
	return(Id-id);
}


//------------------------------------------------------------------------------
GGroupHistoryCursor GGroupHistory::GetChildrenCursor(void)
{
	GGroupHistoryCursor cur(Childrens);
	return(cur);
}


//------------------------------------------------------------------------------
GGroupHistory::~GGroupHistory(void)
{
}
