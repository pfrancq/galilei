/*

	GALILEI Research Project

	GPrgInstFor.h

	"for" Instruction - Header.

	Copyright 2002 by the Universit� Libre de Bruxelles.

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
	R::RString Var;

	/**
	* Values of the variable.
	*/
	R::RContainer<GPrgVar,unsigned int,true,false> Values;

	/**
	* List of all "Instructions" to execute.
	*/
	R::RContainer<GPrgInst,unsigned int,true,false> Insts;

	/**
	* Identation of for.
	*/
	unsigned int Tabs;

public:
	/**
	* Create a Instruction.
	* @param line           Rest of the line.
	* @param t              Identation.
	*/
	GPrgInstFor(char* line,unsigned int t) throw(bad_alloc);

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
	* Get the number of tabs.
	* @returns unsigned int.
	*/
	unsigned int GetTabs(void) const {return(Tabs);}

	/**
	* Destructor.
	*/
	virtual ~GPrgInstFor(void);
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif 
