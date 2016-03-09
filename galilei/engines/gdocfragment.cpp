/*

	GALILEI Research Project

	GDocFragment.cpp

	Relevant Document for a Search - Implementation.

   Copyright 2003-2015 by Pascal Francq (pascal@francq.info).
	Copyright 2003-2004 by Valery Vandaele.
	Copyright 2003-2008 Université Libre de Bruxelles (ULB).

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
// include files for ANSI C/C++
#include <stdio.h>
#include <iostream>
#include <cstdlib>


//------------------------------------------------------------------------------
// include files for GALILEI
#include <gmetaengine.h>
#include <gdocfragment.h>
#include <gdoc.h>
#include <gdocref.h>
#include <gsession.h>
#include <gfilter.h>
using namespace GALILEI;
using namespace R;
using namespace std;



//------------------------------------------------------------------------------
//
// class GDocFragment::Search
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GDocFragment::Search::Search(size_t docid,size_t pos,bool wholedoc)
	: DocId(docid), Pos(pos)
{
}



//------------------------------------------------------------------------------
//
// class GDocFragment
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GDocFragment::GDocFragment(GDoc* doc,const GConceptRecord* root,size_t pos,size_t spos,size_t begin,size_t end,const R::RDate& proposed)
	: Doc(doc), Root(root), Pos(pos), SyntacticPos(spos), Begin(begin), End(end), Proposed(proposed), WholeDoc(false), Children(5,5)
{
	if(!Doc)
		mThrowGException("Cannot have a null document reference");
	if(Begin>End)
		mThrowGException("Invalid window positions "+RString::Number(Begin)+">"+RString::Number(End));
}


//------------------------------------------------------------------------------
GDocFragment::GDocFragment(GDoc* doc,size_t begin,size_t end,const R::RDate& proposed)
	: Doc(doc), Root(0), Pos(0), SyntacticPos(0), Begin(begin), End(end), Proposed(proposed), WholeDoc(true), Children(5,5)
{
	if(!Doc)
		mThrowGException("Cannot have a null document reference");
}


//------------------------------------------------------------------------------
int GDocFragment::Compare(const GDocFragment& d) const
{
	int i(CompareIds(Doc->GetId(),d.Doc->GetId()));
	if(!i)
	{
		if(WholeDoc&&d.WholeDoc)
			return(true);
		return(CompareIds(Pos,d.Pos));
	}
	return(i);
}


//------------------------------------------------------------------------------
int GDocFragment::Compare(const Search& search) const
{
	int i(CompareIds(Doc->GetId(),search.DocId));
	if(!i)
	{
		if(WholeDoc&&search.WholeDoc)
			return(true);
		return(CompareIds(Pos,search.Pos));
	}
	return(i);
}


//------------------------------------------------------------------------------
bool GDocFragment::IsFlat(void) const
{
	if(WholeDoc)
		return(true);

	if(!Root)
		return(true);

	if(!Root->GetSyntacticDepth())
		return(true);

	return(false);
}


//------------------------------------------------------------------------------
R::RCursor<const GConceptRecord> GDocFragment::GetChildren(void) const
{
	return(RCursor<const GConceptRecord>(Children));
}


//------------------------------------------------------------------------------
R::RString GDocFragment::GetFragment(size_t max)
{
	if(Fragment.IsEmpty())
	{
		GFilter* Filter(GALILEIApp->FindMIMEType(Doc));
		if(Filter)
		{
			size_t size(End-Begin+1);
			if((!size)||(size<=max))
				size=End;
			else
				size=Begin+max-1;
			Fragment=Filter->GetTextFragment(Doc,Begin,size);
		}
	}
	return(Fragment);
}


//------------------------------------------------------------------------------
void GDocFragment::AddChild(const GConceptRecord* rec)
{
	// compute the extract to associate to that node
	if(!WholeDoc)
	{
		size_t max(0), min(0);
		if(rec->GetType()==ttText)
		{
			min=rec->GetSyntacticPos();
			max=rec->GetSyntacticPos();
		}
		else
		{
			min=max=rec->GetSyntacticPos();
		}
		if(max>End)
			End=max;
		if(min<Begin)
			Begin=min;
	}
	bool Find;
	size_t Idx(Children.GetIndex(*rec,Find));
	if(!Find)
		Children.InsertPtrAt(rec,Idx,false);
}


//------------------------------------------------------------------------------
bool GDocFragment::Overlap(const GDocFragment* fragment) const
{
	// If the same document -> It cannot overlap
	if(Doc!=fragment->Doc)
		return(false);

	// If at least one fragment represents the whole document -> they overlap
	if((!Root)||(!fragment->Root))
		return(true);

	if(WholeDoc||fragment->WholeDoc)
		return(true);

 	// Verify if the two fragments have the same selected node
	GConceptRecord Parent,Parent2;
	bool HasParent(Doc->GetSession()->FindParentRecord(Doc,*Root,Parent));
	bool HasParent2(Doc->GetSession()->FindParentRecord(Doc,*fragment->GetRoot(),Parent2));
	if(HasParent||HasParent2&&(Parent!=Parent2))
		return(false);

	// Look if the intervals overlap
	if((fragment->End<Begin)||(fragment->Begin>End))
		return(false);

	// Code by Faiza Abbacci to verify if the children of each node are not to
	// far away. Replace by comparing only the intervals
//	RCursor<GConceptNode> Child(Children);
//	RCursor<GConceptNode> Child2( fragment->Children);
//	for(Child.Start();!Child.End();Child.Next())
//	{
//		size_t Pos(Child()->GetSyntacticPos());
//		for(Child2.Start();!Child2.End();Child2.Next())
//		{
//			size_t Pos2(Child2()->GetSyntacticPos());
//			if(((Pos<Pos2)&&(Pos+Begin<Pos2))||((Pos2<Pos)&&(Pos2+End<Pos)))
//				return(false);
//		}
//	}

	return(true);
}


//------------------------------------------------------------------------------
void GDocFragment::Merge(const GDocFragment* fragment)
{
	// Merge the two node lists
	RCursor<const GConceptRecord> Child(fragment->Children);
	for(Child.Start();!Child.End();Child.Next())
	{
		bool Find;
		size_t Index(Children.GetIndex(*Child(),Find));
		if(Find)
			continue;
		Children.InsertPtrAt(Child(),Index,false);
		if(!WholeDoc)
		{
			if(Child()->GetPos()>End)
				End=Child()->GetPos();
			if(Child()->GetPos()<Begin)
				Begin=Child()->GetPos();
		}
	}
}


//------------------------------------------------------------------------------
void GDocFragment::Print(void) const
{
	cout<<"Fragment : Doc="<<Doc->GetId()<<" ; Node=";
	if(Root)
		cout<<Root->GetConceptId()<<endl;
	else
		cout<<"Null"<<endl;
	cout<<"\t"<<Pos<<" in ["<<Begin<<","<<End<<"]"<<endl<<"\tChildren: ";
	RCursor<const GConceptRecord> Child(Children);
	for(Child.Start();!Child.End();Child.Next())
		cout<<Child()->GetConceptId();
	cout<<endl;
}


//------------------------------------------------------------------------------
GDocFragment::~GDocFragment(void)
{
}
