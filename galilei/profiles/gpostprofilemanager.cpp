/*

	GALILEI Research Project

	GPostProfileManager.cpp

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
#include <profiles/gpostprofilemanager.h>
#include <profiles/gpostprofile.h>
using namespace GALILEI;
using namespace R;



//------------------------------------------------------------------------------
//
// class GPostProfileManager
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GPostProfileManager::GPostProfileManager(const char* path,bool dlg) throw(std::bad_alloc,GException)
	: RContainer<GFactoryPostProfile,true,true>(10,5),GPluginManager("PostProfile",path)
{
	RString Path(path);
	Path+="/postprofile";

	LoadPlugins<GFactoryPostProfile,GFactoryPostProfileInit,GPostProfileManager>(this,Path.Latin1(),API_POSTGROUP_VERSION, dlg);

}

//------------------------------------------------------------------------------
void GPostProfileManager::Connect(GSession* session) throw(GException)
{
	R::RCursor<GFactoryPostProfile> Cur;
	GPostProfile* calc;

	Cur.Set(this);
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		calc=Cur()->GetPlugin();
		if(calc)
			calc->Connect(session);
	}
}


//------------------------------------------------------------------------------
void GPostProfileManager::Disconnect(GSession* session) throw(GException)
{
	R::RCursor<GFactoryPostProfile> Cur;
	GPostProfile* calc;

	Cur.Set(this);
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		calc=Cur()->GetPlugin();
		if(calc)
			calc->Disconnect(session);
	}
}


//-----------------------------------------------------------------------------
R::RCursor<GFactoryPostProfile> GPostProfileManager::GetPostProfileCursor(void)
{
	R::RCursor<GFactoryPostProfile> cur(this);
	return(cur);
}


//------------------------------------------------------------------------------
GPostProfileManager::~GPostProfileManager(void)
{
}
