/*

	GALILEI Research Project

	GALILEI.h

	GALILEI Header - Header.

	(C) 2001 by P. Francq.

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
#ifndef GALILEIH
#define GALILEIH


//-----------------------------------------------------------------------------
// include files for Rainbow
#include <rstd/rstd.h>
#include <rstd/rstring.h>
using namespace RStd;

const unsigned cNoRef=0xFFFFFFFF;

//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
* The GException class provides a basic representation for an exception.
* @author Pascal Francq
* @short Basic Exception.
*/
class GException
{
	/**
	* A code representing the exception.
	*/
	unsigned int Code;

	/**
	* Message holding some information.
	*/
	RString Msg;

public:

	/**
	* Predefined Constants for Errors.
	*/
	enum eErrorType{NoDefinedError=0,OpenSessionError=1,Languages=2,Dic=3,Docs=4};

	/**
	* Construct an exception.
	* @param str                      Message of the error.
	*/
	GException(const RString &str) throw(bad_alloc)
		: Msg(str) {}

	/**
	* Construct an exception.
	* @param code                     Code of the error.
	* @param str                      Message of the error.
	*/
	GException(const unsigned code,const RString &str) throw(bad_alloc)
		: Code(code), Msg(str) {}
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
