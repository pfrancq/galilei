/*

	GALILEI Research Project

	GSubProfileDesc.h

	Generic Subprofile Description Method - Header.

	Copyright 2002 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).

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
#ifndef GSubProfileDescH
#define GSubProfileDescH


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// forward class declaration
class GSubProfile;


//-----------------------------------------------------------------------------
/**
* The GSubProfileDesc class provides a representation for a description method
* for the subprofiles.
* @author Pascal Francq.
* @short Generic Subprofile Description Method.
*/
class GSubProfileDesc
{
protected:

	/**
	* Name of the Profile Description Method.
	*/
	RStd::RString ProfDescName;

	/**
	* Pointer a function used for creating a subprofile description.
	*/
	GSubProfile* (*Create)(GProfile* prof,unsigned int id,GLang*,GGroup*,const char*);

public:

	/**
	* Constructor of the subprofile description method.
	* @param name           Name of the method.
	* @param func           Function used to create such a subprofile
	*                       description.
	*/
	GSubProfileDesc(const char* name,GSubProfile* (*func)(GProfile* prof,unsigned int id,GLang*,GGroup*,const char*)) throw(bad_alloc);

	/**
	* Compare methods used by RStd::RContainer.
	*/
	int Compare(const GSubProfileDesc& desc) const;

	/**
	* Compare methods used by RStd::RContainer.
	*/
	int Compare(const GSubProfileDesc* desc) const;

	/**
	* Compare methods used by RStd::RContainer.
	*/
	int Compare(const char* name) const;

	/**
	* Get the name of the profile description method.
	* @returns Pointer to a C string.
	*/
	const char* GetProfDescName(void) {return(ProfDescName());}

	/**
	*Destructor
	*/
	virtual ~GSubProfileDesc(void);
};


//-----------------------------------------------------------------------------
/**
* The GSubProfileDescCursor class provides a way to go trough a set of
* sub-profile descriptions.
* @short SubProfile Descriptions Cursor
*/
CLASSCURSOR(GSubProfileDescCursor,GSubProfileDesc,unsigned int)


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif

