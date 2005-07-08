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
using namespace GALILEI;
using namespace R;



//------------------------------------------------------------------------------
//
// class GPostProfileManager
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GPostProfileManager::GPostProfileManager(void)
	: GPluginManager<GPostProfileManager,GFactoryPostProfile,GFactoryPostProfileInit,GPostProfile>("PostProfile",API_POSTPROFILE_VERSION)
{
}


//------------------------------------------------------------------------------
void GPostProfileManager::ReadConfig(RXMLTag* t)
{
	if(!t) return;
	R::RCursor<GFactoryPostProfile> Cur(*this);
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		Cur()->ReadConfig(t);
	}
}


//------------------------------------------------------------------------------
void GPostProfileManager::SaveConfig(RXMLStruct* xml,RXMLTag* t)
{
	R::RCursor<GFactoryPostProfile> Cur(*this);
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		Cur()->SaveConfig(xml,t);
	}
}


//------------------------------------------------------------------------------
GPostProfileManager::~GPostProfileManager(void)
{
}
