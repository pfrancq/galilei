/*

	GALILEI Research Project

	GGroupsMng.cpp

	Manager of Groups - Implementation.

	Copyright 2001 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).

	Version $Revision$

	Last Modify: $Date$

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, write to the Free Software
	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

*/



//---------------------------------------------------------------------------
// include files for GALILEI
#include <groups/ggroupsmng.h>
#include <groups/ggroups.h>
#include <groups/ggroup.h>
#include <profiles/gsubprofile.h>
using namespace GALILEI;
using namespace RStd;



//-----------------------------------------------------------------------------
//
// class GGroupsMng
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GALILEI::GGroupsMng::GGroupsMng(unsigned int g)
	: Groups(g+g/2,g/2)
{
}


//-----------------------------------------------------------------------------
GGroupsCursor& GALILEI::GGroupsMng::GetGroupsCursor(void)
{
	GGroupsCursor *cur=GGroupsCursor::GetTmpCursor();
	cur->Set(Groups);
	return(*cur);
}


//-----------------------------------------------------------------------------
GGroups* GALILEI::GGroupsMng::GetGroups(const GLang* lang) const
{
	return(Groups.GetPtr<const GLang*>(lang));
}


//-----------------------------------------------------------------------------
void GALILEI::GGroupsMng::ClearGroups(GLang* lang)
{
	unsigned int i;
	GGroups* grps=Groups.GetPtr<const GLang*>(lang);
	GGroup* grp;

	// Go through the groups and delete all invalid groups.
	for(i=grps->NbPtr+1;--i;)
	{
		grp=(*grps->Tab);
 		grp->DeleteSubProfiles();
		DeleteGroup(grp);
		grps->DeletePtr(grp);
	}
}


//-----------------------------------------------------------------------------
GALILEI::GGroupsMng::~GGroupsMng(void)
{
}
