/*

	GALILEI Research Project

	gwordref.h

	Basic Information - Implementation.

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

#include <stdlib.h>

//include files for GALILEI
#include<gusers.h>

using namespace GALILEI;

//-----------------------------------------------------------------------------

//---------------------------------------------------------------------------
//
//  HGsers
//
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
GUsers::GUsers(unsigned int nb,GSession *session) throw(bad_alloc)
  : RContainer<GUser,unsigned,true,true>(nb+nb/2,nb/2),Session(session)
{
}


//---------------------------------------------------------------------------
GSubProfile* GUsers::GetSubProfile(unsigned int profileid)
{
	GSubProfile *sub;
	GUser *usr;

	for(Start();!End();Next())
	{
		usr=(*this)();
		for(usr->Start();!usr->End();usr->Next())
		{
			sub=(*usr)()->GetPtr<unsigned int>(profileid);
			if(sub) return(sub);
		}
	}
	return(0);
}



