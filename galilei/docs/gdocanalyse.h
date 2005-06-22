/*

	GALILEI Research Project

	GDocAnalyse.h

	Generic Document Analysis - Header.

	Copyright 2001-2004 by the Université libre de Bruxelles.

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
#define API_DOCANALYSE_VERSION "2.0"


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
	* Current document to analyse.
	*/
	GDoc* Doc;

public:

	/**
	* Constructor of the document analysis method.
	* @param fac             Factory of the plugin.
	*/
	GDocAnalyse(GFactoryDocAnalyse* fac) throw(std::bad_alloc);

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
	* @param doc            Reference to the document to analyze.
	* @param tmpDocs        Container that will hold the references of the added
	*                       docs.
	*/
	virtual void Analyze(GDocXML* xml,GDoc* doc,R::RContainer<GDoc,false,true>* tmpDocs=0) throw(GException)=0;

	/**
	* Destructor of the document analysis method.
	*/
	virtual ~GDocAnalyse(void);
};


//------------------------------------------------------------------------------
/**
* The GFactoryDocAnalyse represent a factory for a given document analysis
* method.
* @author Pascal Francq
* @short Generic Document Analysis Factory.
*/
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
		 : GFactoryPlugin<GFactoryDocAnalyse,GDocAnalyse,GDocAnalyseManager>(mng,n,f,"GFactoryDocAnalyse") {}

	/**
	* Destructor.
	*/
	virtual ~GFactoryDocAnalyse(void) {}
};


//------------------------------------------------------------------------------
/**
* Signature of the method used to initiliaze a document analysis factory.
*/
typedef GFactoryDocAnalyse* GFactoryDocAnalyseInit(GDocAnalyseManager*,const char*);


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


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
