/*

	GALILEI Research Project

	GGroupHistory.cpp

	History of Group for a given language  - Implementation.

	Copyright 2001 by the Université Libre de Bruxelles.

	Authors:
		David Wartel (dwartel@ulb.ac.be).

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
#include <historic/ggrouphistory.h>
#include <historic/giwordsweightshistory.h>
using namespace GALILEI;
using namespace R;



//-----------------------------------------------------------------------------
//
//  GGroupHistory
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GALILEI::GGroupHistory::GGroupHistory(const unsigned int id,GLang* lang, GGroupsHistory* grps) throw(std::bad_alloc)
	: RContainer<GIWordsWeightsHistory,unsigned int,false,true>(20,10)
{
	 Id=id;
	 Lang=lang;
	 Modified=false;
	 Parent=grps;
	 Childrens=new  RContainer<GGroupHistory,unsigned int,false,true>(2,1);
	 Parents=new  RContainer<GGroupHistory,unsigned int,false,true>(2,1);

}

//-----------------------------------------------------------------------------
unsigned int GALILEI::GGroupHistory::GetId(void)
{
	return Id;
}

//-----------------------------------------------------------------------------
void GALILEI::GGroupHistory::AddSubProfile(GIWordsWeightsHistory* giwwh)
{
	InsertPtr(giwwh);
	giwwh->SetParent(this);
}

//-----------------------------------------------------------------------------
void GALILEI::GGroupHistory::SetModified(bool b)
{
	Modified=b;
}

//-----------------------------------------------------------------------------
bool GALILEI::GGroupHistory::GetModified(void)
{
	return Modified;
}


//-----------------------------------------------------------------------------
void GALILEI::GGroupHistory::SetSubject(GSubject* sub)
{
	Subject=sub;
}


//-----------------------------------------------------------------------------
GSubject* GALILEI::GGroupHistory::GetSubject(void)
{
	return Subject;
}


//-----------------------------------------------------------------------------
void GALILEI::GGroupHistory::InsertChildren(GGroupHistory* grp)
{
	Childrens->InsertPtr(grp);
	grp->InsertParent(this);
}


//-----------------------------------------------------------------------------
void GALILEI::GGroupHistory::InsertParent(GGroupHistory* grp)
{
	Parents->InsertPtr(grp); 
}


//-----------------------------------------------------------------------------
int GALILEI::GGroupHistory::Compare(const unsigned int id) const
{
	return(Id-id);
}


//-----------------------------------------------------------------------------
int GALILEI::GGroupHistory::Compare(const GGroupHistory& grouph) const
{
	return(Id-grouph.Id);
}


//-----------------------------------------------------------------------------
int GALILEI::GGroupHistory::Compare(const GGroupHistory* grouph) const
{
	return(Id-grouph->Id);
}


//-----------------------------------------------------------------------------
GGroupHistoryCursor& GALILEI::GGroupHistory::GetChildrenCursor(void)
{
	GGroupHistoryCursor* cur=GGroupHistoryCursor::GetTmpCursor();
	cur->Set(Childrens);
	return(*cur);
}


//-----------------------------------------------------------------------------
GGroupHistoryCursor& GALILEI::GGroupHistory::GetParentsCursor(void)
{
	GGroupHistoryCursor* cur=GGroupHistoryCursor::GetTmpCursor();
	cur->Set(Parents);
	return(*cur);
}

//-----------------------------------------------------------------------------
GALILEI::GGroupHistory::~GGroupHistory(void)
{
}
