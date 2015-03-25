/*

	GALILEI Research Project

	GQueryRes.cpp

	Query Result - Implementation.

	Copyright 2010-2015 by Pascal Francq (pascal@francq.info).
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
// include files for GALILEI
#include <gconceptnode.h>
#include <gconcepttree.h>
#include <gdoc.h>
#include <gdocref.h>


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
	: RContainer<GDocRef,true,true>(0)
{
}


//------------------------------------------------------------------------------
GQueryRes::GQueryRes(size_t size)
	: RContainer<GDocRef,true,true> (size)
{
}


//------------------------------------------------------------------------------
void GQueryRes::AddDoc(GEngineXML* engine,GDoc* doc,GConcept* concept)
{
	// Create the list of nodes
	GDocRef* Occurs(new GDocRef(doc));
	InsertPtr(Occurs);

	// Fill it by using the structure
	const GConceptTree* Tree(engine->GetTree(doc->GetId()));
	RCursor<GConceptNode> Cur(Tree->GetNodes(concept));
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		if(engine->OnlyDocs)
		{
			// Select the whole document
			bool Exist;
			Occurs->AddFragment(doc,Cur(),0,Tree->GetMaxPos(Tree->GetNearestNode(0),engine->EndWindowPos),Exist);
		}
		else
		{
			if(Cur()->GetType()==ttText)
			{
				bool Exist;

				// If the text has a parent node, its parent must be the root node of the result
				if(Cur()->GetParent())
					Occurs->AddFragment(Cur()->GetParent(),Cur(),Exist); // Insert a node and its parent
				else
				{
					// Add a fragment with the text window
					Occurs->AddFragment(Cur(),
											  Cur()->GetPos(),
											  Cur()->GetSyntacticPos(),
											  Tree->GetMinPos(Cur(),engine->BeginWindowPos),
											  Tree->GetMaxPos(Cur(),engine->EndWindowPos),
											  false,Exist);
				}
			}
			else
				Occurs->AddFragment(Cur()); // Insert a node and its children
		}
	}
}


//------------------------------------------------------------------------------
void GQueryRes::InsertDoc(GDocRef* doc)
{
	InsertPtr(doc);
}


//------------------------------------------------------------------------------
void GQueryRes::Print(void) const
{
	RCursor<GDocRef> Docs(*this);
	for(Docs.Start();!Docs.End();Docs.Next())
	{
		cout<<"DocId "<<Docs()->GetDoc()->GetId()<<endl;
		RCursor<GDocFragment> Fragment(Docs()->GetFragments());
		for(Fragment.Start();!Fragment.End();Fragment.Next())
			Fragment()->Print();
		cout<<endl;
	}
}


//------------------------------------------------------------------------------
RCursor<GDocRef> GQueryRes::GetDocs(void) const
{
	return(RCursor<GDocRef>(*this));
}


//------------------------------------------------------------------------------
void GQueryRes::Reduce(void)
{
	RCursor<GDocRef> Docs(*this);
	for(Docs.Start();!Docs.End();Docs.Next())
		Docs()->Reduce(false);
}