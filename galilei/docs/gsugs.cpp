/*

	GALILEI Research Project

	GSugs.cpp

	Suggestions - Implementation.

	Copyright 2005 by the Université libre de Bruxelles.

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
#include <docs/gsugs.h>
#include <docs/gdoc.h>
using namespace GALILEI;



//-----------------------------------------------------------------------------
//
// class GSugs
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GSugs::GSugs(tObjType type,unsigned int id,unsigned int max)
	: Type(type), Id(id), Docs(max)
{
	if((Type!=otProfile)||(Type!=otGroup))
		throw GException("Suggestions only for profiles or groups");
}


//-----------------------------------------------------------------------------
int GSugs::Compare(const GSugs& sugs) const
{
	return(Id-sugs.Id);
}


//-----------------------------------------------------------------------------
int GSugs::Compare(const GSugs* sugs) const
{
	return(Id-sugs->Id);
}


//-----------------------------------------------------------------------------
int GSugs::Compare(const unsigned id) const
{
	return(Id-id);
}


//-----------------------------------------------------------------------------
void GSugs::AddSugs(unsigned int id)
{
	Docs.Insert(id);
}


//-----------------------------------------------------------------------------
void GSugs::AddSugs(const GDoc* doc)
{
	Docs.Insert(doc->GetId());
}
