/*

	GALILEI Research Project

	GCommunityCalc.h

	Generic Group Computing Method - Header.

	Copyright 2002-2008 by the Université Libre de Bruxelles.

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
#ifndef GCommunityCalcH
#define GCommunityCalcH


//------------------------------------------------------------------------------
// include files for GALILEI
#include <gplugin.h>
#include <gpluginmanager.h>


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// API VERSION
#define API_COMMUNITYCALC_VERSION "2.0"


//------------------------------------------------------------------------------
// forward declaration
class GCommunityRef;


//------------------------------------------------------------------------------
/**
* The GCommunityCalc class provides a representation for a generic method to compute
* the description of a specific group.
* @author Pascal Francq
* @short Generic Group Computing Method.
*/
class GCommunityCalc : public GPlugin<GFactoryCommunityCalc>
{
public:

	/**
	* Constructor.
	* @param fac             Factory of the plugin.
	*/
	GCommunityCalc(GFactoryCommunityCalc* fac);

	/**
	* Compute a group.
	* @param grp            Reference to the group to compute.
	*/
	virtual void Compute(GCommunity* grp)=0;

	/**
	* Destructor.
	*/
	virtual ~GCommunityCalc(void);
};


//------------------------------------------------------------------------------
/*
* The GFactoryCommunityCalc represent a factory for a given group computing method.
* @author Pascal Francq
* @short Generic Grouping Computing Factory.
*/
class GFactoryCommunityCalc : public GFactoryPlugin<GFactoryCommunityCalc,GCommunityCalc,GCommunityCalcManager>
{
public:

	/*
	* Constructor.
	* @param mng             Manager of the plugin.
	* @param n               Name of the Factory/Plugin.
	* @param f               Lib of the Factory/Plugin.
	*/
	GFactoryCommunityCalc(GCommunityCalcManager* mng,const char* n,const char* f)
		 : GFactoryPlugin<GFactoryCommunityCalc,GCommunityCalc,GCommunityCalcManager>(mng,n,f) {}
};


//-----------------------------------------------------------------------------
/**
* The GCommunityCalcManager class provides a representation for a manager
* responsible to manage all the group computing methods.
* @author Pascal Francq
* @short Group Comuting Methods Manager.
*/
class GCommunityCalcManager : public GPluginManager<GCommunityCalcManager,GFactoryCommunityCalc,GCommunityCalc>
{
public:

	/**
	* Construct the group computing method manager.
	*/
	GCommunityCalcManager(void);

	/**
	* Destructor of the group computing methods manager.
	*/
	virtual ~GCommunityCalcManager(void);
};


//------------------------------------------------------------------------------
#define CREATE_COMMUNITYCALC_FACTORY(name,plugin)\
	CREATE_FACTORY(GCommunityCalcManager,GFactoryCommunityCalc,GCommunityCalc,plugin,"CommunityCalc",API_COMMUNITYCALC_VERSION,name)


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
