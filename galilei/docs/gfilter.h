/*

	GALILEI Research Project

	GFilter.h

	Generic Filter for documents - Header.

	Copyright 2001-2003 by the Université Libre de Bruxelles.

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



//------------------------------------------------------------------------------
#ifndef GFilterH
#define GFilterH


//------------------------------------------------------------------------------
// include files for GALILEI
#include <sessions/gplugin.h>


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// API VERSION
#define API_FILTER_VERSION "1.0"


//------------------------------------------------------------------------------
/**
* The GFilter class provides a representation of a generic filter that transform
* a document of a given type into a DocXML document. Each filter is associated
* with a given set of MIME types. This association is used to determine which
* filter should analyse a document.
* @author Pascal Francq
* @short Generic Filter.
*/
class GFilter : public GPlugin<GFactoryFilter>
{
protected:

	/**
	* DocXML document that will contain the result of the document analysis.
	*/
	GDocXML* Doc;

public:

	/**
	* Construct the filter.
	* @param fac            Factory.
	*/
	GFilter(GFactoryFilter* fac) throw(std::bad_alloc);

protected:

	/**
	* Add a specific MIME-Type for the filter.
	* @param mime           Name of the MIME-Type.
	*/
	void AddMIME(const char* mime) throw(std::bad_alloc);

	/**
	* Test if a given character represent the end of a sentence.
	* @param c               8-bit character to test.
	* @return true if it is a end sentence character.
	*/
	static inline bool IsEndSentence(char c)
	{
		return((c=='.')||(c=='!')||(c=='?')||(c==';')||(c==':'));
	}

	/**
	* Test if a given character represent the end of a sentence.
	* @param c               Unicode character to test.
	* @return true if it is a end sentence character.
	*/
	static inline bool IsEndSentence(R::RChar c)
	{
		return((c.Latin1()=='.')||(c.Latin1()=='!')||(c.Latin1()=='?')||(c.Latin1()==';')||(c.Latin1()==':'));
	}

	/**
	* Analyse a block of text and create a list of tags 'docxml:sentence'.
	* @param block          Block containing the text.
	* @param attach         XML tag where the sentences must be attach.
	*/
	void AnalyzeBlock(char* block,R::RXMLTag* attach) throw(std::bad_alloc,GException);

	/**
	* Analyse a block of text and create a list of tags 'docxml:sentence'.
	* @param block          Block containing the text.
	* @param attach         XML tag where the sentences must be attach.
	*/
	void AnalyzeBlock(R::RChar* block,R::RXMLTag* attach) throw(std::bad_alloc,GException);

	/**
	* Analyse a list of keywords sepating by a single character.
	* @param list           List of keywords.
	* @param sep            Separator to use.
	* @param attach         XML tag where the sentences must be attach.
	*/
	void AnalyzeKeywords(char* list,char sep,R::RXMLTag* attach) throw(std::bad_alloc,GException);

	/**
	* Analyse a list of keywords sepating by a single character.
	* @param list           List of keywords.
	* @param sep            Separator to use.
	* @param attach         XML tag where the sentences must be attach.
	*/
	void AnalyzeKeywords(R::RChar* list,R::RChar sep,R::RXMLTag* attach) throw(std::bad_alloc,GException);

public:

	/**
	* Analyze the document and construct the DocXML document with the
	* information about its content. This method must be reimplemented in the
	* child classes.
	* @param doc            XML Document that will represent the document.
	*/
	virtual bool Analyze(GDocXML* doc) throw(std::bad_alloc,GException)=0;

	/**
	* Destructor of the filter.
	*/
	virtual ~GFilter(void);
};


//------------------------------------------------------------------------------
class GFactoryFilter : public GFactoryPlugin<GFactoryFilter,GFilter,GFilterManager>
{
public:
	/**
	* Constructor.
	* @param mng             Manager of the plugin.
	* @param n               Name of the Factory/Plugin.
	* @param f               Lib of the Factory/Plugin.
	*/
	GFactoryFilter(GFilterManager* mng,const char* n,const char* f) : GFactoryPlugin<GFactoryFilter,GFilter,GFilterManager>(mng,n,f) {}

	/**
	* Destructor.
	*/
	virtual ~GFactoryFilter(void) {}
};


//------------------------------------------------------------------------------
typedef GFactoryFilter*(*GFactoryFilterInit)(GFilterManager*,const char*);


//-------------------------------------------------------------------------------
#define CREATE_FILTER_FACTORY(name,C)                                                     \
class TheFactory : public GFactoryFilter                                                  \
{                                                                                         \
private:                                                                                  \
	static GFactoryFilter* Inst;                                                          \
	TheFactory(GFilterManager* mng,const char* l) : GFactoryFilter(mng,name,l)            \
	{                                                                                     \
		C::CreateParams(this);                                                            \
	}                                                                                     \
	virtual ~TheFactory(void) {}                                                          \
public:                                                                                   \
	static GFactoryFilter* CreateInst(GFilterManager* mng,const char* l)                  \
	{                                                                                     \
		if(!Inst)                                                                         \
			Inst = new TheFactory(mng,l);                                                 \
		return(Inst);                                                                     \
	}                                                                                     \
	virtual const char* GetAPIVersion(void) const {return(API_FILTER_VERSION);}           \
	virtual void Create(void) throw(GException)                                           \
	{                                                                                     \
		if(Plugin) return;                                                                \
		Plugin=new C(this);                                                               \
		Plugin->ApplyConfig();                                                            \
	}                                                                                     \
	virtual void Delete(void) throw(GException)                                           \
	{                                                                                     \
		if(!Plugin) return;                                                               \
		Mng->DelMIMES(Plugin);                                                            \
		delete Plugin;                                                                    \
		Plugin=0;                                                                         \
	}                                                                                     \
	virtual void Create(GSession*) throw(GException)  {}                                  \
	virtual void Delete(GSession*) throw(GException)  {}                                  \
};                                                                                        \
                                                                                          \
GFactoryFilter* TheFactory::Inst = 0;                                                     \
                                                                                          \
extern "C"                                                                                \
{                                                                                         \
	GFactoryFilter* FactoryCreate(GFilterManager* mng,const char* l)                      \
	{                                                                                     \
		return(TheFactory::CreateInst(mng,l));                                            \
	}                                                                                     \
}


//------------------------------------------------------------------------------
/**
* The GFactoryFilterCursor class provides a way to go trough a set of factories
* of filters.
* @short Filters Factories Cursor
*/
CLASSCURSOR(GFactoryFilterCursor,GFactoryFilter,unsigned int)


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
