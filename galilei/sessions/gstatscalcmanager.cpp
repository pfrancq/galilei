/*

	R Project Library

	GStatsCalcManager.cpp

	Manager to handle statistics  - Implementation.

	Copyright 2001 by the Université Libre de Bruxelles.

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



//-----------------------------------------------------------------------------
// include files for ANSI C/C++
#include <ctype.h>
#include <stdexcept>
#include <dirent.h>


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <sessions/gstatscalcmanager.h>
#include <sessions/gstatscalc.h>
#include <sessions/gsession.h>
using namespace GALILEI;
using namespace R;
using namespace ltmm;



//-----------------------------------------------------------------------------
//
// class GStatsCalcManager
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GStatsCalcManager::GStatsCalcManager(const char* path) throw(GException)
	: RContainer<GFactoryStatsCalc,unsigned int,true,true>(10,5)
{
	DIR* dp;
	struct dirent* ep;
	RString Path(path);
	RString Msg;

	loader<>& l=loader<>::instance();
	Path+="/stats";
	l.addto_search_path(Path());
	dp=opendir(Path);
	if(!dp) return;
	while((ep=readdir(dp)))
	{
		if(strcmp(&ep->d_name[strlen(ep->d_name)-3],".la")) continue;
		try
		{
			handle<>& myhandle = l.load(ep->d_name);
			symbol* myinit   = myhandle.find_symbol("FactoryCreate");
			GFactoryStatsCalc* myfactory = ((GFactoryStatsCalcInit)(*(*myinit)))(this,ep->d_name);
			if(strcmp(API_STATSCALC_VERSION,myfactory->GetAPIVersion()))
			{
				Msg+=ep->d_name;
				Msg+=" - Plugin not compatible with API Version\n";
				continue;
			}
			InsertPtr(myfactory);
		}
		catch(std::exception& e)
		{
			Msg+=ep->d_name;
			Msg+=" - ";
			Msg+=e.what();
			Msg+="\n";
		}
	}
	closedir(dp);

	// If something in Msg -> error
	if(Msg.GetLen())
		throw(GException(Msg));
}


//-----------------------------------------------------------------------------
void GStatsCalcManager::Connect(GSession* session)
{
	GFactoryStatsCalcCursor Cur;
	GStatsCalc* calc;

	Cur.Set(this);
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		calc=Cur()->GetPlugin();
		if(calc)
			calc->Connect(session);
	}
}


//-----------------------------------------------------------------------------
void GStatsCalcManager::Disconnect(GSession* session)
{
	GFactoryStatsCalcCursor Cur;
	GStatsCalc* calc;

	Cur.Set(this);
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		calc=Cur()->GetPlugin();
		if(calc)
			calc->Connect(session);
	}
}


//-----------------------------------------------------------------------------
GStatsCalc* GStatsCalcManager::Get(const char* name)
{
	GFactoryStatsCalc* fac;

	fac=GetPtr<const char*>(name);
	if(fac)
		return(fac->GetPlugin());
	return(0);
}


//-----------------------------------------------------------------------------
GFactoryStatsCalcCursor& GStatsCalcManager::GetStatsCalcsCursor(void)
{
	GFactoryStatsCalcCursor *cur=GFactoryStatsCalcCursor::GetTmpCursor();
	cur->Set(this);
	return(*cur);
}


//-----------------------------------------------------------------------------
GStatsCalcManager::~GStatsCalcManager(void)
{
}
