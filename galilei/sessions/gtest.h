/*

	GALILEI Research Project

	GTest.h

	Test of SubProfiles Description and Grouping - Header.

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
#ifndef GTestH
#define GTestH


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
* The GTest class provides a representation of a test concerning the profiles
* description and their grouping.
* @author Pascal Francq
* @short Test.
*/
class GTest
{
	/**
	* Identificator of the test.
	*/
	RStd::tId Id;

	/**
	* Name of the Test.
	*/
	RStd::RString Name;

public:

	/**
	* Constructor.
	* @param id             Identificator.
	* @param name           Name of the test.
	*/
	GTest(const RStd::tId id,const char* name) throw(bad_alloc);

	/**
	* Compare function needed by RStd::RContainer.
	*/
	int Compare(const GTest* test);

	/**
	* Compare function needed by RStd::RContainer.
	*/
	int Compare(const GTest& test);

	/**
	* Compare function needed by RStd::RContainer.
	*/
	int Compare(const RStd::tId id);

	/**
	* Compare function needed by RStd::RContainer.
	*/
	int Compare(const RStd::RString& name);
	
	/**
	* Get the identificator of the test.
	*/
	RStd::tId GetId(void) {return(Id);}

	/**
	* Get the name of the test.
	*/
	const char* GetName(void) {return(Name());}

	/**
	* Destructor.
	*/
	~GTest(void);
};


//-----------------------------------------------------------------------------
/**
* The GTestCursor class provides a way to go trough a set of tests.
* @short Tests Cursor
*/
CLASSCURSOR(GTestCursor,GTest,unsigned int)


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
