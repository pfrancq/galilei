/*

	GALILEI Research Project

	PointOfView.cpp

	Compare a ideal groupement with a computed one - Implementation

	Copyright 2002 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).
		Stphane Rideau (srideau@ulb.ac.be).

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



// ----------------------------------------------------------------------------
//include file for GALILEI
#include <postgroups/ginoutputbase.h>
#include <postgroups/gstandartinoutput.h>
using namespace GALILEI;


//-----------------------------------------------------------------------------
// include files for R Project
#include <rstd/rcontainer.h>
#include <rstd/rcursor.h>
using namespace R;



//-----------------------------------------------------------------------------
//
// class GInOutputBase
//
//-----------------------------------------------------------------------------


// ----------------------------------------------------------------------------
GALILEI::GInOutputBase::GInOutputBase()
{
	StdInOutPut = new RContainer<GStandardInOutPut,unsigned int,false,true>(10,5);   //**** debug
}


// ----------------------------------------------------------------------------
GStandardInOutPutCursor& GALILEI::GInOutputBase::GetGStandardInOutPutCursor()
{
	GStandardInOutPutCursor* cur=GStandardInOutPutCursor::GetTmpCursor();
	cur->Set(StdInOutPut);
	return(*cur);
}


// ----------------------------------------------------------------------------
int GALILEI::GInOutputBase::GetKwds(void)
{
//	GStandardInOutPutCursor Outpt = GetGStandardInOutPutCursor();
//	if(Outpt->NbPtr == 0)
//		return NULL;
//	return(StdInOutPut);
	return(0);
}


// ----------------------------------------------------------------------------
int GALILEI::GInOutputBase::FormatOutBase(void)
{

	GStandardInOutPutCursor Outpt = GetGStandardInOutPutCursor();

	for(Outpt.Start(); !Outpt.End(); Outpt.Next())
	{
		// save datas   ;
	}
//	GAlgobase *final;
//
//	StdOutPut = new RContainer<StandardOutPut,unsigned int,false,true>(10,5);   //**** debug
//	final = new GAlgobase();
//	final->FormatOutData();

	// Save final->dataOut
	return 1;  // dat a accessible coherente
}




