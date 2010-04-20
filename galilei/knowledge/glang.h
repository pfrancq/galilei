/*

	GALILEI Research Project

	GLang.h

	Generic Language - Header.

	Copyright 2001-2010 by Pascal Francq (pascal@francq.info).
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



//------------------------------------------------------------------------------
#ifndef GLangH
#define GLangH


//------------------------------------------------------------------------------
// include files for R Project
#include <rlang.h>


//------------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>
#include <gplugin.h>
#include <gpluginmanager.h>


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* The GLang class provides a representation for a generic language. Each
* language has to be implemented as a plug-in. The virtual function GetStemming
* must be implemented for the different languages.
*
* Each language is composed from:
* - A name ("English") and a code ("en") ;
* - A set of dictionaries of concepts (stems, stopwords, ...);
* - A list of words to skip during the analysis.
* @author Pascal Francq.
* @short Generic Language.
*/
class GLang : public R::RLang, public GPlugIn
{
protected:
	class SkipWord;

	/**
	* Dictionaries of stopwords.
	*/
	GConceptType* Stop;

	/**
	* Dictionaries of stems.
	*/
	GConceptType* Dict;

	/**
	* List of word that must be skipped when there are part of a sequence
	* beginning with a number.
	*/
	R::RContainer<SkipWord,true,true> SkipWords;

public:

	/**
	* Constructor of a language.
	* @param session        Session.
	* @param fac            Factory.
	* @param lang           Name of the language.
	* @param code           Code of the language.
	*/
	GLang(GSession* session,GPlugInFactory* fac,const R::RString& lang,const char* code);

	/**
	 * @return the name of the language.
	 */
	R::RString GetLangName(void) const {return(RLang::GetName());}

	/**
	 * @return the name of the plug-in.
	 */
	R::RString GetPlugInName(void) const {return(GPlugIn::GetName());}

	/**
	* Compare two languages by comparing their code.
	* @see R::RContainer
	* @param lang            Language.
	* @return int
	*/
	int Compare(const GLang& lang) const;

	/**
	* Compare two languages by comparing their code.
	* @see R::RContainer
	* @param lang            Pointer to a language.
	* @return int
	*/
	int Compare(const GLang* lang) const;

	/**
	* Compare two languages by comparing their code.
	* @see R::RContainer
	* @param lang            Pointer to a language.
	* @return int
	*/
	int Compare(const R::RLang* lang) const;

	/**
	* Compare a code of a language with a string representing a code.
	* @see R::RContainer
	* @param code            Code.
	* @return int
	*/
	int Compare(const char* code) const;

	/**
	* During the analysis of a document, some words beginning with a number may
	* appear as valid with respect to the rules, but these words do not
	* represent usefull content (such as "1th", "2nd", "3nd", ...). Therefore,
	* it is possible to skip these words.
	*
	* This method add a word to the list to skip. In fact, only the sequence
	* after the numbers must be given (example "nd" for "2nd", "3nd", ...).
	* @param word            Word to skip.
	*/
	void SkipSequence(const R::RString& word);

	/**
	 * Fill a container with all the stop words associated with the language.
	 * @param stop           Container.
	 */
	virtual void GetStopWords(R::RContainer<R::RString,true,false>& stop)=0;

	/**
	* Function that computes the stem of a word. Of course, this method must be
	* overloaded by the child classes.
	* @param kwd            Word for which the stem must be computed.
	* @return A R::RString representing the stem of the word.
	*/
	virtual R::RString GetStemming(const R::RString& kwd)=0;

	/**
	* Get the session.
	* @return Pointer to GSession.
	*/
	GSession* GetSession(void) const {return(Session);}

	/**
	* Get the dictionary of words attached to the language.
	* @return Pointer to GDict.
	*/
	GConceptType* GetDict(void) const;

	/**
	* Get the dictionary of stopwords attached to the language.
	* @return Pointer to GDict.
	*/
	GConceptType* GetStop(void) const;

	/**
	* Look if a name corresponds to a data stored in the dictionary of
	* stopwords.
	* @param name            Name to lookup.
	* @return true if it was found in the dictionary of stopwords.
	*/
	bool InStop(const R::RString& name) const;

	/**
	* Look if a given sequence must be skipped.
	* @param seq             Sequence.
	* @return true if the sequence must be skipped.
	*/
	bool MustSkipSequence(const R::RChar* seq);

	/**
	* Destruct.
	*/
	virtual ~GLang(void);
};


//------------------------------------------------------------------------------
#define CREATE_LANG_FACTORY(name,desc,plugin)\
	CREATE_FACTORY(GLang,plugin,"Lang",R::RString::Null,name,desc)


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
