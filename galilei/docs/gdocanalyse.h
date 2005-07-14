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
#include <gplugin.h>
#include <gpluginmanager.h>


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
/*
* The GFactoryDocAnalyse represent a factory for a given document analysis
* method.
* @author Pascal Francq
* @short Generic Document Analysis Factory.
*/
class GFactoryDocAnalyse : public GFactoryPlugin<GFactoryDocAnalyse,GDocAnalyse,GDocAnalyseManager>
{
public:

	/*
	* Constructor.
	* @param mng             Manager of the plugin.
	* @param n               Name of the Factory/Plugin.
	* @param f               Lib of the Factory/Plugin.
	*/
	GFactoryDocAnalyse(GDocAnalyseManager* mng,const char* n,const char* f)
		 : GFactoryPlugin<GFactoryDocAnalyse,GDocAnalyse,GDocAnalyseManager>(mng,n,f) {}
};


//------------------------------------------------------------------------------
/**
* The GDocAnalyseManager class provides a representation for a manager
* responsible to manage all the doc analysis methods.
* @author Pascal Francq
* @short Doc Analysis Method Manager.
*/
class GDocAnalyseManager : public GPluginManager<GDocAnalyseManager,GFactoryDocAnalyse,GDocAnalyse>
{
public:

	/**
	* Constructor of a manager.
	*/
	GDocAnalyseManager(void);

	/**
	* Destructor of the manager.
	*/
	virtual ~GDocAnalyseManager(void);
};


//-------------------------------------------------------------------------------
#define CREATE_DOCANALYSE_FACTORY(name,plugin)\
	CREATE_FACTORY(GDocAnalyseManager,GFactoryDocAnalyse,GDocAnalyse,plugin,"DocAnalyse",API_DOCANALYSE_VERSION,name)


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
