/*

	GALILEI Research Project

	GLangs.h

	List of the languages - Header.

	Copyright 2001 by the Université Libre de Bruxelles.

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



//-----------------------------------------------------------------------------
#ifndef GLangsH
#define GLangsH

//-----------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/**
* The GLangs class provides a representation for a list of languages that are
* handle by the system.
* @author Pascal Francq
* @short Languages.
*/
class GLangs : public R::RContainer<GFactoryLang,unsigned,true,true>
{
public:

	/**
	* Constructor.
	* @param path            Path to find the plugins.
	* @param dlg             Load the existing dialog.
	*/
	GLangs(const char* path,bool dlg=true) throw(GException);

	/**
	* Connect to a Session.
	* @param session         The session.
	*/
	void Connect(GSession* session);

	/**
	* Disconnect from a Session.
	* @param session         The session.
	*/
	void Disconnect(GSession* session);

	/**
	* Get a pointer to a language while having its code.
	* @param code             Code of the language.
	* @returns Pointer to the language.
	*/
	GLang* GetLang(const char* code) const;

	/**
	* Get a cursor over the profiles used in the system.
	*/
	GFactoryLangCursor& GetLangsCursor(void);

	/**
	* Destructor.
	*/
	virtual ~GLangs(void);
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
