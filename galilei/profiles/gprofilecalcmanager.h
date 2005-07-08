/*

	GALILEI Research Project

	GProfileCalcManager.h

	Profile Computing Methods Manager - Header.

	Copyright 2001-2003 by the Universit�Libre de Bruxelles.

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
#ifndef GProfileCalcManagerH
#define GProfileCalcManagerH


//------------------------------------------------------------------------------
// include files for GALILEI
#include <sessions/galilei.h>
#include <sessions/gplugins.h>
#include <profiles/gprofilecalc.h>


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* The GProfileCalcManager class provides a representation for a manager
* responsible to manage all the profile computing methods.
* @author Pascal Francq
* @short Profile Computing Methods Manager.
*/
class GProfileCalcManager : public GPluginManager<GProfileCalcManager,GFactoryProfileCalc,GFactoryProfileCalcInit,GProfileCalc>
{
	/**
	* Method currently selected.
	*/
	GProfileCalc* Current;

public:

	/**
	* Construct the profile computing methods manager.
	*/
	GProfileCalcManager(void);

	/**
	* Set the current method.
	* @param name            Name of the method.
	*/
	void SetCurrentMethod(const char* name);

	/**
	* Get the current method.
	* @return Pointer to GProfileCalc.
	*/
	GProfileCalc* GetCurrentMethod(void);

	/**
	* Read config of the manager
	*/
	virtual void ReadConfig(RXMLTag* t);

	/**
	* Store config of the manager
	*/
	virtual void SaveConfig(R::RXMLStruct* xml,R::RXMLTag* t);

	/**
	* Destructor of the profile computing methods manager.
	*/
	virtual ~GProfileCalcManager(void);
};


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
