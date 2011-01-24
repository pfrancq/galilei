/*

	GALILEI Research Project

	GSugs.cpp

	Suggestions - Implementation.

	Copyright 2005-2011 by Pascal Francq (pascal@francq.info).
	Copyright 2005-2008 Université Libre de Bruxelles (ULB).

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
#include <gsugs.h>
#include <gsuggestion.h>
#include <gdoc.h>
using namespace R;
using namespace GALILEI;
using namespace std;



//-----------------------------------------------------------------------------
//
// class GSugs
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GSugs::GSugs(size_t max)
	: RContainer<GSuggestion,true,false>(max), Type(otCommunity), Id(0)
{
}


//-----------------------------------------------------------------------------
GSugs::GSugs(tObjType type,size_t id,size_t max)
	: RContainer<GSuggestion,true,false>(max), Type(type), Id(id)
{
	if((Type!=otProfile)&&(Type!=otCommunity))
		ThrowGException("Suggestions only for profiles or groups");
}


//-----------------------------------------------------------------------------
int GSugs::Compare(const GSugs& sugs) const
{
	if(Type!=sugs.Type)
		ThrowGException("Suggestions must be from the same type");
	return(CompareIds(Id,sugs.Id));
}


//-----------------------------------------------------------------------------
void GSugs::SetAddresseeId(tObjType type,size_t id)
{
	Id=id;
	Type=type;
	RContainer<GSuggestion,true,false>::Clear();
}
