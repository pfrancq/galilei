/*

	GALILEI Research Project

	PointOfView.h

	Document representing a GALILEI session - Header.

	Copyright 2001 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).
    Stephane Rideau (srideau@ulb.ac.be).

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
#ifndef GInOutputBaseH
#define GInOutputBaseH

//-----------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>
#include <groups/ggroup.h>
#include <sessions/gsession.h>
#include <sessions/gsessionmysql.h>
#include <infos/giwordweight.h>
#include <infos/giwordsweights.h>
#include <postgroups/gstandartinoutput.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------



// ---------------------------------------------------------------------------------------
// class format the outputs
//
// ---------------------------------------------------------------------------------------
class GInOutputBase
{

public :

	/**
	* constructor
	*/
	GInOutputBase();

	/**
	* build the standard outputs
	*/
	int FormatOutBase();

	/**
	* show the point of view in the window
	*/
	int GetKwds(void);

	/**
	* RContainer of standard outputs
	*/
	R::RContainer<GStandardInOutPut,unsigned,false,true>* StdInOutPut;

	/**
	* Cursor on the Rcontainer that contains the outputs of the ExpertKwds
	*/
	GStandardInOutPutCursor& GetGStandardInOutPutCursor();
};



}//-------- End of namespace GALILEI----------------------------------------


#endif



