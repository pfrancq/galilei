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

	/**
	 * Concept.
	 */
	GConcept* Concept;

	/**
	 * Weight of the concept.
    */
	double Weight;

	/**
	 * Document.
    */
	GDoc* Doc;

	bool MustSearch;

	Node(GConcept* concept) : Concept(concept), Weight(0.0), Doc(0), MustSearch(true)
	{
	}

	int Compare(const Node& node) const
	{
		return(CompareIds(Concept->GetId(),node.Concept->GetId()));
	}

	int Compare(const GConcept* concept) const
	{
		return(CompareIds(Concept->GetId(),concept->GetId()));
	}
};



//------------------------------------------------------------------------------
//
// class GComputeSimLink
//
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
GComputeSimLink::GComputeSimLink(GSimPlugIn* plugin)
	: GComputeSim(plugin), Vector1(40), Vector2(40)
{
}


//------------------------------------------------------------------------------
void GComputeSimLink::Fill(RContainer<Node,true,true>& cont,const GVector* vec,GConcept* concept,double weight,size_t hop)
{
	// Decrease the hop
	hop--;

	// Go trough the links
	RCursor<GConceptRef> ptr(vec->GetRefs());
	for(ptr.Start();!ptr.End();ptr.Next())
	{
		// Verify that concept is an URL and that it does not correspond to the other object
		if((ptr()->GetConcept()->GetType()->GetCategory()!=ccLink)&&(ptr()->GetConcept()!=concept))
			continue;

		// Add the node to the container (if necessary) and update its weight
		Node* node(cont.GetInsertPtr(ptr()->GetConcept()));
		double Weight(ptr()->GetWeight()*weight);
		node->Weight+=Weight;

		// Must the graph be further expanded
		if(!hop)
			continue;

		// Look if it is a known document and use its vector
		if(node->MustSearch)
		{
			node->Doc=PlugIn->GetSession()->GetObj(pDoc,node->Concept->GetSearchStr(),true,false);
			node->MustSearch=false;
		}
		if(node->Doc)
		{
			GVector* Vec(node->Doc->GetVector(vec->GetMetaConcept()));
			if(Vec)
				Fill(cont,Vec,concept,Weight,hop);
		}
	}
}


//------------------------------------------------------------------------------
void GComputeSimLink::AddLinks(RContainer<Node,true,true>& cont1,RContainer<Node,true,true>& cont2)
{
	RCursor<Node> node1(cont1);
	RCursor<Node> node2(cont2);
	for(node1.Start(),node2.Start();(!node1.End())&&(!node2.End());node1.Next())
	{
		// Skip all concepts with smaller identifier
		while((!node2.End())&&(node2()->Concept->GetId()<node1()->Concept->GetId()))
			node2.Next();

		// Same concept -> Add to Num and Den
		if((!node2.End())&&(node2()->Concept==node1()->Concept))
		{
			// If both weight are negative -> pass
		   if((node1()->Weight<0.0)&&(node2()->Weight<0.0))
				continue;

			double d;
			if(fabs(node1()->Weight)>fabs(node2()->Weight))
			{
				d=(node2()->Weight*PlugIn->GetIF(node1()->Concept))/node1()->Weight;
				if(fabs(d-1.0)<PlugIn->GetCutoffFrequency())
					d=1.0;
				if(fabs(1.0+d)<PlugIn->GetCutoffFrequency())
					d=-1.0;
			}
			else
				d=1.0;
			Num+=d*PlugIn->GetIF(node1()->Concept);
			Den+=PlugIn->GetIF(node1()->Concept);
		}
	}
}


//------------------------------------------------------------------------------
void GComputeSimLink::Compute(GVector* vec1,GVector* vec2,double& sim,size_t& nb)
{
	// Fill the container with all the neighbors
	Vector1.Clear();
	Vector2.Clear();
	Fill(Vector1,vec1,PlugIn->GetConcept2(),1.0,PlugIn->GetNbHops());
	Fill(Vector2,vec2,PlugIn->GetConcept1(),1.0,PlugIn->GetNbHops());

	// Count the fraction of weighted number of links that are common
	Num=Den=0.0;
	nb=Vector1.GetNb()+Vector2.GetNb();
	AddLinks(Vector1,Vector2);
	AddLinks(Vector2,Vector1);
	if(vec1->IsIn(PlugIn->GetConcept2()))
	{
		Num+=PlugIn->GetIF(PlugIn->GetConcept2());
		Den+=PlugIn->GetIF(PlugIn->GetConcept2());
	}
	if(vec2->IsIn(PlugIn->GetConcept1()))
	{
		Num+=PlugIn->GetIF(PlugIn->GetConcept1());
		Den+=PlugIn->GetIF(PlugIn->GetConcept1());
	}

	// Compute the similarity
	if(nb)
		sim=0.5+(Num/(Den*2));
}

