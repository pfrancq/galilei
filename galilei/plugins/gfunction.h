/*

	GALILEI Research Project

	GFunction.h

	Generic Function - Implementation.

	Copyright 2005-2012 by Pascal Francq (pascal@francq.info).
	Copyright 2005-2008 by the Université Libre de Bruxelles (ULB).

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
#ifndef GFunctionH
#define GFunctionH


//------------------------------------------------------------------------------
// include files for ANSI C/C++
#include <stdarg.h>


//------------------------------------------------------------------------------
// include files for GALILEI
#include <gplugin.h>
#include <gpluginmanager.h>


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* The GFunction class provides a representation for a specific function (for
* example a dictionary that returns if a given word is contained in it).
* @author Pascal Francq
* @short Functions.
*/
class GFunction : public GPlugIn
{
public:

	/**
	* Constructor of a function.
	* @param session         Session.
	* @param fac             Plug-in factory.
	*/
	GFunction(GSession* session,GPlugInFactory* fac);

	/**
	* Call a function. Both pointers are untyped : it is the caller
   * responsibility to send the right type for the parameter and to interpret
   * the corresponding response.
   *
   * In practice, a number is associated to a call in order to implement multiple
   * functions in one plug-in for efficiency (for example because all functions
   * need a same memory consuming structure).
   * @param number          Number of the particular function called.
	* @param param           Pointer to the parameter.
   * @param ret             Pointer to the result returned.
	*
	* Suppose that the method takes a string as first parameter and its length.
   * This method should be written:
	* @code
	* void Call(size_t number,void* param,void* ret)
	* {
   *   if(number!=1)
   *     ThrowGException("Only 1 is a valid function");
	*   R::RString* str(static_cast<R::RString*>(param));
   *   size_t* len(static_cast<size_t*>(ret));
   *   (*len)=str->GetLen();
	* }
	* @endcode
	* The method should be call:
	* @code
	* RString Str("This is an example");
	* size_t Len;
	* ...
	* ptr->Call(1,&Str,&Len);
	* cout<<Len<<endl;
	* @endcode
	*/
	virtual void Call(size_t number,void* param,void* ret)=0;

	/**
	* Exchange a particular information with the plug-in.
	* @param info           Information to get.
   */
	virtual R::RString Info(size_t info);

	/**
	* Destruct the function.
	*/
	virtual ~GFunction(void);
};


//-------------------------------------------------------------------------------
#define CREATE_FUNCTION_FACTORY(type,name,desc,plugin) \
	CREATE_FACTORY(GALILEI::GFunction,plugin,"Functions",type,name,desc)


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
