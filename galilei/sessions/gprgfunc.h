/*

	GALILEI Research Project

	GPrgFunc.h

	Function of a Program - Header.

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
#ifndef GPrgFuncH
#define GPrgFuncH


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/**
* The GPrgFunc provides a class for a generic function.
* @author Pascal Francq
* @short Program Function.
*/
class GPrgFunc
{
protected:

	/**
	* Name of the function.
	*/
	R::RString Name;

public:

	/**
	* Create a function.
	* @param name           Name.
	*/
	GPrgFunc(const char* name) throw(bad_alloc);

	/**
	* Method needed by R::Rcontainer.
	*/
	int Compare(const GPrgFunc* t) const;

	/**
	* Method needed by R::Rcontainer.
	*/
	int Compare(const char* t) const;

	/**
	* Execute the function.
	* @param prg            Program.
	* @param r              Receiver.
	* @param args           Parameters.
	*/
	virtual void Run(GSessionPrg* prg,GSlot* r,R::RContainer<GPrgVar,unsigned int,true,false>* args) throw(GException);
	
	/**
	* Destructor.
	*/
	virtual ~GPrgFunc(void);
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif 
