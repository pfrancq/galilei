/*

	GALILEI Research Project

	GTest.h

	Internal Testing - Header.

	Copyright 2008-2009 by Pascal Francq (pascal@francq.info).
	Copyright 2008 by the Université Libre de Bruxelles (ULB).

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



//------------------------------------------------------------------------------
#ifndef GTestH
#define GTestH


//------------------------------------------------------------------------------
// include files for R/GALILEI Project
#include <robject.h>
#include <rtextfile.h>
#include <galilei.h>


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
 * The GTest provides a class for internal testing purposes. It runs several
 * things (such as idf factors, similarities, etc.) and stores the details in a
 * text file. The tests are run on the current collections. It it supposed that
 * the different parts to test (documents analysis, profiling, etc.) are
 * already computed.
 * @author Pascal Francq
 * @short Internal Testing
 */
class GTest : public R::RObject
{
	/**
	 * Text file for the results.
	 */
	R::RTextFile Results;

public:

	/**
	 * Construct the testing class.
	 * @param file           Name of the file.
	 */
	GTest(const R::RString& file);

	/**
	 * Run the test.
	 */
	virtual void Run(void);

	/**
	 * Get a pointer to the text file that holds the details.
	 */
	R::RTextFile* GetFile(void) {return(&Results);}

	/**
	 * Destruct the testing class.
	 */
	virtual ~GTest(void);
};


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
