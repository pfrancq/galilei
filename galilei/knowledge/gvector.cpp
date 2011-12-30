/*

	GALILEI Research Project

	GVector.cpp

	Concept Reference Vector - Implementation.

	Copyright 2002-2011 by Pascal Francq (pascal@francq.info).
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
// include files for ANSI C/C++
#include <math.h>


//-----------------------------------------------------------------------------
// include files for R
#include <rvector.h>


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <gvector.h>
#include <glang.h>
#include <gconcepttype.h>
#include <gconcept.h>
#include <gsession.h>
using namespace GALILEI;
using namespace R;
using namespace std;


//------------------------------------------------------------------------------
const double Factor=0.5;



//------------------------------------------------------------------------------
//
//  GVector
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GVector::GVector(GConcept* concept)
	: RContainer<GConceptRef,true,true>(100,50), Concept(concept), MaxWeight(NAN),
	  MaxAbsWeight(NAN)
{
	if(!concept)
		ThrowGException("cannot create a vector with an invalid concept");
}


//------------------------------------------------------------------------------
GVector::GVector(GConcept* concept,size_t max)
	: RContainer<GConceptRef,true,true>(max,50), Concept(concept), MaxWeight(NAN),
	  MaxAbsWeight(NAN)
{
	if(!concept)
		ThrowGException("cannot create a vector with an invalid concept");
}


//------------------------------------------------------------------------------
GVector::GVector(const GVector& vector)
	: RContainer<GConceptRef,true,true>(vector), Concept(vector.Concept),
	  MaxWeight(vector.MaxWeight), MaxAbsWeight(vector.MaxAbsWeight)
{
}


//------------------------------------------------------------------------------
int GVector::SortOrder(const void* a,const void* b)
{
	double af=(*((GConceptRef**)(a)))->GetWeight();
	double bf=(*((GConceptRef**)(b)))->GetWeight();

	if(af==bf) return(0);
	if(af>bf)
		return(-1);
	else
		return(1);
}


//------------------------------------------------------------------------------
void GVector::Copy(const R::RContainer<GConceptRef,false,true>& vector)
{
	RContainer<GConceptRef,true,true>::operator=(vector);

	// Invalid maximum weights
	MaxWeight=NAN;
	MaxAbsWeight=NAN;
}


//------------------------------------------------------------------------------
int GVector::Compare(const GVector& vector) const
{
	return(CompareIds(Concept->Id,vector.Concept->Id));
}


//------------------------------------------------------------------------------
int GVector::Compare(const GConcept& concept) const
{
	return(CompareIds(Concept->Id,concept.Id));
}


//------------------------------------------------------------------------------
int GVector::Compare(const GConcept* concept) const
{
	if(!concept)
		ThrowGException("null pointer");
	return(CompareIds(Concept->Id,concept->Id));
}


//------------------------------------------------------------------------------
GConceptRef* GVector::GetRef(const GConceptRef* ref)
{
	if(!ref)
		ThrowGException("null pointer");
	return(GetInsertPtr(ref->GetConcept()));
}


//------------------------------------------------------------------------------
GConceptRef* GVector::GetRef(GConcept* concept)
{
	if(!concept)
		ThrowGException("null pointer");
	return(GetInsertPtr(concept));
}


//------------------------------------------------------------------------------
void GVector::InsertRef(GConceptRef* ref)
{
	if(!ref)
		ThrowGException("null pointer");

	// If NAN -> Recomputing is necessary
	if(MaxWeight!=MaxWeight)
	{
		MaxWeight=ref->GetWeight();
		MaxAbsWeight=fabs(ref->GetWeight());
	}
	else
	{
		if(ref->GetWeight()>MaxWeight)
			MaxWeight=ref->GetWeight();
		if(fabs(ref->GetWeight())>MaxAbsWeight)
			MaxAbsWeight=fabs(ref->GetWeight());
	}
	InsertPtr(ref);
}


//------------------------------------------------------------------------------
void GVector::DeleteRef(GConceptRef* ref)
{
	if(!ref)
		ThrowGException("null pointer");

	// Invalid maximum weights
	MaxWeight=NAN;
	MaxAbsWeight=NAN;

	DeletePtr(*ref);
}


//------------------------------------------------------------------------------
void GVector::DeleteRef(const GConcept* concept)
{
	if(!concept)
		ThrowGException("null pointer");

	// Invalid maximum weights
	MaxWeight=NAN;
	MaxAbsWeight=NAN;

	DeletePtr(*concept);
}


//------------------------------------------------------------------------------
void GVector::AnalyzeWeights(void) const
{
	// If no profile, maximal weight is null.
	if(!GetNb())
	{
		const_cast<GVector*>(this)->MaxWeight=0;
		const_cast<GVector*>(this)->MaxAbsWeight=0;
		return;
	}

	// Find the biggest weight
	const_cast<GVector*>(this)->MaxWeight=-numeric_limits<double>().max();
	const_cast<GVector*>(this)->MaxAbsWeight=-numeric_limits<double>().max();
	RCursor<GConceptRef> Infos(*this);
	for(Infos.Start();!Infos.End();Infos.Next())
	{
		if(Infos()->GetWeight()>MaxWeight)
			const_cast<GVector*>(this)->MaxWeight=Infos()->GetWeight();
		if(fabs(Infos()->GetWeight())>MaxAbsWeight)
			const_cast<GVector*>(this)->MaxAbsWeight=fabs(Infos()->GetWeight());
	}
}


//------------------------------------------------------------------------------
double GVector::GetMaxWeight(void) const
{
	// If NAN -> Recomputing is necessary
	if(MaxWeight!=MaxWeight)
		AnalyzeWeights();
	return(MaxWeight);
}


//------------------------------------------------------------------------------
double GVector::GetMaxAbsWeight(void) const
{
	// If NAN -> Recomputing is necessary
	if(MaxAbsWeight!=MaxAbsWeight)
		AnalyzeWeights();

	return(MaxAbsWeight);
}


//------------------------------------------------------------------------------
void GVector::Extract(GVector& vector,const GConceptType* type)
{
	// Empty destination
	vector.Clear();

	RCursor<GConceptRef> Entities(*this);
	for(Entities.Start();!Entities.End();Entities.Next())
	{
		if(Entities()->GetType()==type)
			vector.InsertPtr(new GConceptRef(*Entities()));
	}
}


//------------------------------------------------------------------------------
void GVector::ComputeTfIdf(tObjType ObjType)
{
	RCursor<GConceptRef> Infos(*this);
	for(Infos.Start();!Infos.End();Infos.Next())
	{
		double iff(Infos()->GetType()->GetRef(ObjType)/static_cast<double>(Infos()->GetConcept()->GetRef(ObjType)));
		Infos()->SetWeight((Infos()->GetWeight()/GetMaxAbsWeight())*log(iff));
	}
}


//------------------------------------------------------------------------------
void GVector::Print(R::RString msg)
{
	cout<<msg<<endl<<"\t(Type,Id)"<<endl;
	RCursor<GConceptRef> Cur(*this);
	for(Cur.Start();!Cur.End();Cur.Next())
		cout<<"\t("<<(int)Cur()->GetType()->GetId()<<","<<Cur()->GetId()<<")"<<endl;
	cout<<"OK : "<<msg<<endl;
}


//------------------------------------------------------------------------------
void GVector::Intersection(const GVector& vector)
{
	RContainer<GConceptRef,false,false> ToDel(20);
	RCursor<GConceptRef> Cur(*this);
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		// If in infos, add it -> else delete
		GConceptRef* Info(vector.GetPtr(*Cur()));
		if(!Info)
			ToDel.InsertPtr(Cur());
		else
			Cur()->Weight+=Info->Weight;
	}

	// Delete the necessary concept references
	Cur.Set(ToDel);
	for(Cur.Start();!Cur.End();Cur.Next())
		DeleteRef(Cur());
}


//------------------------------------------------------------------------------
GVector& GVector::operator=(const GVector& vector)
{
	Concept=vector.Concept;
	MaxWeight=vector.MaxWeight;
	MaxAbsWeight=vector.MaxAbsWeight;
	R::RContainer<GConceptRef,true,true>::Copy(vector);
	return(*this);
}


//------------------------------------------------------------------------------
GVector& GVector::operator+=(const GVector& vector)
{
	if(Concept!=vector.Concept)
		ThrowGException("Operation does work only with vectors associated with the same concept");

	// Invalid maximum weights
	MaxWeight=NAN;
	MaxAbsWeight=NAN;

	RCursor<GConceptRef> Cur(vector);
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		GConceptRef* Info(GetRef(Cur()));
		Info->Weight+=Cur()->Weight;
	}
	return(*this);
}


//------------------------------------------------------------------------------
GVector& GVector::operator-=(const GVector& vector)
{
	if(Concept!=vector.Concept)
		ThrowGException("Operation does work only with vectors associated with the same concept");


	// Invalid maximum weights
	MaxWeight=NAN;
	MaxAbsWeight=NAN;

	RCursor<GConceptRef> Cur(vector);
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		GConceptRef* Info(GetRef(Cur()));
		Info->Weight-=Cur()->Weight;
	}
	return(*this);
}


//------------------------------------------------------------------------------
GVector& GVector::operator*=(const double nb)
{
	// Invalid maximum weights
	MaxWeight=NAN;
	MaxAbsWeight=NAN;

	RCursor<GConceptRef> Cur(*this);
	for(Cur.Start();!Cur.End();Cur.Next())
		Cur()->Weight*=nb;
	return(*this);
}


//------------------------------------------------------------------------------
GVector& GVector::operator/=(const double nb)
{
	// Invalid maximum weights
	MaxWeight=NAN;
	MaxAbsWeight=NAN;

	RCursor<GConceptRef> Cur(*this);
	for(Cur.Start();!Cur.End();Cur.Next())
		Cur()->Weight/=nb;
	return(*this);
}


//------------------------------------------------------------------------------
GVector::~GVector(void)
{
}



//------------------------------------------------------------------------------
//
// General Functions and operators
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GVector GALILEI::operator+(const GVector &arg1,const GVector &arg2)
{
	if(arg1.GetConcept()!=arg2.GetConcept())
		ThrowGException("Operation does work only with vectors associated with the same concept");

	GVector res(arg1);
	res+=arg2;
	return(res);
}


//------------------------------------------------------------------------------
GVector GALILEI::operator-(const GVector &arg1,const GVector &arg2)
{
	if(arg1.GetConcept()!=arg2.GetConcept())
		ThrowGException("Operation does work only with vectors associated with the same concept");
	
	GVector res(arg1);
	res-=arg2;
	return(res);
}


//------------------------------------------------------------------------------
GVector GALILEI::operator*(const double arg1,const GVector &arg2)
{
	GVector res(arg2);
	res*=arg1;
	return(res);
}


//------------------------------------------------------------------------------
GVector GALILEI::operator*(const GVector& arg1,const double arg2)
{
	GVector res(arg1);
	res*=arg2;
	return(res);
}


//------------------------------------------------------------------------------
GVector GALILEI::operator/(const GVector& arg1,const double arg2)
{
	GVector res(arg1);
	res/=arg2;
	return(res);
}
