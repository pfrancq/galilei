/*

	GALILEI Research Project

	GConceptRef.cpp

	Concept Reference - Implementation.

	Copyright 2002-2014 by Pascal Francq (pascal@francq.info).
	Copyright 2002-2008 by the Université Libre de Bruxelles (ULB).

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
#include <gconceptref.h>
#include <glang.h>
#include <gsession.h>
using namespace GALILEI;
using namespace R;



//------------------------------------------------------------------------------
//
//  GConceptRef
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GConceptRef::GConceptRef(GConcept* concept)
  : Concept(concept), Weight(0.0)
{
}


//------------------------------------------------------------------------------
GConceptRef::GConceptRef(GConcept* concept,double w)
  : Concept(concept), Weight(w)
{
}


//------------------------------------------------------------------------------
GConceptRef::GConceptRef(const GConceptRef& ref)
  : Concept(ref.Concept), Weight(ref.Weight)
{
}


//------------------------------------------------------------------------------
int GConceptRef::Compare(const GConceptRef& ref) const
{
	return(CompareIds(Concept->Id,ref.Concept->Id));
}


//------------------------------------------------------------------------------
int GConceptRef::Compare(const GConcept& concept) const
{
	return(CompareIds(Concept->Id,concept.Id));
}


//------------------------------------------------------------------------------
int GConceptRef::Compare(const GConcept* concept) const
{
	if(!concept)
		mThrowGException("null pointer");
	return(CompareIds(Concept->Id,concept->Id));
}


//------------------------------------------------------------------------------
void GConceptRef::SetWeight(double w)
{
	Weight=w;
}


//------------------------------------------------------------------------------
GConceptRef& GConceptRef::operator=(const GConceptRef& ref)
{
	Concept=ref.Concept;
	Weight=ref.Weight;
	return(*this);
}


//------------------------------------------------------------------------------
GConceptRef& GConceptRef::operator+=(double w)
{
	Weight+=w;
	return(*this);
}


//------------------------------------------------------------------------------
GConceptRef& GConceptRef::operator-=(double w)
{
	Weight-=w;
	return(*this);
}


//------------------------------------------------------------------------------
GConceptRef& GConceptRef::operator*=(double w)
{
	Weight*=w;
	return(*this);
}


//------------------------------------------------------------------------------
GConceptRef& GConceptRef::operator/=(double w)
{
	Weight/=w;
	return(*this);
}


//------------------------------------------------------------------------------
GConceptRef::~GConceptRef(void)
{
}
