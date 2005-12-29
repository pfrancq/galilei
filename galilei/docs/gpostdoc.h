/*

	GALILEI Research Project

	GPostDoc.h

	Generic Documents Post-Analysis Method - Header.

	Copyright 2003 by the Universit�Libre de Bruxelles.

	Authors
		 Kumps Nicolas (nkumps@ulb.ac.be)

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
#ifndef GPostDocH
#define GPostDocH


//------------------------------------------------------------------------------
// include file for GALILEI
#include <gplugin.h>
#include <gpluginmanager.h>


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// API VERSION
#define API_POSTDOC_VERSION "2.0"


//------------------------------------------------------------------------------
/**
* The GPostDoc provides a representation for a generic documents post-analysis
* method.
* @author Nicolas Kumps
* @short Generic Document Post Analysis Method.
*/
class GPostDoc : public GPlugin<GFactoryPostDoc>
{
public :

	/**
	* Constructor of the documents post-analysis method.
	* @param fac             Factory of the plugin.
	*/
	GPostDoc(GFactoryPostDoc* fac);

	/**
	* Run the method.
	*/
	virtual void Run(void)=0;

	/**
	* Destructor of the documents post-analysis method.
	*/
	virtual ~GPostDoc(void);
};


//------------------------------------------------------------------------------
/*
* The GFactoryPostDoc represent a factory for a given document post analysis
* method.
* @author Nicolas Kumps
* @short Generic Document Post Analysis Factory.
*/
class GFactoryPostDoc : public GFactoryPlugin<GFactoryPostDoc,GPostDoc,GPostDocManager>
{
public:

	/*
	* Constructor.
	* @param mng             Manager of the plugin.
	* @param n               Name of the Factory/Plugin.
	* @param f               Lib of the Factory/Plugin.
	*/
	GFactoryPostDoc(GPostDocManager* mng,const char* n,const char* f)
		: GFactoryPlugin<GFactoryPostDoc,GPostDoc,GPostDocManager>(mng,n,f) {}
};


//------------------------------------------------------------------------------
/**
* The GPostDocManager class provides a representation for a manager
* responsible to manage all the documents post-analysis methods.
* @author Pascal Francq
* @short Documents Post-Analysis Methods Manager.
*/
class GPostDocManager : public GPluginManager<GPostDocManager,GFactoryPostDoc,GPostDoc>
{
public:

	/**
	* Constructor of a manager.
	*/
	GPostDocManager(void);

	/**
	* Destructor of the manager.
	*/
	virtual ~GPostDocManager(void);
};


//-------------------------------------------------------------------------------
#define CREATE_POSTDOC_FACTORY(name,plugin)\
	CREATE_FACTORY(GPostDocManager,GFactoryPostDoc,GPostDoc,plugin,"PostDoc",API_POSTDOC_VERSION,name)


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
