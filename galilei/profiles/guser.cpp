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
using namespace GALILEI;
using namespace RStd;



//-----------------------------------------------------------------------------
//
//  GUser
//
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
GUser::GUser(const unsigned int id,const char* name) throw(bad_alloc)
  : RContainer<GProfile,unsigned,true,true>(20,10),Id(id),Name(name)
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



