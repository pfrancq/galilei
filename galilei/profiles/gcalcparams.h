/*

	GALILEI Research Project

	GCalcParams.h

	Generic Parameters for a Computing Method - Header.

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
#ifndef GCalcParamsH
#define GCalcParamsH


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/**
* The GCalcParams provides a representation for generic parameters for a
* grouping method.
* @author Pascal Francq
* @short Generic Computing Paramters.
*/
class GCalcParams
{
	/**
	* Name of the grouping.
	*/
	R::RString ComputingName;

public:

	/**
	* Constructor.
	* @param n              Name of the grouping.
	*/
	GCalcParams(const char* n) throw(bad_alloc);

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
	* Compare method used for R::RContainer.
	*/
	int Compare(const GCalcParams* grp) const;

	/**
	* Compare method used for R::RContainer.
	*/
	int Compare(const GCalcParams& grp) const;

	/**
	* Compare method used for R::RContainer.
	*/
	int Compare(const GProfileCalc* grp) const;

	/**
	* Compare method used for R::RContainer.
	*/
	int Compare(const char* name) const;

	/**
	* Assignment operator.
	* @param p              Parameters used as source.
	*/
	GCalcParams& operator=(const GCalcParams& src);

	/**
	* Destructor.
	*/
	virtual ~GCalcParams(void);
};


//-----------------------------------------------------------------------------
/**
* The GCalcParamsCursor class provides a way to go trough a set of grouping
* parameters.
* @short Computing Parameters Cursor
*/
CLASSCURSOR(GCalcParamsCursor,GCalcParams,unsigned int)


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
