/*

	Genetic Community Algorithm

	NNGGAInst.h

	Instance - Implementation

	Copyright 2003-2012 by Pascal Francq (pascal@francq.info).
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
// include files for R Project
#include <rpromsol.h>
#include <rpromcriterion.h>
#include <rgroupingheuristic.h>


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <gprofile.h>
#include <ggalileiapp.h>
#include <ggroup.h>
#include <gsession.h>
#include <gmeasure.h>


//-----------------------------------------------------------------------------
// include files for NNGGA
#include <nnggainst.h>
#include <nnggachromo.h>
#include <nnggagroup.h>
using namespace R;
using namespace std;



//-----------------------------------------------------------------------------
//
// NNGGAInstDoc
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
NNGGAInstDoc::NNGGAInstDoc(GSession* ses,RCursor<GDoc> objs,RParamsNN* p,RDebug *debug,tObjType type)
	: RInstNN<NNGGAInstDoc,NNGGAChromoDoc,NNGGAThreadDataDoc,NNGGAGroupDoc,GDoc>(objs,p,debug),
	  Session(ses), Sims(0),Agree(0), Disagree(0), Type(type)
{
	// Init measures
	Sims=GALILEIApp->GetCurrentPlugIn<GMeasure>("Measures","Document Similarities");
	Agree=GALILEIApp->GetCurrentPlugIn<GMeasure>("Measures","Document Agreements");
	Disagree=GALILEIApp->GetCurrentPlugIn<GMeasure>("Measures","Document Disagreements");

	if((!Sims)||(!Agree)||(!Disagree))
		mThrowGException(GetObjType(Type,true,true)+" are not supported");
}


//-----------------------------------------------------------------------------
RGroupingHeuristic<NNGGAGroupDoc,GDoc,NNGGAChromoDoc>* NNGGAInstDoc::CreateHeuristic(void)
{
	return(new NNGGAHeuristicDoc(Random,Objs,Params,Debug));
}


//-----------------------------------------------------------------------------
const RMaxVector* NNGGAInstDoc::GetDisagreementRatios(const GDoc* obj) const
{
	const RMaxVector* Vec;
	Disagree->Info(3,obj->GetId(),&Vec);
	return(Vec);
}


//-----------------------------------------------------------------------------
const RMaxVector* NNGGAInstDoc::GetAgreementRatios(const GDoc* obj) const
{
	const RMaxVector* Vec;
	Agree->Info(3,obj->GetId(),&Vec);
	return(Vec);
}


//-----------------------------------------------------------------------------
const RMaxVector* NNGGAInstDoc::GetSims(const GDoc* obj) const
{
	const RMaxVector* Vec;
	Sims->Info(3,obj->GetId(),&Vec);
	return(Vec);
}



//-----------------------------------------------------------------------------
//
// NNGGAInstProfile
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
NNGGAInstProfile::NNGGAInstProfile(GSession* ses,RCursor<GProfile> objs,RParamsNN* p,RDebug *debug,tObjType type)
	: RInstNN<NNGGAInstProfile,NNGGAChromoProfile,NNGGAThreadDataProfile,NNGGAGroupProfile,GProfile>(objs,p,debug),
	  Session(ses), Sims(0),Agree(0), Disagree(0), Type(type)
{
	// Init measures
	Sims=GALILEIApp->GetCurrentPlugIn<GMeasure>("Measures","Profile Similarities");
	Agree=GALILEIApp->GetCurrentPlugIn<GMeasure>("Measures","Profile Agreements");
	Disagree=GALILEIApp->GetCurrentPlugIn<GMeasure>("Measures","Profile Disagreements");

	if((!Sims)||(!Agree)||(!Disagree))
		mThrowGException(GetObjType(Type,true,true)+" are not supported");
}


//-----------------------------------------------------------------------------
RGroupingHeuristic<NNGGAGroupProfile,GProfile,NNGGAChromoProfile>* NNGGAInstProfile::CreateHeuristic(void)
{
	return(new NNGGAHeuristicProfile(Random,Objs,Params,Debug));
}


//-----------------------------------------------------------------------------
/*double NNGGAInst::GetDisagreementRatio(const NNGGAObj* obj1,const NNGGAObj* obj2) const
{
	double d;
	Disagree->Measure(0,obj1->GetElementId(),obj2->GetElementId(),&d);
	return(d);
}


//-----------------------------------------------------------------------------
double NNGGAInst::GetAgreementRatio(const NNGGAObj* obj1,const NNGGAObj* obj2) const
{
	double d;
	Agree->Measure(0,obj1->GetElementId(),obj2->GetElementId(),&d);
	return(d);
}


//-----------------------------------------------------------------------------
double NNGGAInst::GetSim(const NNGGAObj* obj1,const NNGGAObj* obj2) const
{
	double d;
	Sims->Measure(0,obj1->GetElementId(),obj2->GetElementId(),&d);
	return(d);
}*/


//-----------------------------------------------------------------------------
const RMaxVector* NNGGAInstProfile::GetDisagreementRatios(const GProfile* obj) const
{
	const RMaxVector* Vec;
	Disagree->Info(3,obj->GetId(),&Vec);
	return(Vec);
}


//-----------------------------------------------------------------------------
const RMaxVector* NNGGAInstProfile::GetAgreementRatios(const GProfile* obj) const
{
	const RMaxVector* Vec;
	Agree->Info(3,obj->GetId(),&Vec);
	return(Vec);
}


//-----------------------------------------------------------------------------
const RMaxVector* NNGGAInstProfile::GetSims(const GProfile* obj) const
{
	const RMaxVector* Vec;
	Sims->Info(3,obj->GetId(),&Vec);
	return(Vec);
}
