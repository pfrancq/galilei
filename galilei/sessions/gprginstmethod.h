/*

	GALILEI Research Project

	GPrgInstMethod.h

	Method of a class - Header.

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
#ifndef GPrgInstMethodH
#define GPrgInstMethodH


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>
#include <sessions/gprginst.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/**
* The GPrgInstMethod provides a class for a class method.
* @author Pascal Francq
* @short Class Method.
*/
class GPrgInstMethod : public GPrgInst
{
protected:

	/**
	* Owner of the method.
	*/
	GSessionPrg* Owner;

public:

	/**
	* Create a Instruction.
	* @param name           Name.
	*/
	GPrgInstMethod(const char* name) throw(bad_alloc);

	/**
	* Program holding the instruction.
	* @param prg            Program.
	*/
	virtual void Run(GSessionPrg* prg) throw(GException);
	
	/**
	* Destructor.
	*/
	virtual ~GPrgInstMethod(void);
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif 
