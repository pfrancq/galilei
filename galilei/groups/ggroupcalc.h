/*

	GALILEI Research Project

	GGroupCalc.h

	Generic Group Computing Method - Header.

	Copyright 2002-2003 by the Universit�Libre de Bruxelles.

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
#ifndef GGroupCalcH
#define GGroupCalcH


//------------------------------------------------------------------------------
// include files for GALILEI
#include <gplugin.h>
#include <gpluginmanager.h>


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// API VERSION
#define API_GROUPCALC_VERSION "2.0"


//------------------------------------------------------------------------------
// forward declaration
class GGroupRef;


//------------------------------------------------------------------------------
/**
* The GGroupCalc class provides a representation for a generic method to compute
* the description of a specific group.
* @author Pascal Francq
* @short Generic Group Computing Method.
*/
class GGroupCalc : public GPlugin<GFactoryGroupCalc>
{
public:

	/**
	* Constructor.
	* @param fac             Factory of the plugin.
	*/
	GGroupCalc(GFactoryGroupCalc* fac);

	/**
	* Compute a group.
	* @param grp            Reference to the group to compute.
	*/
	virtual void Compute(GGroup* grp)=0;

	/**
	* Destructor.
	*/
	virtual ~GGroupCalc(void);
};


//------------------------------------------------------------------------------
/*
* The GFactoryGroupCalc represent a factory for a given group computing method.
* @author Pascal Francq
* @short Generic Grouping Computing Factory.
*/
class GFactoryGroupCalc : public GFactoryPlugin<GFactoryGroupCalc,GGroupCalc,GGroupCalcManager>
{
public:

	/*
	* Constructor.
	* @param mng             Manager of the plugin.
	* @param n               Name of the Factory/Plugin.
	* @param f               Lib of the Factory/Plugin.
	*/
	GFactoryGroupCalc(GGroupCalcManager* mng,const char* n,const char* f)
		 : GFactoryPlugin<GFactoryGroupCalc,GGroupCalc,GGroupCalcManager>(mng,n,f) {}
};


//-----------------------------------------------------------------------------
/**
* The GGroupCalcManager class provides a representation for a manager
* responsible to manage all the group computing methods.
* @author Pascal Francq
* @short Group Comuting Methods Manager.
*/
class GGroupCalcManager : public GPluginManager<GGroupCalcManager,GFactoryGroupCalc,GGroupCalc>
{
public:

	/**
	* Construct the group computing method manager.
	*/
	GGroupCalcManager(void);

	/**
	* Destructor of the group computing methods manager.
	*/
	virtual ~GGroupCalcManager(void);
};


//------------------------------------------------------------------------------
#define CREATE_GROUPCALC_FACTORY(name,plugin)\
	CREATE_FACTORY(GGroupCalcManager,GFactoryGroupCalc,GGroupCalc,plugin,"GroupCalc",API_GROUPCALC_VERSION,name)


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
