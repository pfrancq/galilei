/*

	GALILEI Research Project

	GSubProfiles.cpp

	List of SubProfiles for a given Language - Implementation.

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
#include <profiles/gsubprofiles.h>
#include <infos/glang.h>
#include <profiles/gsubprofile.h>
using namespace GALILEI;
using namespace R;



//-----------------------------------------------------------------------------
//
//  GSubProfiles
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GALILEI::GSubProfiles::GSubProfiles(GLang* lang,unsigned int s) throw(bad_alloc)
	: RContainer<GSubProfile,unsigned int,true,true>(s,s/2), Lang(lang)
{
}


//-----------------------------------------------------------------------------
int GALILEI::GSubProfiles::Compare(const GLang* lang) const
{
	return(Lang->Compare(lang));
}


//-----------------------------------------------------------------------------
int GALILEI::GSubProfiles::Compare(const GSubProfiles& s) const
{
	return(Lang->Compare(s.Lang));
}


//-----------------------------------------------------------------------------
int GALILEI::GSubProfiles::Compare(const GSubProfiles* s) const
{
	return(Lang->Compare(s->Lang));
}


//-----------------------------------------------------------------------------
void GALILEI::GSubProfiles::InsertSubProfile(GSubProfile* s) throw(bad_alloc)
{
	InsertPtr(s);
}


//-----------------------------------------------------------------------------
GSubProfile* GALILEI::GSubProfiles::GetSubProfile(const unsigned int id) const
{
	return(GetPtr<unsigned int>(id));
}


//-----------------------------------------------------------------------------
GSubProfileCursor& GALILEI::GSubProfiles::GetSubProfilesCursor(void)
{
	GSubProfileCursor *cur=GSubProfileCursor::GetTmpCursor();
	cur->Set(this);
	return(*cur);
}


//-----------------------------------------------------------------------------
GALILEI::GSubProfiles::~GSubProfiles(void)
{
}
