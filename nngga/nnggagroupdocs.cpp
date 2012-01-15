/*

	GALILEI Research Project

	NNGGAGroupDocs.cpp

	NNGGA Plug-in for Documents - Implementation

	Copyright 2008-2012 by Pascal Francq (pascal@francq.info).

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
using namespace GALILEI;
using namespace R;


//-----------------------------------------------------------------------------
// include files for NNGGA
#include <nnggaplugin.h>
#include <nnggaplugin.hh>
#include <nnggainst.h>
#include <nnggagroup.h>


//-----------------------------------------------------------------------------
/**
* The NNGGAGroupDocs provides a representation for a method to group some
* documents using the NNGGA.
* @author Pascal Francq
* @short GGA for Documents.
*/
class NNGGAGroupDocs : public NNGGAPlugIn<GGroupDocs,GDoc,GTopic,NNGGAInstDoc,NNGGAGroupDoc>
{
	using NNGGAPlugIn<GGroupDocs,GDoc,GTopic,NNGGAInstDoc,NNGGAGroupDoc>::FindParam;

public:

	/**
	* Constructor.
	* @param session        Session.
	* @param f              Factory.
	*/
	NNGGAGroupDocs(GSession* session,GPlugInFactory* fac)
		: NNGGAPlugIn<GGroupDocs,GDoc,GTopic,NNGGAInstDoc,NNGGAGroupDoc>(session,fac,"Documents Grouping",otDoc,otTopic) {}

	/**
	 * Class name.
	 */
	virtual R::RCString GetClassName(void) const {return("NNGGAGroupDocs");}

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
		Incremental=FindParam<RParamValue>("Incremental")->GetBool();
		InternalRandom=FindParam<RParamValue>("InternalRandom")->GetBool();
		Seed=FindParam<RParamValue>("Seed")->GetInt();
		ParamsSim=FindParam<R::RParamStruct>("Sim Criterion");
		ParamsAgreement=FindParam<R::RParamStruct>("Agreement Criterion");
		ParamsDisagreement=FindParam<R::RParamStruct>("Disagreement Criterion");
		Cout=FindParam<RParamValue>("Cout")->GetBool();
		NbMinObjs=FindParam<RParamValue>("NbMinObjs")->GetUInt();
		NbMaxObjs=FindParam<RParamValue>("NbMaxObjs")->GetUInt();
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
CREATE_GROUPDOCS_FACTORY("NNGGA for Documents","NNGGA for Documents (Experimental)",NNGGAGroupDocs)
