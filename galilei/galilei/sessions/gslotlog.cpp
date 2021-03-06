/*

	GALILEI Research Project

	GSlotLogLog.cpp

	Log File Slot - Implementation.

	Copyright 2003-2015 by Pascal Francq (pascal@francq.info).
	Copyright 2003-2008 by the Universit√© Libre de Bruxelles (ULB).

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
GSlotLog::GSlotLog(const RURI& uri)
	: GSlot(), RTextFile(uri,"utf-8")
{
	RTextFile::Open(RIO::Append);
}


//------------------------------------------------------------------------------
void GSlotLog::NextDoc(const GDoc* doc)
{
	WriteStr("Analyze "+doc->GetName()+" ("+RString::Number(doc->GetId())+")");
}


//------------------------------------------------------------------------------
void GSlotLog::NextProfile(const GProfile* prof)
{
	WriteStr("Compute "+(prof->GetUser()->GetName()+"/"+prof->GetName())+"("+RString::Number(prof->GetId())+")");
}


//------------------------------------------------------------------------------
void GSlotLog::WriteStr(const RString& str)
{
	mLog.Lock();
	WriteLog(str,true);
	mLog.UnLock();
}


//------------------------------------------------------------------------------
void GSlotLog::StartJob(const R::RString& job)
{
	WriteStr(job);
}

//------------------------------------------------------------------------------
void GSlotLog::EndJob(const R::RString& msg)
{
	if(msg!=RString::Null)
		WriteStr(msg);
}


//------------------------------------------------------------------------------
void GSlotLog::Warning(const R::RString& msg)
{
	WriteStr("Warning: "+msg);
}


//------------------------------------------------------------------------------
void GSlotLog::Error(const R::RString& msg)
{
	WriteStr("Error: "+msg);
}


//------------------------------------------------------------------------------
void GSlotLog::Alert(const R::RString& msg)
{
	WriteStr("Alert: "+msg);
}


//------------------------------------------------------------------------------
GSlotLog::~GSlotLog(void)
{
}
