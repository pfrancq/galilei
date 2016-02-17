/*

	GALILEI Research Project

	GComputeSim.h

	Generic Similarity Measure - Implementation.

	Copyright 2003-2014 by Pascal Francq (pascal@francq.info).

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
// include files for GALILEI
#include <ggalileiapp.h>


//------------------------------------------------------------------------------
// include files for the plug-in
#include <gsimplugin.h>
#include <gcomputesimcos.h>
#include <gcomputesimmeta.h>
#include <gcomputesimlink.h>



//------------------------------------------------------------------------------
//
// GSimPlugIn
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GSimPlugIn::GSimPlugIn(GSession* session,GPlugInFactory* fac,tObjType lines,tObjType cols)
	: GMatrixMeasure(session,fac,lines,cols,lines==cols), Weighting(0)
{
	Sims=new GComputeSim*[GetNbConceptCats()];
	Sims[ccToken]=Sims[ccStructure]=new GComputeSimCos(this);
	Sims[ccMetadata]=new GComputeSimMeta(this);
	Sims[ccLink]=new GComputeSimLink(this);
}


//------------------------------------------------------------------------------
void GSimPlugIn::Init(void)
{
	GMatrixMeasure::Init();
	Weighting=GALILEIApp->GetCurrentPlugIn<GMeasure>("Measures","Features Evaluation",0);
	InsertObserver(HANDLER(GSimPlugIn::HandleCurrentPlugIn),hCurrentPlugIn,GALILEIApp->GetManager("Measures")->GetPlugInList("Features Evaluation"));
}


//------------------------------------------------------------------------------
void GSimPlugIn::HandleCurrentPlugIn(const R::RNotification& notification)
{
	Weighting=dynamic_cast<GMeasure*>(GetData<GPlugIn*>(notification));
}


//------------------------------------------------------------------------------
double GSimPlugIn::GetIF(GConcept* concept) const
{
	if(!Weighting)
		mThrowGException("No plug-in selected for \"Features Evaluation\"");

	double IF;
	Weighting->Measure(0,concept,GetLinesType(),&IF);
	if(GetLinesType()!=GetColsType())
	{
		double res;
		Weighting->Measure(0,concept,GetColsType(),&res);
		IF+=res;
	}
	return(IF);
}


//------------------------------------------------------------------------------
GSimPlugIn::~GSimPlugIn(void)
{
	delete Sims[ccToken];
	delete Sims[ccLink];
	delete Sims[ccMetadata];
	delete Sims[ccStructure];
	delete[] Sims;
}
