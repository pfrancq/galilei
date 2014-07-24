/*

	GALILEI Research Project

	GConceptNode.cpp

	Concept Node - Implementation.

	Copyright 2008-2014 by Pascal Francq (pascal@francq.info).

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
// include files for R Project
#include <rnodecursor.h>


//------------------------------------------------------------------------------
// include files for GALILEI
#include <gconceptnode.h>
#include <gconcepttree.h>
#include <gconcept.h>
using namespace GALILEI;
using namespace R;
using namespace std;



//------------------------------------------------------------------------------
//
//  GConceptNode
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GConceptNode::GConceptNode(tTokenType type,size_t conceptid,size_t synpos,size_t pos,size_t depth)
	: RNode<GConceptTree,GConceptNode,false>(), Type(type), ConceptId(conceptid),
	  SyntacticPos(synpos), Pos(pos), SyntacticDepth(depth)
{
}


//------------------------------------------------------------------------------
int GConceptNode::Compare(const GConceptNode& node) const
{
	if(ConceptId!=node.ConceptId)
		return(CompareIds(ConceptId,node.ConceptId));

	if(Depth==node.Depth)
	{
		if(Pos==node.Pos)
			return(0);
		else if (Pos>node.Pos)
			return(1);
		else
			return(-1);

	}

	if(Depth>node.Depth)
		return(1);
	else
		return(-1);
}


//------------------------------------------------------------------------------
void GConceptNode::Clear(void)
{
	RNode<GConceptTree,GConceptNode,false>::Clear();
}


//------------------------------------------------------------------------------
GConceptNode* GConceptNode::GetParent(void) const
{
	return(RNode<GConceptTree,GConceptNode,false>::GetParent());
}


//------------------------------------------------------------------------------
GConceptNode* GConceptNode::GetFirst(void) const
{
	return(RNode<GConceptTree,GConceptNode,false>::GetFirst());
}


//------------------------------------------------------------------------------
GConceptNode* GConceptNode::GetNext(void) const
{
	return(RNode<GConceptTree,GConceptNode,false>::GetNext());
}


//------------------------------------------------------------------------------
size_t GConceptNode::GetNbNodes(void) const
{
	return(RNode<GConceptTree,GConceptNode,false>::GetNbNodes());
}


//------------------------------------------------------------------------------
size_t GConceptNode::GetNbTotalNodes(void) const
{
	return(RNode<GConceptTree,GConceptNode,false>::GetNbNodes());
}


//------------------------------------------------------------------------------
void GConceptNode::Print(void) const
{
	for(size_t i=GetDepth()+1;--i;)
		cout<<" ";
	cout<<"Node "<<ConceptId<<","<<Pos<<","<<SyntacticPos<<SyntacticDepth<<endl;
	RNodeCursor<GConceptTree,GConceptNode> Children(this);
	for(Children.Start();!Children.End();Children.Next())
		Children()->Print();
}