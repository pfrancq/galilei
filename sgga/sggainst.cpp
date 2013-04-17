/*

	Genetic Community Algorithm

	SGGAInst.h

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
#include <gsubject.h>
#include <gmeasure.h>


//-----------------------------------------------------------------------------
// include files for SGGA
#include <sggainst.h>
#include <sggachromo.h>
#include <sggagroup.h>



//-----------------------------------------------------------------------------
//
// SGGAInstDoc
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
SGGAInstDoc::SGGAInstDoc(GSession* ses,RCursor<GDoc> objs,RParamsSG* p,RDebug *debug,tObjType type)
	: RInstSG<SGGAInstDoc,SGGAChromoDoc,SGGAThreadDataDoc,SGGAGroupDoc,GDoc>(objs,p,debug),
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
RGroupingHeuristic<SGGAGroupDoc,GDoc,SGGAChromoDoc>* SGGAInstDoc::CreateHeuristic(void)
{
	return(new SGGAHeuristicDoc(Random,Objs,Params,Debug));
}


//-----------------------------------------------------------------------------
double SGGAInstDoc::GetDisagreementRatio(const GDoc* obj1,const GDoc* obj2) const
{
	double d;
	Disagree->Measure(0,obj1->GetId(),obj2->GetId(),&d);
	return(d);
}


//-----------------------------------------------------------------------------
double SGGAInstDoc::GetAgreementRatio(const GDoc* obj1,const GDoc* obj2) const
{
	double d;
	Agree->Measure(0,obj1->GetId(),obj2->GetId(),&d);
	return(d);
}


//-----------------------------------------------------------------------------
double SGGAInstDoc::GetSim(const GDoc* obj1,const GDoc* obj2) const
{
	double d;
	Sims->Measure(0,obj1->GetId(),obj2->GetId(),&d);
	return(d);
}



//-----------------------------------------------------------------------------
//
// SGGAInstProfile
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
SGGAInstProfile::SGGAInstProfile(GSession* ses,RCursor<GProfile> objs,RParamsSG* p,RDebug *debug,tObjType type)
	: RInstSG<SGGAInstProfile,SGGAChromoProfile,SGGAThreadDataProfile,SGGAGroupProfile,GProfile>(objs,p,debug),
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
RGroupingHeuristic<SGGAGroupProfile,GProfile,SGGAChromoProfile>* SGGAInstProfile::CreateHeuristic(void)
{
	return(new SGGAHeuristicProfile(Random,Objs,Params,Debug));
}


//-----------------------------------------------------------------------------
double SGGAInstProfile::GetDisagreementRatio(const GProfile* obj1,const GProfile* obj2) const
{
	double d;
	Disagree->Measure(0,obj1->GetId(),obj2->GetId(),&d);
	return(d);
}


//-----------------------------------------------------------------------------
double SGGAInstProfile::GetAgreementRatio(const GProfile* obj1,const GProfile* obj2) const
{
	double d;
	Agree->Measure(0,obj1->GetId(),obj2->GetId(),&d);
	return(d);
}


//-----------------------------------------------------------------------------
double SGGAInstProfile::GetSim(const GProfile* obj1,const GProfile* obj2) const
{
	double d;
	Sims->Measure(0,obj1->GetId(),obj2->GetId(),&d);
	return(d);
}
