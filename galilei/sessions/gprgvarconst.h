/*

	GALILEI Research Project

	GPrgInstConst.h

	Const Variable - Header.

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
#ifndef GPrgVarConstH
#define GPrgVarConstH


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>
#include <sessions/gprgvar.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/**
* The GPrgVarCons provides a class for a const variable.
* @author Pascal Francq
* @short Const Variable.
*/
class GPrgVarConst : public GPrgVar
{
public:

	/**
	* Create a variable.
	* @param owner          Owner.
	* @param value          Value
	*/
	GPrgVarConst(GPrgVar* owner,const char* value) throw(bad_alloc);

	/**
	* Assign some data to the variable.
	* @param data           Data.
	*/
	virtual void Assign(const void* data) throw(GException);

	/**
	* Get the value of the variable.
	* @param prg            Program.
	*/
	virtual const char* GetValue(GSessionPrg* prg) throw(GException);

	/**
	* Destructor.
	*/
	virtual ~GPrgVarConst(void);
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif 
