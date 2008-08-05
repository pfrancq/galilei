/*

	GALILEI Research Project

	GSlot.cpp

	Generic Slot for GALILEI Session - Implementation.

	Copyright 2002-2005 by the Université Libre de Bruxelles.

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
// include files for GALILEI
#include <gslot.h>
using namespace GALILEI;
using namespace R;



//------------------------------------------------------------------------------
//
//  class GSlot
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GSlot::GSlot(void)
	: RPrgOutput()
{
}


//------------------------------------------------------------------------------
void GSlot::NextDoc(const GDoc*)
{
}


//------------------------------------------------------------------------------
void GSlot::NextProfile(const GProfile*)
{
}


//------------------------------------------------------------------------------
void GSlot::WriteStr(const RString&)
{
}


//------------------------------------------------------------------------------
void GSlot::StartJob(const R::RString)
{
}


//------------------------------------------------------------------------------
void GSlot::EndJob(void)
{
}


//------------------------------------------------------------------------------
void GSlot::Interact(void)
{
}


//------------------------------------------------------------------------------
void GSlot::Warning(R::RString)
{
}


//------------------------------------------------------------------------------
void GSlot::Error(R::RString)
{
}


//------------------------------------------------------------------------------
void GSlot::Alert(R::RString)
{
}


//------------------------------------------------------------------------------
GSlot::~GSlot(void)
{
}
