/*

	GALILEI Research Project

	GUsers.cpp

	Users - Implementation.

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
#include<profiles/gusers.h>
#include<profiles/guser.h>
#include<profiles/gprofile.h>
#include<profiles/gsubprofile.h>
#include<profiles/gprofdoc.h>
using namespace GALILEI;



//-----------------------------------------------------------------------------
//
//  GGUsers
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GALILEI::GUsers::GUsers(unsigned int u,unsigned int p,unsigned int s) throw(bad_alloc)
	: RContainer<GUser,unsigned,true,true>(u,u/2), bUsers(false)
{
	Profiles=new RContainer<GProfile,unsigned int,true,true>(p,p/2);
	SubProfiles=new RContainer<GSubProfile,unsigned int,true,true>(p*s,(p/2)*s);
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
void GALILEI::GUsers::InsertSubProfile(GSubProfile* s) throw(bad_alloc)
{
	SubProfiles->InsertPtr(s);
}


//-----------------------------------------------------------------------------
GSubProfile* GALILEI::GUsers::GetSubProfile(const unsigned int id) const
{
	return(SubProfiles->GetPtr<unsigned int>(id));
}


//-----------------------------------------------------------------------------
GSubProfileCursor& GALILEI::GUsers::GetSubProfilesCursor(void)
{
	GSubProfileCursor *cur=GSubProfileCursor::GetTmpCursor();
	cur->Set(SubProfiles);
	return(*cur);
}

//-----------------------------------------------------------------------------
GALILEI::GUsers::~GUsers(void)
{
	if(SubProfiles) delete SubProfiles;
	if(Profiles) delete Profiles;
}
