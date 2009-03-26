/*

	GALILEI Research Project

	GDocStruct.cpp

	Structure of the document content - Implementation.

	Copyright 2008-2009 by Pascal Francq (pascal@francq.info).

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
#include <gdocstruct.h>
#include <gweightinfo.h>
#include <gconcepttype.h>
using namespace GALILEI;
using namespace R;
using namespace std;


//------------------------------------------------------------------------------
//
//  GVTDRec
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GVTDRec::GVTDRec(GConcept* concept,GVTDRec::RecType type,size_t pos,char depth)
	: Concept(concept), Type(type), Pos(pos), Depth(depth)
{
}



//------------------------------------------------------------------------------
//
//  GLCEntry
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
class GLCEntry
{
public:
	GVTDRec* Rec;
	size_t Child;
	GLCEntry(GVTDRec* rec,size_t child) : Rec(rec), Child(child) {}
	int Compare(const GLCEntry&) const {return(-1);}
};



//------------------------------------------------------------------------------
//
//  GDocSruct::GLC
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
class GDocStruct::GLC : public RContainer<GLCEntry,true,false>
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
GDocStruct::GDocStruct(void)
	: Recs(2000), LCs(new R::RContainer<GLC,true,false>(10))
{
}


//------------------------------------------------------------------------------
GDocStruct::GDocStruct(const GDocStruct& docstruct)
	: Recs(docstruct.Recs.GetNb()), LCs(new R::RContainer<GLC,true,false>(docstruct.LCs->GetNb()))
{
	// Recreate correctly the structure
	R::RCursor<GVTDRec> Recs(docstruct.Recs);
	for(Recs.Start();!Recs.End();Recs.Next())
	{
		size_t child=docstruct.GetFirstChild(Recs());
		AddRecord(Recs()->GetConcept(),Recs()->GetType(),Recs()->GetPos(),Recs()->GetDepth(),child);
	}
}


//------------------------------------------------------------------------------
GDocStruct::GDocStruct(size_t vtd,size_t lc)
	: Recs(vtd), LCs(new R::RContainer<GLC,true,false>(lc))
{
}


//------------------------------------------------------------------------------
void GDocStruct::SetSizes(size_t vtd,size_t lc)
{
	Recs.VerifyTab(vtd);
	LCs->VerifyTab(lc);
}


//------------------------------------------------------------------------------
size_t GDocStruct::GetNbRecs(void) const
{
	return(Recs.GetNb());
}



//------------------------------------------------------------------------------
size_t GDocStruct::GetNbLCs(void) const
{
	return(LCs->GetNb());
}


//------------------------------------------------------------------------------
size_t GDocStruct::GetNbLCEntries(size_t level) const
{
	if(level>=LCs->GetNb())
		return(0);
	return((*LCs)[level]->GetNb());
}


//------------------------------------------------------------------------------
void GDocStruct::SetNbLCEntries(size_t level,size_t size) const
{
	if(!LCs->VerifyIndex(level))
		LCs->InsertPtrAt(new GLC(size),level);
}


//------------------------------------------------------------------------------
GVTDRec* GDocStruct::AddRecord(GConcept* concept,GVTDRec::RecType type,size_t pos,char depth,size_t child,size_t nbrecs)
{
	// Create the record and insert it
	GVTDRec* ptr=new GVTDRec(concept,type,pos,depth);
	Recs.InsertPtr(ptr);

	// Insert it the location cache if necessary
	if(type==GVTDRec::Tag)
	{
		GLC* Level;
		if(LCs->VerifyIndex(depth))
			Level=(*LCs)[depth];
		else
		{
			if(!nbrecs)
				nbrecs=200;
			LCs->InsertPtrAt(Level=new GLC(nbrecs),depth);
		}
		Level->InsertPtr(new GLCEntry(ptr,child));
	}

	// Return
	return(ptr);
}


//------------------------------------------------------------------------------
R::RCursor<GVTDRec> GDocStruct::GetRecs(void) const
{
	return(R::RCursor<GVTDRec>(Recs));
}


//------------------------------------------------------------------------------
size_t GDocStruct::GetFirstChild(GVTDRec* rec) const
{
	if(rec->GetType()!=GVTDRec::Tag)
		return(SIZE_MAX);
	RCursor<GLCEntry> Cur(*(*LCs)[rec->GetDepth()]);
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		if(Cur()->Rec==rec)
			return(Cur()->Child);
	}
	cerr<<"Big problem in GDocStruct::GetFirstChild"<<endl;
	return(SIZE_MAX);
}



//------------------------------------------------------------------------------
void GDocStruct::Clear(void)
{
	Recs.Clear();
	LCs->Clear();
}


//------------------------------------------------------------------------------
GDocStruct::~GDocStruct(void)
{
	delete LCs;
}
