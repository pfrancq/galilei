/*

	GALILEI Research Project

	GLangManager.h

	Manager for language plugins - Header.

	Copyright 2001-2003 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).

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



//------------------------------------------------------------------------------
#ifndef GLangManagerH
#define GLangManagerH


//------------------------------------------------------------------------------
// include files for GALILEI
#include <sessions/galilei.h>


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* The GLangManager class provides a representation for a manager for the languages
* handled by the system. In fact, each language is a plugin.
* @author Pascal Francq
* @short Languages.
*/
class GLangManager : public R::RContainer<GFactoryLang,unsigned,true,true>
{
public:

	/**
	* Constructor of the manager.
	* @param path            Path to find the plugins.
	* @param dlg             Load the existing dialog.
	*/
	GLangManager(const char* path,bool dlg=true) throw(bad_alloc,GException);

	/**
	* Connect to a Session.
	* @param session         Session.
	*/
	void Connect(GSession* session) throw(GException);

	/**
	* Disconnect from a Session.
	* @param session         Session.
	*/
	void Disconnect(GSession* session) throw(GException);

	/**
	* Find the language corresponding to a given code.
	* @param code             Code of the language.
	* @returns Pointer to GLang.
	*/
	GLang* GetLang(const char* code) const;

	/**
	* Create a cursor on the container of plugins representing the languages.
	* @param GFactoryLangCursor.
	*/
	GFactoryLangCursor& GetLangsCursor(void);

	/**
	* Destructor of the manager.
	*/
	virtual ~GLangManager(void);
};


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
