/*

	GALILEI Research Project

	GPrgClass.h

	Generic Class - Header.

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
#ifndef GPrgClassH
#define GPrgClassH


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>
#include <sessions/gprgfunc.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/**
* The GPrgClass provides a class for a generic class.
* @author Pascal Francq
* @short Generic Class.
*/
class GPrgClass
{
protected:

	/**
	* Name of the class.
	*/
	RStd::RString Name;

	/**
	* Variables.
	*/
	RStd::RContainer<GPrgVar,unsigned int,true,true> Vars;

	/**
	* Methods.
	*/
	RStd::RContainer<GPrgFunc,unsigned int,true,true> Methods;

public:

	/**
	* Create a class.
	* @param name           Name.
	*/
	GPrgClass(const char* name) throw(bad_alloc);

	/**
	* Method needed by RStd::Rcontainer.
	*/
	int Compare(const GPrgClass* c) const;

	/**
	* Method needed by RStd::Rcontainer.
	*/
	int Compare(const char* c) const;

	/**
	* Find a method.
	* @param method         Name of the method.
	* @returns Pointer to GPrgInst.
	*/
	GPrgFunc* GetMethod(const char* method) const;

	/**
	* Destructor.
	*/
	virtual ~GPrgClass(void);
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif 
