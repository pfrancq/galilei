/*

	GALILEI Research Project

	GLangDE.h

	German Language - Header.

	Copyright 2001 by the Universit�Libre de Bruxelles.

	Authors:
		David Wartel (dwartel@ulb.ac.be).

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
#ifndef GLangDEH
#define GLangDEH


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <sessions/galilei.h>
#include <infos/glang.h>


//-----------------------------------------------------------------------------
// include files for GALILEI_PLUGINS
#include <german_stem.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
* The GLangDE class provides a representation for the german language.
* @author David Wartel
* @short German language.
*/
class GLangDE : public GLang , public stemming::german_stem<char, std::char_traits<char> >
{

public:

	/**
	* Constructor of the german language.
	* @param fac             Factory.
	*/
	GLangDE(GFactoryLang* fac) throw(std::bad_alloc);

	/**
	* Function that return stemming of a word.
	* @param _kwd            Word to find the stemming.
	* @return The stemming of the word.
	*/
	virtual R::RString GetStemming(const R::RString& _kwd) throw(GException);

public:

	/**
	* Create the parameters.
	* @param params          Parameters to configure.
	*/
	static void CreateParams(GParams* params);

	/**
	* Destructor.
	*/
	virtual ~GLangDE(void);
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
