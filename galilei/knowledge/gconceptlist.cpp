/*

	GALILEI Research Project

	GConceptList.cpp

	XML Index - Implementation.

	Copyright 2007-2011 by Pascal Francq (pascal@francq.info).
	Copyright 2007-2008 by the Université Libre de Bruxelles (ULB).

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
#include <gconceptlist.h>
#include <gconcepttype.h>
#include <gsession.h>
#include <glang.h>
using namespace GALILEI;
using namespace R;
using namespace std;



//-----------------------------------------------------------------------------
//
// GConceptList
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GConceptList::GConceptList(const GConceptList* list)
	: GConcept(list), Concepts(list->Concepts)
{
}


//-----------------------------------------------------------------------------
GConceptList::GConceptList(GSession* session,const RString& name,GConceptType* type,const R::RString& def)
	: GConcept(session,name,type), Concepts(10)
{
	BuildFromDef(def);
}


//-----------------------------------------------------------------------------
GConceptList::GConceptList(GSession* session,GConceptType* type)
	: GConcept(session,RString::Null,type), Concepts(10)
{
}



//-----------------------------------------------------------------------------
GConceptList::GConceptList(GSession* session,GConceptType* type,const R::RString& name,R::RContainer<GConcept,false,true>& concepts)
	: GConcept(session,name,type), Concepts(concepts)
{
	if(Name.IsEmpty())
		Name=GetDef();
}



//-----------------------------------------------------------------------------
GConceptList::GConceptList(GSession* session,size_t id, const R::RString& name, GConceptType* type,const R::RString& def,
		size_t refdocs, size_t idxdocs,
		size_t refprofiles, size_t idxprofiles,
		size_t refcommunities, size_t idxcommunities,
		size_t reftopics, size_t idxtopics,
		size_t refclasses, size_t idxclasses)
	: GConcept(session,id,name,type,refdocs,idxdocs,refprofiles,idxprofiles,refcommunities,idxcommunities,reftopics,idxtopics,refclasses,idxclasses),
	  Concepts(10)
{
	BuildFromDef(def);
}


//-----------------------------------------------------------------------------
void GConceptList::BuildFromDef(const R::RString& def)
{
	int Pos(0);
	int Old(0);
	RString Id;

	// Find the concepts
	for(Pos=def.Find(':',Old);Pos!=-1;Pos=def.Find(':',Old))
	{
		Id=def.Mid(static_cast<size_t>(Old),Pos-Old);
		Old=Pos+1;
		GConcept* ptr=Session->GetConcept(Id.ToSizeT());
		if(!ptr)
			ThrowGException("Concept "+Id+" does not exist");
		Concepts.InsertPtr(ptr);
	}
}


//-----------------------------------------------------------------------------
RString GConceptList::GetDef(void) const
{
	RString Def;
	bool Double(false);
	RCursor<GConcept> Cur(Concepts);
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		if(Double)
			Def+=":";
		else
			Double=true;
		Def+=RString::Number(Cur()->GetId());
	}
	return(Def);
}


//-----------------------------------------------------------------------------
GConcept* GConceptList::DeepCopy(void) const
{
	return(new GConceptList(this));
}


//-----------------------------------------------------------------------------
void GConceptList::Clear(void)
{
	GConcept::Clear();
	Concepts.Clear();
}


//-----------------------------------------------------------------------------
R::RCursor<GConcept> GConceptList::GetConcepts(void) const
{
	return(RCursor<GConcept>(Concepts));
}


//-----------------------------------------------------------------------------
void GConceptList::AddConcept(GConcept* concept)
{
	if(!Concepts.IsIn(concept))
		Concepts.InsertPtr(concept);
}



//-----------------------------------------------------------------------------
double GConceptList::GetSimilarity(const GConceptList* list) const
{
	size_t Min(std::min(Concepts.GetNb(),list->Concepts.GetNb()));

	size_t nb(0);

	// Concepts
	RCursor<GConcept> Concept1(Concepts);
	RCursor<GConcept> Concept2(list->Concepts);
	for(Concept1.Start(),Concept2.Start();!Concept1.End();Concept1.Next())
	{
		while((!Concept2.End())&&(Concept2()->GetId()<Concept1()->GetId()))
			Concept2.Next();
		if((!Concept2.End())&&(Concept2()->GetId()==Concept1()->GetId()))
		{
			nb++;
			Concept2.Next();
		}
	}
	return(static_cast<double>(nb)/static_cast<double>(Min));
}



//-----------------------------------------------------------------------------
GConceptList::~GConceptList(void)
{
}
