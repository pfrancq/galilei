/*

	GALILEI Research Project

	GConceptTree.cpp

	Concepts Tree - Implementation.

	Copyright 2008-2012 by Pascal Francq (pascal@francq.info).

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
#include <gconcepttree.h>
#include <gconceptref.h>
#include <gconcepttype.h>
using namespace GALILEI;
using namespace R;
using namespace std;



//------------------------------------------------------------------------------
//
//  GLCEntry
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
class GLCEntry
{
public:
	GConceptNode* Rec;
	size_t Child;
	GLCEntry(GConceptNode* rec,size_t child) : Rec(rec), Child(child) {}
	int Compare(const GLCEntry&) const {return(-1);}
};



//------------------------------------------------------------------------------
//
//  GDocSruct::GLC
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
class GConceptTree::GLC : public RContainer<GLCEntry,true,false>
{
public:

	GLC(size_t max) : RContainer<GLCEntry,true,false>(max) {}
	int Compare(const GLC&) const {return(-1);}
	~GLC(void) {}
};



//------------------------------------------------------------------------------
//
//  GDocSruct
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GConceptTree::GConceptTree(void)
	: Nodes(2000), LCs(10)
{
}


//------------------------------------------------------------------------------
GConceptTree::GConceptTree(const GConceptTree& tree)
	: Nodes(tree.Nodes.GetNb()), LCs(tree.LCs.GetNb())
{
	// Recreate correctly the structure
	R::RCursor<GConceptNode> Node(tree.Nodes);
	for(Node.Start();!Node.End();Node.Next())
	{
		size_t child(tree.GetFirstChild(Node()));
		AddNode(Node()->GetConcept(),Node()->GetPos(),Node()->GetDepth(),child);
	}
}


//------------------------------------------------------------------------------
GConceptTree::GConceptTree(size_t vtd,size_t lc)
	: Nodes(vtd), LCs(lc)
{
}


//------------------------------------------------------------------------------
void GConceptTree::SetSizes(size_t vtd,size_t lc)
{
	Nodes.VerifyTab(vtd);
	LCs.VerifyTab(lc);
}


//------------------------------------------------------------------------------
size_t GConceptTree::GetNbNodes(void) const
{
	return(Nodes.GetNb());
}



//------------------------------------------------------------------------------
size_t GConceptTree::GetNbLCs(void) const
{
	return(LCs.GetNb());
}


//------------------------------------------------------------------------------
size_t GConceptTree::GetNbLCEntries(size_t level) const
{
	if(level>=LCs.GetNb())
		return(0);
	return(LCs[level]->GetNb());
}


//------------------------------------------------------------------------------
void GConceptTree::SetNbLCEntries(size_t level,size_t size) const
{
	if(!LCs.VerifyIndex(level))
		const_cast<GConceptTree*>(this)->LCs.InsertPtrAt(new GLC(size),level);
}


//------------------------------------------------------------------------------
GConceptNode* GConceptTree::AddNode(GConcept* concept,size_t pos,char depth,size_t child,size_t nbrecs)
{
	// Create the node and insert it
	GConceptNode* ptr=new GConceptNode(concept,pos,depth);
	Nodes.InsertPtr(ptr);

	// Insert it the location cache if necessary
	GLC* Level;
	if(LCs.VerifyIndex(depth))
		Level=LCs[depth];
	else
	{
		if(!nbrecs)
			nbrecs=200;
		LCs.InsertPtrAt(Level=new GLC(nbrecs),depth);
	}
	Level->InsertPtr(new GLCEntry(ptr,child));

	// Return
	return(ptr);
}


//------------------------------------------------------------------------------
R::RCursor<GConceptNode> GConceptTree::GetNodes(void) const
{
	return(R::RCursor<GConceptNode>(Nodes));
}


//------------------------------------------------------------------------------
size_t GConceptTree::GetFirstChild(GConceptNode* rec) const
{
	RCursor<GLCEntry> Cur(*LCs[rec->GetDepth()]);
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		if(Cur()->Rec==rec)
			return(Cur()->Child);
	}
	cerr<<"Big problem in GConceptTree::GetFirstChild"<<endl;
	return(cNoRef);
}



//------------------------------------------------------------------------------
void GConceptTree::Clear(void)
{
	Nodes.Clear();
	LCs.Clear();
}


//------------------------------------------------------------------------------
GConceptTree::~GConceptTree(void)
{
}



//------------------------------------------------------------------------------
//
//  GConceptNode
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GConceptNode::GConceptNode(GConcept* concept,size_t pos,char depth)
	: Concept(concept), Pos(pos), Depth(depth)
{
}
