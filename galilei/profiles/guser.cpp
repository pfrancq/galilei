/*

	GALILEI Research Project

	GUser.cpp

	User - Implementation.

	Copyright 2001-2003 by the Université Libre de Bruxelles.

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



//------------------------------------------------------------------------------
// include files for GALILEI
#include<profiles/guser.h>
#include<profiles/gprofile.h>
#include<profiles/gsubprofile.h>
#include<profiles/gprofdoc.h>
using namespace GALILEI;
using namespace R;



//------------------------------------------------------------------------------
//
//  GUser
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GUser::GUser(unsigned int id,const char* name,const char* fullname,unsigned int nb) throw(std::bad_alloc)
	: RContainer<GProfile,unsigned,false,true>(nb+nb/2+1,nb/2+1),Id(id),Name(name),
	  FullName(fullname)
{
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
void GUser::SetId(unsigned int id) throw(GException)
{
	if(Id==cNoRef)
		throw GException("Cannot assign cNoRef to a user");
	Id=id;
}


//------------------------------------------------------------------------------
RString& GUser::GetName(void) const
{
	RString* tmp=RString::GetString();

	(*tmp)=Name;
	return(*tmp);
}


//------------------------------------------------------------------------------
RString& GUser::GetFullName(void) const
{
	RString* tmp=RString::GetString();

	(*tmp)=FullName;
	return(*tmp);
}


//------------------------------------------------------------------------------
GUser::~GUser(void)
{
}
