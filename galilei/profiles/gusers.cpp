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
