/*

	GALILEI Research Project

	GGroups.cpp

	Groups for a given language - Implementation.

	(C) 2001 by P. Francq.

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
// include files for R Project
#include <rstd/rcontainercursor.h>
using namespace RStd;


//-----------------------------------------------------------------------------
//include files for GALILEI
#include<groups/ggroups.h>
#include<langs/glang.h>
#include<groups/ggroup.h>
#include<profiles/gsubprofile.h>
#include<profiles/gusers.h>
#include<sessions/gsession.h>
using namespace GALILEI;



//-----------------------------------------------------------------------------
//
// class GGroups
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GALILEI::GGroups::GGroups(GLang* lang) throw(bad_alloc)
	: RContainer<GGroup,unsigned int,true,false>(20,10), Lang(lang)
{
}


//-----------------------------------------------------------------------------
int GALILEI::GGroups::Compare(const GGroups& groups) const
{
	return(Lang->Compare(groups.Lang));
}


//-----------------------------------------------------------------------------
int GALILEI::GGroups::Compare(const GGroups* groups) const
{
	return(Lang->Compare(groups->Lang));
}


//-----------------------------------------------------------------------------
int GALILEI::GGroups::Compare(const GLang* lang) const
{
	return(Lang->Compare(lang));
}


//-----------------------------------------------------------------------------
GGroupCursor& GALILEI::GGroups::GetGroupCursor(void)
{
	GGroupCursor *cur=GGroupCursor::GetTmpCursor();
	cur->Set(this);
	return(*cur);
}


//-----------------------------------------------------------------------------
GGroup* GALILEI::GGroups::GetGroup(const GSubProfile* sub) const
{
	RContainerCursor<GGroup,unsigned int,true,false> cur(this);

	for(cur.Start();!cur.End();cur.Next())
	{
		if(cur()->IsIn<const unsigned int>(sub->GetId()))
			return(cur());
	}
	return(0);
}
