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
#ifndef GProfileH
#define GProfileH
//---------------------------------------------------------------------------
// include files for GALILEI
#include <gprofiles/gsubprofile.h>
#include <gprofiles/gprofdoc.h>
#include <gprofiles/guser.h>

using namespace GALILEI ;
//---------------------------------------------------------------------------
//include files for Rainbow
#include <rstd/rcontainer.h>
#include <rstd/rstring.h>

//---------------------------------------------------------------------------
//include file for ANSI C/C++
#include <time.h>
//---------------------------------------------------------------------------
namespace GALILEI{

class GUser;
//class GWordCalcs;
class GSubProfile;
//---------------------------------------------------------------------------
// class GProfile
class GProfile : public RStd::RContainer<GSubProfile,unsigned,true,true>
{
public:

	/**
	* The owner of the profile.
	*/
	GUser *Owner;

	unsigned Id;       		// Identifier
	RString Name;           // Name
	bool Modified;          // Profile modified since last analyse?
	RContainer<GProfDoc,unsigned,true,true> FdbkDocs; // Documents juged by profile
	time_t	Updated;			// Date of updated

	GProfile(GUser *owner) throw(bad_alloc);
	GProfile(GUser *owner,unsigned int id,RString &name) throw(bad_alloc);
	int Compare(const unsigned int &id);
	int Compare(const GProfile &profile);
	int Compare(const GProfile *profile);
	void Calc(void);
	virtual void Load(void) throw(bad_alloc,GException)=0;
	virtual void Save(void) throw(bad_alloc,GException)=0;
};
}
#endif
