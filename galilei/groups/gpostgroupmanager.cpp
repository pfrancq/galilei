/*

	GALILEI Research Project

	GPostGroupManager.cpp

	Post-Group Computing Methods Manager- Implementation.

	Copyright 2003 by the Universit�Libre de Bruxelles.

	Authors:
		Vandaele Valery (vavdaele@ulb.ac.be).

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
#include <groups/gpostgroupmanager.h>
#include <groups/gpostgroup.h>
using namespace GALILEI;
using namespace R;



//------------------------------------------------------------------------------
//
// class GPostGroupManager
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GPostGroupManager::GPostGroupManager(RContainer<RString, true, false>* paths,bool dlg) throw(std::bad_alloc,GException)
	: RContainer<GFactoryPostGroup,true,true>(10,5),GPluginManager("PostGroup",paths)
{
	RCursor<RString> Cur(*paths);
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		RString Path(Cur());
		Path+="/postgroups";
		LoadPlugins<GFactoryPostGroup,GFactoryPostGroupInit,GPostGroupManager>(this,Path.Latin1(),API_POSTGROUP_VERSION, dlg);
	}
}

//------------------------------------------------------------------------------
void GPostGroupManager::Connect(GSession* session) throw(GException)
{
	R::RCursor<GFactoryPostGroup> Cur;
	GPostGroup* calc;

	Cur.Set(*this);
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		calc=Cur()->GetPlugin();
		if(calc)
			calc->Connect(session);
	}
}


//------------------------------------------------------------------------------
void GPostGroupManager::Disconnect(GSession* session) throw(GException)
{
	R::RCursor<GFactoryPostGroup> Cur;
	GPostGroup* calc;

	Cur.Set(*this);
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		calc=Cur()->GetPlugin();
		if(calc)
			calc->Disconnect(session);
	}
}


//-----------------------------------------------------------------------------
R::RCursor<GFactoryPostGroup> GPostGroupManager::GetPostGroupsCursor(void)
{
	R::RCursor<GFactoryPostGroup> cur(*this);
	return(cur);
}


//------------------------------------------------------------------------------
void GPostGroupManager::ReadConfig(RXMLTag* t)
{
	R::RCursor<GFactoryPostGroup> Cur;
	
	if(!t) return;
	Cur=GetPostGroupsCursor();
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		Cur()->ReadConfig(t);
	}
}


//------------------------------------------------------------------------------
void GPostGroupManager::SaveConfig(RXMLStruct* xml,RXMLTag* t)
{
	R::RCursor<GFactoryPostGroup> Cur;
	Cur=GetPostGroupsCursor();
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		Cur()->SaveConfig(xml,t);
	}
}


//------------------------------------------------------------------------------
GPostGroupManager::~GPostGroupManager(void)
{
}
