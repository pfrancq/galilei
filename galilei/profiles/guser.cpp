/*

 GALILEI Research Project

 GUser.cpp

 User - Implementation.

 Copyright 2001-2008 by the Université Libre de Bruxelles.

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
#include <guser.h>
#include <gprofile.h>
#include <gsession.h>
using namespace GALILEI;
using namespace R;



//------------------------------------------------------------------------------
//
//  GUser
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GUser::GUser(unsigned int id,const R::RString name,const R::RString fullname,unsigned int nb)
 : RContainer<GProfile,false,true>(nb+nb/2+1,nb/2+1),Id(id),Name(name),
   FullName(fullname)
{
	if(Id!=cNoRef)
		GSession::Event(this,eObjNew);
}


//------------------------------------------------------------------------------
int GUser::Compare(const GUser &user) const
{
	return(Id-user.Id);
}


//------------------------------------------------------------------------------
int GUser::Compare(const GUser *user) const
{
	return(Id-user->Id);
}


//------------------------------------------------------------------------------
int GUser::Compare(const unsigned int id) const
{
	return(Id-id);
}


//------------------------------------------------------------------------------
int GUser::Compare(const R::RString& name) const
{
	return(Name.Compare(name));
}


//------------------------------------------------------------------------------
void GUser::SetId(unsigned int id)
{
	if(id==cNoRef)
		throw GException("Cannot assign cNoRef to a user");
	Id=id;
}


//------------------------------------------------------------------------------
GProfile* GUser::GetProfile(const RString& name) const
{
	return(GetPtr(name,false));
}


//------------------------------------------------------------------------------
R::RCursor<GProfile> GUser::GetProfiles(void)
{
	return(R::RCursor<GProfile>(*this));
}


//------------------------------------------------------------------------------
size_t GUser::GetNbProfiles(void) const
{
	return(this->GetNb());
}


//------------------------------------------------------------------------------
RString GUser::GetName(void) const
{
	return(Name);
}


//------------------------------------------------------------------------------
RString GUser::GetFullName(void) const
{
	return(FullName);
}


//------------------------------------------------------------------------------
void GUser::SetName(const RString& name,const RString& fullname)
{
	Name=name;
	if(fullname.IsEmpty())
		FullName=name;
	else
		FullName=fullname;
}


//------------------------------------------------------------------------------
GUser::~GUser(void)
{
 GSession::Event(this,eObjDelete);
}
