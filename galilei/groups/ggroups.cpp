/*

	 Research Project

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
// include files for 
#include<groups/ggroups.h>
#include<groups/ggroupvector.h>
#include<infos/glang.h>
#include<profiles/gsubprofile.h>
using namespace GALILEI;
using namespace R;



//-----------------------------------------------------------------------------
//
// class GGroups
//
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
/**
* The GGroups class provides a representation for all the groups of a given
* language. The GGroups are ordered by languages.
* @author Pascal Francq
* @short Languages Groups.
*/
class GGroups::GGroupsLang : public R::RContainer<GGroup,unsigned int,false,true>
{
public:

	GLang* Lang;              // Language

	// Constructor and Compare methods.
	GGroupsLang(GLang* lang) throw(bad_alloc)
		: RContainer<GGroup,unsigned int,false,true>(20,10), Lang(lang) {}
	int Compare(const GGroupsLang* groups) const {return(Lang->Compare(groups->Lang));}
	int Compare(const GLang* lang) const {return(Lang->Compare(lang));}

	// Get the group of a given subprofile
	GGroup* GetGroup(const GSubProfile* sub);

	// Destructor
	~GGroupsLang(void) {}
};


//-----------------------------------------------------------------------------
GGroup* GGroups::GGroupsLang::GetGroup(const GSubProfile* sub)
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
//
// class GGroups
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GGroups::GGroups(unsigned int g)
	: RContainer<GGroup,unsigned int,true,true>(g+(g/2),g/2), GroupsLang(2,1)
{
}

//-----------------------------------------------------------------------------
GGroupCursor& GGroups::GetGroupsCursor(void)
{
	GGroupCursor *cur=GGroupCursor::GetTmpCursor();
	cur->Set(this);
	return(*cur);
}


//-----------------------------------------------------------------------------
GGroupCursor& GGroups::GetGroupsCursor(GLang* lang) throw(GException)
{
	GGroupsLang* ptr;
	GGroupCursor *cur=GGroupCursor::GetTmpCursor();

	ptr=GroupsLang.GetPtr<GLang*>(lang);
	if(!ptr)
	{
		cur->Clear();
		return(*cur);
	}
	cur->Set(ptr);
	return(*cur);
}




//-----------------------------------------------------------------------------
void GGroups::InsertGroup(GGroup* grp) throw(bad_alloc)
{
	GGroupsLang* groupsLang;

	InsertPtr(grp);
	groupsLang = GroupsLang.GetInsertPtr<GLang*>(grp->GetLang());
	groupsLang->InsertPtr(grp);
}


//-----------------------------------------------------------------------------
GGroup* GGroups::GetGroup(const GSubProfile* sub) throw(GException)
{
	GGroupsLang* groupsLang;

	groupsLang = GroupsLang.GetInsertPtr<GLang*>(sub->GetLang());
	if(!groupsLang)
		throw GException("No language defined");
	return(groupsLang->GetGroup(sub));
}


//-------------------------------------------------------------------------------
GGroup* GGroups::GetGroup(unsigned int id) throw(bad_alloc)
{
	GGroup* grp;

	grp=GetPtr<unsigned int>(id);
	return(grp);
}


//-----------------------------------------------------------------------------
unsigned int GGroups::GetNbGroups(GLang* lang) const
{
	GGroupsLang* grps;

	grps=GroupsLang.GetPtr<const GLang*>(lang);
	if(!grps)
		return(0);
	return(grps->NbPtr);
}


//-----------------------------------------------------------------------------
unsigned int GGroups::GetMaxId(void) const
{
	if(NbPtr) return(0);
	return(Tab[NbPtr]->GetId());
}


//-----------------------------------------------------------------------------
void GGroups::Clear(GLang* lang)
{
	unsigned int i;
	GGroupsLang* grps=GroupsLang.GetPtr<const GLang*>(lang);
	GGroup* grp;

	// Go through the groups and delete all invalid groups.
	if(!grps) return;
	for(i=grps->NbPtr+1;--i;)
	{
		grp=(*grps->Tab);
 		grp->DeleteSubProfiles();
		DeleteGroup(grp);
		grps->DeletePtr(grp);
	}
}


//-----------------------------------------------------------------------------
void GGroups::Clear(void) throw(GException)
{
	GroupsLang.Clear();
}



//-----------------------------------------------------------------------------
GGroups::~GGroups(void)
{
}
