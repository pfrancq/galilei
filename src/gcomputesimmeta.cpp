/*

	GALILEI Research Project

	GComputeSimMeta.h

	Metadata Similarity Measure - Implementation.

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
#include <gcomputesimmeta.h>



//------------------------------------------------------------------------------
//
// class GComputeSimMeta
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void GComputeSimMeta::Compute(GVector* vec1,GVector* vec2,double& sim,size_t& nb)
{
	double Num(0.0), Den(0.0), NbCommon(0),NbDiff(0);

	RCursor<GConceptRef> ptr1(vec1->GetRefs());
	RCursor<GConceptRef> ptr2(vec2->GetRefs());
	for(ptr1.Start(),ptr2.Start();!ptr1.End();ptr1.Next())
	{
		NbDiff++;

		for(;(!ptr2.End())&&(ptr2()->GetId()<ptr1()->GetId());ptr2.Next())
			NbDiff++;

		// Same concept -> Add Num
		if((!ptr2.End())&&(ptr2()->GetId()==ptr1()->GetId()))
		{
			double w1(ptr1()->GetWeight()*PlugIn->GetIF(ptr1()->GetConcept()));
			double w2(ptr2()->GetWeight()*PlugIn->GetIF(ptr2()->GetConcept()));

			// If both weight are negative -> pass
		   if((w1<0.0)&&(w2<0.0))
				continue;

			// Compute the product
			NbCommon++;
			double d(w2*w1);
			if(fabs(d-1.0)<PlugIn->GetCutoffFrequency())
				d=1.0;
			if(fabs(1.0+d)<PlugIn->GetCutoffFrequency())
				d=-1.0;
			Num+=d;
			Den+=fabs(d);
		}
	}

	// Compute the rest of the NbDiff
	for(;!ptr2.End();ptr2.Next())
		NbDiff++;

	// Assign to the category
	sim=0.5+((Num*NbCommon)/(2*NbDiff*Den));
	nb=NbCommon;
}

