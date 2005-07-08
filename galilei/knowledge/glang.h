/*

	GALILEI Research Project

	GLang.h

	Generic Language - Header.

	Copyright 2001-2003 by the Universit�Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).

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
#include <rstd/rlang.h>


//------------------------------------------------------------------------------
// include files for GALILEI
#include <sessions/galilei.h>
#include <sessions/gplugin.h>


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// API VERSION
#define API_LANG_VERSION "2.0"


//------------------------------------------------------------------------------
/**
* The GLang class provides a representation for a generic language. Each
* language has to be implemented as a plugin. The virtual function GetStemming
* must be implemented for the different languages.
*
* Each language is composed from :
* - A name ("English") and a code ("en") ;
* - A dictionary of data (words, list of words, ...);
* - A dictionary of stopwords ("of", "the", "or", "le", ...);
* - A list of words to skip during the analysis.
* @author Pascal Francq.
* @short Generic Language.
*/
class GLang : public R::RLang, public GPlugin<GFactoryLang>
{
protected:
	class SkipWord;

	/**
	* Session responsible for the dictionary.
	*/
	GSession* Session;

	/**
	* Dictionary of data.
	*/
	GDict* Dict;

	/**
	* Dictionary of stopwords.
	*/
	GDict* Stop;

	/**
	* List of word that must be skipped when there are part of a sequence
	* beginning with a number.
	*/
	R::RContainer<SkipWord,true,true> SkipWords;

public:

	/**
	* Constructor of a language.
	* @param fac            Factory.
	* @param lang           Name of the language.
	* @param code           Code of the language.
	*/
	GLang(GFactoryLang* fac,const R::RString& lang,const char* code) throw(std::bad_alloc);

	/**
	* Connect to a Session.
	* @param session         The session.
	*/
	virtual void Connect(GSession* session) throw(GException);

	/**
	* Disconnect from a Session.
	* @param session         The session.
	*/
	virtual void Disconnect(GSession* session) throw(GException);

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

proxy:

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
	void SkipSequence(const R::RString& word) throw(std::bad_alloc);

	/**
	* Function that computes the stem of a word. Of course, this method must be
	* overloaded by the child classes.
	* @param kwd            Word for which the stem must be computed.
	* @return A R::RString representing the stem of the word.
	*/
	virtual R::RString GetStemming(const R::RString& kwd) throw(GException);

	/**
	* Get the session.
	* @return Pointer to GSession.
	*/
	GSession* GetSession(void) const {return(Session);}

	/**
	* Get the dictionary of data attached to the language.
	* @return Pointer to GDict.
	*/
	GDict* GetDict(void) const;

	/**
	* Get the dictionary of stopwords attached to the language.
	* @return Pointer to GDict.
	*/
	GDict* GetStop(void) const;

	/**
	* Look if a name corresponds to a data stored in the dictionary of
	* stopwords.
	* @param name            Name to lookup.
	* @return true if it was found in the dictionary of stopwords.
	*/
	bool InStop(const R::RString& name) const throw(GException);

	/**
	* Increase the number of objects of a given type that make a reference to a
	* data.
	* @param id             Identificator of the data.
	* @param ObjType        Type of the object.
	*/
	void IncRef(unsigned int id,tObjType ObjType) throw(GException);

	/**
	* Decrease the number of objects of a given type that make a reference to a
	* data.
	* @param id             Identificator of the data.
	* @param ObjType        Type of the object.
	*/
	void DecRef(unsigned int id,tObjType ObjType) throw(GException);

	/**
	* Get the number of objects of a given type that make a reference to a data.
	* @param id             Identificator of the data.
	* @param ObjType        Type of the object.
	* @return unsigned int.
	*/
	unsigned int GetRef(unsigned int id,tObjType ObjType) throw(GException);

	/**
	* Increase the number of objects of a given type using the dictionary of the
	* language.
	* @param ObjType        Type of the object.
	*/
	void IncRef(tObjType ObjType) throw(GException);

	/**
	* Decrease the number of objects of a given type using the dictionary of the
	* language.
	* @param ObjType        Type of the object.
	*/
	void DecRef(tObjType ObjType) throw(GException);

	/**
	* Get the number of objects of a given type using the dictionary of the
	* language.
	* @param ObjType        Type of the object.
	*/
	unsigned int GetRef(tObjType ObjType) throw(GException);

	/**
	* Get the number of elements contained in the dictionary of data.
	* @returns unsigned int.
	*/
	unsigned int GetTotal(void) const throw(GException);

	/**
	* Get the list of the elements currently defined in the dictionary of data.
	* @returns const pointer to array of GWord*.
	*/
	const GData** GetDatas(void) const throw(GException);

	/**
	* Look if a given sequence must be skipped.
	* @param seq             Sequence.
	* @return true if the sequence must be skipped.
	*/
	bool MustSkipSequence(const R::RChar* seq) throw(GException);

public:

	/**
	* Destructor.
	*/
	virtual ~GLang(void);
};


//-----------------------------------------------------------------------------
/**
* The GFactoryLang represent a factory for a given language.
* @author Pascal Francq
* @short Generic Language Factory.
*/
class GFactoryLang : public GFactoryPlugin<GFactoryLang,GLang,GLangManager>
{
	/**
	* Code of the language.
	*/
	char Code[3];

public:
	/**
	* Constructor.
	* @param mng             Manager of the plugin.
	* @param n               Name of the Factory/Plugin.
	* @param f               Lib of the Factory/Plugin.
	* @param c               Code of the language.
	*/
	GFactoryLang(GLangManager* mng,const char* n,const char* f,const char* c)
		: GFactoryPlugin<GFactoryLang,GLang,GLangManager>(mng,n,f) {strcpy(Code,c);}

	/**
	* Compare function like strcmp used in particular for RContainer class.
	* @param lang           Factory used for the comparaison.
	*/
	int Compare(const GFactoryLang& lang) const {return(strcmp(Code,lang.Code));}

	/**
	* Compare function like strcmp used in particular for RContainer class.
	* @param code           Code used for the comparaison.
	*/
	int Compare(const char* code) const {return(strcmp(Code,code));}

	/**
	* Destructor.
	*/
	virtual ~GFactoryLang(void) {}
};


//-----------------------------------------------------------------------------
/**
* Signature of the method used to initiliaze a language factory.
*/
typedef GFactoryLang* GFactoryLangInit(GLangManager*,const char*);


//------------------------------------------------------------------------------
#define CREATE_LANG_FACTORY(name,C,code)                                                  \
class TheFactory : public GFactoryLang                                                    \
{                                                                                         \
private:                                                                                  \
	static GFactoryLang* Inst;                                                            \
	TheFactory(GLangManager* mng,const char* l) : GFactoryLang(mng,name,l,code)           \
	{                                                                                     \
		C::CreateParams(this);                                                            \
	}                                                                                     \
	virtual ~TheFactory(void) {}                                                          \
public:                                                                                   \
	static GFactoryLang* CreateInst(GLangManager* mng,const char* l)                      \
	{                                                                                     \
		if(!Inst)                                                                         \
			Inst = new TheFactory(mng,l);                                                 \
		return(Inst);                                                                     \
	}                                                                                     \
	virtual void Create(void) throw(GException)                                           \
	{                                                                                     \
		if(Plugin) return;                                                                \
		Plugin=new C(this);                                                               \
		Plugin->ApplyConfig();                                                            \
	}                                                                                     \
	virtual void Delete(void) throw(GException)                                           \
	{                                                                                     \
		if(!Plugin) return;                                                               \
		delete Plugin;                                                                    \
		Plugin=0;                                                                         \
	}                                                                                     \
	virtual void Create(GSession* ses) throw(GException)                                  \
	{                                                                                     \
		if(!Plugin)                                                                       \
		{                                                                                 \
			Plugin=new C(this);                                                           \
			Plugin->ApplyConfig();                                                        \
		}                                                                                 \
		if(ses)                                                                           \
			Plugin->Connect(ses);                                                         \
	}                                                                                     \
	virtual void Delete(GSession* ses) throw(GException)                                  \
	{                                                                                     \
		if(!Plugin) return;                                                               \
		if(ses)                                                                           \
			Plugin->Disconnect(ses);                                                      \
		delete Plugin;                                                                    \
		Plugin=0;                                                                         \
	}                                                                                     \
	virtual const char* GetAPIVersion(void) const {return(API_LANG_VERSION);}             \
};                                                                                        \
                                                                                          \
GFactoryLang* TheFactory::Inst = 0;                                                       \
                                                                                          \
extern "C"                                                                                \
{                                                                                         \
	GFactoryLang* FactoryCreate(GLangManager* mng,const char* l)                          \
	{                                                                                     \
		return(TheFactory::CreateInst(mng,l));                                            \
	}                                                                                     \
	const char* LibType(void)                                                             \
	{                                                                                     \
		return("Lang");                                                                   \
	}                                                                                     \
}


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
