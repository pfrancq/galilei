/*

	GALILEI Research Project

	GLang.h

	Basic Language - Header.

	Copyright 2001 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).

	Version $Revision$

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
#include <rstd/rcursor.h>
#include <rstd/rstring.h>
#include <rinter/rlang.h>


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
* The GLang class provides a representation for a basic language. The virtual
* function GetStemming must be implemented for the different languages.
* @author Pascal Francq.
* @short Generic Language.
*/
class GLang : public RInter::RLang
{
protected:

	/**
	* Defines if the language is activ.
	*/
	bool Activ;

	double mult;

	/**
	* Dictionnary.
	*/
	GDict* Dict;

	/**
	* This class represents words that must be skipped when numbers are
	* presented. For example: 16nd, or 12h20.
	*/
	class SkipWord
	{
	public:
		RString Word;
	
		SkipWord(const char* w) : Word(w) {}
		int Compare(const SkipWord* w)
			{return(Word.Compare(w->Word));}
		int Compare(const SkipWord& w)
			{return(Word.Compare(w.Word));}
		int Compare(const char* w)
			{return(Word.Compare(w));}
	};

	RStd::RContainer<SkipWord,unsigned int,true,true> SkipWords;

public:

	/**
	* Constructor of a language.
	* @param lang           Name of the language.
	* @param code           Code of the language.
	*/
	GLang(const RStd::RString& lang,const char* code,GDict* dict) throw(bad_alloc);

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
	virtual RString& GetStemming(const RStd::RString& kwd);

	void SetMult(double m) {mult=m;}
	double GetMult() {return(mult);}

	/**
	* See if a given word is a valid one, don't content text and numbers that
	* are to skip.
	*/
	bool ValidWord(const RStd::RString& kwd);

	/**
	* Assign the dictionnary to the lang.
	*/
	void AssignDict(GDict* dict);

	/**
	* Get the dictionnary attached to the language.
	* @return Pointer to GDict.
	*/
	GDict* GetDict(void) {return(Dict);}

	/**
	* Get the number of references on a word.
	* @param id             Identificator.
	* @param ObjType        Type of the reference.
	* @returns unsigned int.
	*/
	unsigned int GetRef(unsigned int id,tObjType ObjType);

	unsigned int GetTotal();

	unsigned int GetNbWordList();

	/**
	* Get the total number of references.
	* @param ObjType        Type of the reference.
	* @returns unsigned int.
	*/
	unsigned int GetRef(tObjType ObjType,GWordType WordType);

	/**
	* Destructor.
	*/
	virtual ~GLang(void);
};


//-----------------------------------------------------------------------------
/**
* The GLangCursor class provides a way to go trough a set of languages.
* @short Language Cursor
*/
CLASSCURSOR(GLangCursor,GLang,unsigned int)


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
