/*

	GALILEI Research Project

	GSlotLogLog.cpp

	Log File Slot - Implementation.

	Copyright 2003 by the Université Libre de Bruxelles.

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
// include files for GALILEI
#include <sessions/gslotlog.h>
#include <docs/gdoc.h>
#include <profiles/gprofile.h>
#include <infos/glang.h>
using namespace GALILEI;
using namespace R;



//------------------------------------------------------------------------------
//
//  class GSlotLog
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GALILEI::GSlotLog::GSlotLog(const char* name) throw(bad_alloc,RException)
	: GSlot(), RTextFile(name,Append)
{
}


//------------------------------------------------------------------------------
void GALILEI::GSlotLog::receiveNextDoc(const GDoc* doc) throw(bad_alloc,RException)
{
	sprintf(Buf,"Analyse Doc %u",doc->GetId());
	WriteLog(Buf);
}


//------------------------------------------------------------------------------
void GALILEI::GSlotLog::receiveNextProfile(const GProfile* prof) throw(bad_alloc,RException)
{
	sprintf(Buf,"Compute Profile %u",prof->GetId());
	WriteLog(Buf);
}


//------------------------------------------------------------------------------
void GALILEI::GSlotLog::NextGroupLang(const GLang* lang) throw(bad_alloc,RException)
{
	sprintf(Buf,"Group %s Profiles",lang->GetName());
	WriteLog(Buf);
}


//------------------------------------------------------------------------------
void GALILEI::GSlotLog::WriteStr(const char*) throw(bad_alloc,RException)
{
}


//------------------------------------------------------------------------------
void GALILEI::GSlotLog::receiveNextChromosome(unsigned int) throw(bad_alloc,RException)
{
}


//------------------------------------------------------------------------------
void GALILEI::GSlotLog::receiveNextMethod(unsigned int) throw(bad_alloc,RException)
{
}


//------------------------------------------------------------------------------
GALILEI::GSlotLog::~GSlotLog(void)
{
}
