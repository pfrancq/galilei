/*

	GALILEI Research Project

	GUsers.cpp

	Users - Implementation.

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
// include files for R Project
#include <rstd/rcursor.h>
using namespace R;


//-----------------------------------------------------------------------------
//include files for GALILEI
#include<profiles/gusers.h>
#include<profiles/guser.h>
#include<profiles/gprofile.h>
#include<profiles/gsubprofile.h>
#include<profiles/gsubprofiles.h>
#include<profiles/gprofdoc.h>
using namespace GALILEI;



//-----------------------------------------------------------------------------
//
//  GUsers
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GALILEI::GUsers::GUsers(unsigned int u,unsigned int p) throw(bad_alloc)
	: RContainer<GUser,unsigned,true,true>(u,u/2), bUsers(false)
{
	Profiles=new RContainer<GProfile,unsigned int,true,true>(p,p/2);
	SubProfiles=new RContainer<GSubProfiles,unsigned int,true,true>(10,5);
}


//-----------------------------------------------------------------------------
GUserCursor& GALILEI::GUsers::GetUsersCursor(void)
{
	GUserCursor *cur=GUserCursor::GetTmpCursor();
	cur->Set(this);
	return(*cur);
}


//-----------------------------------------------------------------------------
void GALILEI::GUsers::InsertUser(GUser* usr) throw(bad_alloc)
{
	InsertPtr(usr);
}


//-----------------------------------------------------------------------------
GUser* GALILEI::GUsers::GetUser(unsigned int id)
{
	return(GetPtr<unsigned int>(id));
}


//-----------------------------------------------------------------------------
void GALILEI::GUsers::InsertProfile(GProfile* p) throw(bad_alloc)
{
	Profiles->InsertPtr(p);
}


//-----------------------------------------------------------------------------
GProfile* GALILEI::GUsers::GetProfile(const unsigned int id) const
{
	return(Profiles->GetPtr<unsigned int>(id));
}


//-----------------------------------------------------------------------------
GProfileCursor& GALILEI::GUsers::GetProfilesCursor(void)
{
	GProfileCursor *cur=GProfileCursor::GetTmpCursor();
	cur->Set(Profiles);
	return(*cur);
}


//-----------------------------------------------------------------------------
unsigned int GALILEI::GUsers::GetProfilesNb(void) const
{
	return(Profiles->NbPtr);
}


//-----------------------------------------------------------------------------
void GALILEI::GUsers::InsertSubProfile(GSubProfile* s) throw(bad_alloc)
{
	GLang* l;
	GSubProfiles* list;

	l=s->GetLang();
	list=SubProfiles->GetPtr<const GLang*>(l);
	if(!list)
		SubProfiles->InsertPtr(list=new GSubProfiles(l,Profiles->MaxPtr));
	list->InsertPtr(s);
}


//-----------------------------------------------------------------------------
GSubProfile* GALILEI::GUsers::GetSubProfile(const unsigned int id) const
{
	GSubProfilesCursor Cur;
	GSubProfile* ptr;

	Cur.Set(SubProfiles);
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		ptr=Cur()->GetSubProfile(id);
		if(ptr) return(ptr);
	}
	return(0);
}


//-----------------------------------------------------------------------------
GSubProfile* GALILEI::GUsers::GetSubProfile(const unsigned int id,GLang* lang) const
{
	return(SubProfiles->GetPtr<const GLang*>(lang)->GetSubProfile(id));
}


//-----------------------------------------------------------------------------
GSubProfileCursor& GALILEI::GUsers::GetSubProfilesCursor(GLang* lang)
{
	GSubProfiles* ptr=SubProfiles->GetPtr<const GLang*>(lang);
	if(ptr)
		return(ptr->GetSubProfilesCursor());
	GSubProfileCursor* cur=GSubProfileCursor::GetTmpCursor();
	cur->Set(static_cast<GSubProfiles*>(0));
	return(*cur);
}


//-----------------------------------------------------------------------------
void GALILEI::GUsers::ClearSubProfilesGroups(void)
{
	GSubProfilesCursor Cur;
	GSubProfileCursor Cur2;

	Cur.Set(SubProfiles);
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		Cur2.Set(Cur());
		for(Cur2.Start();!Cur2.End();Cur2.Next())
		{
			Cur2()->SetGroup(0);
		}
	}
}


//-----------------------------------------------------------------------------
GALILEI::GUsers::~GUsers(void)
{
	if(SubProfiles) delete SubProfiles;
	if(Profiles) delete Profiles;
}
