/*

	GALILEI Research Project

	GUser.cpp

	User - Implementation.

	(C) 2001 by P. Francq.

	Version $Revision$

	Last Modify: $Date$

*/



//-----------------------------------------------------------------------------
//include files for GALILEI
#include<gprofiles/guser.h>
#include <gprofiles/gprofile.h>
#include<gprofiles/gsubprofile.h>
#include<gprofiles/gprofdoc.h>
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



