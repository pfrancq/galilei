/*

	GALILEI Research Project

	GComputeSugs.h

	Generic Suggestions Computing Method - Header.

	Copyright 2008-2009 by Pascal Francq (pascal@francq.info).

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
#ifndef GComputeSugsH
#define GComputeSugsH


//------------------------------------------------------------------------------
// include files for GALILEI
#include <gplugin.h>
#include <gpluginmanager.h>


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// API VERSION
#define API_COMPUTESUGS_VERSION "2.0"


//------------------------------------------------------------------------------
/**
* The GComputeSugs class provides a representation for a generic method to
* compute suggestions.
* @author Pascal Francq
* @short Generic Suggestions Computing Method.
*/
class GComputeSugs : public GPlugin<GFactoryComputeSugs>
{
public:

	/**
	* Constructor.
	* @param fac             Factory of the plug-in.
	*/
	GComputeSugs(GFactoryComputeSugs* fac);

	/**
	* Run the suggestions computing method.
	*/
	virtual void Run(void)=0;

	/**
	* Destructor.
	*/
	virtual ~GComputeSugs(void);
};


//------------------------------------------------------------------------------
/**
* The GFactoryComputeSugs represent a factory for a given suggestions
* computing method.
* @author Pascal Francq
* @short Generic Suggestions Computing Factory.
*/
class GFactoryComputeSugs : public GFactoryPlugin<GFactoryComputeSugs,GComputeSugs,GComputeSugsManager>
{
public:

	/**
	* Constructor.
	* @param mng             Manager of the plug-in.
	* @param n               Name of the Factory/Plug-in.
	* @param f               Lib of the Factory/Plug-in.
	*/
	GFactoryComputeSugs(GComputeSugsManager* mng,const char* n,const char* f)
	 : GFactoryPlugin<GFactoryComputeSugs,GComputeSugs,GComputeSugsManager>(mng,n,f) {}
};


//------------------------------------------------------------------------------
/**
* The GComputeSugsManager class provides a representation for the manager of
* all the suggestion computing methods.
* @author Pascal Francq
* @short Suggestions Computing Methods Manager.
*/
class GComputeSugsManager : public GPluginManager<GComputeSugsManager,GFactoryComputeSugs,GComputeSugs>
{
public:

	/**
	* Construct the suggestions computing methods manager.
	*/
	GComputeSugsManager(void);

	/**
	* Destruct the suggestions computing methods manager.
	*/
	virtual ~GComputeSugsManager(void);
};


//------------------------------------------------------------------------------
#define CREATE_COMPUTESUGS_FACTORY(name,plugin)\
	CREATE_FACTORY(GALILEI::GComputeSugsManager,GALILEI::GFactoryComputeSugs,GALILEI::GComputeSugs,plugin,"ComputeSugs",API_COMPUTESUGS_VERSION,name)


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
