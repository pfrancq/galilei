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
using namespace GALILEI;
using namespace R;



//------------------------------------------------------------------------------
//
// class GMetaEngineManager
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GMetaEngineManager::GMetaEngineManager(void)
	: GPluginManager<GMetaEngineManager,GFactoryMetaEngine,GFactoryMetaEngineInit,GMetaEngine>("MetaEngine",API_METAENGINE_VERSION),Current(0)
{
}


//------------------------------------------------------------------------------
void GMetaEngineManager::SetCurrentMethod(const char* name)
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
void GMetaEngineManager::ReadConfig(RXMLTag* t)
{
	if(!t) return;
	R::RCursor<GFactoryMetaEngine> Cur(*this);
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
	R::RCursor<GFactoryMetaEngine> Cur(*this);
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
