/*

	GALILEI Research Project

	GLinkCalcManager.h

	Link Methods Manager - Header.

	Copyright 2002-2003 by the Universit�Libre de Bruxelles.

	Authors:
		Vandaele Valery (vavdaele@ulb.ac.be).

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
#ifndef GLinkCalcManagerH
#define GLinkCalcManagerH


//------------------------------------------------------------------------------
// include files for GALILEI
#include <sessions/galilei.h>
#include <docs/glinkcalc.h>
#include <sessions/gplugins.h>


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* The GLinkCalcManager class provides a representation for a manager
* responsible to manage all the links computing methods.
* @author Vandaele Valery
* @short Link Methods Manager.
*/
class GLinkCalcManager : public GPluginManager<GLinkCalcManager,GFactoryLinkCalc,GFactoryLinkCalcInit,GLinkCalc>
{
	/**
	* Method currently selected.
	*/
	GLinkCalc* Current;

public:

	/**
	* Constructor of a manager.
	*/
	GLinkCalcManager(void);

	/**
	* Set the current method.
	* @param name            Name of the method.
	*/
	void SetCurrentMethod(const char* name);

	/**
	* Get the current method.
	* @return Pointer to GLinkCalc.
	*/
	GLinkCalc* GetCurrentMethod(void);

	/**
	* Read config of the manager
	*/
	virtual void ReadConfig(RXMLTag* t);

	/**
	* Store config of the manager
	*/
	virtual void SaveConfig(R::RXMLStruct* xml,R::RXMLTag* t);

	/**
	* Destructor of the manager.
	*/
	virtual ~GLinkCalcManager(void);
};


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
