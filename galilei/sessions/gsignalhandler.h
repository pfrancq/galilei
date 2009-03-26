/*

	GALILEI Research Project

	GSignalHandler.h

	Generic Signal Handler for a GALILEI Session - Header.

	Copyright 2003-2009 by Pascal Francq (pascal@francq.info).
	Copyright 2003-2008 by the Université Libre de Bruxelles (ULB).

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
#ifndef GSignalHandlerH
#define GSignalHandlerH


//------------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* The GSignalHandler class provides a representation for a handler of signals
* emit by a GALILEI session.
* @author Pascal Francq
* @short Generic Signal Handler.
*/
class GSignalHandler
{
public:

	/**
	* Constructor.
	*/
	GSignalHandler(void);

	/**
	* Compare method used by R::RContainer.
	*/
	int Compare(const GSignalHandler& handler) const;

	/**
	* A specific lang has changed.
	* @param lang            Language.
	* @param event           Event.
	*/
	virtual void Event(GLang* lang, tEvent event);

	/**
	* A specific document has changed.
	* @param doc             Document.
	* @param event           Event.
	*/
	virtual void Event(GDoc* doc, tEvent event);

	/**
	* A specific user has changed.
	* @param user            User.
	* @param event           Event.
	*/
	virtual void Event(GUser* user, tEvent event);

	/**
	* A specific profile has changed.
	* @param prof            Profile.
	* @param event           Event.
	*/
	virtual void Event(GProfile* prof, tEvent event);

	/**
	* A specific community has changed.
	* @param community       Community.
	* @param event           Event.
	*/
	virtual void Event(GCommunity* community, tEvent event);

	/**
	* A specific topic has changed.
	* @param topic           Topic.
	* @param event           Event.
	*/
	virtual void Event(GTopic* topic, tEvent event);

	/**
	* Destructor.
	*/
	virtual ~GSignalHandler(void);
};


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
