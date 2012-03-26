/*

	GALILEI Research Project

	GQueryRes.cpp

	Query Result - Implementation.

	Copyright 2004-2012 by Pascal Francq.
   Copyright 2004-2005 by Jean-Baptiste Valsamis.
	Copyright 2005-2009 by Faïza Abbaci.

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


//------------------------------------------------------------------------------
// include files for current project
#include <gqueryres.h>
#include <genginexml.h>
#include <gconcepttree.h>
#include <gconcept.h>
#include <gconcepttype.h>



//------------------------------------------------------------------------------
//
// class GQueryRes
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GQueryRes::GQueryRes(void)
	: RContainer<GResNodes,true,true>(0)
{
}


//------------------------------------------------------------------------------
GQueryRes::GQueryRes(size_t size)
	: RContainer<GResNodes,true,true> (size)
{
}


//------------------------------------------------------------------------------
void GQueryRes::AddDoc(GEngineXML* engine,size_t docid,GConcept* concept)
{
	// Create the list of nodes
	GResNodes* Occurs(new GResNodes(docid));
	InsertPtr(Occurs);

	// Fill it by using the structure
	const GConceptTree* Tree(engine->GetTree(docid));
	RCursor<GConceptNode> Cur(Tree->GetNodes(concept));
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		if(Cur()->GetType()==ttText)
			Occurs->InsertPtr(Cur()->GetParent(),Cur()); // Insert a node and its parent
		else
			Occurs->InsertPtr(Cur()); // Insert a node and its children
	}
}


//------------------------------------------------------------------------------
void GQueryRes::InsertDoc(GResNodes* nodes)
{
	InsertPtr(nodes);
}


//------------------------------------------------------------------------------
void GQueryRes::Print(void) const
{
	RCursor<GResNodes> Nodes(*this);
	for(Nodes.Start();!Nodes.End();Nodes.Next())
	{
		cout<<"DocId "<<Nodes()->GetDocId()<<endl;
		RCursor<GResNode> Node(Nodes()->GetNodes());
		for(Node.Start();!Node.End();Node.Next())
		{
			cout<<"  "<<Node()->GetNode()->GetConceptId()<<","<<Node()->GetNode()->GetPos()<<","<<Node()->GetNode()->GetSyntacticPos()<<","<<Node()->GetNode()->GetSyntacticDepth()<<endl;
		}
	}
}


//------------------------------------------------------------------------------
RCursor<GResNodes> GQueryRes::GetDocs(void) const
{
	return(RCursor<GResNodes>(*this));
}


//------------------------------------------------------------------------------
void GQueryRes::Reduce(void)
{
	RCursor<GResNodes> Nodes(*this);
	for(Nodes.Start();!Nodes.End();Nodes.Next())
		Nodes()->Reduce(false);
}