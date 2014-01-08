/*

	GALILEI Research Project

	SGGAGroupDocs.cpp

	SGGA Plug-in for Documents - Implementation

	Copyright 2008-2014 by Pascal Francq (pascal@francq.info).

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
#include <ggroupdocs.h>


//-----------------------------------------------------------------------------
// include files for SGGA
#include <sggaplugin.h>
#include <sggaplugin.hh>
#include <sggainst.h>
#include <sggagroup.h>
using namespace GALILEI;



//-----------------------------------------------------------------------------
/**
* The SGGAGroupDocs provides a representation for a method to group some
* documents using the SGGA.
* @author Pascal Francq
* @short GGA for Documents.
*/
class SGGAGroupDocs : public SGGAPlugIn<GGroupDocs,GDoc,GTopic,SGGAInstDoc,SGGAGroupDoc>
{
	using SGGAPlugIn<GGroupDocs,GDoc,GTopic,SGGAInstDoc,SGGAGroupDoc>::FindParam;

public:

	/**
	* Constructor.
	* @param f              Factory.
	*/
	SGGAGroupDocs(GSession* session,GPlugInFactory* fac)
		: SGGAPlugIn<GGroupDocs,GDoc,GTopic,SGGAInstDoc,SGGAGroupDoc>(session,fac,"Documents Grouping",otDoc,otTopic) {}

	/**
	 * Class name.
	 */
	virtual R::RCString GetClassName(void) const {return("SGGAGroupDocs");}

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
	virtual R::RCursor<GDoc> GetObjs(void) const {return(RCursor<GDoc>(Docs));}

	/**
	* Make the grouping for a specific Language.
	*/
	virtual void Run(void) {RunGrouping(Session,"Document");}
};


//------------------------------------------------------------------------------
CREATE_GROUPDOCS_FACTORY("SGGA for Documents","SGGA for Documents",SGGAGroupDocs)
