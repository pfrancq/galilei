/*

	GALILEI Research Project

	GEvaluateGroupingMethods.cpp

	Compare a ideal groupement with a computed one - Implementation

	Copyright 2002 by the Université Libre de Bruxelles.

	Authors:
		David Wartel (dwartel@ulb.ac.be).

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
// include files for R Project
#include <rstd/rcursor.h>
using namespace R;


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <groups/gevaluategroupingmethods.h>
using namespace GALILEI;



//-----------------------------------------------------------------------------
//
//  GEvaluateGrouping
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GALILEI::GEvaluateGroupingMethods::GEvaluateGroupingMethods() throw(bad_alloc)
{
	Methods= new RContainer<GEvaluateGrouping,unsigned int,true,true>(10,5);
}



//-----------------------------------------------------------------------------
void GALILEI::GEvaluateGroupingMethods::RegisterEvaluationMethod(GEvaluateGrouping* method)
{
 	Methods->InsertPtr(method); 
}


//-----------------------------------------------------------------------------
GEvaluateGroupingCursor& GALILEI::GEvaluateGroupingMethods::GetEvaluateGroupingCursor(void)
{
	GEvaluateGroupingCursor *cur=GEvaluateGroupingCursor::GetTmpCursor();
	cur->Set(Methods);
	return(*cur);
}


//-----------------------------------------------------------------------------
GALILEI::GEvaluateGroupingMethods::~GEvaluateGroupingMethods(void)
{

}
