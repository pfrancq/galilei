/*

	GALILEI Research Project

	GGroupCalcGravitation.cpp

	Group Description is Gravitational Point Computing Method - Implementation.

	(C) 2002 by P. Francq.

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
#include <groups/ggroupcalcgravitation.h>
#include <groups/ggroup.h>
#include <sessions/gsession.h>
using namespace GALILEI;



//-----------------------------------------------------------------------------
//
//  class GGroupCalcGravitation
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GALILEI::GGroupCalcGravitation::GGroupCalcGravitation(GSession* session) throw(bad_alloc)
	: GGroupCalc("Gravitational Point",session)
{
}


//-----------------------------------------------------------------------------
void GALILEI::GGroupCalcGravitation::Compute(GGroup* /*grp*/)
{
}


//-----------------------------------------------------------------------------
const char* GALILEI::GGroupCalcGravitation::GetSettings(void)
{
	return("");
}


//-----------------------------------------------------------------------------
void GALILEI::GGroupCalcGravitation::SetSettings(const char*)
{
}


//-----------------------------------------------------------------------------
GALILEI::GGroupCalcGravitation::~GGroupCalcGravitation(void)
{
}
