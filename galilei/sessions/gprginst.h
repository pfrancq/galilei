/*

	GALILEI Research Project

	GPrgInst.h

	Instruction of a Program - Header.

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
#ifndef GPrgInstH
#define GPrgInstH


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/**
* The GPrgInst provides a class for a generic instruction.
* @author Pascal Francq
* @short Program Instruction.
*/
class GPrgInst
{
public:

	/**
	* Create a Instruction.
	*/
	GPrgInst(void) throw(bad_alloc);

	/**
	* Method needed by RStd::Rcontainer.
	*/
	int Compare(const GPrgInst* t) const;

	/**
	* Method needed by RStd::Rcontainer.
	*/
	int Compare(const char* t) const;

	/**
	* Program holding the instruction.
	* @param prg            Program.
	* @param r              Receiver.
	*/
	virtual void Run(GSessionPrg* prg,GSlot* r) throw(GException);
	
	/**
	* Destructor.
	*/
	virtual ~GPrgInst(void);
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif 
