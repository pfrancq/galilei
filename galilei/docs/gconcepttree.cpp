/*

	GALILEI Research Project

	GConceptTree.cpp

	Concepts Tree - Implementation.

	Copyright 2008-2015 by Pascal Francq (pascal@francq.info).

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
#include <gconcepttree.h>
#include <gconceptnode.h>
#include <gconceptnodes.h>
#include <gconceptref.h>
#include <gconcepttype.h>
using namespace GALILEI;
using namespace R;
using namespace std;



//------------------------------------------------------------------------------
//
//  GConceptTree
//
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
GConceptTree::GConceptTree(const GConceptTree& tree)
	: RTree<GConceptTree,GConceptNode,false>(), RContainer<GConceptNode,true,false>(tree.RContainer<GConceptNode,true,false>::GetNb()),
		  Pos(tree.Pos.GetNb()), Refs(tree.Refs.GetNb()), DocId(tree.DocId)
{
	RNodeCursor<GConceptTree,GConceptNode> Node(tree);
	for(Node.Start();!Node.End();Node.Next())
		CopyNode(0,Node());
}


//------------------------------------------------------------------------------
GConceptTree::GConceptTree(size_t docid,size_t max,size_t nb)
	: RTree<GConceptTree,GConceptNode,false>(), RContainer<GConceptNode,true,false>(max),
	  Pos(max), Refs(nb), DocId(docid)
{
}


//------------------------------------------------------------------------------
void GConceptTree::Verify(size_t docid,size_t max,size_t nb)
{
	RTree<GConceptTree,GConceptNode,false>::Clear();
	RContainer<GConceptNode,true,false>::VerifyTab(max);
	Pos.Clear(nb);
	Refs.Clear(nb);
	DocId=docid;
}


//------------------------------------------------------------------------------
void GConceptTree::CopyNode(GConceptNode* parent,GConceptNode* node)
{
	// Create a new node
	GConceptNode* NewNode(InsertNode(parent,node->Type,node->ConceptId,node->SyntacticPos,node->Pos,node->Depth));

	// Parse its children
	RNodeCursor<GConceptTree,GConceptNode> Node(node);
	for(Node.Start();!Node.End();Node.Next())
		CopyNode(NewNode,Node());
}


//------------------------------------------------------------------------------
GConceptNode* GConceptTree::InsertNode(GConceptNode* parent,tTokenType type,size_t conceptid,size_t synpos,size_t pos,size_t depth)
{
	GConceptNode* Node;
	if(GetNbNodes()>=GetNb())
	{
		InsertPtr(Node=new GConceptNode(this,type,conceptid,synpos,pos,depth));
		Node->Index=GetNb()-1;
	}
	else
	{
		Node=(*this)[GetNbNodes()];
		Node->ConceptId=conceptid;
		Node->Pos=pos;
		Node->SyntacticPos=synpos;
		Node->SyntacticDepth=depth;
	}

	// Insert the node
	RTree<GConceptTree,GConceptNode,false>::InsertNode(parent,Node);
	GConceptNodes* Ref(Refs.GetInsertPtr(conceptid));
	Ref->InsertPtr(Node);
	Pos.VerifyTab(synpos);
	Pos.InsertPtrAt(Node,synpos,true);

	// Return
	return(Node);
}


//------------------------------------------------------------------------------
void GConceptTree::Clear(void)
{
	RTree<GConceptTree,GConceptNode,false>::Clear();
	Pos.Clear();
	Refs.Clear();
}


//------------------------------------------------------------------------------
RCursor<GConceptNode> GConceptTree::GetNodes(GConcept* concept) const
{
	GConceptNodes* Nodes(Refs.GetPtr(concept->GetId()));
	if(Nodes)
		return(RCursor<GConceptNode>(*Nodes));
	return(RCursor<GConceptNode>());
}


//------------------------------------------------------------------------------
RCursor<GConceptNode> GConceptTree::GetNodes(size_t min,size_t max) const
{
	return(RCursor<GConceptNode>(Pos,min,max));
}


//------------------------------------------------------------------------------
const GConceptNode* GConceptTree::GetRoot(const GConceptNode* node1,const GConceptNode* node2) const
{
	// Put Node1 on the same depth than Node2
	while(node1&&(node1->GetDepth()>node2->GetDepth()))
		node1=node1->GetParent();

	// Put Node2 on the same depth than Node1
	size_t RefDepth(0);
	if(node1)
		RefDepth=node1->GetDepth();
	while(node2&&(node2->GetDepth()>RefDepth))
		node2=node2->GetParent();

	// The two nodes are on the same depth
	// While they are not equal goes higher in the hierarchy
	while(node1!=node2)
	{
		node1=node1->GetParent();
		node2=node2->GetParent();
	}

	return(node1);
}


//------------------------------------------------------------------------------
const GConceptNode* GConceptTree::GetNearestNode(size_t synpos) const
{
	const GConceptNode* Find(0);
	size_t before(synpos);
	size_t after(synpos);
	size_t max(GetMaxSyntacticPos());

	while(true)
	{
		if(before!=cNoRef)
		{
			Find=Pos.GetPtrAt(before);
			if(before>0)
				before--;
			else
				before=cNoRef;
		}
		if(Find)
			break;
		if(after!=cNoRef)
		{
			Find=Pos.GetPtrAt(after);
			if(after<max)
				after++;
			else
				after=cNoRef;
		}
		if(Find)
			break;
		if((before==cNoRef)&&(after==cNoRef))
			break;
	}
	return(Find);
}


//------------------------------------------------------------------------------
const GConceptNode* GConceptTree::GetNearestNode(size_t synpos,bool after) const
{
	const GConceptNode* Find(0);
	size_t Max(GetMaxSyntacticPos()),oldpos(synpos);
	while(true)
	{
		Find=Pos.GetPtrAt(synpos);
		if(Find)
			break;

		if(after)
		{
			synpos++;
			if(synpos>Max)
				return(GetNearestNode(oldpos,false));
		}
		else
		{
			if(synpos==0)
				return(GetNearestNode(oldpos,true));
			synpos--;
		}
	}
	return(Find);
}



//------------------------------------------------------------------------------
size_t GConceptTree::GetMinPos(const GConceptNode* node,size_t nbbefore) const
{
	if(nbbefore>node->GetSyntacticPos())
		nbbefore=0;
	else
		nbbefore=node->GetSyntacticPos()-nbbefore;
	const GConceptNode* Node(GetNearestNode(nbbefore,false));
	if(!Node)
		mThrowGException("Cannot find a valid position");
	return(Node->GetPos());
}


//------------------------------------------------------------------------------
size_t GConceptTree::GetMaxPos(const GConceptNode* node,size_t nbafter) const
{
	nbafter+=node->GetSyntacticPos();
	size_t Max(GetMaxSyntacticPos());
	if(nbafter>Max)
		nbafter=Max;
	const GConceptNode* Node(GetNearestNode(nbafter,true));
	if(!Node)
		mThrowGException("Cannot find a valid position");
	return(Node->GetPos());
}


//------------------------------------------------------------------------------
size_t GConceptTree::GetMaxPos(void) const
{
	if(!Pos.GetMaxPos())
		return(0);
	return(Pos[Pos.GetMaxPos()]->GetPos());
}


//------------------------------------------------------------------------------
size_t GConceptTree::GetMaxSyntacticPos(void) const
{
	if(!Pos.GetMaxPos())
		return(0);
	return(Pos[Pos.GetMaxPos()]->GetSyntacticPos());
}


//------------------------------------------------------------------------------
void GConceptTree::Print(void) const
{
	RNodeCursor<GConceptTree,GConceptNode> Children(*this);
	for(Children.Start();!Children.End();Children.Next())
		Children()->Print();
	cout<<endl;
}


//------------------------------------------------------------------------------
GConceptTree::~GConceptTree(void)
{
}