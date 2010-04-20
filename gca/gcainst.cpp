/*

	Genetic Community Algorithm

	GCAInst.h

	Instance - Implementation

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
#include <gsubjects.h>
#include <gmeasure.h>


//-----------------------------------------------------------------------------
// include files for GCA
#include <gcainst.h>
#include <gcachromo.h>
#include <gcagroup.h>
using namespace R;
using namespace std;



//-----------------------------------------------------------------------------
//
// GCAInstDoc
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GCAInstDoc::GCAInstDoc(GSession* ses,RCursor<GDoc> objs,RParamsSC* p,RDebug *debug,tObjType type)
	: RInstSC<GCAInstDoc,GCAChromoDoc,GCAThreadDataDoc,GCAGroupDoc,GDoc>(objs,p,debug),
	  Session(ses), Sims(0),Agree(0), Disagree(0), Type(type)
{
	// Init measures
	Sims=GALILEIApp->GetCurrentPlugIn<GMeasure>("Measures","Documents Similarities");
	Agree=GALILEIApp->GetCurrentPlugIn<GMeasure>("Measures","Documents Agreements");
	Disagree=GALILEIApp->GetCurrentPlugIn<GMeasure>("Measures","Documents Disagreements");

	if((!Sims)||(!Agree)||(!Disagree))
		ThrowGException(GetObjType(Type,true,true)+" are not supported");
}


//-----------------------------------------------------------------------------
RGroupingHeuristic<GCAGroupDoc,GDoc,GCAChromoDoc>* GCAInstDoc::CreateHeuristic(void)
{
	return(new GCAHeuristicDoc(Random,Objs,Params,Debug));
}


//-----------------------------------------------------------------------------
const RMaxVector* GCAInstDoc::GetDisagreementRatios(const GDoc* obj) const
{
	const RMaxVector* Vec;
	Disagree->Info(3,obj->GetId(),&Vec);
	return(Vec);
}


//-----------------------------------------------------------------------------
const RMaxVector* GCAInstDoc::GetAgreementRatios(const GDoc* obj) const
{
	const RMaxVector* Vec;
	Agree->Info(3,obj->GetId(),&Vec);
	return(Vec);
}


//-----------------------------------------------------------------------------
const RMaxVector* GCAInstDoc::GetSims(const GDoc* obj) const
{
	const RMaxVector* Vec;
	Sims->Info(3,obj->GetId(),&Vec);
	return(Vec);
}



//-----------------------------------------------------------------------------
//
// GCAInstProfile
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GCAInstProfile::GCAInstProfile(GSession* ses,RCursor<GProfile> objs,RParamsSC* p,RDebug *debug,tObjType type)
	: RInstSC<GCAInstProfile,GCAChromoProfile,GCAThreadDataProfile,GCAGroupProfile,GProfile>(objs,p,debug),
	  Session(ses), Sims(0),Agree(0), Disagree(0), Type(type)
{
	// Init measures
	Sims=GALILEIApp->GetCurrentPlugIn<GMeasure>("Measures","Profiles Similarities");
	Agree=GALILEIApp->GetCurrentPlugIn<GMeasure>("Measures","Profiles Agreements");
	Disagree=GALILEIApp->GetCurrentPlugIn<GMeasure>("Measures","Profiles Disagreements");

	if((!Sims)||(!Agree)||(!Disagree))
		ThrowGException(GetObjType(Type,true,true)+" are not supported");
}


//-----------------------------------------------------------------------------
RGroupingHeuristic<GCAGroupProfile,GProfile,GCAChromoProfile>* GCAInstProfile::CreateHeuristic(void)
{
	return(new GCAHeuristicProfile(Random,Objs,Params,Debug));
}


//-----------------------------------------------------------------------------
/*double GCAInst::GetDisagreementRatio(const GCAObj* obj1,const GCAObj* obj2) const
{
	double d;
	Disagree->Measure(0,obj1->GetElementId(),obj2->GetElementId(),&d);
	return(d);
}


//-----------------------------------------------------------------------------
double GCAInst::GetAgreementRatio(const GCAObj* obj1,const GCAObj* obj2) const
{
	double d;
	Agree->Measure(0,obj1->GetElementId(),obj2->GetElementId(),&d);
	return(d);
}


//-----------------------------------------------------------------------------
double GCAInst::GetSim(const GCAObj* obj1,const GCAObj* obj2) const
{
	double d;
	Sims->Measure(0,obj1->GetElementId(),obj2->GetElementId(),&d);
	return(d);
}*/


//-----------------------------------------------------------------------------
const RMaxVector* GCAInstProfile::GetDisagreementRatios(const GProfile* obj) const
{
	const RMaxVector* Vec;
	Disagree->Info(3,obj->GetId(),&Vec);
	return(Vec);
}


//-----------------------------------------------------------------------------
const RMaxVector* GCAInstProfile::GetAgreementRatios(const GProfile* obj) const
{
	const RMaxVector* Vec;
	Agree->Info(3,obj->GetId(),&Vec);
	return(Vec);
}


//-----------------------------------------------------------------------------
const RMaxVector* GCAInstProfile::GetSims(const GProfile* obj) const
{
	const RMaxVector* Vec;
	Sims->Info(3,obj->GetId(),&Vec);
	return(Vec);
}
