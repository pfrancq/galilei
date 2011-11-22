/*

	GALILEI Research Project

	GDescription.hh

	Description - Implementation.

	Copyright 2009-2011 by Pascal Francq (pascal@francq.info).

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
#include <rindexfile.h>


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <gdescription.h>
#include <gsession.h>
using namespace GALILEI;
using namespace R;
using namespace std;



//------------------------------------------------------------------------------
//
//  GDescription
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GDescription::GDescription(void)
	: Vectors(0)
{
}


//------------------------------------------------------------------------------
bool GDescription::LoadVectors(void)
{
	return(false);
}


//------------------------------------------------------------------------------
RCursor<GVector> GDescription::GetVectors(void) const
{
	if(!Vectors)
	{
		if(!const_cast<GDescription*>(this)->LoadVectors())
			const_cast<GDescription*>(this)->Vectors=new R::RContainer<GVector,true,true>(1);
	}
	return(RCursor<GVector>(*Vectors));
}


//------------------------------------------------------------------------------
const GVector* GDescription::GetVector(GConcept* concept) const
{
	if(!Vectors)
	{
		if(!const_cast<GDescription*>(this)->LoadVectors())
			const_cast<GDescription*>(this)->Vectors=new R::RContainer<GVector,true,true>(1);

	}
	return(Vectors->GetPtr(concept));
}


//------------------------------------------------------------------------------
GVector* GDescription::GetVector(GConcept* concept)
{
	if(!Vectors)
	{
		if(!const_cast<GDescription*>(this)->LoadVectors())
			const_cast<GDescription*>(this)->Vectors=new R::RContainer<GVector,true,true>(1);

	}
	return(Vectors->GetPtr(concept));
}


//------------------------------------------------------------------------------
GVector* GDescription::GetInsertVector(GConcept* concept)
{
	if(!Vectors)
	{
		if(!const_cast<GDescription*>(this)->LoadVectors())
			const_cast<GDescription*>(this)->Vectors=new R::RContainer<GVector,true,true>(1);
	}
	return(Vectors->GetInsertPtr(concept));
}


//------------------------------------------------------------------------------
bool GDescription::IsDefined(void) const
{
	RCursor<GVector> Vector(GetVectors());
	for(Vector.Start();!Vector.End();Vector.Next())
		if(Vector()->IsDefined())
			return(true);
	return(false);
}


//------------------------------------------------------------------------------
void GDescription::Copy(GConcept* concept,const R::RContainer<GConceptRef,false,true>& list)
{
	if(list.GetNb())
	{
		if(!Vectors)
			Vectors=new R::RContainer<GVector,true,true>(1);
		GVector* Vector(Vectors->GetInsertPtr(concept));
		Vector->Copy(list);
	}
	else
	{
		if(!Vectors)
			return;
		GVector* Vector(Vectors->GetPtr(concept));
		if(Vector)
			Vector->Clear();
	}
}


//------------------------------------------------------------------------------
void GDescription::Clear(void)
{
	if(Vectors)
		Vectors->Clear();
}


//------------------------------------------------------------------------------
void GDescription::Transfer(R::RContainer<GVector,true,true>& vectors)
{
	RCursor<GVector> Vector(vectors);
	for(Vector.Start();!Vector.End();Vector.Next())
	{
		GVector* MyVector(GetInsertVector(Vector()->GetConcept()));
		MyVector->Transfer(*Vector());
	}
}


//------------------------------------------------------------------------------
void GDescription::AddRefs(GSession* session,tObjType ObjType) const
{
	// Get the vector (if necessary)
	RCursor<GVector> Vector(GetVectors());
	if(!Vectors->GetNb()) return;

	// Init
	RVectorBool Types(session->GetNbConceptTypes()+1);
	Types.Init(session->GetNbConceptTypes()+1,true);

	// Parse the vectors
	for(Vector.Start();!Vector.End();Vector.Next())
	{
		RCursor<GConceptRef> ptr(Vector()->GetRefs());
		if(!ptr.GetNb())
			continue;

		GConceptType* type(Vector()->GetConcept()->GetType());
		size_t TypeId(type->GetId());
		if(Types[TypeId])
		{
			// Yes -> A new object uses this concept type.
			type->IncRef(ObjType);
			Types[TypeId]=false;
		}

			// IncRef for the concept
			type->IncRef(Vector()->GetConcept(),ObjType);

		for(ptr.Start();!ptr.End();ptr.Next())
		{
			GConceptType* type(ptr()->GetConcept()->GetType());
			size_t TypeId(type->GetId());
			if(Types[TypeId])
			{
				// Yes -> A new object uses this concept type.
				type->IncRef(ObjType);
				Types[TypeId]=false;
			}

			// IncRef for the concept
			type->IncRef(ptr()->GetConcept(),ObjType);
		}
	}
}


//------------------------------------------------------------------------------
void GDescription::DelRefs(GSession* session,tObjType ObjType) const
{
	// Get the vector (if necessary)
	RCursor<GVector> Vector(GetVectors());
	if(!Vectors->GetNb()) return;

	// Init
	RVectorBool Types(session->GetNbConceptTypes()+1);
	Types.Init(session->GetNbConceptTypes()+1,true);

	// Parse the vectors
	for(Vector.Start();!Vector.End();Vector.Next())
	{
		RCursor<GConceptRef> ptr(Vector()->GetRefs());
		if(!ptr.GetNb())
			continue;

		// Reference of the concept associated with the vector
		GConceptType* type(Vector()->GetConcept()->GetType());
		size_t TypeId(type->GetId());
		if(Types[TypeId])
		{
			// Yes -> An old object uses this concept type.
			type->DecRef(ObjType);
			Types[TypeId]=false;
		}

		// DecRef for the concept
		type->DecRef(Vector()->GetConcept(),ObjType);


		for(ptr.Start();!ptr.End();ptr.Next())
		{
			GConceptType* type(ptr()->GetConcept()->GetType());
			size_t TypeId(type->GetId());
			if(Types[TypeId])
			{
				// Yes -> An old object uses this concept type.
				type->DecRef(ObjType);
				Types[TypeId]=false;
			}

			// DecRef for the concept
			type->DecRef(ptr()->GetConcept(),ObjType);
		}
	}
}


//------------------------------------------------------------------------------
GDescription& GDescription::operator+=(const GDescription& desc)
{
	RCursor<GVector> Cur(desc.GetVectors());
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		GVector* Vector(GetInsertVector(Cur()->GetConcept()));
		(*Vector)+=(*Cur());
	}
	return(*this);
}


//------------------------------------------------------------------------------
GDescription& GDescription::operator-=(const GDescription& desc)
{
	RCursor<GVector> Cur(desc.GetVectors());
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		GVector* Vector(GetInsertVector(Cur()->GetConcept()));
		(*Vector)-=(*Cur());
	}
	return(*this);
}


//------------------------------------------------------------------------------
GDescription& GDescription::operator*=(const double nb)
{
	RCursor<GVector> Cur(GetVectors());
	for(Cur.Start();!Cur.End();Cur.Next())
		(*Cur())*=nb;
	return(*this);
}


//------------------------------------------------------------------------------
GDescription& GDescription::operator/=(const double nb)
{
	// Invalid maximum weights
	RCursor<GVector> Cur(GetVectors());
	for(Cur.Start();!Cur.End();Cur.Next())
		(*Cur())/=nb;
	return(*this);
}


//------------------------------------------------------------------------------
GDescription::~GDescription(void)
{
	delete Vectors;
}



//------------------------------------------------------------------------------
//
// General Functions and operators
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GDescription GALILEI::operator+(const GDescription &arg1,const GDescription &arg2)
{
	GDescription res(arg1);
	res+=arg2;
	return(res);
}


//------------------------------------------------------------------------------
GDescription GALILEI::operator-(const GDescription &arg1,const GDescription &arg2)
{
	GDescription res(arg1);
	res-=arg2;
	return(res);
}


//------------------------------------------------------------------------------
GDescription GALILEI::operator*(const double arg1,const GDescription &arg2)
{
	GDescription res(arg2);
	res*=arg1;
	return(res);
}


//------------------------------------------------------------------------------
GDescription GALILEI::operator*(const GDescription& arg1,const double arg2)
{
	GDescription res(arg1);
	res*=arg2;
	return(res);
}


//------------------------------------------------------------------------------
GDescription GALILEI::operator/(const GDescription& arg1,const double arg2)
{
	GDescription res(arg1);
	res/=arg2;
	return(res);
}

