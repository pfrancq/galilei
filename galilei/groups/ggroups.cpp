/*

	GALILEI Research Project

	GGroups.cpp

	Groups - Implementation.

	Copyright 2001-2003 by the Universit�Libre de Bruxelles.

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



//------------------------------------------------------------------------------
// include files for GALILEI
#include <ggroups.h>
#include <ggroup.h>
#include <glang.h>
#include <gdict.h>
#include <gsubprofile.h>
#include <gsession.h>
#include <gstorage.h>
using namespace GALILEI;
using namespace R;



//------------------------------------------------------------------------------
//
// class GGroups::GFreeId
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
class GGroups::GFreeId
{
public:
	unsigned int Id;          // Identificator.

	// Constructor and Compare methods.
	GFreeId(unsigned int id) : Id(id) {}
	int Compare(const GFreeId& f) const {return(Id-f.Id);}
	int Compare(unsigned int id) const {return(Id-id);}

	// Destructor
	~GFreeId(void) {}
};



//------------------------------------------------------------------------------
//
// class GGroups::GGroupsLang
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
class GGroups::GGroupsLang : public R::RContainer<GGroup,false,true>
{
public:

	GLang* Lang;              // Language

	// Constructor and Compare methods.
	GGroupsLang(GLang* lang)
		: RContainer<GGroup,false,true>(20,10), Lang(lang) {}
	int Compare(const GGroupsLang& groups) const {return(Lang->Compare(*groups.Lang));}
	int Compare(const GLang* lang) const {return(Lang->Compare(lang));}

	// Get the group of a given subprofile
	GGroup* GetGroup(const GSubProfile* sub);

	// Destructor
	~GGroupsLang(void) {}
};


//------------------------------------------------------------------------------
GGroup* GGroups::GGroupsLang::GetGroup(const GSubProfile* sub)
{
	R::RCursor<GGroup> Groups;

	Groups.Set(*this);
	for(Groups.Start();!Groups.End();Groups.Next())
	{
		if(Groups()->IsIn(sub))
			return(Groups());
	}
	return(0);
}



//------------------------------------------------------------------------------
//
// class GGroups
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GGroups::GGroups(unsigned int g)
	: RContainer<GGroup,true,true>(g+(g/2),g/2), GroupsLang(20,10),
	  FreeIds(50,25)
{
}


//------------------------------------------------------------------------------
R::RCursor<GGroup> GGroups::GetGroupsCursor(void) const
{
	R::RCursor<GGroup> cur(*this);
	return(cur);
}


//------------------------------------------------------------------------------
R::RCursor<GGroup> GGroups::GetGroupsCursor(GLang* lang)
{
	GGroupsLang* ptr;
	R::RCursor<GGroup> cur;

	ptr=GroupsLang.GetPtr<GLang*>(lang);
	if(!ptr)
	{
		cur.Clear();
		return(cur);
	}
	cur.Set(*ptr);
	return(cur);
}


//------------------------------------------------------------------------------
void GGroups::InsertGroup(GGroup* grp)
{
	GGroupsLang* groupsLang;

	InsertPtr(grp);
	groupsLang = GroupsLang.GetInsertPtr<GLang*>(grp->GetLang());
	groupsLang->InsertPtr(grp);
}


//------------------------------------------------------------------------------
void GGroups::DeleteGroup(GGroup* grp)
{
	GGroupsLang* groupsLang;

	groupsLang = GroupsLang.GetInsertPtr<GLang*>(grp->GetLang());
	groupsLang->DeletePtr(grp);
	FreeIds.InsertPtr(new GFreeId(grp->GetId()));
	DeletePtr(grp);
}


//------------------------------------------------------------------------------
GGroup* GGroups::GetGroup(const GSubProfile* sub)
{
	GGroupsLang* groupsLang;

	groupsLang = GroupsLang.GetInsertPtr<GLang*>(sub->GetLang());
	if(!groupsLang)
		throw GException("No language defined");
	return(groupsLang->GetGroup(sub));
}


//------------------------------------------------------------------------------
GGroup* GGroups::GetGroup(unsigned int id) const
{
	GGroup* grp;

	grp=GetPtr<unsigned int>(id);
	return(grp);
}


//------------------------------------------------------------------------------
unsigned int GGroups::GetNbGroups(GLang* lang) const
{
	GGroupsLang* grps;

	grps=GroupsLang.GetPtr<const GLang*>(lang);
	if(!grps)
		return(0);
	return(grps->GetNb());
}


//------------------------------------------------------------------------------
unsigned int GGroups::GetNewId(void) const
{
	// Is there a free identificator
	// -> Take the first one.
	if(FreeIds.GetNb())
	{
		unsigned int id=FreeIds[0]->Id;
		delete FreeIds[0];
		return(id);
	}
	if(!GetNb()) return(1);
	return((*this)[GetNb()-1]->GetId()+1);
}


//------------------------------------------------------------------------------
void GGroups::Clear(GLang* lang)
{
	size_t i,nb;
	GGroupsLang* grps=GroupsLang.GetPtr<const GLang*>(lang);
	GGroup* grp;

	// Go through the groups and delete all invalid groups.
	if(!grps) return;

	if(lang&&lang->GetDict())
		lang->GetDict()->Clear(otGroup);
	for(nb=grps->GetNb(),i=0;i<nb;i++)
	{
		grp=(*grps)[i];
		DeletePtr(grp);
	}
	grps->Clear();
}


//------------------------------------------------------------------------------
void GGroups::ClearGroups(void)
{
	RCursor<GGroupsLang> Groups(GroupsLang);
	for(Groups.Start();!Groups.End();Groups.Next())
	{
		Clear(Groups()->Lang);
	}
}


//------------------------------------------------------------------------------
void GGroups::UpdateGroups(GSubProfile* sub)
{
	// If there is a group -> propagate in memory
	if(sub->GetGroup())
		sub->GetGroup()->HasUpdate(sub);

	// Use database
	GSession* session=GSession::Get();
	if(session&&session->GetStorage()&&(!session->GetStorage()->MustLoadAll()))
		session->GetStorage()->UpdateGroups(sub);
}


//------------------------------------------------------------------------------
GGroups::~GGroups(void)
{
}
