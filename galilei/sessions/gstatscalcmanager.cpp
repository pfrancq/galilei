/*

	GALILEI Research Project

	GStatsCalcManager.cpp

	Sstatistics Methods Manager   - Implementation.

	Copyright 2003 by the Universit�Libre de Bruxelles.

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
// include files for ANSI C/C++
#include <ctype.h>
#include <stdexcept>

//------------------------------------------------------------------------------
// include files for GALILEI
#include <sessions/gstatscalcmanager.h>
#include <sessions/gstatscalc.h>
#include <sessions/gsession.h>
using namespace GALILEI;
using namespace R;




//------------------------------------------------------------------------------
//
// class GStatsCalcManager
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GStatsCalcManager::GStatsCalcManager(RContainer<RString, true, false>* paths,bool dlg) throw(std::bad_alloc,GException)
	: RContainer<GFactoryStatsCalc,true,true>(10,5),GPluginManager("StatsCalc",paths)
{
	for(paths->Start(); !paths->End(); paths->Next())
	{
		RString Path((*paths)());
		Path+="/stats";
		LoadPlugins<GFactoryStatsCalc,GFactoryStatsCalcInit,GStatsCalcManager>(this,Path.Latin1(),API_STATSCALC_VERSION, dlg);
	}
}


//------------------------------------------------------------------------------
void GStatsCalcManager::Connect(GSession* session) throw(GException)
{
	R::RCursor<GFactoryStatsCalc> Cur;
	GStatsCalc* calc;

	Cur.Set(this);
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		calc=Cur()->GetPlugin();
		if(calc)
			calc->Connect(session);
	}
}


//------------------------------------------------------------------------------
void GStatsCalcManager::Disconnect(GSession* session) throw(GException)
{
	R::RCursor<GFactoryStatsCalc> Cur;
	GStatsCalc* calc;

	Cur.Set(this);
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		calc=Cur()->GetPlugin();
		if(calc)
			calc->Disconnect(session);
	}
}


//------------------------------------------------------------------------------
GStatsCalc* GStatsCalcManager::Get(const char* name)
{
	GFactoryStatsCalc* fac;

	fac=GetPtr<const char*>(name);
	if(fac)
		return(fac->GetPlugin());
	return(0);
}


//------------------------------------------------------------------------------
R::RCursor<GFactoryStatsCalc> GStatsCalcManager::GetStatsCalcsCursor(void)
{
	R::RCursor<GFactoryStatsCalc> cur(this);
	return(cur);
}


//------------------------------------------------------------------------------
void GStatsCalcManager::ReadConfig(RXMLTag* t)
{
	R::RCursor<GFactoryStatsCalc> Cur;
	
	if(!t) return;
	Cur=GetStatsCalcsCursor();
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		Cur()->ReadConfig(t);
	}
}


//------------------------------------------------------------------------------
void GStatsCalcManager::SaveConfig(RXMLStruct* xml,RXMLTag* t)
{
	R::RCursor<GFactoryStatsCalc> Cur;
	Cur=GetStatsCalcsCursor();
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		Cur()->SaveConfig(xml,t);
	}
}


//------------------------------------------------------------------------------
GStatsCalcManager::~GStatsCalcManager(void)
{
}
