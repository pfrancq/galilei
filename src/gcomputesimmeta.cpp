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
	RCursor<GConceptRef> ptr1(vec1->GetRefs());
	RCursor<GConceptRef> ptr2(vec2->GetRefs());
	Compute(ptr1,ptr2,sim,nb);
}

