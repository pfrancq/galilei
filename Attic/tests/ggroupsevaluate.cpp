/*

	GALILEI Research Project

	GGroupsEvaluate.cpp

	GroupsEvaluate for a given language - Implementation.

	Copyright 2001 by the Université Libre de Bruxelles.

	Authors:
		Julien Lamoral (jlamoral@ulb.ac.be).

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
#include <tests/ggroupsevaluate.h>
#include <tests/ggroupevaluate.h>
#include <langs/glang.h>
using namespace GALILEI;
using namespace R;



//-----------------------------------------------------------------------------
//
// class GGroupsEvaluate
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GALILEI::GGroupsEvaluate::GGroupsEvaluate(GLang* lang) throw(bad_alloc)
	: RContainer<GGroupEvaluate,unsigned int,false,true>(20,10), Lang(lang)
{
}


//-----------------------------------------------------------------------------
int GALILEI::GGroupsEvaluate::Compare(const GGroupsEvaluate& groups) const
{
	return(Lang->Compare(groups.Lang));
}


//-----------------------------------------------------------------------------
int GALILEI::GGroupsEvaluate::Compare(const GGroupsEvaluate* groups) const
{
	return(Lang->Compare(groups->Lang));
}


//-----------------------------------------------------------------------------
int GALILEI::GGroupsEvaluate::Compare(const GLang* lang) const
{
	return(Lang->Compare(lang));
}


//-----------------------------------------------------------------------------
GGroupEvaluateCursor& GALILEI::GGroupsEvaluate::GetGroupEvaluateCursor(void)
{
	GGroupEvaluateCursor *cur=GGroupEvaluateCursor::GetTmpCursor();
	cur->Set(this);
	return(*cur);
}

