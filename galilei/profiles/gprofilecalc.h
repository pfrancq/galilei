/*

	GALILEI Research Project

	GProfileCalc.h

	Generic Profile Computing Method - Header.

	Copyright 2001-2002 by the Université Libre de Bruxelles.

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
#ifndef GProfileCalcH
#define GProfileCalcH


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/**
* The GProfileCalc class provides a representation for a method to compute a
* specific profile, i.e. its sub-profiles.
* @author Pascal Francq
* @short Generic Profile Computing Method.
*/
class GProfileCalc
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
	GProfileCalc(const char* name,GSession* session) throw(bad_alloc);

	/**
	* Set a parameter of the grouping method.
	* @param param          Name of the parameter.
	* @param value          Value of the parameter.
	*/
	virtual void SetParam(const char* param,const char* value);

	/**
	* Compute a profile.
	* @param profile        Profile to compute.
	*/
	virtual void Compute(GSubProfile* subprofile)=0;

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
	int Compare(const GProfileCalc& desc) const;

	/**
	* Compare methods used by R::RContainer.
	*/
	int Compare(const GProfileCalc* desc) const;

	/**
	* Compare methods used by R::RContainer.
	*/
	int Compare(const char* name) const;

	/**
	* Compare methods used by R::RContainer.
	*/
	int Compare(const tSubProfileDesc t) const;

	/**
	* Get the type of the method implemented. This is used to find the
	* correspondance between a profiles description and the compute method
	* associated.
	* @returns tSubProfileDesc enum type.
	*/
	virtual tSubProfileDesc GetType(void) const=0;

	/**
	* Destructor.
	*/
	virtual ~GProfileCalc(void);
};


//-----------------------------------------------------------------------------
/**
* The GProfileCalcCursor class provides a way to go trough a set of computing
* method for the profiles.
* @short Profiles Computing Methods Cursor
*/
CLASSCURSOR(GProfileCalcCursor,GProfileCalc,unsigned int)


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif

