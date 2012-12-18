/*

	GALILEI Research Project

	GComputeSimCos.h

	Cosine Similarity Measure - Implementation.

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
#include <gcomputesimcos.h>



//------------------------------------------------------------------------------
//
// class GComputeSimCos
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void GComputeSimCos::Compute(GVector* vec1,GVector* vec2,double& sim,size_t& nb)
{
	double Num(0.0),Norm1(0.0),Norm2(0.0),NbCommon(0);

	RCursor<GConceptRef> ptr1(vec1->GetRefs());
	RCursor<GConceptRef> ptr2(vec2->GetRefs());
	for(ptr1.Start(),ptr2.Start();!ptr1.End();ptr1.Next())
	{
		double w1(ptr1()->GetWeight()*PlugIn->GetIF(ptr1()->GetConcept()));
		Norm1+=w1*w1;
		for(;(!ptr2.End())&&(ptr2()->GetId()<ptr1()->GetId());)
		{
			double w2(ptr2()->GetWeight()*PlugIn->GetIF(ptr2()->GetConcept()));
			Norm2+=w2*w2;
			ptr2.Next();
		}

		if((!ptr2.End())&&(ptr2()->GetId()==ptr1()->GetId()))
		{
			double w2(ptr2()->GetWeight()*PlugIn->GetIF(ptr2()->GetConcept()));
			Norm2+=w2*w2;

			// If both weight are negative -> pass
		   if((w1<0.0)&&(w2<0.0))
				continue;

			// Compute the product
			double d(w2*w1);
			if(fabs(d-1.0)<PlugIn->GetCutoffFrequency())
				d=1.0;
			if(fabs(1.0+d)<PlugIn->GetCutoffFrequency())
				d=-1.0;
			Num+=d;
			NbCommon++;
		}
	}

	// Rest of Norm2
	for(;!ptr2.End();ptr2.Next())
	{
		double w2(ptr2()->GetWeight()*PlugIn->GetIF(ptr2()->GetConcept()));
		Norm2+=w2*w2;
	}

	// Assign to the right category
	sim=0.5+(Num/(2.0*sqrt(Norm1)*sqrt(Norm2)));
	nb=NbCommon;
}
