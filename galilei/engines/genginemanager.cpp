/*

	GALILEI Research Project

	GEngine.cpp

	Generic extractor of results from a search engine - Implementation.

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
#include <engines/gmetaengine.h>
using namespace GALILEI;
using namespace R;



//------------------------------------------------------------------------------
//
// class GEngine
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GEngineManager::GEngineManager(const char* path,bool dlg) throw(std::bad_alloc,GException)
	: RContainer<GFactoryMetaEngine,true,true>(10,5), R::RContainer<GFactoryEngine,true,true>(10,5),Current(0)
{
	RString Path(path);
	Path+="/metaengines";
	LoadPlugins<GFactoryMetaEngine,GFactoryMetaEngineInit,GEngineManager>(this,Path.Latin1(),API_METAENGINE_VERSION, dlg);

	Path= path;
	Path+="/engines";
	LoadPlugins<GFactoryEngine,GFactoryEngineInit,GEngineManager>(this,Path.Latin1(),API_ENGINE_VERSION, dlg);
}


//------------------------------------------------------------------------------
void GEngineManager::Connect(GSession* session) throw(GException)
{
	R::RCursor<GFactoryEngine> Cur;
	GEngine* eng;

	Cur.Set(this);
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		eng=Cur()->GetPlugin();
		if(eng)
			eng->Connect(session);
	}

	R::RCursor<GFactoryMetaEngine> CurM;
	GMetaEngine* meta;

	CurM.Set(this);
	for(CurM.Start();!CurM.End();CurM.Next())
	{
		meta=CurM()->GetPlugin();
		if(meta)
			meta->Connect(session);
	}
}


//------------------------------------------------------------------------------
void GEngineManager::Disconnect(GSession* session) throw(GException)
{
	R::RCursor<GFactoryEngine> Cur;
	GEngine* eng;

	Cur.Set(this);
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		eng=Cur()->GetPlugin();
		if(eng)
			eng->Disconnect(session);
	}

	R::RCursor<GFactoryMetaEngine> CurM;
	GMetaEngine* meta;

	CurM.Set(this);
	for(CurM.Start();!CurM.End();CurM.Next())
	{
		meta=CurM()->GetPlugin();
		if(meta)
			meta->Disconnect(session);
	}
}


//------------------------------------------------------------------------------
void GEngineManager::SetCurrentMethod(const char* name) throw(GException)
{
	GFactoryMetaEngine* fac;
	GMetaEngine* tmp;

	fac=RContainer<GFactoryMetaEngine,true,true>::GetPtr<const char*>(name);
	if(fac)
		tmp=fac->GetPlugin();
	else
		tmp=0;
	Current=tmp;
	if(!tmp)
		throw GException(RString("Computing method '")+name+"' doesn't exists.");
}


//------------------------------------------------------------------------------
GMetaEngine* GEngineManager::GetCurrentMethod(void)
{
	return(Current);
}


//------------------------------------------------------------------------------
GEngine* GEngineManager::GetEngine(R::RString name)
{
	return (RContainer<GFactoryEngine,true,true>::GetPtr(name.Latin1())->GetPlugin());
}


//------------------------------------------------------------------------------
void GEngineManager::InsertPtr(GFactoryMetaEngine* ptr) throw(std::bad_alloc)
{
	RContainer<GFactoryMetaEngine,true,true>::InsertPtr(ptr);
}


//------------------------------------------------------------------------------
void GEngineManager::InsertPtr(GFactoryEngine* ptr) throw(std::bad_alloc)
{
	RContainer<GFactoryEngine,true,true>::InsertPtr(ptr);
}


//------------------------------------------------------------------------------
R::RCursor<GFactoryEngine> GEngineManager::GetEnginesCursor(void)
{
	R::RCursor<GFactoryEngine> cur(this);
	return(cur);
}


//------------------------------------------------------------------------------
R::RCursor<GFactoryMetaEngine> GEngineManager::GetMetaEnginesCursor(void)
{
	R::RCursor<GFactoryMetaEngine> cur(this);
	return(cur);
}


//------------------------------------------------------------------------------
GEngineManager::~GEngineManager(void)
{
}
