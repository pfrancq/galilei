/*

	GALILEI Research Project

	GEvaluateGrouping.cpp

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
//include files for GALILEI
#include<groups/gevaluategrouping.h>
#include<groups/ggroup.h>
#include<groups/ggroups.h>
#include<profiles/gsubprofile.h>
#include<sessions/gsession.h>
using namespace GALILEI;
using namespace R;



//-----------------------------------------------------------------------------
//
//  GEvaluateGrouping
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GALILEI::GEvaluateGrouping::GEvaluateGrouping(RString name, GSession* s,RContainer<GGroupsEvaluate,unsigned int,false,false>* groups, unsigned int i) throw(bad_alloc)
	: Name(name), Id(i), Session(s), Groups(groups)
{
}


//-----------------------------------------------------------------------------
GALILEI::GEvaluateGrouping::~GEvaluateGrouping(void)
{

}
