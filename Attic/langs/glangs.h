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
// include files for R Project
#include <rstd/rcontainer.h>


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
class GLangs : public RStd::RContainer<GLang,unsigned,true,true>
{
protected:

	/**
	* State of the dictionnaries and stoplists.
	*/
	bool bDics;

	/**
	* Stoplists of the different languages.
	*/
	GDicts* Stops;

	/**
	* Dictionnaries of the different languages.
	*/
	GDicts* Dics;

public:

	/**
	* Constructor.
	* @param nb             Number of Languages.
	*/
	GLangs(unsigned nb) throw(bad_alloc);

	/**
	* Get a pointer to a language while having its code.
	* @param code             Code of the language.
	* @returns Pointer to the language.
	*/
	GLang* GetLang(const char* code);

	/**
	* Get the number of languages treated by the system.
	* @returns Number of languages.
	*/
	unsigned int GetNbLangs(void) const
		{return(NbPtr);}

	/**
	* Get a pointer to the languages.
	* @returns Pointer to the languages.
	*/
	GLangs* GetLangs(void) {return(this);}

	/**
	* Get a cursor over the profiles used in the system.
	*/
	GLangCursor& GetLangsCursor(void);

	/**
	* Verify if the dictionnaries/stoplists are loaded.
	* @returns true, if loaded.
	*/
	bool IsDicsLoad(void) const {return(bDics);}

	/**
	* Load the dictionnaries and stoplists.
	*/
	void InitDics(void) throw(bad_alloc,GException);

	/**
	* Get a pointer to a dictionnary corresponding to a given language.
	* @param lang       Pointer to the language.
	* @returns Pointer to a dictionnary.
	*/
	GDict* GetDic(const GLang* lang) const throw(GException);

	/**
	* Get a pointer to a stoplist corresponding to a given language.
	* @param lang       Pointer to the language.
	* @returns Pointer to a stoplist.
	*/
	GDict* GetStop(const GLang* lang) const throw(GException);

	/**
	* Get the word corresponding to a given identificator for a language.
	* @param id         Identificator.
	* @param lang       Pointer to the corresponding language.
	* @returns Pointer to a C string.
	*/
	const char* GetWord(const unsigned int id,const GLang* lang);

	/**
	* Get the word corresponding to a given identificator of a dictionnary.
	* @param dic        Pointer to the corresponding dictionnary.
	* @returns Pointer to a C string.
	*/
	const char* GetWord(const unsigned int id,const GDict* dict) const;

	/**
	* Return the identifier of a new word of a dictionnary.
	* @param word           Word to find.
	* @param dict           Dictionnary.
	*/
	virtual unsigned int GetDicNextId(const char* word,const GDict* dict)=0;

protected:

	/**
	* Loading a dictionnary/stoplist.
	* @param code           Code of the languague.
	* @param stop           Is it a stop list.
	*/
	virtual void LoadDic(const char* code,bool s) throw(bad_alloc,GException)=0;

	/**
	* Load a specific word from a dictionnary.
	* @param id             Idenfificator of the word.
	* @param code           Code of the languague.
	*/
	virtual const char* LoadWord(const unsigned int id,const char* code)=0;

public:

	/**
	* Destructor.
	*/
	virtual ~GLangs(void);
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
