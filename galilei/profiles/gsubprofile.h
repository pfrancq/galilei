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

#ifndef GSubProfileH
#define GSubProfileH
//-----------------------------------------------------------------------------
// include files for GALILEI
#include <glangs/glang.h>
#include <gdocs/gwordcalcs.h>
#include <gdocs/gwordlist.h>
#include <gprofiles/gprofdoc.h>
#include <gprofiles/gprofile.h>
 #include <time.h>

using namespace GALILEI;
//-----------------------------------------------------------------------------
// include files for Rainbow

//-----------------------------------------------------------------------------
namespace GALILEI{

class GProfDoc;
class GProfile;
class GWordCalcs;
//-----------------------------------------------------------------------------
/**
* This class represents a sub-profile for a specific language.
* @author Pascal Francq.
* @short Sub-Profile class.
*/
class GSubProfile
{
public:
	GProfile *Owner;
	unsigned Id;     		 // Identifier
	GLang *Lang;          // Language
	GWordCalcs *MOK;		
	GWordCalcs *MKO;		
	GWordList *OK;        // List OK
	GWordList *KO;        // List KO
	GWordList *Common;    // List Common
   unsigned int NbOK;
	unsigned int NbKO;
	bool bToAttach;                   // Must Attach to group
	time_t Attached;

	GSubProfile(void) throw(bad_alloc);
	GSubProfile(GProfile *owner,unsigned int id,GLang *lang) throw(bad_alloc);
	bool NeedOK(void);
	bool NeedKO(void);
	int Compare(const unsigned int id);
	int Compare(const GLang* lang);
	int Compare(const GSubProfile &subprofile);
	int Compare(const GSubProfile *subprofile);	
	void Analyse(GProfDoc *profdoc);
	void Analyse(void);
	bool Similar(GSubProfile *sub);
	~GSubProfile(void);
};

}

#endif

