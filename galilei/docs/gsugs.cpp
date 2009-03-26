/*

	GALILEI Research Project

	GSugs.cpp

	Suggestions - Implementation.

	Copyright 2005-2009 by Pascal Francq (pascal@francq.info).
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
#include <gdoc.h>
using namespace R;
using namespace GALILEI;



//-----------------------------------------------------------------------------
//
// class GSugs
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GSugs::GSugs(tObjType type,size_t id,size_t max)
	: Type(type), Id(id), Docs(max)
{
	if((Type!=otProfile)&&(Type!=otCommunity))
		throw GException("Suggestions only for profiles or groups");
}


//-----------------------------------------------------------------------------
int GSugs::Compare(const GSugs& sugs) const
{
	return(CompareIds(Id,sugs.Id));
}


//-----------------------------------------------------------------------------
int GSugs::Compare(const GSugs* sugs) const
{
	return(CompareIds(Id,sugs->Id));
}


//-----------------------------------------------------------------------------
int GSugs::Compare(const size_t id) const
{
	return(CompareIds(Id,id));
}


//-----------------------------------------------------------------------------
void GSugs::AddSugs(size_t id)
{
	Docs.Insert(id);
}


//-----------------------------------------------------------------------------
void GSugs::AddSugs(const GDoc* doc)
{
	Docs.Insert(doc->GetId());
}
