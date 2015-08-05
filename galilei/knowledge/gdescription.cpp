/*

	GALILEI Research Project

	GDescription.hh

	Description - Implementation.

	Copyright 2009-2015 by Pascal Francq (pascal@francq.info).

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
#include <gdescription.h>
#include <gdescriptionset.h>
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
GDescription::GDescription(const GDescription& src)
	: Vectors(0)
{
	if(!src.Vectors)
		return;
	Vectors=new R::RContainer<GVector,true,true>(src.Vectors->GetNb());
	RCursor<GVector> Vector(*src.Vectors);
	for(Vector.Start();!Vector.End();Vector.Next())
	{
		GVector* MyVector(GetInsertVector(Vector()->GetMetaConcept()));
		(*MyVector)=(*Vector());
	}
}


//------------------------------------------------------------------------------
bool GDescription::LoadVectors(void)
{
	return(false);
}


//------------------------------------------------------------------------------
void GDescription::ReleaseVectors(void)
{
	if(Vectors)
	{
		delete Vectors;
		Vectors=0;
	}
}


//------------------------------------------------------------------------------
RConstCursor<GVector> GDescription::GetVectors(void) const
{
	if(!Vectors)
	{
		if(!const_cast<GDescription*>(this)->LoadVectors())
			const_cast<GDescription*>(this)->Vectors=new R::RContainer<GVector,true,true>(5,20);
	}
	return(RConstCursor<GVector>(*Vectors));
}


//------------------------------------------------------------------------------
RCursor<GVector> GDescription::GetVectors(void)
{
	if(!Vectors)
	{
		if(!const_cast<GDescription*>(this)->LoadVectors())
			const_cast<GDescription*>(this)->Vectors=new R::RContainer<GVector,true,true>(5,20);
	}
	return(RCursor<GVector>(*Vectors));
}


//------------------------------------------------------------------------------
const GVector* GDescription::GetVector(GConcept* metaconcept) const
{
	if(!Vectors)
	{
		if(!const_cast<GDescription*>(this)->LoadVectors())
			const_cast<GDescription*>(this)->Vectors=new R::RContainer<GVector,true,true>(5,20);

	}
	return(Vectors->GetPtr(metaconcept));
}


//------------------------------------------------------------------------------
GVector* GDescription::GetVector(GConcept* metaconcept)
{
	if(!Vectors)
	{
		if(!const_cast<GDescription*>(this)->LoadVectors())
			const_cast<GDescription*>(this)->Vectors=new R::RContainer<GVector,true,true>(5,20);

	}
	return(Vectors->GetPtr(metaconcept));
}


//------------------------------------------------------------------------------
GVector* GDescription::GetInsertVector(GConcept* metaconcept)
{
	if(!Vectors)
	{
		if(!const_cast<GDescription*>(this)->LoadVectors())
			const_cast<GDescription*>(this)->Vectors=new R::RContainer<GVector,true,true>(5,20);
	}
	return(Vectors->GetInsertPtr(metaconcept));
}


//------------------------------------------------------------------------------
void GDescription::DeleteVector(GConcept* metaconcept)
{
	if(!Vectors)
	{
		if(!const_cast<GDescription*>(this)->LoadVectors())
			const_cast<GDescription*>(this)->Vectors=new R::RContainer<GVector,true,true>(5,20);
	}
	Vectors->DeletePtr(metaconcept);
}


//------------------------------------------------------------------------------
bool GDescription::IsDefined(void) const
{
	RConstCursor<GVector> Vector(GetVectors());
	for(Vector.Start();!Vector.End();Vector.Next())
		if(Vector()->IsDefined())
			return(true);
	return(false);
}


//------------------------------------------------------------------------------
bool GDescription::IsIn(const GConcept* concept) const
{
	RConstCursor<GVector> Vector(GetVectors());
	for(Vector.Start();!Vector.End();Vector.Next())
		if(Vector()->IsIn(concept))
			return(true);
	return(false);
}


//------------------------------------------------------------------------------
void GDescription::Copy(GConcept* metaconcept,const R::RContainer<GConceptRef,false,true>& list)
{
	if(list.GetNb())
	{
		if(!Vectors)
			Vectors=new R::RContainer<GVector,true,true>(1);
		GVector* Vector(Vectors->GetInsertPtr(metaconcept));
		Vector->Copy(list);
	}
	else
	{
		if(!Vectors)
			return;
		GVector* Vector(Vectors->GetPtr(metaconcept));
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
void GDescription::Normalize(void)
{
	RCursor<GVector> Vector(GetVectors());
	for(Vector.Start();!Vector.End();Vector.Next())
	{
		(*Vector())/=Vector()->GetMaxAbsWeight();
	}
}


//------------------------------------------------------------------------------
GDescription& GDescription::operator=(const GDescription& desc)
{
	if(Vectors)
	{
		Vectors->Clear();
		if(!desc.Vectors)
			return(*this);
	}
	else
	{
		if(!desc.Vectors)
			return(*this);
		Vectors=new R::RContainer<GVector,true,true>(desc.Vectors->GetNb());
	}
	RConstCursor<GVector> Vector(*desc.Vectors);
	for(Vector.Start();!Vector.End();Vector.Next())
	{
		GVector* MyVector(GetInsertVector(Vector()->GetMetaConcept()));
		(*MyVector)=(*Vector());
	}
	return(*this);
}


//------------------------------------------------------------------------------
GDescription& GDescription::operator+=(const GDescription& desc)
{
	RConstCursor<GVector> Cur(desc.GetVectors());
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		GVector* Vector(GetInsertVector(Cur()->GetMetaConcept()));
		(*Vector)+=(*Cur());
	}
	return(*this);
}


//------------------------------------------------------------------------------
GDescription& GDescription::operator-=(const GDescription& desc)
{
	RConstCursor<GVector> Cur(desc.GetVectors());
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		GVector* Vector(GetInsertVector(Cur()->GetMetaConcept()));
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

