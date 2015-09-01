/*

	GALILEI Research Project

	GConceptType.cpp

	Concept Type - Implementation.

	Copyright 2006-2015 by Pascal Francq (pascal@francq.info).
	Copyright 2006-2008 by the Université Libre de Bruxelles (ULB).

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
#include <math.h>
using namespace std;


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <gconcepttype.h>
#include <gconcept.h>
#include <gsession.h>
#include <gstorage.h>
using namespace GALILEI;
using namespace R;



//-----------------------------------------------------------------------------
//
// GConceptType
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GConceptType::GConceptType(GSession* session,tConceptCat cat,size_t id,const RString& name,const RString& desc,size_t s)
	: GObject(session,id,name,otConceptType), Category(cat), Neutral(0), Concepts(20000,10000),
	  Description(desc)
{
}


//-----------------------------------------------------------------------------
int GConceptType::Compare(const GConceptType& type) const
{
	return(GetName().Compare(type.GetName()));
}


//-----------------------------------------------------------------------------
int GConceptType::Compare(const GConceptType* type) const
{
	return(GetName().Compare(type->GetName()));
}


//------------------------------------------------------------------------------
int GConceptType::Compare(const R::RString& name) const
{
	return(GetName().Compare(name));
}


//------------------------------------------------------------------------------
GConcept* GConceptType::GetNeutral(void) const
{
	if(Neutral)
		const_cast<GConceptType*>(this)->Neutral=Session->InsertObj(pConcept,const_cast<GConceptType*>(this),"*");
	return(Neutral);
}


//------------------------------------------------------------------------------
GConcept* GConceptType::GetObj(const GConcept*,const RString& name,bool null) const
{
	GConcept Search(Session,name,const_cast<GConceptType*>(this));
 	GConcept* Concept(Concepts.GetPtr(Search));
	if(Concept)
		return(Concept);
	if(null)
		return(0);
	mThrowGException("Unknown concept '"+name+"' for the type '"+GetName()+"'");
}


//------------------------------------------------------------------------------
void GConceptType::InsertObj(GConcept* concept)
{
	Concepts.InsertPtr(concept);
}


//------------------------------------------------------------------------------
void GConceptType::DeleteObj(GConcept* concept)
{
	Concepts.DeletePtr(*concept);
}


//------------------------------------------------------------------------------
GConceptType::~GConceptType(void)
{
}
