/*

	GALILEI Research Project

	GCAGroupProfiles.h

	GCA Plug-in for Profiles - Implementation

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
// include files for GCA
#include <gcaplugin.h>
#include <gcaplugin.hh>
#include <gcakmeans.h>
using namespace GALILEI;



//-----------------------------------------------------------------------------
/**
* The GCAGroupProfiles provides a representation for a method to group some
* profiles using the GCA.
* @author Pascal Francq
* @short GGA for Profiles.
*/
class GCAGroupProfiles : public GCAPlugIn<GGroupProfiles,GProfile,GCommunity,GCAInstProfile,GCAGroupProfile,kMeansProfile,CGroupProfile,CGroupsProfile>
{
	using GCAPlugIn<GGroupProfiles,GProfile,GCommunity,GCAInstProfile,GCAGroupProfile,kMeansProfile,CGroupProfile,CGroupsProfile>::FindParam;

public:

	/**
	* Constructor.
	* @param session        Session.
	* @param f              Factory.
	*/
	GCAGroupProfiles(GSession* session,GPlugInFactory* fac)
		: GCAPlugIn<GGroupProfiles,GProfile,GCommunity,GCAInstProfile,GCAGroupProfile,kMeansProfile,CGroupProfile,CGroupsProfile>(session,fac,"Profiles Grouping",otProfile,otCommunity) {}

	/**
	 * Class name.
	 */
	virtual R::RCString GetClassName(void) const {return("GCAGroupProfiles");}

	/**
	* Configurations were applied from the factory.
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
		Incremental=FindParam<RParamValue>("Incremental")->GetBool();
		InternalRandom=FindParam<RParamValue>("InternalRandom")->GetBool();
		Seed=FindParam<RParamValue>("Seed")->GetInt();
		ParamsSim=FindParam<R::RParamStruct>("Sim Criterion");
		ParamsAgreement=FindParam<R::RParamStruct>("Agreement Criterion");
		ParamsDisagreement=FindParam<R::RParamStruct>("Disagreement Criterion");
		RString what(FindParam<RParamValue>("Clustering Method")->Get());
		ClusteringMethod=0;
		if(what=="GA")
			ClusteringMethod=1;
		if(what=="k-Means")
			ClusteringMethod=2;
		NbClusters=FindParam<RParamValue>("NbClusters")->GetUInt();
		Cout=FindParam<RParamValue>("Cout")->GetBool();
		NbMinObjs=FindParam<RParamValue>("NbMinObjs")->GetUInt();
		NbMaxObjs=FindParam<RParamValue>("NbMaxObjs")->GetUInt();
	}

protected:

	/**
	 * Get a pointer to the objects to cluster.
	 */
	virtual R::RCursor<GProfile> GetObjs(void) const {return(Session->GetProfiles());}

	/**
	* Make the grouping for a specific Language.
	*/
	virtual void Run(void) {RunGrouping(Session,"Profiles",Session->GetCommunities(),Session->GetProfiles());}
};


//------------------------------------------------------------------------------
CREATE_GROUPPROFILES_FACTORY("GCA for Profiles","GCA for Profiles",GCAGroupProfiles)
