/*

	GALILEI Research Project

	GIWordWeight.cpp

	Weight of a word - Implementation.

	Copyright 2002 by the Universit� Libre de Bruxelles.

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



//-----------------------------------------------------------------------------
// include files for ANSI C/C++
#include<math.h>


//-----------------------------------------------------------------------------
// include files for GALILEI
#include<infos/giwordweight.h>
#include<langs/gdict.h>
using namespace GALILEI;
using namespace R;



//-----------------------------------------------------------------------------
//
//  GIWordWeight
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GALILEI::GIWordWeight::GIWordWeight(unsigned id) throw(bad_alloc)
  : GIWord(id), Weight(0.0)
{
	Type=infoWordCalc;
}


//-----------------------------------------------------------------------------
GALILEI::GIWordWeight::GIWordWeight(unsigned id,double w) throw(bad_alloc)
  : GIWord(id), Weight(w)
{
	Type=infoWordCalc;
}

//-----------------------------------------------------------------------------
GALILEI::GIWordWeight::GIWordWeight(unsigned id,double w,GInfoType type) throw(bad_alloc)
  : GIWord(id), Weight(w)
{
	Type=type;
}

//-----------------------------------------------------------------------------
GALILEI::GIWordWeight::GIWordWeight(const GIWordWeight* w) throw(bad_alloc)
  : GIWord(w->Id), Weight(w->Weight)
{
	Type=w->InfoType();
}


//-----------------------------------------------------------------------------
const RString GALILEI::GIWordWeight::ClassName(void) const
{
	return("GIWordWeight");
}


//-----------------------------------------------------------------------------
const GInfoType GALILEI::GIWordWeight::InfoType(void) const
{
	return(Type);
}


//-----------------------------------------------------------------------------
int GALILEI::GIWordWeight::Compare(const GIWordWeight &calc) const
{
  return(Id-calc.Id);
}


//-----------------------------------------------------------------------------
int GALILEI::GIWordWeight::Compare(const GIWordWeight *calc) const
{
  return(Id-calc->Id);
}


//-----------------------------------------------------------------------------
double GALILEI::GIWordWeight::GetQueryWeight(tObjType ObjType,GDict* dict,double max) const
{
	if(Type==4) return(Weight/max)*log(static_cast<double>(dict->GetRef(ObjType,tWordList))/static_cast<double>(dict->GetRef(Id,ObjType)));
	else return(Weight/max)*log(static_cast<double>(dict->GetRef(ObjType,tWord))/static_cast<double>(dict->GetRef(Id,ObjType)));
}


//-----------------------------------------------------------------------------
GALILEI::GIWordWeight::~GIWordWeight(void)
{
}
