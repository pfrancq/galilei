/*

	GALILEI Research Project

	GDocRef.h

	Document Reference - Implemetation.

	Copyright 2008-2015 by Pascal Francq (pascal@francq.info).

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).

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



//-----------------------------------------------------------------------------
// include files for R
#include <rnodecursor.h>
using namespace std;
using namespace R;


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <gdocref.h>
#include <gdoc.h>
#include <gdocfragment.h>
#include <gconceptnode.h>
#include <gconcepttree.h>
using namespace GALILEI;



//-----------------------------------------------------------------------------
//
// class GDocRef
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GDocRef::GDocRef(GDoc* doc)
	: Doc(doc), Fragments(2,4)
{
	if(!Doc)
		mThrowGException("Cannot pass a null document pointer");
}


//-----------------------------------------------------------------------------
int GDocRef::Compare(const GDocRef& ref) const
{
	return(CompareIds(Doc->GetId(),ref.Doc->GetId()));
}


//-----------------------------------------------------------------------------
int GDocRef::Compare(const GDoc* doc) const
{
	return(CompareIds(Doc->GetId(),doc->GetId()));
}


//-----------------------------------------------------------------------------
void GDocRef::Clear(void)
{
	Fragments.Clear();
}


//-----------------------------------------------------------------------------
GDocFragment* GDocRef::AddFragment(GConceptNode* node,size_t pos,size_t begin,size_t end,bool children,bool& exist)
{
	GDocFragment* Fragment;

	size_t idx(Fragments.GetIndex(GDocFragment::Search(Doc->GetId(),pos),exist));

	if(exist)
	{
		Fragment=Fragments[idx];
	}
	else
		Fragments.InsertPtrAt(Fragment=new GDocFragment(Doc,node,pos,begin,end),idx,false);

	if(node&&children)
	{
		RNodeCursor<GConceptTree,GConceptNode> Child(node);
		for(Child.Start();!Child.End();Child.Next())
			Fragment->AddChild(Child());
	}

	return(Fragment);
}


//-----------------------------------------------------------------------------
GDocFragment* GDocRef::AddFragment(GConceptNode* node)
{
	bool Exist;
	return(AddFragment(node,node->GetPos(),node->GetPos(),node->GetTree()->GetMaxPos(node,1),false,Exist));
}


//-----------------------------------------------------------------------------
GDocFragment* GDocRef::AddFragment(GConceptNode* node,GConceptNode* child,size_t pos,size_t begin,size_t end,bool children,bool& exist)
{
	GDocFragment* Fragment(AddFragment(node,pos,begin,end,children,exist));
	Fragment->AddChild(child);
	return(Fragment);
}


//-----------------------------------------------------------------------------
GDocFragment* GDocRef::AddFragment(GConceptNode* node,GConceptNode* child)
{
	bool Exist;
	size_t Min(node->GetPos());
	if(Min>child->GetPos())
		Min=child->GetPos();
	size_t Max(node->GetPos());
	if(Max<child->GetPos())
		Max=child->GetPos();
	return(AddFragment(node,child,node->GetPos(),Min,Max,false,Exist));
}


//-----------------------------------------------------------------------------
GDocFragment* GDocRef::CopyFragment(const GDocFragment* tocopy,bool& exist)
{
	GDocFragment* Fragment;

	size_t idx(Fragments.GetIndex(GDocFragment::Search(tocopy->GetDoc()->GetId(),tocopy->GetPos()),exist));
	if(exist)
	{
		Fragment=Fragments[idx];
	}
	else
		Fragments.InsertPtrAt(Fragment=new GDocFragment(tocopy->GetDoc(),tocopy->GetNode(),tocopy->GetPos(),tocopy->GetBegin(),tocopy->GetEnd()),idx,false);

	return(Fragment);
}


//-----------------------------------------------------------------------------
void GDocRef::CopyFragments(const GDocRef* tocopy)
{
	RCursor<GDocFragment> Fragment(tocopy->Fragments);
	for(Fragment.Start();!Fragment.End();Fragment.Next())
	{
		bool Find;
		size_t idx(Fragments.GetIndex(GDocFragment::Search(Fragment()->GetDoc()->GetId(),Fragment()->GetPos()),Find));
		if(!Find)
			Fragments.InsertPtrAt(new GDocFragment(Fragment()->GetDoc(),Fragment()->GetNode(),Fragment()->GetPos(),Fragment()->GetBegin(),Fragment()->GetEnd()),idx,false);
	}
}


//-----------------------------------------------------------------------------
RCursor<GDocFragment> GDocRef::GetFragments(void) const
{
	return(RCursor<GDocFragment>(Fragments));
}


//------------------------------------------------------------------------------
bool GDocRef::FindSibling(GDocFragment* fragment) const
{
	GConceptNode* Parent(fragment->GetNode()->GetParent());
	RCursor<GDocFragment> Nodes(Fragments);
	for(Nodes.Start();!Nodes.End();Nodes.Next())
	{
		if(Nodes()->GetNode()->GetParent()==Parent)
			return(true);
	}
	return(false);
}


//------------------------------------------------------------------------------
bool GDocRef::FindChild(GDocFragment* fragment) const
{
	size_t Depth(fragment->GetNode()->GetSyntacticDepth());
	RCursor<GDocFragment> Nodes(Fragments);
	for(Nodes.Start();!Nodes.End();Nodes.Next())
	{
		if(Nodes()->GetNode()->GetSyntacticDepth()>Depth)
		{
			// Search the if one of the parent is node
			GConceptNode* Parent(Nodes()->GetNode()->GetParent());
			while(Parent&&(Parent->GetSyntacticDepth()>=Depth))
			{
				if(Parent==fragment->GetNode())
					return(false);
				Parent=Parent->GetParent();
			}
			return(false);
		}
	}
	return(false);
}


//------------------------------------------------------------------------------
void GDocRef::Reduce(bool force)
{
	// If less than two results -> nothing to merges
	if(Fragments.GetNb()<2)
		return;

	// Look which node to merge (and those to delete)
	RContainer<GDocFragment,false,true> ToDel(10);
	RCursor<GDocFragment> Fragment(Fragments,0,Fragments.GetNb()-2);
	for(Fragment.Start();!Fragment.End();Fragment.Next())
	{
		// Don't treat a node already deleted
		if((!Fragment())||(ToDel.IsIn(*Fragment())))
			continue;

		RCursor<GDocFragment> Fragment2(Fragments,Fragment.GetPos()+1,Fragments.GetNb()-1);
		for(Fragment2.Start();!Fragment2.End();Fragment2.Next())
		{
			// Don't treat a node already deleted
			if((!Fragment2())||(ToDel.IsIn(*Fragment2())))
				continue;

			// Verify if the two fragment are force to be merged if they have the same node or if they overlap
			if((force&&(Fragment()->GetNode()==Fragment2()->GetNode()))||((!force)&&Fragment()->Overlap(Fragment2())))
			{
				Fragment()->Merge(Fragment2());
				ToDel.InsertPtr(Fragment2());
			}
		}
	}

	// Delete the necessary nodes
	RCursor<GDocFragment> Del(ToDel);
	for(Del.Start();!Del.End();Del.Next())
		Fragments.DeletePtr(*Del());
}


//------------------------------------------------------------------------------
void GDocRef::Print(void)
{
	cout<<Doc->GetId()<<endl;
	RCursor<GDocFragment> Fragment(Fragments);
	for(Fragment.Start();!Fragment.End();Fragment.Next())
	{
		cout<<"\t"<<Fragment()->GetNode()->GetSyntacticPos()<<" ("<<Fragment()->GetNode()->GetConceptId()<<")"<<endl;
		RCursor<GConceptNode> Child(Fragment()->GetChildren());
		for(Child.Start();!Child.End();Child.Next())
			cout<<"\t\t"<<Child()->GetSyntacticPos()<<" ("<<Child()->GetConceptId()<<")"<<endl;
	}
}
