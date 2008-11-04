/*

	GALILEI Research Project

	GWeightInfo.cpp

	Weighted information entity - Implementation.

	Copyright 2002-2008 by the Université Libre de Bruxelles.

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



//------------------------------------------------------------------------------
// include files for ANSI C/C++
#include<math.h>


//------------------------------------------------------------------------------
// include files for GALILEI
#include <gweightinfo.h>
#include <glang.h>
using namespace GALILEI;
using namespace R;



//------------------------------------------------------------------------------
//
//  GWeightInfo
//
//------------------------------------------------------------------------------

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
	if(Concept->Type==calc.Concept->Type)
		return(CompareIds(Concept->Id,calc.Concept->Id));
	return(Concept->Type->Id-calc.Concept->Type->Id);
}


//------------------------------------------------------------------------------
int GWeightInfo::Compare(const GConcept& concept) const
{
	if(Concept->Type==concept.Type)
		return(CompareIds(Concept->Id,concept.Id));
	return(Concept->Type->Id-concept.Type->Id);
}


//------------------------------------------------------------------------------
int GWeightInfo::Compare(const GConcept* concept) const
{
	if(Concept->Type==concept->Type)
		return(CompareIds(Concept->Id,concept->Id));
	return(Concept->Type->Id-concept->Type->Id);
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
bool GWeightInfo::operator==(const GWeightInfo &other) const
{
	if(Concept->Type==other.Concept->Type)
		return(Concept->Id==other.Concept->Id);
	return(false);
}


//------------------------------------------------------------------------------
bool GWeightInfo::operator!=(const GWeightInfo &other) const
{
	if(Concept->Type==other.Concept->Type)
		return(Concept->Id!=other.Concept->Id);
	return(true);

}


//------------------------------------------------------------------------------
bool GWeightInfo::operator<(const GWeightInfo &other) const
{
	if(Concept->Type==other.Concept->Type)
		return(Concept->Id<other.Concept->Id);
	return(Concept->Type->Id<other.Concept->Type->Id);

}


//------------------------------------------------------------------------------
bool GWeightInfo::operator>(const GWeightInfo &other) const
{
	if(Concept->Type==other.Concept->Type)
		return(Concept->Id>other.Concept->Id);
	return(Concept->Type->Id>other.Concept->Type->Id);

}


//------------------------------------------------------------------------------
double GWeightInfo::GetQueryWeight(tObjType ObjType,double max) const
{
	return((Weight/max)*log10(static_cast<double>(Concept->Type->GetRef(ObjType))/static_cast<double>(Concept->Type->GetRef(Concept->Id,ObjType))));
}


//------------------------------------------------------------------------------
GWeightInfo::~GWeightInfo(void)
{
}
