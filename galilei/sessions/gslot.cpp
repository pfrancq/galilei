/*

	GALILEI Research Project

	GSlot.cpp

	Generic Slot for GALILEI Session - Implementation.

	Copyright 2002-2003 by the Université Libre de Bruxelles.

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
#include <sessions/gslot.h>
using namespace GALILEI;
using namespace R;



//------------------------------------------------------------------------------
//
//  class GSlot
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GSlot::GSlot(void) throw(std::bad_alloc,RException)
	: RPrgOutput()
{
}


//------------------------------------------------------------------------------
void GSlot::receiveNextDoc(const GDoc*) throw(std::bad_alloc,RException)
{
}


//------------------------------------------------------------------------------
void GSlot::receiveNextProfile(const GProfile*) throw(std::bad_alloc,RException)
{
}


//------------------------------------------------------------------------------
void GSlot::receiveNextProfileExport(const GProfile*) throw(std::bad_alloc,RException)
{
}


//------------------------------------------------------------------------------
void GSlot::receiveNextDocumentExport(const GDoc*) throw(std::bad_alloc,RException)
{
}


//------------------------------------------------------------------------------
void GSlot::receiveNextGroupExport(const GGroup*) throw(std::bad_alloc,RException)
{
}


//------------------------------------------------------------------------------
void GSlot::NextGroupLang(const GLang*) throw(std::bad_alloc,RException)
{
}


//------------------------------------------------------------------------------
void GSlot::receiveNextChromosome(unsigned int) throw(std::bad_alloc,RException)
{
}


//------------------------------------------------------------------------------
void GSlot::receiveNextMethod(unsigned int) throw(std::bad_alloc,RException)
{
}


//------------------------------------------------------------------------------
GSlot::~GSlot(void)
{
}
