/*

	GALILEI Research Project

	GDocAnalyse.h

	Generic Document Analysis - Header.

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
#ifndef GDocAnalyseH
#define GDocAnalyseH


//------------------------------------------------------------------------------
// include files for GALILEI
#include <sessions/gplugin.h>


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// API VERSION
#define API_DOCANALYSE_VERSION "1.0"


//------------------------------------------------------------------------------
/**
* The GDocAnalyse class provides a generic method to analyse a document.
* @author Pascal Francq
* @short Generic Document Analysis.
*/
class GDocAnalyse : public GPlugin<GFactoryDocAnalyse>
{
protected:

	/**
	* Corresponding session;
	*/
	GSession* Session;

	/**
	* Document to analyse;
	*/
	GDoc* Doc;

public:

	/**
	* Constructor of the document analysis method.
	* @param fac             Factory of the plugin.
	*/
	GDocAnalyse(GFactoryDocAnalyse* fac) throw(bad_alloc);

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
	* Analyse a XML representation of a document for a session and store the
	* results in this document. The method must set the Doc variable.
	*
	* During the analysis of the document, if links point to document that does
	* not exist in the system, these documents are added in a container.
	* @param xml            DocXML document used.
	* @param doc            Corresponding document.
	* @param tmpDocs        Container that will hold the added docs.
	*/
	virtual void Analyse(GDocXML* xml,GDoc* doc,R::RContainer<GDoc,unsigned int,false,true>* tmpDocs=0) throw(GException)=0;

	/**
	* Update the feedbacks of the profiles and subprofiles.
	*/
	void UpdateFdbks(GLang* oldlang, GLang* newlang) throw(GException);

	/**
	* Destructor of the document analysis method.
	*/
	virtual ~GDocAnalyse(void);
};


//------------------------------------------------------------------------------
class GFactoryDocAnalyse : public GFactoryPlugin<GFactoryDocAnalyse,GDocAnalyse,GDocAnalyseManager>
{
public:
	/**
	* Constructor.
	* @param mng             Manager of the plugin.
	* @param n               Name of the Factory/Plugin.
	* @param f               Lib of the Factory/Plugin.
	*/
	GFactoryDocAnalyse(GDocAnalyseManager* mng,const char* n,const char* f)
		 : GFactoryPlugin<GFactoryDocAnalyse,GDocAnalyse,GDocAnalyseManager>(mng,n,f) {}

	/**
	* Destructor.
	*/
	virtual ~GFactoryDocAnalyse(void) {}
};


//------------------------------------------------------------------------------
typedef GFactoryDocAnalyse*(*GFactoryDocAnalyseInit)(GDocAnalyseManager*,const char*);


//-------------------------------------------------------------------------------
#define CREATE_DOCANALYSE_FACTORY(name,C)                                                 \
class TheFactory : public GFactoryDocAnalyse                                              \
{                                                                                         \
private:                                                                                  \
	static GFactoryDocAnalyse* Inst;                                                      \
	TheFactory(GDocAnalyseManager* mng,const char* l) : GFactoryDocAnalyse(mng,name,l)    \
	{                                                                                     \
		C::CreateParams(this);                                                            \
	}                                                                                     \
	virtual ~TheFactory(void) {}                                                          \
public:                                                                                   \
	static GFactoryDocAnalyse* CreateInst(GDocAnalyseManager* mng,const char* l)          \
	{                                                                                     \
		if(!Inst)                                                                         \
			Inst = new TheFactory(mng,l);                                                 \
		return(Inst);                                                                     \
	}                                                                                     \
	virtual const char* GetAPIVersion(void) const {return(API_DOCANALYSE_VERSION);}       \
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
};                                                                                        \
                                                                                          \
GFactoryDocAnalyse* TheFactory::Inst = 0;                                                 \
                                                                                          \
extern "C"                                                                                \
{                                                                                         \
	GFactoryDocAnalyse* FactoryCreate(GDocAnalyseManager* mng,const char* l)              \
	{                                                                                     \
		return(TheFactory::CreateInst(mng,l));                                            \
	}                                                                                     \
}


//------------------------------------------------------------------------------
/**
* The GFactoryDocAnalyseCursor class provides a way to go trough a set of
* factories of document analysis method.
* @short Document Analysis Methods Factories Cursor
*/
CLASSCURSOR(GFactoryDocAnalyseCursor,GFactoryDocAnalyse,unsigned int)


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
