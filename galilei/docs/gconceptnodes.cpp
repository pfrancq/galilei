/*

	GALILEI Research Project

	GConceptNodes.cpp

	Concept Node Container - Implementation.

	Copyright 2011-2015 by Pascal Francq (pascal@francq.info).

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
#include <gconceptnodes.h>
#include <gconceptnode.h>
using namespace GALILEI;
using namespace R;
using namespace std;



//------------------------------------------------------------------------------
//
//  GConceptNodes
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GConceptNodes::GConceptNodes(size_t conceptid)
	: RContainer<GConceptNode,false,true>(2000), ConceptId(conceptid)
{
}


//------------------------------------------------------------------------------
GConceptNodes::GConceptNodes(size_t conceptid,size_t max)
	: RContainer<GConceptNode,false,true>(max), ConceptId(conceptid)
{
}


//------------------------------------------------------------------------------
GConceptNodes::GConceptNodes(const GConceptNodes& nodes)
	: RContainer<GConceptNode,false,true>(nodes), ConceptId(nodes.ConceptId)
{
}


//------------------------------------------------------------------------------
int GConceptNodes::Compare(const GConceptNodes& node) const
{
	return(CompareIds(ConceptId,node.ConceptId));
}


//------------------------------------------------------------------------------
int GConceptNodes::Compare(const size_t conceptid) const
{
	return(CompareIds(ConceptId,conceptid));
}
