/*

	GALILEI Research Project

	GWeightInfo.cpp

	Information entity representing a word associated with a weight - Implementation.

	Copyright 2002-2003 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).

	Version $Revision$

	Last Modify: $Date$

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
#include<infos/gweightinfo.h>
#include<infos/gdict.h>
using namespace GALILEI;
using namespace R;



//------------------------------------------------------------------------------
//
//  GWeightInfo
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GWeightInfo::GWeightInfo(unsigned id,GInfoType type) throw(bad_alloc)
  : GInfo(id,type), Weight(0.0)
{
}


//------------------------------------------------------------------------------
GWeightInfo::GWeightInfo(unsigned id,double w,GInfoType type) throw(bad_alloc)
  : GInfo(id,type), Weight(w)
{
}


//------------------------------------------------------------------------------
GWeightInfo::GWeightInfo(const GWeightInfo* w) throw(bad_alloc)
  : GInfo(w->Id,w->Type), Weight(w->Weight)
{
}


//------------------------------------------------------------------------------
int GWeightInfo::Compare(const GWeightInfo& calc) const
{
  return(Id-calc.Id);
}


//------------------------------------------------------------------------------
int GWeightInfo::Compare(const GWeightInfo* calc) const
{
  return(Id-calc->Id);
}


//------------------------------------------------------------------------------
double GWeightInfo::GetQueryWeight(tObjType ObjType,GDict* dict,double max) const
{
	return((Weight/max)*log(static_cast<double>(dict->GetRef(ObjType,Type))/static_cast<double>(dict->GetRef(Id,ObjType))));
}


//------------------------------------------------------------------------------
GWeightInfo::~GWeightInfo(void)
{
}
