/*

	GALILEI Research Project

	GUsers.cpp

	Users - Implementation.

	(C) 2001 by P. Francq.

	Version $Revision$

	Last Modify: $Date$

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
GALILEI::GUsers::GUsers(unsigned int nb) throw(bad_alloc)
	: RContainer<GUser,unsigned,true,true>(nb+nb/2,nb/2)
{
}


//-----------------------------------------------------------------------------
GSubProfile* GALILEI::GUsers::GetSubProfile(unsigned int profileid) const
{
	GSubProfile* sub;
	GUser* usr;
	RContainerCursor<GUser,unsigned,true,true> cur(this);

	for(cur.Start();!cur.End();cur.Next())
	{
		usr=cur();
		for(usr->Start();!usr->End();usr->Next())
		{
			sub=(*usr)()->GetPtr<unsigned int>(profileid,false);
			if(sub) return(sub);
		}
	}
	return(0);
}
