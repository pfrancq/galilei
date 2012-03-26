/*

	GALILEI Research Project

	GResNodes.cpp

	Resulting Node Container - Implementation.

	Copyright 2011-2012 by Pascal Francq (pascal@francq.info).

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
#include <gresnodes.h>
#include <gresnode.h>
using namespace GALILEI;
using namespace R;
using namespace std;



//------------------------------------------------------------------------------
//
//  GResNodes
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GResNodes::GResNodes(size_t docid)
	: RContainer<GResNode,true,true>(2000), DocId(docid)
{
}


//------------------------------------------------------------------------------
GResNodes::GResNodes(size_t docid,size_t max)
	: RContainer<GResNode,true,true>(max), DocId(docid)
{
}


//------------------------------------------------------------------------------
GResNodes::GResNodes(const GResNodes& nodes)
	: RContainer<GResNode,true,true>(nodes), DocId(nodes.DocId)
{
}


//------------------------------------------------------------------------------
int GResNodes::Compare(const GResNodes& node) const
{
	return(CompareIds(DocId,node.DocId));
}


//------------------------------------------------------------------------------
int GResNodes::Compare(const size_t docid) const
{
	return(CompareIds(DocId,docid));
}


//------------------------------------------------------------------------------
bool GResNodes::FindSibling(GResNode* node) const
{
	GConceptNode* Parent(node->GetNode()->GetParent());
	RCursor<GResNode> Nodes(*this);
	for(Nodes.Start();!Nodes.End();Nodes.Next())
	{
		if(Nodes()->GetNode()->GetParent()==Parent)
			return(true);
	}
	return(false);
}


//------------------------------------------------------------------------------
bool GResNodes::FindChild(GResNode* node) const
{
	size_t Depth(node->GetNode()->GetSyntacticDepth());
	RCursor<GResNode> Nodes(*this);
	for(Nodes.Start();!Nodes.End();Nodes.Next())
	{
		if(Nodes()->GetNode()->GetSyntacticDepth()>Depth)
		{
			// Search the if one of the parent is node
			GConceptNode* Parent(Nodes()->GetNode()->GetParent());
			while(Parent&&(Parent->GetSyntacticDepth()>=Depth))
			{
				if(Parent==node->GetNode())
					return(false);
				Parent=Parent->GetParent();
			}
			return(false);
		}
	}
	return(false);
}


//------------------------------------------------------------------------------
void GResNodes::InsertPtrAt(const GResNode* node,size_t idx)
{
	RContainer<GResNode,true,true>::InsertPtrAt(new GResNode(this,node->GetNode()),idx,false);
}


//------------------------------------------------------------------------------
GResNode* GResNodes::InsertPtr(GConceptNode* parent,GConceptNode* node)
{
	GResNode* New(new GResNode(this,parent));
	RContainer<GResNode,true,true>::InsertPtr(New);
	New->AddChild(node);
	return(New);
}


//------------------------------------------------------------------------------
GResNode* GResNodes::InsertPtr(GConceptNode* node,bool all)
{
	GResNode* New(new GResNode(this,node));
	RContainer<GResNode,true,true>::InsertPtr(New);
	if(all)
	{
		RNodeCursor<GConceptTree,GConceptNode> Child(node);
		for(Child.Start();!Child.End();Child.Next())
			New->AddChild(Child());
	}
	return(New);
}


//------------------------------------------------------------------------------
/**
 * The class represents a node to delete.
 */
class cNodeDel
{
public:
	GResNode* Node;

	cNodeDel(GResNode* node) : Node(node) {}
	int Compare(const cNodeDel& node) const {return(Node-node.Node);}
	int Compare(const GResNode* node) const {return(Node-node);}
};


//------------------------------------------------------------------------------
void GResNodes::Reduce(bool force)
{
	// If less than two results -> nothing to merges
	if(GetNb()<2)
		return;

	// Look which node to merge (and those to delete)
	RContainer<cNodeDel,true,true> ToDel(10);
	RCursor<GResNode> Nodes(*this,0,GetNb()-2);
	for(Nodes.Start();!Nodes.End();Nodes.Next())
	{
		// Don't treat a node already deleted
		if(ToDel.IsIn(Nodes()))
			continue;

		RCursor<GResNode> Nodes2(*this,Nodes.GetPos()+1,GetNb()-1);
		for(Nodes2.Start();!Nodes2.End();Nodes2.Next())
		{
			// Don't treat a node already deleted
			if(ToDel.IsIn(Nodes2()))
				continue;

			// Verify if the two fragment are force to be merged if they have the same node or if they overlap
			if((force&&(Nodes()->GetNode()==Nodes2()->GetNode()))||((!force)&&Nodes()->Overlap(Nodes2())))
			{
				Nodes()->Merge(Nodes2());
				ToDel.InsertPtr(new cNodeDel(Nodes2()));
			}
		}
	}

	// Delete the necessary nodes
	RCursor<cNodeDel> Del(ToDel);
	for(Del.Start();!Del.End();Del.Next())
		DeletePtr(*Del()->Node);
}


//------------------------------------------------------------------------------
void GResNodes::Print(void)
{
	cout<<DocId<<endl;
	RCursor<GResNode> Nodes(*this);
	for(Nodes.Start();!Nodes.End();Nodes.Next())
	{
		cout<<"\t"<<Nodes()->GetNode()->GetSyntacticPos()<<" ("<<Nodes()->GetNode()->GetConceptId()<<")"<<endl;
		RCursor<GConceptNode> Child(Nodes()->GetChildren());
		for(Child.Start();!Child.End();Child.Next())
			cout<<"\t\t"<<Child()->GetSyntacticPos()<<" ("<<Child()->GetConceptId()<<")"<<endl;
	}
}