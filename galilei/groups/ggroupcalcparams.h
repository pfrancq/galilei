/*

	GALILEI Research Project

	GGroupCalcParams.h

	Generic Parameters for a Grouping Method - Header.

	Copyright 2001 by the Université Libre de Bruxelles.

	Authors:
		David Wartel (dwartel@ulb.ac.be).

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
#ifndef GGroupCalcParamsH
#define GGroupCalcParamsH


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/**
* The GGroupCalcParams provides a representation for generic parameters for a
* group description computing..
* @author David Wartel
* @short Group Description parameters Paramters.
*/
class GGroupCalcParams
{
	/**
	* Name of the grouping.
	*/
	RStd::RString ComputingName;

public:

	/**
	* Constructor.
	* @param n              Name of the grouping.
	*/
	GGroupCalcParams(const char* n) throw(bad_alloc);

	/**
	* Get the name of the grouping method.
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
	* Compare method used for RStd::RContainer.
	*/
	int Compare(const GGroupCalcParams* p) const;

	/**
	* Compare method used for RStd::RContainer.
	*/
	int Compare(const GGroupCalcParams& p) const;

	/**
	* Compare method used for RStd::RContainer.
	*/
	int Compare(const GGroupCalc* calc) const;

	/**
	* Compare method used for RStd::RContainer.
	*/
	int Compare(const char* name) const;

	/**
	* Assignment operator.
	* @param p              Parameters used as source.
	*/
	GGroupCalcParams& operator=(const GGroupCalcParams& src);

	/**
	* Destructor.
	*/
	virtual ~GGroupCalcParams(void);
};


//-----------------------------------------------------------------------------
/**
* The GGroupCalcParamsCursor class provides a way to go trough a set of grouping
* parameters.
* @short Grouping Parameters Cursor
*/
CLASSCURSOR(GGroupCalcParamsCursor,GGroupCalcParams,unsigned int)


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
