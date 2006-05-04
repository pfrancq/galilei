/*

	GALILEI Research Project

	GSlotLogLog.cpp

	Log File Slot - Implementation.

	Copyright 2003-2005 by the Université Libre de Bruxelles.

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
#include <gslotlog.h>
#include <gdoc.h>
#include <gprofile.h>
#include <glang.h>
using namespace GALILEI;
using namespace R;
using namespace std;



//------------------------------------------------------------------------------
//
//  class GSlotLog
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GSlotLog::GSlotLog(const char* name)
	: GSlot(), RTextFile(name)
{
	RTextFile::Open(RIO::Append);
}


//------------------------------------------------------------------------------
void GSlotLog::NextDoc(const GDoc* doc)
{
	sprintf(Buf,"Analyse Doc %u",doc->GetId());
	WriteLog(Buf);
}


//------------------------------------------------------------------------------
void GSlotLog::NextProfile(const GProfile* prof)
{
	sprintf(Buf,"Compute Profile %u",prof->GetId());
	WriteLog(Buf);
}


//------------------------------------------------------------------------------
void GSlotLog::NextGroupLang(const GLang* lang)
{
	sprintf(Buf,"Group %s Profiles",lang->GetName().Latin1());
	WriteLog(Buf);
}


//------------------------------------------------------------------------------
void GSlotLog::WriteStr(const RString&)
{
}


//------------------------------------------------------------------------------
void GSlotLog::NextChromosome(unsigned int)
{
}


//------------------------------------------------------------------------------
void GSlotLog::NextMethod(unsigned int)
{
}


//------------------------------------------------------------------------------
void GSlotLog::StartJob(const R::RString job)
{
	WriteLog(job);
}


//------------------------------------------------------------------------------
void GSlotLog::Warning(R::RString msg)
{
	WriteLog("Warning: "+msg);
}


//------------------------------------------------------------------------------
void GSlotLog::Error(R::RString msg)
{
	WriteLog("Error: "+msg);
}


//------------------------------------------------------------------------------
void GSlotLog::Alert(R::RString msg)
{
	WriteLog("Alert: "+msg);
}


//------------------------------------------------------------------------------
GSlotLog::~GSlotLog(void)
{
}
