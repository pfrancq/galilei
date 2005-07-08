/*

	GALILEI Research Project

	GGroupCalcManager.h

	Group Comuting Methods Manager - Header.

	Copyright 2003 by the Universit�Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).

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
#ifndef GGroupCalcManagerH
#define GGroupCalcManagerH


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <sessions/galilei.h>
#include <sessions/gplugins.h>
#include <groups/ggroupcalc.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
* The GGroupCalcManager class provides a representation for a manager
* responsible to manage all the group computing methods.
* @author Pascal Francq
* @short Group Comuting Methods Manager.
*/
class GGroupCalcManager : public GPluginManager<GGroupCalcManager,GFactoryGroupCalc,GFactoryGroupCalcInit,GGroupCalc>
{
	/**
	* Method currently selected.
	*/
	GGroupCalc* Current;

public:

	/**
	* Construct the group computing method manager.
	*/
	GGroupCalcManager(void);

	/**
	* Set the current method.
	* @param name            Name of the method.
	*/
	void SetCurrentMethod(const char* name);

	/**
	* Get the current method.
	* @return Pointer to GGroupCalc.
	*/
	GGroupCalc* GetCurrentMethod(void);

	/**
	* Read config of the manager
	*/
	virtual void ReadConfig(RXMLTag* t);

	/**
	* Store config of the manager
	*/
	virtual void SaveConfig(R::RXMLStruct* xml,R::RXMLTag* t);

	/**
	* Destructor of the group computing methods manager.
	*/
	virtual ~GGroupCalcManager(void);
};


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
