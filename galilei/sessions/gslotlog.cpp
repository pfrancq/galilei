/*

	GALILEI Research Project

	GSlotLogLog.cpp

	Log File Slot - Implementation.

	Copyright 2003-2010 by Pascal Francq (pascal@francq.info).
	Copyright 2003-2008 by the Université Libre de Bruxelles (ULB).

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
#include <guser.h>
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
	WriteLog("Analyze "+doc->GetURL()()+" ("+RString::Number(doc->GetId())+")");
}


//------------------------------------------------------------------------------
void GSlotLog::NextProfile(const GProfile* prof)
{
	WriteLog("Compute "+(prof->GetUser()->GetName()+"/"+prof->GetName())+"("+RString::Number(prof->GetId())+")");
}


//------------------------------------------------------------------------------
void GSlotLog::WriteStr(const RString& str)
{
	WriteLog(str.Latin1());
}


//------------------------------------------------------------------------------
void GSlotLog::StartJob(const R::RString job)
{
	WriteLog(job);
}


//------------------------------------------------------------------------------
void GSlotLog::Warning(const R::RString& msg)
{
	WriteLog("Warning: "+msg);
}


//------------------------------------------------------------------------------
void GSlotLog::Error(const R::RString& msg)
{
	WriteLog("Error: "+msg);
}


//------------------------------------------------------------------------------
void GSlotLog::Alert(const R::RString& msg)
{
	WriteLog("Alert: "+msg);
}


//------------------------------------------------------------------------------
GSlotLog::~GSlotLog(void)
{
}
