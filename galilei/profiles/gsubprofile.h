/*

	GALILEI Research Project

	gsubprofile.h

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



//-----------------------------------------------------------------------------
#ifndef GSubProfileH
#define GSubProfileH


//-----------------------------------------------------------------------------
// include files for ANSI C/C++
#include <time.h>


//-----------------------------------------------------------------------------
// include files for R Project


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <glangs/glang.h>
#include <gdocs/gwordcalcs.h>
#include <gdocs/gwordlist.h>
#include <gprofiles/gprofdoc.h>
#include <gprofiles/gprofile.h>
using namespace GALILEI;



//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// forward declaration
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
	/**
	* Owner profile of the subprofile.
	*/
	GProfile *Owner;

	/**
	* Identifier
	*/
	unsigned Id;

	/**
	* Language
	*/
	GLang *Lang;

	GWordCalcs *MOK;
	GWordCalcs *MKO;
	
	/**
	* List OK.
	*/
	GWordList *OK;

	/**
	* List KO.
	*/
	GWordList *KO;

	/**
	* List Common.
	*/
	GWordList *Common;
	
	unsigned int NbOK;
	unsigned int NbKO;
	
	/**
	* Must Attach to group ? (T/F).
	*/
	bool bToAttach;

	time_t Attached;

	/**
	* Default Constructor.
	*/
	GSubProfile(void) throw(bad_alloc);


	/** Constructor of the subprofile.
	* @param owner          owner profile
	* @param id             identifier
	* @param lang           Language of the subprofile
	*/
	GSubProfile(GProfile *owner,unsigned int id,GLang *lang) throw(bad_alloc);

	/** boolean functions for calculation of OK, KO and Common lists.
	*/
	bool NeedOK(void);
	bool NeedKO(void);

	/**
	* Comparaison functions
	*/
	int Compare(const unsigned int id);
	int Compare(const GLang* lang);
	int Compare(const GSubProfile &subprofile);
	int Compare(const GSubProfile *subprofile);	

	/**
	* Analyse of the subprofile
	*/
	void Analyse(GProfDoc *profdoc);
	void Analyse(void);

	/**
	* Boolean function for calculation of similarity between SubProfiles.
	*/
	bool Similar(GSubProfile *sub);

	/**
	*Destructor
	*/
	~GSubProfile(void);
};

}  //-------- End of namespace GALILEI ----------------------------------------

//-----------------------------------------------------------------------------

#endif

