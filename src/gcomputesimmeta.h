/*

	GALILEI Research Project

	GComputeSimMeta.h

	Metadata Similarity Measure - Header.

	Copyright 2003-2014 by Pascal Francq (pascal@francq.info).
	Copyright 2003 by Valery Vandaele.
	Copyright 2003-2007 by the Université Libre de Bruxelles (ULB).

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
#ifndef GComputeSimMeta_H
#define GComputeSimMeta_H


//------------------------------------------------------------------------------
// include files for the plug-in
#include <gsimplugin.h>


//------------------------------------------------------------------------------
/**
 * The ComputeSimMeta class proposes a way to compute the similarity between two
 * vectors representing a same metadata. In practice, it counts the number of
 * fraction of common concepts between both vectors weighted by their idf.
 *
 * To take negative concept weights into account, two vectors sharing no
 * concepts have a similarity of 0.5.
 */
class GComputeSimMeta : public GComputeSim
{
public:

	/**
	 * Constructor.
	 */
	GComputeSimMeta(GSimPlugIn* plugin) : GComputeSim(plugin) {}

	/**
	 * Compute the similarity between two vectors.
	 * @tparam e              Class of the elements composing the vectors
    * @param ptr1            Cursor over the first vector.
    * @param ptr2            Cursor over the second vector.
    * @param sim             Similarity computed.
    * @param nb              Number of comparisons done.
    */
	template<class e>
		void Compute(RConstCursor<e>& ptr1,RConstCursor<e>& ptr2,double& sim,size_t& nb)
	{
		double Num(0.0), Den(0.0);
		nb=0.0;

		for(ptr1.Start(),ptr2.Start();!ptr1.End();ptr1.Next())
		{
			for(;(!ptr2.End())&&(ptr2()->GetId()<ptr1()->GetId());ptr2.Next())
			{
				double w2(ptr2()->GetWeight()*PlugIn->GetIF(ptr2()->GetConcept()));
				Den+=w2*w2;
			}

			// Same concept -> Add Num
			double w1(ptr1()->GetWeight()*PlugIn->GetIF(ptr1()->GetConcept()));
			if((!ptr2.End())&&(ptr2()->GetId()==ptr1()->GetId()))
			{
				double w2(ptr2()->GetWeight()*PlugIn->GetIF(ptr2()->GetConcept()));

				// If both weight are negative -> pass
				if((w1<0.0)&&(w2<0.0))
					continue;

				// Compute the product
				nb++;
				Num+=w2*w1;
				if(w2>w1)
					Den+=w2*w2;
				else
					Den+=w1*w1;
			}
			else
				Den+=w1*w1;
		}

		// Compute the rest of the NbDiff
		for(;!ptr2.End();ptr2.Next())
		{
			double w2(ptr2()->GetWeight()*PlugIn->GetIF(ptr2()->GetConcept()));
			Den+=w2*w2;
		}

		// Compute
		if(Den>0.0)
			Num/=Den;
		else
			Num=0;
		if(fabs(Num-1.0)<PlugIn->GetCutoffFrequency())
			Num=1.0;
		if(fabs(1.0+Num)<PlugIn->GetCutoffFrequency())
			Num=-1.0;
		sim=0.5+(Num/2.0);
	}

	/**
	 * Compute the similarity.
    * @param vec1            First vector.
    * @param vec2            Second vector.
    * @param sim             Similarity computed.
    * @param nb              Number of comparisons done.
    */
	virtual void Compute(const GVector* vec1,const GVector* vec2,double& sim,size_t& nb);
};


//------------------------------------------------------------------------------
#endif

