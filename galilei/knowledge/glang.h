/*

	GALILEI Research Project

	GLang.h

	Generic Language - Header.

	Copyright 2001-2015 by Pascal Francq (pascal@francq.info).
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
	 * The stopwords must be loaded.
	 */
	bool MustLoadStop;

	/**
	* Dictionaries of terms.
	*/
	GConceptType* Dict;

	/**
	 * The terms must be loaded.
	 */
	bool MustLoadDict;

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
	 * Compute the stem of a word. Of course, this method must be overloaded by
	 * the child classes.
	 * @param kwd            Word for which the stem must be computed.
	 * @return A R::RString representing the stem of the word.
	 */
	virtual R::RString GetStemming(const R::RString& kwd)=0;

	/**
	 * Compute the stems of a list of tokens. Tokens that corresponds to
	 * stopwords are ignored.
	 * @param tokens         Initial container of tokens.
	 * @param stems          List of stems for each token, except soptwords that
	 *                       are not represented anymore.
	 */
	void GetStemming(const R::RContainer<R::RString,true,false>& tokens,R::RContainer<R::RString,true,false>& stems);

	/**
	 * Create a new expression of a given type. Each token composing the
	 * expression is identified (eventually added to the dictionary of tokens)
	 * excluded the stopwords. These tokens are lowered and stemmed.
	 * @param expr           String containing the expression. It is trimmed.
	 * @param type           Type of the expression to create.
	 * @return the concept created to represent the token.
	 */
	GConcept* CreateExpression(const R::RString& expr,GConceptType* type);

	/**
	 * Get the name of the language (aka the plug-in).
    * @return a R::RString.
    */
	R::RString GetName(void) const {return(GPlugIn::GetName());}

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


//-------------------------------------------------------------------------------
/**
 * The GLangFactory is a factory for a language plug-in. The main element is a
 * specific method to get the stopwords, the name and the code of the language.
 *
 * In practice, if the CREATE_LANG_FACTORY macro is used (which is the right
 * thing to do), it is only necessary to create a static method GetStopWords in
 * the created plugin. Here is an example:
 * @code
 * void MyLanguage::GetStopWords(RContainer<RString,true,false>& stop)
 * {
 *   stop.InsertPtr(new RString("a"));
 *   stop.InsertPtr(new RString("able"));
 *   stop.InsertPtr(new RString("about"));
 * }
 * @endcode
 */
class GLangFactory : public GPlugInFactory
{
public:

	/**
	* Constructor.
	* @param mng             Manager of the plug-in.
	* @param name            Name of the language (the code).
	* @param desc            Description of the plug-in (the name of the
	*                        language).
	* @param lib             Library of the Factory/Plug-in.
	*/
	GLangFactory(GPlugInManager* mng,const R::RString& name,const R::RString& desc,const R::RString& lib)
		: GPlugInFactory(mng,name,desc,lib,R::RString::Null)
	{
	}

	/**
	 * Fill a container with all the stop words associated with the language.
	 * @param stop           Container.
	 * @param code           Code of the language (must be filled by the
	 *                       function.
	 * @param name           Name of the language (must be filled by the
	 *                       function.
	 */
	virtual void CreateStopWords(R::RContainer<R::RString,true,false>& stop,R::RString& code,R::RString& name)=0;
};


//-------------------------------------------------------------------------------
#define CREATE_LANG_FACTORY(name,desc,plugin)                                                                         \
class TheFactory : public GALILEI::GLangFactory                                                                       \
{                                                                                                                     \
	static GALILEI::GPlugInFactory* Inst;                                                                             \
	TheFactory(GALILEI::GPlugInManager* mng,const char* t)                                                            \
		: GALILEI::GLangFactory(mng,name,desc,t)                                                                      \
	{                                                                                                                 \
	}                                                                                                                 \
	virtual ~TheFactory(void) {}                                                                                      \
	virtual void CreateStopWords(R::RContainer<R::RString,true,false>& stop,R::RString& thecode,R::RString& thename)  \
	{                                                                                                                 \
		thecode=name;                                                                                                 \
		thename=desc;                                                                                                 \
		plugin::GetStopWords(stop);                                                                                   \
	}                                                                                                                 \
	virtual void CreateConfig(void)                                                                                   \
	{                                                                                                                 \
		plugin::CreateParams(this);                                                                                   \
	}                                                                                                                 \
public:                                                                                                               \
	static GALILEI::GPlugInFactory* CreateInst(GALILEI::GPlugInManager* mng,const char* t)                            \
	{                                                                                                                 \
		if(!Inst)                                                                                                     \
			Inst = new TheFactory(mng,t);                                                                             \
		return(Inst);                                                                                                 \
	}                                                                                                                 \
	virtual const char* GetAPIVersion(void) const                                                                     \
		{return(API_PLUG_IN_VERSION);}                                                                                \
		                                                                                                              \
	virtual GALILEI::GPlugIn* NewPlugIn(GSession* session)                                                            \
	{                                                                                                                 \
		GALILEI::GLang* ptr(new plugin(session,this));                                                                \
		return(ptr);                                                                                                  \
	}                                                                                                                 \
};                                                                                                                    \
GALILEI::GPlugInFactory* TheFactory::Inst = 0;                                                                        \
                                                                                                                      \
extern "C" GALILEI::GPlugInFactory* FactoryCreate(GALILEI::GPlugInManager* mng,const char* t)                         \
{                                                                                                                     \
	return(TheFactory::CreateInst(mng,t));                                                                            \
}                                                                                                                     \
extern "C" const char* LibType(void)                                                                                  \
{                                                                                                                     \
	return("Lang");                                                                                                   \
}



}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
