/*

	GALILEI Research Project

	GProfileCalc.h

	Generic Profile Computing Method - Header.

	Copyright 2001-2009 by Pascal Francq (pascal@francq.info).
	Copyright 2001-2008 by the Université Libre de Bruxelles (ULB).

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
#ifndef GProfileCalcH
#define GProfileCalcH


//------------------------------------------------------------------------------
// include files for GALILEI
#include <gplugin.h>
#include <gpluginmanager.h>
#include <gweightinfos.h>


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// API VERSION
#define API_PROFILECALC_VERSION "2.0"


//------------------------------------------------------------------------------
// forward declaration
class GSubProfileRef;


//------------------------------------------------------------------------------
/**
* The GProfileCalc class provides a representation for a method to compute a
* specific profile.
* @author Pascal Francq
* @short Generic Profile Computing Method.
*/
class GProfileCalc : public GPlugin<GFactoryProfileCalc>
{
protected:

	/**
	 * Container that should contain the structure after the analyze.
	 */
	GWeightInfos Infos;

public:

	/**
	* Construct the computing method.
	* @param fac             Factory of the plugin.
	*/
	GProfileCalc(GFactoryProfileCalc* fac);

	/**
	* Compute a profile.
	* @param profile         Reference to the profile to compute.
	*/
	virtual void Compute(const GProfile* profile)=0;

	/**
	* Destructor of the computing method.
	*/
	virtual ~GProfileCalc(void);

	friend class GSession;
};


//------------------------------------------------------------------------------
/**
* The GFactoryProfileCalc represent a factory for a given profile computing
* method.
* @author Pascal Francq
* @short Generic Profile Computing Factory.
*/
class GFactoryProfileCalc : public GFactoryPlugin<GFactoryProfileCalc,GProfileCalc,GProfileCalcManager>
{
public:

	/**
	* Constructor.
	* @param mng             Manager of the plugin.
	* @param n               Name of the Factory/Plugin.
	* @param f               Lib of the Factory/Plugin.
	*/
	GFactoryProfileCalc(GProfileCalcManager* mng,const char* n,const char* f)
		 : GFactoryPlugin<GFactoryProfileCalc,GProfileCalc,GProfileCalcManager>(mng,n,f) {}
};


//------------------------------------------------------------------------------
/**
* The GProfileCalcManager class provides a representation for a manager
* responsible to manage all the profile computing methods.
* @author Pascal Francq
* @short Profile Computing Methods Manager.
*/
class GProfileCalcManager : public GPluginManager<GProfileCalcManager,GFactoryProfileCalc,GProfileCalc>
{
public:

	/**
	* Construct the profile computing methods manager.
	*/
	GProfileCalcManager(void);

	/**
	* Destructor of the profile computing methods manager.
	*/
	virtual ~GProfileCalcManager(void);
};


//-------------------------------------------------------------------------------
#define CREATE_PROFILECALC_FACTORY(name,plugin)\
	CREATE_FACTORY(GProfileCalcManager,GFactoryProfileCalc,GProfileCalc,plugin,"ProfileCalc",API_PROFILECALC_VERSION,name)


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
