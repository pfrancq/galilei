/*

	GALILEI Research Project

	GCommunityCalc.h

	Generic Community Computing Method - Header.

	Copyright 2002-2009 by Pascal Francq (pascal@francq.info).
	Copyright 2002-2008 by the Université Libre de Bruxelles (ULB).

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
#include <gweightinfos.h>


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* The GCommunityCalc class provides a representation for a generic method to
* compute the description of a specific community.
* @author Pascal Francq
* @short Generic Community Computing Method.
*/
class GCommunityCalc : public GPlugin
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
	GCommunityCalc(GPluginFactory* fac);

	/**
	* Compute a community.
	* @param com            Community to compute.
	*/
	virtual void Compute(const GCommunity* com)=0;

	/**
	* Destruct.
	*/
	virtual ~GCommunityCalc(void);

	friend class GSession;
};


//------------------------------------------------------------------------------
#define CREATE_COMMUNITYCALC_FACTORY(name,desc,plugin)\
	CREATE_FACTORY(GALILEI::GCommunityCalc,plugin,"CommunityCalc",R::RString::Null,name,desc)


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
