/*

	GALILEI Research Project

	GPreProfileManager.cpp

	Post-Profile Computing Methods Manager- Implementation.

	Copyright 2003 by the Universit�Libre de Bruxelles.

	Authors:
		David Wartel (dwartel@ulb.ac.be).

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
#include <profiles/gpreprofilemanager.h>
#include <profiles/gpreprofile.h>
using namespace GALILEI;
using namespace R;



//------------------------------------------------------------------------------
//
// class GPreProfileManager
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GPreProfileManager::GPreProfileManager(RContainer<RString, true, false>* paths,bool dlg) throw(std::bad_alloc,GException)
	: RContainer<GFactoryPreProfile,true,true>(10,5),GPluginManager("PreProfile",paths)
{
	for(paths->Start(); !paths->End(); paths->Next())
	{
		RString Path((*paths)());
		Path+="/preprofile";
		LoadPlugins<GFactoryPreProfile,GFactoryPreProfileInit,GPreProfileManager>(this,Path.Latin1(),API_PREPROFILE_VERSION, dlg);
	}

}

//------------------------------------------------------------------------------
void GPreProfileManager::Connect(GSession* session) throw(GException)
{
	R::RCursor<GFactoryPreProfile> Cur;
	GPreProfile* calc;

	Cur.Set(this);
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		calc=Cur()->GetPlugin();
		if(calc)
			calc->Connect(session);
	}
}


//------------------------------------------------------------------------------
void GPreProfileManager::Disconnect(GSession* session) throw(GException)
{
	R::RCursor<GFactoryPreProfile> Cur;
	GPreProfile* calc;

	Cur.Set(this);
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		calc=Cur()->GetPlugin();
		if(calc)
			calc->Disconnect(session);
	}
}


//-----------------------------------------------------------------------------
R::RCursor<GFactoryPreProfile> GPreProfileManager::GetPreProfileCursor(void)
{
	R::RCursor<GFactoryPreProfile> cur(this);
	return(cur);
}


//------------------------------------------------------------------------------
void GPreProfileManager::ReadConfig(RXMLTag* t)
{
	R::RCursor<GFactoryPreProfile> Cur;
	
	if(!t) return;
	Cur=GetPreProfileCursor();
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		Cur()->ReadConfig(t);
	}
}


//------------------------------------------------------------------------------
void GPreProfileManager::SaveConfig(RXMLStruct* xml,RXMLTag* t)
{
	R::RCursor<GFactoryPreProfile> Cur;
	Cur=GetPreProfileCursor();
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		Cur()->SaveConfig(xml,t);
	}
}


//------------------------------------------------------------------------------
GPreProfileManager::~GPreProfileManager(void)
{
}
