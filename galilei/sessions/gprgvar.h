/*

	GALILEI Research Project

	GPrgInst.h

	Variable of a Program - Header.

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
#ifndef GPrgVarH
#define GPrgVarH


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/**
* The GPrgVar provides a class for a generic variable.
* @author Pascal Francq
* @short Program Variable.
*/
class GPrgVar
{
	/**
	* Name of the variable.
	*/
	RStd::RString Name;

	/**
	* Owner of the variable.
	*/
	GPrgVar* Owner;

public:

	/**
	* Create a variable.
	* @param name           Name.
	* @param owner          Owner.
	*/
	GPrgVar(const char* name,GPrgVar* owner) throw(bad_alloc);

	/**
	* Method needed by RStd::Rcontainer.
	*/
	int Compare(const GPrgVar* v) const;

	/**
	* Method needed by RStd::Rcontainer.
	*/
	int Compare(const char* v) const;

	/**
	* Assign some data to the variable.
	* @param data           Data.
	*/
	virtual void Assign(const void* data) throw(GException);

	/**
	* Destructor.
	*/
	virtual ~GPrgVar(void);
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif 
