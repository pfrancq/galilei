/*

	GALILEI Research Project

	GEngineManager.cpp

	Generic engine Manager - Implementation.

	Copyright 2004 by the Universit�Libre de Bruxelles.

	Authors:
		Valery Vandaele (vavdaele@ulb.ac.be)

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
#include <engines/genginemanager.h>
#include <engines/gengine.h>
using namespace GALILEI;
using namespace R;



//------------------------------------------------------------------------------
//
// class GEngine
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GEngineManager::GEngineManager(RContainer<RString, true, false>* paths,bool dlg) throw(std::bad_alloc,GException)
	: R::RContainer<GFactoryEngine,true,true>(10,5),GPluginManager("Engine",paths)
{
	RCursor<RString> Cur(*paths);
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		RString Path(*Cur());
		Path+="/engines";
		LoadPlugins<GFactoryEngine,GFactoryEngineInit,GEngineManager>(this,Path.Latin1(),API_ENGINE_VERSION, dlg);
	}
}


//------------------------------------------------------------------------------
void GEngineManager::Connect(GSession* session) throw(GException)
{
	R::RCursor<GFactoryEngine> Cur;
	GEngine* eng;

	Cur.Set(*this);
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		eng=Cur()->GetPlugin();
		if(eng)
			eng->Connect(session);
	}
}


//------------------------------------------------------------------------------
void GEngineManager::Disconnect(GSession* session) throw(GException)
{
	R::RCursor<GFactoryEngine> Cur;
	GEngine* eng;

	Cur.Set(*this);
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		eng=Cur()->GetPlugin();
		if(eng)
			eng->Disconnect(session);
	}
}

//------------------------------------------------------------------------------
GEngine* GEngineManager::GetEngine(R::RString name)
{
	return (GetPtr(name.Latin1())->GetPlugin());
}


//------------------------------------------------------------------------------
R::RCursor<GFactoryEngine> GEngineManager::GetEnginesCursor(void)
{
	R::RCursor<GFactoryEngine> cur(*this);
	return(cur);
}


//------------------------------------------------------------------------------
void GEngineManager::ReadConfig(RXMLTag* t)
{
	R::RCursor<GFactoryEngine> Cur;

	if(!t) return;
	Cur=GetEnginesCursor();
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		Cur()->ReadConfig(t);
	}
}


//------------------------------------------------------------------------------
void GEngineManager::SaveConfig(RXMLStruct* xml,RXMLTag* t)
{
	R::RCursor<GFactoryEngine> Cur;
	Cur=GetEnginesCursor();
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		Cur()->SaveConfig(xml,t);
	}
}


//------------------------------------------------------------------------------
GEngineManager::~GEngineManager(void)
{
}
