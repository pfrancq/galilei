/*

	GALILEI Research Project

	GComputeSim.h

	Generic Similarity Measure - Implementation.

	Copyright 2003-2012 by Pascal Francq (pascal@francq.info).

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
	: GMatrixMeasure(session,fac,lines,cols,lines==cols)
{
	Sims=new GComputeSim*[GetNbConceptCats()];
	Sims[ccText]=Sims[ccSemantic]=new GComputeSimCos(this);
	Sims[ccMetadata]=new GComputeSimMeta(this);
	Sims[ccLink]=new GComputeSimLink(this);
}


//------------------------------------------------------------------------------
double GSimPlugIn::GetIF(GConcept* concept) const
{
	double IF(concept->GetIF(GetLinesType()));
	if(GetLinesType()!=GetColsType())
		IF+=concept->GetIF(GetColsType());
	return(IF);
}


//------------------------------------------------------------------------------
GSimPlugIn::~GSimPlugIn(void)
{
	delete Sims[ccText];
	delete Sims[ccLink];
	delete Sims[ccMetadata];
	delete[] Sims;
}
