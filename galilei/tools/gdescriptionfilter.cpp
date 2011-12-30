/*

	GALILEI Research Project

	GDescriptionFilter.cpp

	Description Set - Implementation.

	Copyright 2006-2011 by Pascal Francq (pascal@francq.info).
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



//-----------------------------------------------------------------------------
// include files for the R Project
#include <rdir.h>


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <gdescriptionfilter.h>
using namespace GALILEI;
using namespace R;
using namespace std;



//-----------------------------------------------------------------------------
//
// GDescriptionFilter
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GDescriptionFilter::GDescriptionFilter(void)
	: Order(0), MaxOrderSize(5000)
{
	Order=new const GConceptRef*[MaxOrderSize];
}


//-----------------------------------------------------------------------------
void GDescriptionFilter::CopyFilter(const GDescription& src,GDescription& dest,size_t lmax,size_t lmin)
{
	// Go through the vector of the source description
	RCursor<GVector> Vector(src.GetVectors());
	for(Vector.Start();!Vector.End();Vector.Next())
	{
		if(!Vector()->GetNb())
			continue;

		// Reallocate Order if necessary, copy the elements and order it
		if(Vector()->GetNb()+1>MaxOrderSize)
		{
			if(Order) delete[] Order;
			MaxOrderSize=static_cast<size_t>((static_cast<double>(Vector()->GetNb())+1)*1.1);
			Order=new const GConceptRef*[MaxOrderSize];
		}
		Vector()->GetTab(Order);
		if(Vector()->GetNb())
			qsort(static_cast<void*>(Order),Vector()->GetNb(),sizeof(GConceptRef*),GVector::SortOrder);
		Order[Vector()->GetNb()]=0;

		// Create the vector in the destination vector
		GVector* Ins(dest.GetInsertVector(Vector()->GetConcept()));

		// Compute the number of concepts to copy
		size_t nb,nb2;
		if(lmax)
		{
			nb=lmax;
			if(nb>Vector()->GetNb())
				nb=Vector()->GetNb();
			if(Vector()->GetNb()-nb)
			{
				nb2=lmin;
				if(nb2>Vector()->GetNb()-nb)
					nb2=Vector()->GetNb()-nb;
			}
			else
				nb2=0;
		}
		else
		{
			nb=Vector()->GetNb();
			nb2=0;
		}

		// Copy the most relevant concepts
		size_t i;
		const GConceptRef** ptr;
		for(i=nb+1,ptr=Order;--i;ptr++)
		{
			GConceptRef* Ref(Ins->GetRef((*ptr)->GetConcept()));
			(*Ref)+=(*ptr)->GetWeight();
		}

		// Copy the most irrelevant concepts (negative weights).
		for(i=nb2+1,ptr=&Order[Vector()->GetNb()-1];--i;ptr--)
		{
			GConceptRef* Ref(Ins->GetRef((*ptr)->GetConcept()));
			if((*ptr)->GetWeight()>=0)
				break;
			(*Ref)+=(*ptr)->GetWeight();
		}
	}
}


//------------------------------------------------------------------------------
void GDescriptionFilter::WriteFile(const RString& dir,size_t id,size_t nb,GConcept* concept)
{
	RString name(RString::Number(id)+"-"+RString::Number(concept->GetId())+".txt");
	RDir::CreateDirIfNecessary(dir);
	RTextFile Out(dir+RFile::GetDirSeparator()+name);
	Out.Open(RIO::Create);
	const GConceptRef** ptr;
	size_t i;
	for(i=nb+1,ptr=Order;--i;ptr++)
		Out<<(*ptr)->GetId()<<(*ptr)->GetType()<<(*ptr)->GetWeight()<<endl;
}


//-----------------------------------------------------------------------------
GDescriptionFilter::~GDescriptionFilter(void)
{
	if(Order) delete[] Order;
}