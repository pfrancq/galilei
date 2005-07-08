/*

	GALILEI Research Project

	GStatsCalcManager.h

	Sstatistics Methods Manager - Header.

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



//------------------------------------------------------------------------------
#ifndef GStatsCalcManagerH
#define GStatsCalcManagerH


//------------------------------------------------------------------------------
// include files for GALILEI
#include <sessions/galilei.h>
#include <sessions/gplugins.h>
#include <sessions/gstatscalc.h>


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* The GStatsCalcManager class provides a representation for a manager
* responsible to manage all the statistics.
* @author Pascal Francq
* @short Statistics Methods Manager.
*/
class GStatsCalcManager : public GPluginManager<GStatsCalcManager,GFactoryStatsCalc,GFactoryStatsCalcInit,GStatsCalc>
{
public:

	/**
	* Construct the statistics methods manager.
	*/
	GStatsCalcManager(void);

	/**
	* Get a particular statistics method.
	* @param name            Name of the method.
	* @return Pointer to GStatsCalc.
	*/
	GStatsCalc* Get(const char* name);

	/**
	* Read config of the manager
	*/
	virtual void ReadConfig(RXMLTag* t);

	/**
	* Store config of the manager
	*/
	virtual void SaveConfig(R::RXMLStruct* xml,R::RXMLTag* t);

	/**
	* Destructor of the statistics methods manager.
	*/
	virtual ~GStatsCalcManager(void);
};


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
