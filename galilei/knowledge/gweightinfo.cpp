/*

	GALILEI Research Project

	GWeightInfo.cpp

	Weighted information entity - Implementation.

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
// include files for GALILEI
#include <gweightinfo.h>
#include <glang.h>
#include <gsession.h>
using namespace GALILEI;
using namespace R;



//------------------------------------------------------------------------------
//
//  GWeightInfo
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GWeightInfo::GWeightInfo(GConcept* concept)
  : Concept(concept), Weight(0.0)
{
}


//------------------------------------------------------------------------------
GWeightInfo::GWeightInfo(GConcept* concept,double w)
  : Concept(concept), Weight(w)
{
}


//------------------------------------------------------------------------------
GWeightInfo::GWeightInfo(const GWeightInfo& w)
  : Concept(w.Concept), Weight(w.Weight)
{
}


//------------------------------------------------------------------------------
int GWeightInfo::Compare(const GWeightInfo& calc) const
{
	return(CompareIds(Concept->Id,calc.Concept->Id));
}


//------------------------------------------------------------------------------
int GWeightInfo::Compare(const GConcept& concept) const
{
	return(CompareIds(Concept->Id,concept.Id));
}


//------------------------------------------------------------------------------
int GWeightInfo::Compare(const GConcept* concept) const
{
	return(CompareIds(Concept->Id,concept->Id));
}


//------------------------------------------------------------------------------
void GWeightInfo::SetWeight(double w)
{
	Weight=w;
}


//------------------------------------------------------------------------------
GWeightInfo& GWeightInfo::operator=(const GWeightInfo& i)
{
	Concept=i.Concept;
	Weight=i.Weight;
	return(*this);
}


//------------------------------------------------------------------------------
GWeightInfo& GWeightInfo::operator+=(double w)
{
	Weight+=w;
	return(*this);
}


//------------------------------------------------------------------------------
GWeightInfo& GWeightInfo::operator-=(double w)
{
	Weight-=w;
	return(*this);
}


//------------------------------------------------------------------------------
GWeightInfo& GWeightInfo::operator*=(double w)
{
	Weight*=w;
	return(*this);
}


//------------------------------------------------------------------------------
GWeightInfo& GWeightInfo::operator/=(double w)
{
	Weight/=w;
	return(*this);
}


//------------------------------------------------------------------------------
double GWeightInfo::GetQueryWeight(tObjType ObjType,double max) const
{
	return((Weight/max)*Concept->GetIF(ObjType));
}


//------------------------------------------------------------------------------
GWeightInfo::~GWeightInfo(void)
{
}
