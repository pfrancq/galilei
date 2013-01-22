/*

	GALILEI Research Project

	GComputeSimLink.h

	Link Similarity Measure - Implementation.

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
// include files for GALILEI
#include <gdoc.h>
#include <gsession.h>


//------------------------------------------------------------------------------
// include files for the plug-in
#include <gcomputesimlink.h>

//------------------------------------------------------------------------------
//
// class GComputeSimLink::Node
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
class GComputeSimLink::Node
{
public:
	GConcept* Concept;
	double Weight;
	GDoc* Doc;
	bool Search;

	Node(GConcept* concept) : Concept(concept), Weight(0.0), Doc(0), Search(true) {}
	int Compare(const Node& node) const {return(CompareIds(Concept->GetId(),node.Concept->GetId()));}
	int Compare(const GConcept* concept) const {return(CompareIds(Concept->GetId(),concept->GetId()));}
	double GetWeight(void) const {return(Weight);}
	size_t GetId(void) const {return(Concept->GetId());}
	GConcept* GetConcept(void) const {return(Concept);}
};



//------------------------------------------------------------------------------
//
// class GComputeSimLink
//
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
GComputeSimLink::GComputeSimLink(GSimPlugIn* plugin)
	: GComputeSimMeta(plugin), Neighbors1(40), Neighbors2(40)
{
}


//------------------------------------------------------------------------------
void GComputeSimLink::Fill(RContainer<Node,true,true>& cont,const GVector* vec,double weight,size_t hop)
{
	// Decrease the hop
	hop--;

	// Go trough the links
	RCursor<GConceptRef> ptr(vec->GetRefs());
	for(ptr.Start();!ptr.End();ptr.Next())
	{
		// Verify that concept is an URL and that it does not correspond to the other object
		if(ptr()->GetConcept()->GetType()->GetCategory()!=ccLink)
			continue;

		// Find the node in the container
		double Weight(weight*ptr()->GetWeight());
		Node* Link(cont.GetInsertPtr(ptr()->GetConcept()));
		Link->Weight+=weight;

		// Must the graph be further expanded
		if(!hop)
			continue;

		// Look if it is a known document and use its vector
		if(Link->Search)
		{
			Link->Doc=PlugIn->GetSession()->GetObj(pDoc,ptr()->GetConcept()->GetSearchStr(),true,true);
			Link->Search=false;
		}
		if(Link->Doc)
		{
			GVector* Vec(Link->Doc->GetVector(vec->GetMetaConcept()));
			if(Vec)
				Fill(cont,Vec,Weight,hop);
		}
	}
}


//------------------------------------------------------------------------------
void GComputeSimLink::Compute(GVector* vec1,GVector* vec2,double& sim,size_t& nb)
{
	// Clear the container
	Neighbors1.Clear();
	Neighbors2.Clear();

	// If one vector points to the another object -> insert them
	if(PlugIn->GetConcept1())
	{
		GConceptRef* Ref(vec2->GetRef(PlugIn->GetConcept1()));
		if(Ref)
		{
			Node* Link(Neighbors2.GetInsertPtr(PlugIn->GetConcept1()));
			Link->Weight+=Ref->GetWeight();
		}
	}
	if(PlugIn->GetConcept2())
	{
		GConceptRef* Ref(vec1->GetRef(PlugIn->GetConcept2()));
		if(Ref)
		{
			Node* Link(Neighbors1.GetInsertPtr(PlugIn->GetConcept2()));
			Link->Weight+=Ref->GetWeight();
		}
	}
	Fill(Neighbors1,vec1,1.0,PlugIn->GetNbHops());
	Fill(Neighbors2,vec2,1.0,PlugIn->GetNbHops());

	// Count the fraction of weighted number of links that are common and different
	RCursor<Node> ptr1(Neighbors1);
	RCursor<Node> ptr2(Neighbors2);
	GComputeSimMeta::Compute(ptr1,ptr2,sim,nb);
}

