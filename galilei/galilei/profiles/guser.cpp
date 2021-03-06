/*

	GALILEI Research Project

	GUser.cpp

	User - Implementation.

	Copyright 2001-2015 by Pascal Francq (pascal@francq.info).
	Copyright 2001-2008 by the Universit√© Libre de Bruxelles (ULB).

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
#include <guser.h>
#include <gprofile.h>
#include <gfdbk.h>
#include <gsession.h>
#include <gstatement.h>
using namespace GALILEI;
using namespace R;



//------------------------------------------------------------------------------
//
//  GUser
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GUser::GUser(GSession* session,size_t id,const R::RString& name,const R::RString& fullname,size_t nb)
 : GObject(session,id,name,otUser), RContainer<GProfile,false,true>(nb),
	FullName(fullname)
{
}


//------------------------------------------------------------------------------
int GUser::Compare(const GUser &user) const
{
	return(CompareIds(Id,user.Id));
}


//------------------------------------------------------------------------------
void GUser::SetId(size_t id)
{
	if(id==cNoRef)
		throw GException("Cannot assign cNoRef to a user");
	Id=id;
}


//------------------------------------------------------------------------------
GProfile* GUser::GetProfile(const RString& name) const
{
	RCursor<GProfile> Profile(*this);
	for(Profile.Start();!Profile.End();Profile.Next())
	{
		if(Profile()->GetName()==name)
			return(Profile());
	}
	return(0);
}


//------------------------------------------------------------------------------
R::RCursor<GProfile> GUser::GetProfiles(void)
{
	return(R::RCursor<GProfile>(*this));
}


//------------------------------------------------------------------------------
size_t GUser::GetNbProfiles(void) const
{
	return(GetNb());
}


//------------------------------------------------------------------------------
RString GUser::GetFullName(void) const
{
	return(FullName);
}


//------------------------------------------------------------------------------
GUser::~GUser(void)
{
	RContainer<GProfile,false,true> Tmp(*this);
	RCursor<GProfile> Profile(Tmp);
	for(Profile.Start();!Profile.End();Profile.Next())
		Session->DeleteObj(Profile());
}
