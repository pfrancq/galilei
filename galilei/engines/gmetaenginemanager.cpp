/*

	GALILEI Research Project

	GMetaEngineManager.cpp

	Generic Meta engine Manager Implementation.

	Copyright 2004 by the Université libre de Bruxelles.

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
#include <engines/gmetaenginemanager.h>
#include <engines/gmetaengine.h>
using namespace GALILEI;
using namespace R;



//------------------------------------------------------------------------------
//
// class GMetaEngineManager
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GMetaEngineManager::GMetaEngineManager(const char* path,bool dlg) throw(std::bad_alloc,GException)
	: RContainer<GFactoryMetaEngine,true,true>(10,5),GPluginManager("MetaEngine",path),Current(0)
{
	RString Path(path);
	Path+="/metaengines";
	LoadPlugins<GFactoryMetaEngine,GFactoryMetaEngineInit,GMetaEngineManager>(this,Path.Latin1(),API_METAENGINE_VERSION, dlg);
}


//------------------------------------------------------------------------------
void GMetaEngineManager::Connect(GSession* session) throw(GException)
{
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
void GMetaEngineManager::Disconnect(GSession* session) throw(GException)
{
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
void GMetaEngineManager::SetCurrentMethod(const char* name) throw(GException)
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
GMetaEngine* GMetaEngineManager::GetCurrentMethod(void)
{
	return(Current);
}


//------------------------------------------------------------------------------
R::RCursor<GFactoryMetaEngine> GMetaEngineManager::GetMetaEnginesCursor(void)
{
	R::RCursor<GFactoryMetaEngine> cur(this);
	return(cur);
}


//------------------------------------------------------------------------------
void GMetaEngineManager::ReadConfig(RXMLTag* t)
{
	R::RCursor<GFactoryMetaEngine> Cur;

	if(!t) return;
	Cur=GetMetaEnginesCursor();
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		Cur()->ReadConfig(t);
	}
	try
	{
		SetCurrentMethod(t->GetAttrValue("current"));
	}
	catch(GException)
	{
	}
}


//------------------------------------------------------------------------------
void GMetaEngineManager::SaveConfig(RXMLStruct* xml,RXMLTag* t)
{
	R::RCursor<GFactoryMetaEngine> Cur;
	Cur=GetMetaEnginesCursor();
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		Cur()->SaveConfig(xml,t);
	}
	if(Current)
		t->InsertAttr("current",Current->GetFactory()->GetName());
	else
		t->InsertAttr("current","None");
}


//------------------------------------------------------------------------------
GMetaEngineManager::~GMetaEngineManager(void)
{
}
