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
#include <gsession.h>
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
GDocFragment* GDocRef::AddFragment(const GConceptRecord* rec,size_t pos,size_t spos,size_t begin,size_t end,bool& exist)
{
	GDocFragment* Fragment;

	size_t idx(Fragments.GetIndex(GDocFragment::Search(Doc->GetId(),pos,false),exist));

	if(exist)
	{
		Fragment=Fragments[idx];
	}
	else
		Fragments.InsertPtrAt(Fragment=new GDocFragment(Doc,rec,pos,spos,begin,end),idx,false);

	return(Fragment);
}


//-----------------------------------------------------------------------------
GDocFragment* GDocRef::AddFragment(const GConceptRecord* rec)
{
	if(!rec)
		mThrowGException("Null concept record not allowed");

	bool Exist;
	GConceptRecord find;
	size_t Max(Doc->GetSession()->GetMaxPosRecord(Doc,*rec,1));
	return(AddFragment(rec,rec->GetPos(),rec->GetSyntacticPos(),rec->GetPos(),Max,Exist));
}


//-----------------------------------------------------------------------------
GDocFragment* GDocRef::AddFragment(const GConceptRecord* rec,const GConceptRecord* child,size_t pos,size_t spos,size_t begin,size_t end,bool& exist)
{
	if(!child)
		mThrowGException("Null concept record not allowed");
	GDocFragment* Fragment(AddFragment(rec,pos,spos,begin,end,exist));
	Fragment->AddChild(child);
	return(Fragment);
}


//-----------------------------------------------------------------------------
GDocFragment* GDocRef::AddFragment(const GConceptRecord* rec,const GConceptRecord* child,bool& exist)
{
	if((!rec)||(!child))
		mThrowGException("Null concept record not allowed");

	size_t Min(rec->GetPos());
	if(Min>child->GetPos())
		Min=child->GetPos();
	size_t Max(rec->GetPos());
	if(Max<child->GetPos())
		Max=child->GetPos();
	return(AddFragment(rec,child,rec->GetPos(),rec->GetSyntacticPos(),Min,Max,exist));
}


//-----------------------------------------------------------------------------
GDocFragment* GDocRef::AddFragment(GDoc* doc,const GConceptRecord* child,size_t begin,size_t end,bool& exist)
{
	if(!child)
		mThrowGException("Null concept record not allowed");

	GDocFragment* Fragment;

	size_t idx(Fragments.GetIndex(GDocFragment::Search(doc->GetId(),0,true),exist));

	if(exist)
	{
		Fragment=Fragments[idx];
	}
	else
		Fragments.InsertPtrAt(Fragment=new GDocFragment(doc,begin,end),idx,false);

	if(child)
		Fragment->AddChild(child);

	return(Fragment);
}


//-----------------------------------------------------------------------------
GDocFragment* GDocRef::CopyFragment(const GDocFragment* tocopy,bool& exist)
{
	GDocFragment* Fragment;

	size_t idx(Fragments.GetIndex(GDocFragment::Search(tocopy->GetDoc()->GetId(),tocopy->GetPos(),tocopy->WholeDoc),exist));
	if(exist)
	{
		Fragment=Fragments[idx];
	}
	else
		Fragments.InsertPtrAt(Fragment=new GDocFragment(tocopy->GetDoc(),
																	   tocopy->GetRecord(),
																	   tocopy->GetPos(),
																	   tocopy->GetSyntacticPos(),
																	   tocopy->GetBegin(),
																	   tocopy->GetEnd()),
									                           idx,false);

	return(Fragment);
}


//-----------------------------------------------------------------------------
void GDocRef::CopyFragments(const GDocRef* tocopy)
{
	RCursor<GDocFragment> Fragment(tocopy->Fragments);
	for(Fragment.Start();!Fragment.End();Fragment.Next())
	{
		bool Find;
		size_t idx(Fragments.GetIndex(GDocFragment::Search(Fragment()->GetDoc()->GetId(),Fragment()->GetPos(),Fragment()->WholeDoc),Find));
		if(!Find)
			Fragments.InsertPtrAt(new GDocFragment(Fragment()->GetDoc(),
															   Fragment()->GetRecord(),
															   Fragment()->GetPos(),
																Fragment()->GetSyntacticPos(),
																Fragment()->GetBegin(),
															   Fragment()->GetEnd()),
										                  idx,false);
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
	if(fragment->GetDoc()!=Doc)
		return(false);

	// If the fragment is the whole document, it has a sibling of course
	const GConceptRecord* Child(fragment->GetRecord());
	if(!Child)
		return(true);

	// Find the parent
	GConceptRecord Parent;
	bool HasParent(!Doc->GetSession()->FindParentRecord(Doc,*Child,Parent));
	RCursor<GDocFragment> Recs(Fragments);
	for(Recs.Start();!Recs.End();Recs.Next())
	{
		const GConceptRecord* Child2(Recs()->GetRecord());
		if(!Child2)
			return(true);
		GConceptRecord Parent2;
		bool HasParent2(Doc->GetSession()->FindParentRecord(Doc,*Child2,Parent2));
		if(((!HasParent)&&(!HasParent2))||(HasParent&&HasParent2&&(Parent2==Parent)))
			return(true);
	}
	return(false);
}


//------------------------------------------------------------------------------
bool GDocRef::FindChild(GDocFragment* fragment) const
{
	if(fragment->GetDoc()!=Doc)
		return(false);
	const GConceptRecord* Rec(fragment->GetRecord());
	if(!Rec)
		return(true);

	size_t Depth(fragment->GetRecord()->GetSyntacticDepth());
	RCursor<GDocFragment> Recs(Fragments);
	for(Recs.Start();!Recs.End();Recs.Next())
	{
		if(Recs()->GetRecord()->GetSyntacticDepth()>Depth)
		{
			// Search the if one of the parent is node
			const GConceptRecord* Child(Recs()->GetRecord());
			if(!Child)
				return(true);

			GConceptRecord Parent,Search;
			bool HasParent(Doc->GetSession()->FindParentRecord(Doc,*Child,Parent));
			while(HasParent&&(Parent.GetSyntacticDepth()>=Depth))
			{
				if(Parent==(*Rec))
					return(false);
				Search=Parent;
				HasParent=Doc->GetSession()->FindParentRecord(Doc,Search,Parent);
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
			const GConceptRecord* Rec(Fragment()->GetRecord());
			const GConceptRecord* Rec2(Fragment2()->GetRecord());
			if((force&&(((!Rec)&&(!Rec2))||(Rec&&Rec2&&((*Rec)==(*Rec2)))))||((!force)&&Fragment()->Overlap(Fragment2())))
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
		if(Fragment()->GetRecord())
			cout<<"\t"<<Fragment()->GetRecord()->GetSyntacticPos()<<" ("<<Fragment()->GetRecord()->GetConceptId()<<")"<<endl;
		else
			cout<<"\tWhole document"<<endl;
		RCursor<const GConceptRecord> Child(Fragment()->GetChildren());
		for(Child.Start();!Child.End();Child.Next())
			cout<<"\t\t"<<Child()->GetSyntacticPos()<<" ("<<Child()->GetConceptId()<<")"<<endl;
	}
}
