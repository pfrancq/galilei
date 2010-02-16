/*

	GALILEI Research Project

	LangIT.h

	Italian Language - Header.

	Copyright 2001-2010 by the Snowball Project.
	Copyright 2001-2010 by Pascal Francq.
	Copyright 2001 by David Wartel.
	Copyright 2001-2008 by the Université Libre de Bruxelles (ULB).

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
// include files for R
#include <rtextencoding.h>


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>
#include <glang.h>


//-----------------------------------------------------------------------------
// include files for the Snowball Project
#include <libstemmer.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
* The GLangIT class provides a representation for the Italian language.
* @short Italian language.
*/
class GLangIT : public GLang
{
	/**
	 * Stemmer.
	 */
	struct sb_stemmer* Stemmer;

	/**
	 * Encoding.
	 */
	R::RTextEncoding* StdCodec;

public:

	/**
	* Constructor of the italian language.
	* @param fac             Factory.
	*/
	GLangIT(GPlugInFactory* fac);

	/**
	 * Fill a container with all the stop words associated with the language.
	 * @param stop           Container.
	 */
	virtual void GetStopWords(R::RContainer<R::RString,true,false>& stop);

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
	static void CreateParams(RConfig* params);

	/**
	* Destructor.
	*/
	virtual ~GLangIT(void);
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
