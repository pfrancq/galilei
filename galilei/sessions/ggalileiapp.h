/*

	GALILEI Research Project

	GGALILEIApp.h

	Application handling a GALILEI Session - Header.

	Copyright 2006 by the Université libre de Bruxelles.

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
#ifndef GGALILEIAppH
#define GGALILEIAppH


//------------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>
#include <gslotlog.h>


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* The GALILEIApp class represents a generic application running a session.
* @short GALILEI Application
* @author Pascal Francq
*/
class GGALILEIApp
{
	/**
	* A log file.
	*/
	GSlotLog* Log;

	/**
	* Debug file.
	*/
	R::RDebugXML* Debug;

	/**
	* Path where to search for plug-ins
	*/
	R::RContainer<R::RString,true,false> PlugInsPath;

	/**
	* Name of the file containing the configuration of the plug-ins.
	*/
	R::RString PlugInsConfigName;

	/**
	* The Log file.
	*/
	R::RString LogFileName;

	/**
	* The debug file.
	*/
	R::RString DebugFileName;

	/**
	* Configuration file readed?
	*/
	bool HasReadConfig;

	/**
	* Application initialized?
	*/
	bool HasInitApp;

	/**
	* The Session.
	*/
	GSession* Session;

public:

	/**
	* Construct the application.
	* @param argc            Number of arguments.
	* @param argv            Arguments.
	*/
	GGALILEIApp(int argc, char *argv[]);

	/**
	* Read the configuration file.
	*/
	void ReadConfig(const R::RString config);

	/**
	* Write the configuration file.
	*/
	void WriteConfig(const R::RString config);

	/**
	* Initialisation of the application.
	* @param dlg             Dialog boxes must be loaded too?
	*/
	void Init(bool dlg);

	/**
	* Create the session.
	*/
	GSession* CreateSession(void);

	/**
	* Destructor.
	*/
	virtual ~GGALILEIApp(void);
};


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
