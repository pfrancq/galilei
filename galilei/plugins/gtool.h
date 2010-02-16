/*

	GALILEI Research Project

	GTool.h

	Generic Tool - Header.

	Copyright 2008-2010 by Pascal Francq (pascal@francq.info).
	Copyright 2008 by the Université Libre de Bruxelles (ULB).

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
#ifndef GToolH
#define GToolH


//------------------------------------------------------------------------------
// include files for GALILEI
#include <gplugin.h>
#include <gpluginmanager.h>


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* The GTool class provides a generic tool that could be called.
* @author Pascal Francq
* @short Generic Tool.
*/
class GTool : public GPlugIn
{
public:

	/**
	* Construct the tool.
	* @param fac             Factory of the plug-in.
	*/
	GTool(GPlugInFactory* fac);

	/**
	* Run the tool.
	* @param slot            Slot for comments.
	*/
	virtual void Run(GSlot* slot)=0;

	/**
	* Destructor of the tool.
	*/
	virtual ~GTool(void);
};


//-------------------------------------------------------------------------------
#define CREATE_TOOL_FACTORY(name,desc,plugin)\
	CREATE_FACTORY(GALILEI::GTool,plugin,"Tool",R::RString::Null,name,desc)


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
