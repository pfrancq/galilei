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
// include files for GALILEI
#include <gdocref.h>
#include <gdoc.h>
#include <gdocfragment.h>
using namespace std;
using namespace R;
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
GDocFragment* GDocRef::AddFragment(GConceptNode* node,size_t pos,size_t first,size_t last,bool& exist)
{
	GDocFragment* Fragment;

	size_t idx(Fragments.GetIndex(GDocFragment::Search(Doc->GetId(),pos),exist));

	if(exist)
	{
		Fragment=Fragments[idx];
	}
	else
		Fragments.InsertPtrAt(Fragment=new GDocFragment(Doc,node,pos,first,last),idx,false);

	return(Fragment);
}


//-----------------------------------------------------------------------------
RCursor<GDocFragment> GDocRef::GetFragments(void) const
{
	return(RCursor<GDocFragment>(Fragments));
}
