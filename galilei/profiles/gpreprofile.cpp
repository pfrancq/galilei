/*

	GALILEI Research Project

	GPreProfile.cpp

	Generic Pre-Profiling Computing Method - Implementation.

	Copyright 2003 by the Universit�Libre de Bruxelles.

	Authors:
		David Wartel(dwartel@ulb.ac.be).

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
// include files for GALILEI
#include <gpreprofile.h>
using namespace GALILEI;
using namespace R;



//------------------------------------------------------------------------------
//
//  GPreProfile
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GPreProfile::GPreProfile(GFactoryPreProfile* fac) throw(std::bad_alloc)
	: GPlugin<GFactoryPreProfile>(fac)
{
}


//------------------------------------------------------------------------------
GPreProfile::~GPreProfile(void)
{
}



//------------------------------------------------------------------------------
//
// class GPreProfileManager
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GPreProfileManager::GPreProfileManager(void)
	: GPluginManager<GPreProfileManager,GFactoryPreProfile,GPreProfile>("PreProfile",API_PREPROFILE_VERSION,ptOrdered)
{
}


//------------------------------------------------------------------------------
GPreProfileManager::~GPreProfileManager(void)
{
}
