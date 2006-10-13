/*

	GALILEI Research Project

	GWeightInfo.cpp

	Weighted information entity - Implementation.

	Copyright 2002-2003 by the Universit�Libre de Bruxelles.

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
#include <gconcept.h>
using namespace GALILEI;
using namespace R;



//------------------------------------------------------------------------------
//
//  GWeightInfo
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GWeightInfo::GWeightInfo(unsigned int id,unsigned int type,double w)
  : GInfo(id,type), Weight(w)
{
}


//------------------------------------------------------------------------------
GWeightInfo::GWeightInfo(const GWeightInfo& w)
  : GInfo(w), Weight(w.Weight)
{
}


//------------------------------------------------------------------------------
GWeightInfo::GWeightInfo(const GInfo& w)
  : GInfo(w), Weight(0.0)
{
}


//------------------------------------------------------------------------------
GWeightInfo::GWeightInfo(const GConcept& concept)
  : GInfo(concept), Weight(0.0)
{
}


//------------------------------------------------------------------------------
int GWeightInfo::Compare(const GWeightInfo& calc) const
{
	if(Type==calc.Type)
		return(Id-calc.Id);
	return(Type-calc.Type);
}


//------------------------------------------------------------------------------
int GWeightInfo::Compare(const GConcept& concept) const
{
	if(Type==concept.GetType())
		return(Id-concept.GetId());
	return(Type-concept.GetType());
}


//------------------------------------------------------------------------------
void GWeightInfo::SetWeight(double w)
{
	Weight=w;
}


//------------------------------------------------------------------------------
GWeightInfo& GWeightInfo::operator=(const GWeightInfo& i)
{
	GInfo::operator=(i);
	Weight=i.Weight;
	return(*this);
}


//------------------------------------------------------------------------------
GWeightInfo& GWeightInfo::operator=(const GInfo& i)
{
	GInfo::operator=(i);
	Weight=0.0;
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
double GWeightInfo::GetQueryWeight(tObjType ObjType,GLang* lang,double max) const
{
	return((Weight/max)*log(static_cast<double>(lang->GetRef(ObjType))/static_cast<double>(lang->GetRef(Id,Type,ObjType))));
}


//------------------------------------------------------------------------------
GWeightInfo::~GWeightInfo(void)
{
}
