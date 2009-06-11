/*

	GALILEI Research Project

	GLinkCalc.h

	Generic Link Method - Header.

	Copyright 2003-2004 by Valery Vandaele.
	Copyright 2003-2008 Université Libre de Bruxelles (ULB).

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
#ifndef GLinkCalcH
#define GLinkCalcH


//------------------------------------------------------------------------------
//include file for GALILEI
#include <gplugin.h>
#include <gpluginmanager.h>


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// API VERSION
#define API_LINKCALC_VERSION "2.0"


//------------------------------------------------------------------------------
/**
* The GLinkCalc provides a representation for a generic method to compute links.
* @author Vandaele Valery
* @short Generic Link Method.
*/
class GLinkCalc : public GPlugin<GFactoryLinkCalc>
{
protected:

	/**
	* Container of outgoing links.
	*/
	static R::RContainer<GLinks,true,true> Links_Out;

	/**
	* Status of the graph.
	*/
	static bool Inited;

public:

	/**
	* Construct the computing method.
	* @param fac            Factory of the plugin.
	*/
	GLinkCalc(GFactoryLinkCalc* fac);

	/**
	* Compute the links for a given subprofile.
	* @param prof            Reference to the profile to compute.
	*/
	virtual void Compute(GProfile* prof)=0;

	/**
	* Disconnect from a Session.
	* @param session        Session.
	*/
	virtual void Disconnect(GSession* session);

	/**
	* Init the algorithm for link computation
	*/
	void InitGraph(void);

	/**
	* Add a new doc (not present in the initAlgo phase).
	* @param doc            Document.
	*/
	void AddDoc(GDoc* doc);

	/**
	* destructor of GLinkCalc
	*/
	virtual ~GLinkCalc(void);
};


//------------------------------------------------------------------------------
/**
* The GFactoryLinkCalc represent a factory for a given link method.
* @author Vandaele Valery
* @short Generic Link Factory.
*/
class GFactoryLinkCalc : public GFactoryPlugin<GFactoryLinkCalc,GLinkCalc,GLinkCalcManager>
{
public:

	/*
	* Constructor.
	* @param mng             Manager of the plugin.
	* @param n               Name of the Factory/Plugin.
	* @param f               Lib of the Factory/Plugin.
	*/
	GFactoryLinkCalc(GLinkCalcManager* mng,const char* n,const char* f)
		 : GFactoryPlugin<GFactoryLinkCalc,GLinkCalc,GLinkCalcManager>(mng,n,f) {}
};


//------------------------------------------------------------------------------
/**
* The GLinkCalcManager class provides a representation for a manager
* responsible to manage all the links computing methods.
* @author Vandaele Valery
* @short Link Methods Manager.
*/
class GLinkCalcManager : public GPluginManager<GLinkCalcManager,GFactoryLinkCalc,GLinkCalc>
{
public:

	/**
	* Constructor of a manager.
	*/
	GLinkCalcManager(void);

	/**
	* Destructor of the manager.
	*/
	virtual ~GLinkCalcManager(void);
};


//-------------------------------------------------------------------------------
#define CREATE_LINKCALC_FACTORY(name,plugin)\
	CREATE_FACTORY(GLinkCalcManager,GFactoryLinkCalc,GLinkCalc,plugin,"LinkCalc",API_LINKCALC_VERSION,name)


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
