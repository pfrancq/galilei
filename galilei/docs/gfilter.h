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



//-----------------------------------------------------------------------------
#ifndef GFilterH
#define GFilterH


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>
#include <sessions/gplugin.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// API VERSION
#define API_FILTER_VERSION "1.0"


//-----------------------------------------------------------------------------
/**
* The GFilter class provides a representation of a document to filter in a XML
* structure.
* @author Pascal Francq
* @short Generic Filter.
*/
class GFilter : public GPlugin<GFactoryFilter>
{
protected:

	/**
	* Structure corresponding to the document currently analyzed.
	*/
	GDocXML* Doc;

public:

	/**
	* Construct the filter for a specific document.
	* @param fac            Factory.
	*/
	GFilter(GFactoryFilter* fac);

protected:

	/**
	* Add a specific MIME-Type for this filter.
	* @param mng            Manager.
	* @param mime           Name of the MIME-Type.
	*/
	void AddMIME(GURLManager* mng,const char* mime);

	/**
	* @return true if the character represent a end of sentence.
	*/
	static inline bool IsEndSentence(char c)
	{
		return((c=='.')||(c=='!')||(c=='?')||(c==';')||(c==':'));
	}

	/**
	* @return true if the character represent a end of sentence.
	*/
	static inline bool IsEndSentence(R::RChar c)
	{
		return((c.Latin1()=='.')||(c.Latin1()=='!')||(c.Latin1()=='?')||(c.Latin1()==';')||(c.Latin1()==':'));
	}

	/**
	* Analyse a block of text and create a list of tags 'sentence'.
	* @param block          Block containing the text.
	* @param attach         XML tag where the sentences must be attach.
	*/
	void AnalyzeBlock(char* block,R::RXMLTag* attach);

	/**
	* Analyse a block of text and create a list of tags 'sentence'.
	* @param block          Block containing the text.
	* @param attach         XML tag where the sentences must be attach.
	*/
	void AnalyzeBlock(R::RChar* block,R::RXMLTag* attach);

	/**
	* Analyse a list of keywords sepating by a single character.
	* @param list           List of keywords.
	* @param sep            Separator to use.
	* @param attach         XML tag where the sentences must be attach.
	*/
	void AnalyzeKeywords(char* list,char sep,R::RXMLTag* attach);

public:

	/**
	* Analyze the document and fill the XML structure with the information
	* about its content. This method must be reimplemented in the child
	* classes.
	* @param doc            XML Structure that will represent the document.
	*/
	virtual bool Analyze(GDocXML* doc)=0;

	/**
	* Destructor of the filter.
	*/
	virtual ~GFilter(void);
};


//-----------------------------------------------------------------------------
class GFactoryFilter : public GFactoryPlugin<GFactoryFilter,GFilter,GURLManager>
{
public:
	/**
	* Constructor.
	* @param mng             Manager of the plugin.
	* @param n               Name of the Factory/Plugin.
	* @param f               Lib of the Factory/Plugin.
	*/
	GFactoryFilter(GURLManager* mng,const char* n,const char* f) : GFactoryPlugin<GFactoryFilter,GFilter,GURLManager>(mng,n,f) {}

	/**
	* Destructor.
	*/
	virtual ~GFactoryFilter(void) {}
};


//-----------------------------------------------------------------------------
typedef GFactoryFilter*(*GFactoryFilterInit)(GURLManager*,const char*);


//------------------------------------------------------------------------------
#define CREATE_FILTER_FACTORY(name,C)                                          \
class TheFactory : public GFactoryFilter                                       \
{                                                                              \
private:                                                                       \
	static GFactoryFilter* Inst;                                               \
	TheFactory(GURLManager* mng,const char* l) : GFactoryFilter(mng,name,l)    \
	{                                                                          \
		C::CreateParams(this);                                                 \
	}                                                                          \
	virtual ~TheFactory(void) {}                                               \
public:                                                                        \
	static GFactoryFilter* CreateInst(GURLManager* mng,const char* l)          \
	{                                                                          \
		if(!Inst)                                                              \
			Inst = new TheFactory(mng,l);                                      \
		return(Inst);                                                          \
	}                                                                          \
	virtual const char* GetAPIVersion(void) const {return(API_FILTER_VERSION);}\
	virtual void Create(void) throw(GException)                                \
	{                                                                          \
		if(Plugin) return;                                                     \
		Plugin=new C(this);                                                    \
		Plugin->ApplyConfig();                                                 \
	}                                                                          \
	virtual void Delete(void) throw(GException)                                \
	{                                                                          \
		if(!Plugin) return;                                                    \
		Mng->DelMIMES(Plugin);                                                 \
		delete Plugin;                                                         \
		Plugin=0;                                                              \
	}                                                                          \
	virtual void Create(GSession*) throw(GException)  {}                       \
	virtual void Delete(GSession*) throw(GException)  {}                       \
};                                                                             \
                                                                               \
GFactoryFilter* TheFactory::Inst = 0;                                          \
                                                                               \
extern "C"                                                                     \
{                                                                              \
	GFactoryFilter* FactoryCreate(GURLManager* mng,const char* l)              \
	{                                                                          \
		return(TheFactory::CreateInst(mng,l));                                 \
	}                                                                          \
}


//------------------------------------------------------------------------------
/**
* The GFactoryFilterCursor class provides a way to go trough a set of factories.
* @short Filters Factories Cursor
*/
CLASSCURSOR(GFactoryFilterCursor,GFactoryFilter,unsigned int)


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
