/*

	GALILEI Research Project

	GIRProm.cpp

	PROMETHEE Kernel for Grouping - Implementation.

	Copyright 2001-2002 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).

	Version $Revision$

	Last Modify: $Date$

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
// include files for R Project
#include <rpromethee/rpromsol.h>
#include <rpromethee/rpromcriterion.h>
using namespace RPromethee;


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <groups/girprom.h>
#include <groups/gir.h>
#include <groups/gchromoir.h>
using namespace GALILEI;



//-----------------------------------------------------------------------------
//
// class GPlugin
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
GALILEI::GIRProm::GIRProm(GIRParams* p) throw(bad_alloc)
	: RPromKernel("GALILEI",20,10), Params(p), CritSim(0), CritSimAvgSim(0),
	  CritSimJ(0), CritSimAvgRatio(0),CritSimMinRatio(0),CritSimRatio(0),
	  CritSimWOverB(0), CritSimSimWB(0), CritInfo(0), CritEntropy(0),
	  CritLikelihood(0), CritSameFeedbacks(0), CritDiffFeedbacks(0),
	  CritSocial(0)
{
	// Init Criterion and Solutions of the PROMETHEE part
	if(Params->SimMeasures==sctCorl)
		CritSim=NewCriterion(Maximize,"Similarity",Params->ParamsSim.P,Params->ParamsSim.Q,Params->ParamsSim.Weight);
	else
	{
		if(Params->Measures.GetPtr<const char*>("AvgSim")->Use)
			CritSimAvgSim=NewCriterion(Maximize,"Similarity AvgSim",Params->ParamsSim.P,Params->ParamsSim.Q,Params->ParamsSim.Weight);
		if(Params->Measures.GetPtr<const char*>("J")->Use)
			CritSimJ=NewCriterion(Maximize,"Similarity J",Params->ParamsSim.P,Params->ParamsSim.Q,Params->ParamsSim.Weight);
		if(Params->Measures.GetPtr<const char*>("AvgRatio")->Use)
			CritSimAvgRatio=NewCriterion(Maximize,"Similarity AvgRatio",Params->ParamsSim.P,Params->ParamsSim.Q,Params->ParamsSim.Weight);
		if(Params->Measures.GetPtr<const char*>("MinRatio")->Use)
			CritSimMinRatio=NewCriterion(Maximize,"Similarity MinRatio",Params->ParamsSim.P,Params->ParamsSim.Q,Params->ParamsSim.Weight);
		if(Params->Measures.GetPtr<const char*>("Ratio")->Use)
			CritSimRatio=NewCriterion(Maximize,"Similarity Ratio",Params->ParamsSim.P,Params->ParamsSim.Q,Params->ParamsSim.Weight);
		if(Params->Measures.GetPtr<const char*>("WOverB")->Use)
			CritSimWOverB=NewCriterion(Maximize,"Similarity WoverB",Params->ParamsSim.P,Params->ParamsSim.Q,Params->ParamsSim.Weight);
		if(Params->Measures.GetPtr<const char*>("SimWB")->Use)
			CritSimSimWB=NewCriterion(Maximize,"Similarity SimWB",Params->ParamsSim.P,Params->ParamsSim.Q,Params->ParamsSim.Weight);
	}
	if(Params->ParamsInfo.Weight)
		CritInfo=NewCriterion(Minimize,"Information",Params->ParamsInfo.P,Params->ParamsInfo.Q,Params->ParamsInfo.Weight);
	if(Params->ParamsEntropy.Weight)
		CritEntropy=NewCriterion(Minimize,"Entropy",Params->ParamsEntropy.P,Params->ParamsEntropy.Q,Params->ParamsEntropy.Weight);
	if(Params->ParamsLikelihood.Weight)
		CritLikelihood=NewCriterion(Minimize,"Likelihood",Params->ParamsLikelihood.P,Params->ParamsLikelihood.Q,Params->ParamsLikelihood.Weight);
	if(Params->ParamsSameFeedbacks.Weight)
		CritSameFeedbacks=NewCriterion(Maximize,"Same Feedbacks",Params->ParamsSameFeedbacks.P,Params->ParamsSameFeedbacks.Q,Params->ParamsSameFeedbacks.Weight);
	if(Params->ParamsDiffFeedbacks.Weight)
		CritDiffFeedbacks=NewCriterion(Minimize,"Diff Feedbacks",Params->ParamsDiffFeedbacks.P,Params->ParamsDiffFeedbacks.Q,Params->ParamsDiffFeedbacks.Weight);
	if(Params->ParamsSocial.Weight)
		CritSocial=NewCriterion(Minimize,"Social",Params->ParamsSocial.P,Params->ParamsSocial.Q,Params->ParamsSocial.Weight);
}


//-----------------------------------------------------------------------------
void GALILEI::GIRProm::Assign(RPromethee::RPromSol* s,GChromoIR* c)
{
	RPromKernel::Assign(s,CritSim,c->CritSim);
	RPromKernel::Assign(s,CritSimAvgSim,c->CritSimAvgSim);
	RPromKernel::Assign(s,CritSimJ,c->CritSimJ);
	RPromKernel::Assign(s,CritSimAvgRatio,c->CritSimAvgRatio);
	RPromKernel::Assign(s,CritSimMinRatio,c->CritSimMinRatio);
	RPromKernel::Assign(s,CritSimRatio,c->CritSimRatio);
	RPromKernel::Assign(s,CritSimWOverB,c->CritSimWOverB);
	RPromKernel::Assign(s,CritSimSimWB,c->CritSimSimWB);
	RPromKernel::Assign(s,CritInfo,c->CritInfo);
	RPromKernel::Assign(s,CritEntropy,c->CritEntropy);
	RPromKernel::Assign(s,CritLikelihood,c->CritLikelihood);
	RPromKernel::Assign(s,CritSameFeedbacks,c->CritSameFeedbacks);
	RPromKernel::Assign(s,CritDiffFeedbacks,c->CritDiffFeedbacks);
	RPromKernel::Assign(s,CritSocial,c->CritSocial);
}
