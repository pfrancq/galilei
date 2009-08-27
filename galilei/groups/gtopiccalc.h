/*

	GALILEI Research Project

	GTopicCalc.h

	Generic Topic Computing Method - Header.

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
#ifndef GTopicCalcH
#define GTopicCalcH


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
#define API_TOPICCALC_VERSION "2.0"


//------------------------------------------------------------------------------
/**
* The GTopicCalc class provides a representation for a generic method to compute
* the description of a specific topic.
* @author Pascal Francq
* @short Generic Topic Computing Method.
*/
class GTopicCalc : public GPlugin<GFactoryTopicCalc>
{
protected:

	/**
	 * Container that should contain the structure after the analyze.
	 */
	GWeightInfos Infos;

public:

	/**
	* Constructor.
	* @param fac             Factory of the plug-in.
	*/
	GTopicCalc(GFactoryTopicCalc* fac);

	/**
	* Compute a topic.
	* @param topic           Topic to compute.
	*/
	virtual void Compute(const GTopic* topic)=0;

	/**
	* Destruct.
	*/
	virtual ~GTopicCalc(void);

	friend class GSession;
};


//------------------------------------------------------------------------------
/**
* The GFactoryTopicCalc represent a factory for a given topic computing method.
* @author Pascal Francq
* @short Generic Topic Computing Factory.
*/
class GFactoryTopicCalc : public GFactoryPlugin<GFactoryTopicCalc,GTopicCalc,GTopicCalcManager>
{
public:

	/**
	* Constructor.
	* @param mng             Manager of the plug-in.
	* @param n               Name of the Factory/Plug-in.
	* @param f               Library of the Factory/Plug-in.
	*/
	GFactoryTopicCalc(GTopicCalcManager* mng,const char* n,const char* f)
		 : GFactoryPlugin<GFactoryTopicCalc,GTopicCalc,GTopicCalcManager>(mng,n,f) {}
};


//-----------------------------------------------------------------------------
/**
* The GTopicCalcManager class provides a representation for a manager
* responsible to manage all the topic computing methods.
* @author Pascal Francq
* @short Topic Computing Methods Manager.
*/
class GTopicCalcManager : public GPluginManager<GTopicCalcManager,GFactoryTopicCalc,GTopicCalc>
{
public:

	/**
	* Construct the topic computing method manager.
	*/
	GTopicCalcManager(void);

	/**
	* Destruct the topic computing methods manager.
	*/
	virtual ~GTopicCalcManager(void);
};


//------------------------------------------------------------------------------
#define CREATE_TOPICCALC_FACTORY(name,plugin)\
	CREATE_FACTORY(GTopicCalcManager,GFactoryTopicCalc,GTopicCalc,plugin,"TopicCalc",API_TOPICCALC_VERSION,name)


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
