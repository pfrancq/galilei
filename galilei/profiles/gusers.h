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


#ifndef GUsersH
#define GUsersH
//---------------------------------------------------------------------------
// include files for GALILEI
#include <gprofiles/guser.h>
#include <gsessions/gsession.h>
#include <gprofiles/gprofile.h>
#include <gprofiles/gsubprofile.h>

using namespace GALILEI;


namespace GALILEI{
//--------------------------------------------------------------------------

// include files for Rainbow
#include <rstd/rcontainer.h>

//---------------------------------------------------------------------------
class GUser;           //forward declaration
class GSession;
//class GProfile;
class GSubProfile;
//---------------------------------------------------------------------------
// class GUsers
class GUsers : public RStd::RContainer<GUser,unsigned,true,true>
{
public:
	GSession *Session;

	GUsers(unsigned int nb,GSession *session) throw(bad_alloc);
	template<class hprofile> GProfile* GetProfile(unsigned int userid,unsigned int profileid,const char* name);
	GSubProfile* GetSubProfile(unsigned int profileid);
	virtual void Load(void) throw(bad_alloc,GException)=0;
};
}
#endif

