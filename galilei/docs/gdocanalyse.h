/*

	GALILEI Research Project

	GDocAnalyse.h

	Generic Document Analysis - Header.

	Copyright 2001-2008 by the Université libre de Bruxelles.

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
#include <gweightinfos.h>
#include <gdocstruct.h>


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// API VERSION
#define API_DOCANALYSE_VERSION "2.0"


//------------------------------------------------------------------------------
/**
 * The GDocAnalyse class provides a generic method to analyze a document. The
 * internal structures, Infos and Struct, are used by GSession to manage the
 * results (saving, assign them to the documents, etc.). The plug-in is
 * responsible for their management (such as clearing them between two analysis
 * if necessary.
 * @author Pascal Francq
 * @short Generic Document Analysis.
 */
class GDocAnalyse : public GPlugin<GFactoryDocAnalyse>
{
protected:

	/**
	 * Vector that should contain the structure after the analyze.
	 */
	GWeightInfos Infos;

	/**
	 * Structure that should contain the structure after the analyze.
	 */
	GDocStruct Struct;

	/**
	 * Language associated to the document.
	 */
	GLang* Lang;

public:

	/**
	* Constructor of the document analysis method.
	* @param fac             Factory of the plug-in.
	*/
	GDocAnalyse(GFactoryDocAnalyse* fac);

	/**
	* Analyze a XML of a document for a session.
	* @param doc             Document to analyze.
	* @param file            File to analyze (may be its XML version).
	* @param native          Specify if the document is a native XML file.
	*/
	virtual void Analyze(const GDoc* doc,const R::RURI& file,bool native)=0;

	/**
	* Destruct the document analyzer.
	*/
	virtual ~GDocAnalyse(void);

	friend class GSession;
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
	* Destruct the manager.
	*/
	virtual ~GDocAnalyseManager(void);
};


//-------------------------------------------------------------------------------
#define CREATE_DOCANALYSE_FACTORY(name,plugin)\
	CREATE_FACTORY(GDocAnalyseManager,GFactoryDocAnalyse,GDocAnalyse,plugin,"DocAnalyse",API_DOCANALYSE_VERSION,name)


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
