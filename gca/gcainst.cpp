/*

	Genetic Community Algorithm

	GCAInst.h

	Instance - Implementation

	Copyright 2002-2007 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).

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
// GCAThreadData
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GCAThreadData::GCAThreadData(GCAInst* owner)
	: RThreadDataSC<GCAInst,GCAChromo,GCAThreadData,GCAGroup,GCAObj>(owner)
{
}


//-----------------------------------------------------------------------------
GCAThreadData::~GCAThreadData(void)
{
}



//-----------------------------------------------------------------------------
//
// GCAInst
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GCAInst::GCAInst(GSession* ses,RCursor<GCAObj> objs,RParamsSC* p,RDebug *debug,tObjType type,const R::RString& mes,bool inc)
	: RInstSC<GCAInst,GCAChromo,GCAThreadData,GCAGroup,GCAObj>(objs,p,debug,inc),
	  Session(ses), Sims(0),Agree(0), Disagree(0), Type(type)
{
	// Init measures
	Sims=GALILEIApp->GetManager<GMeasureManager>("Measures")->GetCurrentMethod(mes+" Similarities");
	Agree=GALILEIApp->GetManager<GMeasureManager>("Measures")->GetCurrentMethod(mes+" Agreements");
	Disagree=GALILEIApp->GetManager<GMeasureManager>("Measures")->GetCurrentMethod(mes+" Disagreements");

	if((!Sims)||(!Agree)||(!Disagree))
		throw GException("GCAInst::GCAInst : Type "+GetObjType(Type)+" not supported");
}



//-----------------------------------------------------------------------------
double GCAInst::GetDisagreementRatio(size_t element1,size_t element2) const
{
	double d;
	Disagree->Measure(0,element1,element2,&d);
	return(d);
}


//-----------------------------------------------------------------------------
double GCAInst::GetAgreementRatio(size_t element1,size_t element2) const
{
	double d;
	Agree->Measure(0,element1,element2,&d);
	return(d);
}


//-----------------------------------------------------------------------------
double GCAInst::GetSim(size_t element1,size_t element2) const
{
	double d;
	Sims->Measure(0,element1,element2,&d);
	return(d);
}


//-----------------------------------------------------------------------------
GCAInst::~GCAInst(void)
{
}
