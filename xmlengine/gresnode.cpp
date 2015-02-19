/*

	GALILEI Research Project

	GResNode.cpp

	Resulting Node - Implementation.

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
// include files for R/GALILEI projects
#include <gconceptnode.h>
#include <gconcepttree.h>
#include <rnodecursor.h>


//------------------------------------------------------------------------------
// include files for current project
#include <gresnode.h>
#include <gresnode.h>
using namespace GALILEI;
using namespace R;
using namespace std;


//------------------------------------------------------------------------------
const size_t MaximumSyntacticPos(20);



//------------------------------------------------------------------------------
//
//  GResNode
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GResNode::GResNode(GResNodes* parent,GConceptNode* node)
	: Parent(parent), Node(node), MinSyntacticPos(cNoRef), MaxSyntacticPos(0), Children(20)
{
/*	if(!Node)
		mThrowGException("Cannot insert a null node");*/
	if(Node)
	{
		size_t Pos(Node->GetSyntacticPos());
		if(Node->GetType()==ttText)
		{
			if(Pos>10)
				MinSyntacticPos=Pos-5;
			else
				MinSyntacticPos=0;
			if(Pos+10<Node->GetTree()->GetMaxSyntacticPos())
				MaxSyntacticPos=Node->GetTree()->GetMaxSyntacticPos();
			else
				MaxSyntacticPos=Pos+5;
		}
		else
		{
			MaxSyntacticPos=Pos;
			MinSyntacticPos=Pos;
		}
	}
}


//------------------------------------------------------------------------------
int GResNode::Compare(const GResNode& node) const
{
	// Treat null nodes
	if(!Node)
	{
		if(node.Node)
			return(-1);
		return(0);
	}
	else if(!node.Node)
		return(1);

	// Compare concept
	if(Node!=node.Node)
		return(CompareIds(Node->GetConceptId(),node.Node->GetConceptId()));

	// Compare position if same syntactic depth
	if(Node->GetSyntacticDepth()==node.Node->GetSyntacticDepth())
	{
		if(Node->GetPos()==node.Node->GetPos())
			return(0);
		else if (Node->GetPos()>node.Node->GetPos())
			return(1);
		else
			return(-1);

	}

	// Compare syntactic depth
	if(Node->GetSyntacticDepth()>node.Node->GetSyntacticDepth())
		return(1);
	else
		return(-1);
}


//------------------------------------------------------------------------------
void GResNode::AddChild(GConceptNode* child)
{
	// compute the extract to associate to that node
	size_t max(0), min(0);
	if(child->GetType()==ttText)
	{
		min=child->GetSyntacticPos();
		max=child->GetSyntacticPos();
	}
	else
	{
		min=max=child->GetSyntacticPos();
	}
	if(max>MaxSyntacticPos)
		MaxSyntacticPos=max;
	if(min<MinSyntacticPos)
		MinSyntacticPos=min;
	bool Find;
	size_t Idx(Children.GetIndex(*child,Find));
	if(!Find)
		Children.InsertPtrAt(child,Idx,false);
}


//------------------------------------------------------------------------------
bool GResNode::Overlap(const GResNode* node) const
{
	if(Node!=node->Node)
		return(false);
	RCursor<GConceptNode> Child(Children);
	RCursor<GConceptNode> Child2(node->Children);
	for(Child.Start();!Child.End();Child.Next())
	{
		size_t Pos(Child()->GetSyntacticPos());
		for(Child2.Start();!Child2.End();Child2.Next())
		{
			size_t Pos2(Child2()->GetSyntacticPos());
			if(((Pos<Pos2)&&(Pos+MaximumSyntacticPos<Pos2))||((Pos2<Pos)&&(Pos2+MaximumSyntacticPos<Pos)))
				return(false);
		}
	}
	return(true);
}


//------------------------------------------------------------------------------
void GResNode::Merge(const GResNode* node)
{
	// Merge the two node lists
	RCursor<GConceptNode> Child(node->Children);
	for(Child.Start();!Child.End();Child.Next())
	{
		bool Find;
		size_t Index(Children.GetIndex(*Child(),Find));
		if(Find)
			continue;
		Children.InsertPtrAt(Child(),Index,false);
		if(Child()->GetSyntacticPos()>MaxSyntacticPos)
			MaxSyntacticPos=Child()->GetSyntacticPos();
		if(Child()->GetSyntacticPos()<MinSyntacticPos)
			MinSyntacticPos=Child()->GetSyntacticPos();
	}
}
