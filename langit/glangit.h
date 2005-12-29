/*

	GALILEI Research Project

	GLangIT.h

	Italian Language - Header.

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
#ifndef GLangITH
#define GLangITH


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>
#include <glang.h>


//-----------------------------------------------------------------------------
// include files for GALILEI_PLUGINS
#include <italian_stem.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
* The GLangIT class provides a representation for the italian language.
* @author David Wartel
* @short Italian language.
*/
class GLangIT : public GLang , public stemming::italian_stem<char, std::char_traits<char> >
{

public:

	/**
	* Constructor of the italian language.
	* @param fac             Factory.
	*/
	GLangIT(GFactoryLang* fac);

	/**
	* Function that return stemming of a word.
	* @param _kwd            Word to find the stemming.
	* @return The stemming of the word.
	*/
	virtual R::RString GetStemming(const R::RString& _kwd);


public:

	/**
	* Create the parameters.
	* @param params          Parameters to configure.
	*/
	static void CreateParams(GParams* params);

	/**
	* Destructor.
	*/
	virtual ~GLangIT(void);
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
