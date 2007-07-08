/*

	GALILEI Research Project

	GLangData.cpp

	Language Specific Data - Implementation.

	Copyright 2007 by the Université libre de Bruxelles.

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
// include files for R Project
#include <rvectorint.h>
using namespace R;


//------------------------------------------------------------------------------
// include files for GALILEI
#include <glangdata.h>
#include <glang.h>
#include <gdoc.h>
#include <gsubprofile.h>
#include <ggroup.h>
using namespace GALILEI;


//------------------------------------------------------------------------------
//
// GLangData::Intern
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
struct GLangData::Intern
{
	RContainer<GDoc,false,true> Docs;
	RContainer<GSubProfile,false,true> SubProfiles;
	RContainer<GGroup,true,true> Groups;

	Intern(void) : Docs(10000), SubProfiles(10000), Groups(500)
	{}
};



//------------------------------------------------------------------------------
//
// class GLangData
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GLangData::GLangData(GLang* lang)
	: Data(0), Lang(lang)
{
	Data=new Intern();
}


//-----------------------------------------------------------------------------
int GLangData::Compare(const GLangData& specific) const
{
	if(!Lang)
	{
		if(!specific.Lang) return(0);
		return(-1);
	}
	if(!specific.Lang) return(1);
	return(Lang->Compare(*specific.Lang));
}


//-----------------------------------------------------------------------------
int GLangData::Compare(const GLang* lang) const
{
	if(!Lang)
	{
		if(!lang) return(0);
		return(-1);
	}
	if(!lang) return(1);
	return(Lang->Compare(lang));
}


//------------------------------------------------------------------------------
void GLangData::Clear(tObjType type)
{
	switch(type)
	{
		case otDoc:
			Data->Docs.Clear();
			break;
			
		case otSubProfile:
			Data->SubProfiles.Clear();
			break;
			
		case otGroup:
			Data->Groups.Clear();
			break;
			
		default:
			throw GException("Cannot clear "+GetObjType(type)+" from GLangData");
	}
	if(Lang)
		Lang->Clear(type);
}


//-----------------------------------------------------------------------------
R::RCursor<GDoc> GLangData::GetDocs(void) const
{
	return(R::RCursor<GDoc>(Data->Docs));
}


//-------------------------------------------------------------------------------
size_t GLangData::GetNbDocs(void) const
{
	return(Data->Docs.GetNb());
}



//------------------------------------------------------------------------------
void GLangData::InsertDoc(GDoc* doc)
{
	Data->Docs.InsertPtr(doc);
}


//------------------------------------------------------------------------------
void GLangData::DeleteDoc(GDoc* doc)
{
	Data->Docs.DeletePtr(doc);
}


//------------------------------------------------------------------------------
RCursor<GSubProfile> GLangData::GetSubProfiles(void) const
{
	return(RCursor<GSubProfile>(Data->SubProfiles));
}


//------------------------------------------------------------------------------
size_t GLangData::GetNbSubProfiles(void) const
{
	return(Data->SubProfiles.GetNb());
}


//------------------------------------------------------------------------------
size_t GLangData::GetMaxSubProfileId(void) const
{
	return(Data->SubProfiles[Data->SubProfiles.GetMaxPos()]->GetId());
}


//------------------------------------------------------------------------------
GSubProfile* GLangData::GetSubProfile(unsigned int id) const
{
	if(id>Data->SubProfiles.GetMaxPos())
		return(0);
	return(Data->SubProfiles[id]);
}


//------------------------------------------------------------------------------
void GLangData::AssignId(GSubProfile* sub) const
{
	// The first subprofile has the identificator 1
	if(Data->SubProfiles.GetNb())
		sub->SetId(Data->SubProfiles[Data->SubProfiles.GetMaxPos()]->GetId()+1);  // Not [GetNb()-1] because first subprofile has an identificator of 1
	else
		sub->SetId(1);
}


//------------------------------------------------------------------------------
void GLangData::InsertSubProfile(GSubProfile* sub)
{
	if(sub->GetId()==cNoRef)
		throw GException("Cannot insert a subprofile without an identificator");
	Data->SubProfiles.InsertPtrAt(sub,sub->GetId(),true);
}


//------------------------------------------------------------------------------
R::RCursor<GGroup> GLangData::GetGroups(void) const
{
	return(R::RCursor<GGroup>(Data->Groups));
}


//------------------------------------------------------------------------------
size_t GLangData::GetNbGroups(void) const
{
	return(Data->Groups.GetNb());
}


//------------------------------------------------------------------------------
GGroup* GLangData::GetGroup(unsigned int id) const
{
	return(Data->Groups.GetPtr(id));
}


//------------------------------------------------------------------------------
void GLangData::AssignId(GGroup* grp)
{
	// The first group has the identificator 1
	if(Data->Groups.GetNb())
		grp->SetId(Data->Groups[Data->Groups.GetMaxPos()]->GetId()+1);  // Not [GetNb()-1] because first group has an identificator of 1
	else
		grp->SetId(1);
}


//------------------------------------------------------------------------------
void GLangData::InsertGroup(GGroup* grp)
{
	Data->Groups.InsertPtr(grp);
}


//------------------------------------------------------------------------------
void GLangData::DeleteGroup(GGroup* grp)
{
	Data->Groups.DeletePtr(grp);
}


//------------------------------------------------------------------------------
GLangData::~GLangData(void)
{
	delete Data;
}
