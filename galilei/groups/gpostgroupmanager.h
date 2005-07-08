/*

	GALILEI Research Project

	GPostGroupManager.h

	Post-Group Computing Methods Manager - Header.

	Copyright 2003 by the Universit�Libre de Bruxelles.

	Authors:
		Vandaele Valery(vavdaele@ulb.ac.be).

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
#ifndef GPostGroupManagerH
#define GPostGroupManagerH


//------------------------------------------------------------------------------
// include files for GALILEI
#include <sessions/galilei.h>
#include <sessions/gplugins.h>
#include <groups/gpostgroup.h>



//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* The GPostGroupManager class provides a representation for a manager
* responsible to manage all the postgroup computing methods.
* @author Vandaele Valery
* @short Post-Group Computing Methods Manager.
*/
class GPostGroupManager : public GPluginManager<GPostGroupManager,GFactoryPostGroup,GFactoryPostGroupInit,GPostGroup>
{
public:

	/**
	* Construct the post-group computing methods manager.
	*/
	GPostGroupManager(void);

	/**
	* Read config of the manager
	*/
	virtual void ReadConfig(RXMLTag* t);

	/**
	* Store config of the manager
	*/
	virtual void SaveConfig(R::RXMLStruct* xml,R::RXMLTag* t);

	/**
	* Destruct the post-group computing methods manager.
	*/
	virtual ~GPostGroupManager(void);
};


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
