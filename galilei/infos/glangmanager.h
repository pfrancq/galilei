/*

	GALILEI Research Project

	GLangManager.h

	Manager for language plugins - Header.

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
#ifndef GLangManagerH
#define GLangManagerH


//------------------------------------------------------------------------------
// include files for GALILEI
#include <sessions/galilei.h>
#include <sessions/gplugin.h>


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
class GLangManager : public R::RContainer<GFactoryLang,true,true>, public GPluginManager
{
	/**
	* Must be the dictionnaries (and stoplists) be loaded.
	*/
	bool Load;

public:

	/**
	* Constructor of the manager.
	* @param path            Path to find the plugins.
	* @param load            Must the dictionnaries be loaded?
	* @param dlg             Load the existing dialog.
	*/
	GLangManager(R::RContainer<RString, true, false>* paths,bool load,bool dlg=true) throw(std::bad_alloc,GException);

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
	* Look if the dictionnaries must be loaded
	*/
	bool LoadDict(void) const {return(Load);}

	/**
	* Create a cursor on the container of plugins representing the languages.
	* @return GFactoryLangCursor.
	*/
	R::RCursor<GFactoryLang> GetLangsCursor(void);

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
	virtual ~GLangManager(void);
};


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
