/*

	GALILEI Research Project

	GGroupingManager.cpp

	Grouping Methods Manager - Implementation.

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
#include <groups/ggroupingmanager.h>
#include <groups/ggrouping.h>
using namespace GALILEI;
using namespace R;


//------------------------------------------------------------------------------
//
// class GGroupingManager
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GGroupingManager::GGroupingManager(const char* path,bool dlg) throw(std::bad_alloc,GException)
	: RContainer<GFactoryGrouping,true,true>(10,5), GPluginManager("Grouping",path),Current(0)
{
	RString Path(path);
	Path+="/grouping";

	LoadPlugins<GFactoryGrouping,GFactoryGroupingInit,GGroupingManager>(this,Path.Latin1(),API_GROUPING_VERSION, dlg);

}


//------------------------------------------------------------------------------
void GGroupingManager::Connect(GSession* session) throw(GException)
{
	R::RCursor<GFactoryGrouping> Cur;
	GGrouping* calc;

	Cur.Set(this);
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		calc=Cur()->GetPlugin();
		if(calc)
			calc->Connect(session);
	}
}


//------------------------------------------------------------------------------
void GGroupingManager::Disconnect(GSession* session) throw(GException)
{
	R::RCursor<GFactoryGrouping> Cur;
	GGrouping* calc;

	Cur.Set(this);
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		calc=Cur()->GetPlugin();
		if(calc)
			calc->Disconnect(session);
	}
}


//------------------------------------------------------------------------------
void GGroupingManager::SetCurrentMethod(const char* name) throw(GException)
{
	GFactoryGrouping* fac;
	GGrouping* tmp;

	fac=GetPtr<const char*>(name);
	if(fac)
		tmp=fac->GetPlugin();
	else
		tmp=0;
	Current=tmp;
	if(!tmp)
		throw GException(RString("Computing method '")+name+"' doesn't exists.");
}


//------------------------------------------------------------------------------
GGrouping* GGroupingManager::GetCurrentMethod(void)
{
	return(Current);
}


//------------------------------------------------------------------------------
R::RCursor<GFactoryGrouping> GGroupingManager::GetGroupingsCursor(void)
{
	R::RCursor<GFactoryGrouping> cur(this);
	return(cur);
}


//------------------------------------------------------------------------------
GGroupingManager::~GGroupingManager(void)
{
}
