/*

	GALILEI Research Project

	GGroups.cpp

	Groups for a given language - Implementation.

	Copyright 2001 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).

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
#include<groups/ggroups.h>
#include<langs/glang.h>
#include<groups/ggroupvector.h>
#include<profiles/gsubprofile.h>
#include<profiles/gusers.h>
#include<sessions/gsession.h>
using namespace GALILEI;
using namespace R;



//-----------------------------------------------------------------------------
//
// class GGroups
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GALILEI::GGroups::GGroups(GLang* lang) throw(bad_alloc)
	: RContainer<GGroup,unsigned int,true,true>(20,10), Lang(lang)
{
}


//-----------------------------------------------------------------------------
GALILEI::GGroups::GGroups(GGroups* grps) throw(bad_alloc)
	: RContainer<GGroup,unsigned int,true,true>(grps->NbPtr,10),Lang(grps->Lang)
{
	GGroup* group;

	for(grps->Start();!grps->End();grps->Next())
		InsertPtr(new GGroup(group=(*grps)()));
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
GGroup* GALILEI::GGroups::GetGroup(const GSubProfile* sub)
{
	GGroupCursor Groups;

	Groups.Set(this);
	for(Groups.Start();!Groups.End();Groups.Next())
	{
		if(Groups()->IsIn<const unsigned int>(sub->GetId()))
			return(Groups());
	}
	return(0);
}


//-----------------------------------------------------------------------------
GGroup* GALILEI::GGroups::GetGroup(unsigned int id)
{
	GGroup* grp;

	grp=GetPtr<unsigned int>(id);
	if(!grp)
		InsertPtr(grp=new GGroupVector(id,Lang));
	return(grp);
}


//-----------------------------------------------------------------------------
GGroupCursor& GALILEI::GGroups::GetGroupCursor(void)
{
	GGroupCursor *cur=GGroupCursor::GetTmpCursor();
	cur->Set(this);
	return(*cur);
}


//-----------------------------------------------------------------------------
GGroup* GALILEI::GGroups::NewGroup(void) throw(bad_alloc)
{
	unsigned int id;
	GGroupCursor Groups;
	GGroup* grp;

	// Go through the groups to find the first not used index.
	Groups.Set(this);
	for(Groups.Start(),id=0;!Groups.End();Groups.Next(),id++)
		if(id!=Groups()->GetId())
			break;

	// Create the group with the founded index
	InsertPtr(grp=new GGroupVector(id,Lang));
	return(grp);
}


//-----------------------------------------------------------------------------
void GALILEI::GGroups::DeleteGroup(GGroup* grp)
{
	DeletePtr(grp);
}
