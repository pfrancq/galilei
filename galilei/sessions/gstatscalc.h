/*

	GALILEI Research Project

	GStatsCalc.h

	Generic Statistics Method - Header.

	Copyright 2003-2009 by Pascal Francq (pascal@francq.info).
	Copyright 2003-2008 by the Université Libre de Bruxelles (ULB).

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
#ifndef GStatsCalcH
#define GStatsCalcH


//------------------------------------------------------------------------------
// include files for GALILEI
#include <gplugin.h>
#include <gpluginmanager.h>


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// API VERSION
#define API_STATSCALC_VERSION "2.0"


//------------------------------------------------------------------------------
/**
* The GStatsCalc class provides a representation for a set of statistics to
* compute.
* @author Pascal Francq
* @short Generic Statistics Method.
*/
class GStatsCalc : public GPlugin<GFactoryStatsCalc>
{
public:

	/**
	* Construct the statistics method.
	* @param fac             Factory of the plugin.
	*/
	GStatsCalc(GFactoryStatsCalc* fac);

	/**
	* Compute the statistics.
	* @param xml             XML Structure.
	* @param res             XML tag that will be hold the results.
	*/
	virtual void Compute(R::RXMLStruct* xml,R::RXMLTag& res)=0;

	/**
	* Add a tag representing an element with a value.
	* @param xml             XML Structure.
	* @param parent          XML tag that will be hold the result.
	* @param element         Name of the element.
	* @param val             Value.
	*/
	void AddTag(R::RXMLStruct* xml,R::RXMLTag* parent,R::RString element,double val);

	/**
	* Add a tag representing an element with a value.
	* @param xml             XML Structure.
	* @param parent          XML tag that will be hold the result.
	* @param element         Name of the element.
	* @param val             Value.
	*/
	void AddTag(R::RXMLStruct* xml,R::RXMLTag* parent,R::RString element,R::RString val);

	/**
	* Destructor of the statistics method.
	*/
	virtual ~GStatsCalc(void);
};


//------------------------------------------------------------------------------
/**
* The GFactoryStatsCalc represent a factory for a given statistics method.
* @author Pascal Francq
* @short Generic Statistics Factory.
*/
class GFactoryStatsCalc : public GFactoryPlugin<GFactoryStatsCalc,GStatsCalc,GStatsCalcManager>
{
public:

	/*
	* Constructor.
	* @param mng             Manager of the plugin.
	* @param n               Name of the Factory/Plugin.
	* @param f               Lib of the Factory/Plugin.
	*/
	GFactoryStatsCalc(GStatsCalcManager* mng,const char* n,const char* f)
		 : GFactoryPlugin<GFactoryStatsCalc,GStatsCalc,GStatsCalcManager>(mng,n,f) {}
};


//------------------------------------------------------------------------------
/**
* The GStatsCalcManager class provides a representation for a manager
* responsible to manage all the statistics.
* @author Pascal Francq
* @short Statistics Methods Manager.
*/
class GStatsCalcManager : public GPluginManager<GStatsCalcManager,GFactoryStatsCalc,GStatsCalc>
{
public:

	/**
	* Construct the statistics methods manager.
	*/
	GStatsCalcManager(void);

	/**
	* Destructor of the statistics methods manager.
	*/
	virtual ~GStatsCalcManager(void);
};


//-------------------------------------------------------------------------------
#define CREATE_STATSCALC_FACTORY(name,plugin)\
	CREATE_FACTORY(GStatsCalcManager,GFactoryStatsCalc,GStatsCalc,plugin,"StatsCalc",API_STATSCALC_VERSION,name)


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
