/*

	GALILEI Research Project

	LangRU.h

	Russian Language - Header.

	Copyright 2001-2010 by the Snowball Project.
	Copyright 2008-2010 by Pascal Francq.

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
#ifndef GLangRUH
#define GLangRUH


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
* The GLangSE class provides a representation for the Russian language.
* @short Russian language.
*/
class GLangRU : public GLang
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
	* Constructor of the Rumanian language.
	* @param session         Session.
	* @param fac             Factory.
	*/
	GLangRU(GSession* session,GPlugInFactory* fac);

	/**
	 * Fill a container with all the stop words associated with the language.
	 * @param stop           Container.
	 */
	static void GetStopWords(R::RContainer<R::RString,true,false>& stop);

	/**
	* Function that return stemming of a word.
	* @param _kwd            Word to find the stemming.
	* @return The stemming of the word.
	*/
	virtual R::RString GetStemming(const R::RString& kwd);

public:

	/**
	* Create the parameters.
	*/
	static void CreateParams(GPlugInFactory*);

	/**
	* Destructor.
	*/
	virtual ~GLangRU(void);
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
