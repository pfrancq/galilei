/*

	GALILEI Research Project

	GLang.h

	Basic Language - Header.

	(C) 2001 by P. Francq.

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
#ifndef GLangH
#define GLangH


//-----------------------------------------------------------------------------
// include files for R Project
#include <rstd/rstring.h>
using namespace RStd;
#include <rinter/rlang.h>
using namespace RInter;


//-----------------------------------------------------------------------------
// include files for Galilei
#include <galilei.h>



//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
* The GLang class provides a representation for a basic language. The virtual
* function GetStemming must be implemented for the different languages.
*/
class GLang : public RInter::RLang
{
public:

	/**
	* Name of the language, for example "English".
	*/
//	RString Lang;

	/**
	* Code of the language, for example "en".
	*/
//	char Code[3];

	/**
	* Defines if the language is activ.
	*/
	bool Activ;

	/**
	* Constructor of a language.
	* @param lang           Name of the language.
	* @param code           Code of the language.
	*/
	GLang(const RString& lang,const char* code) throw(bad_alloc);

	/**
	* Comparaison function used by the Container.
	* @param lang           Pointer of the language used for comparaison.
	*/
	int Compare(const GLang& lang) const;

	/**
	* Comparaison function used by the Container.
	* @param lang           Language used of the comparaison.
	*/
	int Compare(const GLang* lang) const;

	/**
	* Compare function like strcmp used in particular for RContainer class.
	* @param code           Code used for the comparaison.
	*/
	int Compare(const char* code) const;

	/**
	* Function that return stemming of a word.
	* @param kwd            Word to find the stemming.
	* @return The stemming of the word.
	*/
	RString& GetStemming(const RString& kwd);

	/**
	* Destructor.
	*/
	virtual ~GLang(void);
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
