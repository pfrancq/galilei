/*

	GALILEI Research Project

	GTool.h

	Generic Tool - Header.

	Copyright 2008-2009 by Pascal Francq (pascal@francq.info).
	Copyright 2008 by the Université Libre de Bruxelles (ULB).

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
#ifndef GToolH
#define GToolH


//------------------------------------------------------------------------------
// include files for GALILEI
#include <gplugin.h>
#include <gpluginmanager.h>


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// API VERSION
#define API_TOOL_VERSION "2.0"


//------------------------------------------------------------------------------
/**
* The GTool class provides a generic tool that could be called.
* @author Pascal Francq
* @short Generic Tool.
*/
class GTool : public GPlugin<GFactoryTool>
{
public:

	/**
	* Construct the tool.
	* @param fac             Factory of the plugin.
	*/
	GTool(GFactoryTool* fac);

	/**
	* Run the tool.
	*/
	virtual void Run(void)=0;

	/**
	* Destructor of the tool.
	*/
	virtual ~GTool(void);
};


//------------------------------------------------------------------------------
/*
* The GFactoryTool represent a factory for a given tool.
* @author Pascal Francq
* @short Generic Tool Factory.
*/
class GFactoryTool : public GFactoryPlugin<GFactoryTool,GTool,GToolManager>
{
public:

	/*
	* Constructor.
	* @param mng             Manager of the plugin.
	* @param n               Name of the Factory/Plugin.
	* @param f               Lib of the Factory/Plugin.
	*/
	GFactoryTool(GToolManager* mng,const char* n,const char* f)
		 : GFactoryPlugin<GFactoryTool,GTool,GToolManager>(mng,n,f) {}
};


//------------------------------------------------------------------------------
/**
* The GToolManager class provides a representation for a manager
* responsible to manage all the tools.
* @author Pascal Francq
* @short Tools Manager.
*/
class GToolManager : public GPluginManager<GToolManager,GFactoryTool,GTool>
{
public:

	/**
	* Construct the statistics methods manager.
	*/
	GToolManager(void);

	/**
	* Destructor of the statistics methods manager.
	*/
	virtual ~GToolManager(void);
};


//-------------------------------------------------------------------------------
#define CREATE_Tool_FACTORY(name,plugin)\
	CREATE_FACTORY(GToolManager,GFactoryTool,GTool,plugin,"Tool",API_TOOL_VERSION,name)


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
