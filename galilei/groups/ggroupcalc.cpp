/*

	GALILEI Research Project

	GGroupCalc.cpp

	Generic Group Description Computing Method - Implementation.

	(C) 2001 by P. Francq.

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
#include <groups/ggroupcalc.h>
#include <sessions/gsession.h>
using namespace GALILEI;



//-----------------------------------------------------------------------------
//
//  GGroupCalc
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GALILEI::GGroupCalc::GGroupCalc(const char* name,GSession* session) throw(bad_alloc)
	: ComputingName(name), Session(session)
{
}


//-----------------------------------------------------------------------------
int GALILEI::GGroupCalc::Compare(const GGroupCalc& desc) const
{
	return(ComputingName.Compare(desc.ComputingName));
}


//-----------------------------------------------------------------------------
int GALILEI::GGroupCalc::Compare(const GGroupCalc* desc) const
{
	return(ComputingName.Compare(desc->ComputingName));
}


//-----------------------------------------------------------------------------
int GALILEI::GGroupCalc::Compare(const char* name) const
{
	return(ComputingName.Compare(name));
}


//-----------------------------------------------------------------------------
GALILEI::GGroupCalc::~GGroupCalc(void)
{
}
