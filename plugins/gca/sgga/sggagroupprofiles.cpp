/*

	GALILEI Research Project

	SGGAGroupProfiles.h

	SGGA Plug-in for Profiles - Implementation

	Copyright 2003-2010 by Pascal Francq (pascal@francq.info).
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



//-----------------------------------------------------------------------------
// include files for GALILEI
#include <ggroupprofiles.h>


//-----------------------------------------------------------------------------
// include files for SGGA
#include <sggaplugin.h>
#include <sggaplugin.hh>
#include <sggainst.h>
#include <sggagroup.h>
using namespace GALILEI;



//-----------------------------------------------------------------------------
/**
* The SGGAGroupProfiles provides a representation for a method to group some
* profiles using the SGGA.
* @author Pascal Francq
* @short GGA for Profiles.
*/
class SGGAGroupProfiles : public SGGAPlugIn<GGroupProfiles,GProfile,GCommunity,SGGAInstProfile,SGGAGroupProfile>
{
	using SGGAPlugIn<GGroupProfiles,GProfile,GCommunity,SGGAInstProfile,SGGAGroupProfile>::FindParam;

public:

	/**
	* Constructor.
	* @param f              Factory.
	*/
	SGGAGroupProfiles(GSession* session,GPlugInFactory* fac)
		: SGGAPlugIn<GGroupProfiles,GProfile,GCommunity,SGGAInstProfile,SGGAGroupProfile>(session,fac,"Profiles Grouping",otProfile,otCommunity) {}

	/**
	 * Class name.
	 */
	virtual R::RCString GetClassName(void) const {return("SGGAGroupProfiles");}

	/**
	* Configurations were applied.
	*/
	virtual void ApplyConfig(void)
	{
		PopSize=FindParam<RParamValue>("Population Size")->GetUInt();
		MaxGen=FindParam<RParamValue>("Max Gen")->GetUInt();
		Step=FindParam<RParamValue>("Step")->GetBool();
		StepGen=FindParam<RParamValue>("Step Gen")->GetUInt();
		MinAgreement=FindParam<RParamValue>("Min Agreement")->GetDouble();
		MinDisagreement=FindParam<RParamValue>("Min Disagreement")->GetDouble();
		MaxKMeans=FindParam<RParamValue>("Max kMeans")->GetUInt();
		Convergence=FindParam<RParamValue>("Convergence")->GetDouble();
		NbDivChromo=FindParam<RParamValue>("NbDivChromos")->GetUInt();
		LocalOptimisation=FindParam<RParamValue>("LocalOptimisation")->GetBool();
		Optimisation=FindParam<RParamValue>("Optimisation")->GetBool();
		Incremental=FindParam<RParamValue>("Incremental")->GetBool();
		InternalRandom=FindParam<RParamValue>("InternalRandom")->GetBool();
		Seed=FindParam<RParamValue>("Seed")->GetInt();
		ParamsSim=FindParam<R::RParamStruct>("Sim Criterion");
		ParamsAgreement=FindParam<R::RParamStruct>("Agreement Criterion");
		ParamsDisagreement=FindParam<R::RParamStruct>("Disagreement Criterion");
		Cout=FindParam<RParamValue>("Cout")->GetBool();
		NbMinObjs=FindParam<RParamValue>("NbMinObjs")->GetUInt();
		NbMaxObjs=FindParam<RParamValue>("NbMaxObjs")->GetUInt();
		AllMinSim=FindParam<RParamValue>("AllMinSim")->GetBool();
	}

protected:

	/**
	 * Get a pointer to the objects to cluster.
	 */
	virtual R::RCursor<GProfile> GetObjs(void) const {return(Session->GetObjs(pProfile));}

	/**
	* Make the grouping for a specific Language.
	*/
	virtual void Run(void) {RunGrouping(Session,"Profile");}

public:
};


//------------------------------------------------------------------------------
CREATE_GROUPPROFILES_FACTORY("SGGA for Profiles","SGGA for Profiles",SGGAGroupProfiles)
