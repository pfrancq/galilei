

#include <stdlib.h>

//include files for GALILEI
#include<guser.h>

using namespace GALILEI;

//-----------------------------------------------------------------------------

//---------------------------------------------------------------------------
//
//  GUser
//
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
GUser::GUser(GUsers *owner) throw(bad_alloc)
  : RContainer<GProfile,unsigned,true,true>(20,10),Owner(owner),Id(0),Name(25),SessNum(0)
{
}


//---------------------------------------------------------------------------
int GUser::Compare(const GUser &user)
{
  return(Id-user.Id);
}


//---------------------------------------------------------------------------
int GUser::Compare(const GUser *user)
{
  return(Id-user->Id);
}


//---------------------------------------------------------------------------
int GUser::Compare(const unsigned int id)
{
	return(Id-id);
}



