/*

	GALILEI Research Project

	GPrgInstFor.h

	"for" Instruction - Header.

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
#ifndef GPrgInstForH
#define GPrgInstForH


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>
#include <sessions/gprginst.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/**
* The GPrgInstFor provides a class for a "for" instruction.
* @author Pascal Francq
* @short "for" Instruction.
*/
class GPrgInstFor : public GPrgInst
{
	/**
	* Variable defined in the for.
	*/
	RStd::RString Var;

	/**
	* Values of the variable.
	*/
	RStd::RContainer<GPrgVar,unsigned int,true,false> Values;

	/**
	* List of all "Instructions" to execute.
	*/
	RStd::RContainer<GPrgInst,unsigned int,true,false> Insts;

public:
	/**
	* Create a Instruction.
	* @param line           Rest of the line.
	*/
	GPrgInstFor(char* line) throw(bad_alloc);

	/**
	* Add an instruction to the for.
	*/
	void AddInst(GPrgInst* ins) throw(bad_alloc);

	/**
	* Program holding the instruction.
	* @param prg            Program.
	* @param r              Receiver.
	*/
	virtual void Run(GSessionPrg* prg,GSlot* r) throw(GException);
	
	/**
	* Destructor.
	*/
	virtual ~GPrgInstFor(void);
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif 
