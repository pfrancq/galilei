/*

	GALILEI Research Project

	GUser.cpp

	User - Implementation.

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
//include files for GALILEI
#include<profiles/guser.h>
#include<profiles/gprofile.h>
#include<profiles/gsubprofile.h>
#include<profiles/gprofdoc.h>
using namespace GALILEI;
using namespace RStd;



//-----------------------------------------------------------------------------
//
//  GUser
//
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
GUser::GUser(const unsigned int id,const char* name,const char* fullname,const unsigned int nb) throw(bad_alloc)
	: RContainer<GProfile,unsigned,false,true>(nb+nb/2+1,nb/2+1),Id(id),Name(name),
	  FullName(fullname)
{
}


//-----------------------------------------------------------------------------
int GALILEI::GUser::Compare(const GUser &user) const
{
	return(Id-user.Id);
}


//-----------------------------------------------------------------------------
int GALILEI::GUser::Compare(const GUser *user) const
{
	return(Id-user->Id);
}


//-----------------------------------------------------------------------------
int GALILEI::GUser::Compare(const unsigned int id) const
{
	return(Id-id);
}



