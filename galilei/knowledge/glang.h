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
#include <rstd/rlang.h>


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>
#include <sessions/gplugin.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// API VERSION
#define API_LANG_VERSION "1.0"


//-----------------------------------------------------------------------------
/**
* The GLang class provides a representation for a basic language. The virtual
* function GetStemming must be implemented for the different languages.
* @author Pascal Francq.
* @short Generic Language.
*/
class GLang : public R::RLang, public GPlugin<GFactoryLang>
{
protected:
	class SkipWord
	{
	public:
		R::RString Word;
		SkipWord(const char* w) : Word(w) {}
		int Compare(const SkipWord* w) {return(Word.Compare(w->Word));}
		int Compare(const SkipWord& w) {return(Word.Compare(w.Word));}
		int Compare(const R::RChar* w) {return(Word.Compare(w));}
	};

	/**
	* Dictionnary.
	*/
	GDict* Dict;

	/**
	* Stop words.
	*/
	GDict* Stop;

	/**
	* List of word that must be skipped when there are part of a sequence
	* beginning with a number.
	*/
	R::RContainer<SkipWord,unsigned int,true,true> SkipWords;

public:

	/**
	* Constructor of a language.
	* @param fac            Factory;
	* @param lang           Name of the language.
	* @param code           Code of the language.
	*/
	GLang(GFactoryLang* fac,const R::RString& lang,const char* code) throw(bad_alloc);

	/**
	* Connect to a Session.
	* @param session         The session.
	*/
	virtual void Connect(GSession* session);

	/**
	* Disconnect from a Session.
	* @param session         The session.
	*/
	virtual void Disconnect(GSession* session);

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
	virtual R::RString& GetStemming(const R::RString& kwd);

	/**
	* Get the dictionnary attached to the language.
	* @return Pointer to GDict.
	*/
	GDict* GetDict(void) const;

	/**
	* Get the soptlist attached to the language.
	* @return Pointer to GDict.
	*/
	GDict* GetStop(void) const;

	/**
	* Look if a name corresponds to a data stored in the stoplist.
	* @param name            Name to lookup.
	* @return true if it was found in the stop list.
	*/
	bool InStop(const R::RString& name) const;

	/**
	* Increase the number of references on a word.
	* @param id             Identificator.
	* @param ObjType        Type of the reference.
	*/
	void IncRef(unsigned int id,tObjType ObjType,GInfoType WordType);

	/**
	* Decrease the number of references on a word.
	* @param id             Identificator.
	* @param ObjType        Type of the reference.
	*/
	void DecRef(unsigned int id,tObjType ObjType,GInfoType WordType);

	/**
	* Get the number of references on a word.
	* @param id             Identificator.
	* @param ObjType        Type of the reference.
	* @returns unsigned int.
	*/
	unsigned int GetRef(unsigned int id,tObjType ObjType);

	/**
	* Increase the number of objects using the dictionnary.
	* @param ObjType        Type of the reference.
	*/
	void IncRef(tObjType ObjType,GInfoType WordType);

	/**
	* Decrease the number of objects using the dictionnary.
	* @param id             Identificator.
	* @param ObjType        Type of the reference.
	*/
	void DecRef(tObjType ObjType,GInfoType WordType);

	/**
	* Get the total number of references.
	* @param ObjType        Type of the reference.
	* @returns unsigned int.
	*/
	unsigned int GetRef(tObjType ObjType,GInfoType WordType);

	/**
	* Get the number of elements in the hash container.
	* @returns Number of elements.
	*/
	unsigned int GetTotal(void) const;

	/**
	* Get the number of group of information entities.
	* @returns Number of groups.
	*/
//	unsigned int GetNbWordList(void) const;

	/**
	* Get the maximal number of word actually supposed.
	*/
	unsigned int GetMaxId(void) const;

	/**
	* Get the list of the words currently defined in the dictionnary.
	* @returns const Pointer to array of GWord*.
	*/
	GData** GetDatas(void) const;

	/**
	* Look if a given word is supposed to be skip.
	* @param wd              Word to test.
	*/
	bool ToSkip(const R::RChar* wd);

	/**
	* Destructor.
	*/
	virtual ~GLang(void);
};


//-----------------------------------------------------------------------------
class GFactoryLang : public GFactoryPlugin<GFactoryLang,GLang,GLangs>
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
	GFactoryLang(GLangs* mng,const char* n,const char* f,const char* c)
		: GFactoryPlugin<GFactoryLang,GLang,GLangs>(mng,n,f) {strcpy(Code,c);}

	/**
	* Compare function like strcmp used in particular for RContainer class.
	* @param code           Code used for the comparaison.
	*/
	int Compare(const GFactoryLang* lang) const {return(strcmp(Code,lang->Code));}

	/**
	* Compare function like strcmp used in particular for RContainer class.
	* @param code           Code used for the comparaison.
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
typedef GFactoryLang*(*GFactoryLangInit)(GLangs*,const char*);


//------------------------------------------------------------------------------
#define CREATE_LANG_FACTORY(name,C,code)                                       \
class TheFactory : public GFactoryLang                                         \
{                                                                              \
private:                                                                       \
	static GFactoryLang* Inst;                                                 \
	TheFactory(GLangs* mng,const char* l) : GFactoryLang(mng,name,l,code)      \
	{                                                                          \
		C::CreateParams(this);                                                 \
	}                                                                          \
	virtual ~TheFactory(void) {}                                               \
public:                                                                        \
	static GFactoryLang* CreateInst(GLangs* mng,const char* l)                 \
	{                                                                          \
		if(!Inst)                                                              \
			Inst = new TheFactory(mng,l);                                      \
		return(Inst);                                                          \
	}                                                                          \
	virtual const char* GetAPIVersion(void) const {return(API_LANG_VERSION);}  \
	virtual void Create(void) throw(GException)                                \
	{                                                                          \
		if(Plugin) return;                                                     \
		Plugin=new C(this);                                                    \
		Plugin->ApplyConfig();                                                 \
	}                                                                          \
	virtual void Delete(void) throw(GException)                                \
	{                                                                          \
		if(!Plugin) return;                                                    \
		delete Plugin;                                                         \
		Plugin=0;                                                              \
	}                                                                          \
	virtual void Create(GSession* ses) throw(GException)                       \
	{                                                                          \
		if(!Plugin)                                                            \
		{                                                                      \
			Plugin=new C(this);                                                \
			Plugin->ApplyConfig();                                             \
		}                                                                      \
		if(ses)                                                                \
			Plugin->Connect(ses);                                              \
	}                                                                          \
	virtual void Delete(GSession* ses) throw(GException)                       \
	{                                                                          \
		if(!Plugin) return;                                                    \
		if(ses)                                                                \
			Plugin->Disconnect(ses);                                           \
		delete Plugin;                                                         \
		Plugin=0;                                                              \
	}                                                                          \
};                                                                             \
                                                                               \
GFactoryLang* TheFactory::Inst = 0;                                            \
                                                                               \
extern "C"                                                                     \
{                                                                              \
	GFactoryLang* FactoryCreate(GLangs* mng,const char* l)                     \
	{                                                                          \
		return(TheFactory::CreateInst(mng,l));                                 \
	}                                                                          \
}


//-----------------------------------------------------------------------------
/**
* The GLangCursor class provides a way to go trough a set of languages.
* @short Language Cursor
*/
CLASSCURSOR(GLangCursor,GLang,unsigned int)


//-----------------------------------------------------------------------------
/**
* The GFactoryLangCursor class provides a way to go trough a set of factories.
* @short Language Factory Cursor
*/
CLASSCURSOR(GFactoryLangCursor,GFactoryLang,unsigned int)


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
