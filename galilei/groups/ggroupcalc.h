/*

	GALILEI Research Project

	GGroupCalc.h

	Generic Group Description Computing Method - Header.

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
#ifndef GGroupCalcH
#define GGroupCalcH


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/**
* The GGroupCalc class provides a representation for a method to compute the
* description of a specific group.
* @author Pascal Francq
* @short Generic Group Description Computing Method.
*/
class GGroupCalc
{
	/**
	* Name of the computing method.
	*/
	R::RString ComputingName;

protected:

	/**
	* Session.
	*/
	GSession* Session;

public:

	/**
	* Constructor.
	* @param name           Name.
	* @param session        Session.
	*/
	GGroupCalc(const char* name,GSession* session) throw(bad_alloc);

	/**
	* Compute a group.
	* @param grp            Group to compute.
	*/
	virtual void Compute(GGroup* grp)=0;

	/**
	* Get the name of the computing method.
	* @returns Pointer to a C string.
	*/
	const char* GetComputingName(void) const {return(ComputingName());}

	/**
	* Get the settings of the method coded in a string.
	* return Pointer to a C string.
	*/
	virtual const char* GetSettings(void) {return("");}

	/**
	* Set the settings for the method using a string.
	* @param char*          C string coding the settings.
	*/
	virtual void SetSettings(const char*) {}

	/**
	* Compare methods used by R::RContainer.
	*/
	int Compare(const GGroupCalc& desc) const;

	/**
	* Compare methods used by R::RContainer.
	*/
	int Compare(const GGroupCalc* desc) const;

	/**
	* Compare methods used by R::RContainer.
	*/
	int Compare(const char* name) const;

	/**
	* Get the name of the model used for the description.
	* @return C String.
	*/
	virtual const char* GetModelName(void) const=0;

	/**
	* Destructor.
	*/
	virtual ~GGroupCalc(void);
};


//-----------------------------------------------------------------------------
/**
* The GGroupCalcCursor class provides a way to go trough a set of group
* description method.
* @short Group Description Methods Cursor
*/
CLASSCURSOR(GGroupCalcCursor,GGroupCalc,unsigned int)


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif

