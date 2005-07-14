/*

	GALILEI Research Project

	GPlugins.cpp

	Generic Plug-In Managers - Implementation.

	Copyright 2003-2005 by the Université libre de Bruxelles.

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
// include files for GALILEI
#include <gpluginmanager.h>
using namespace GALILEI;
using namespace R;
using namespace std;



//------------------------------------------------------------------------------
//
// GPlugin Manager class
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GGenericPluginManager::GGenericPluginManager(RString name,RString version,tPluginsType type)
	: Name(name), Version(version), PluginsType(type)
{
}


//-----------------------------------------------------------------------------
int GGenericPluginManager::Compare(const GGenericPluginManager& pm) const
{
	return(Name.Compare(pm.GetName()));
}


//-----------------------------------------------------------------------------
int GGenericPluginManager::Compare(const RString& name) const
{
	return(Name.Compare(name));
}


//-----------------------------------------------------------------------------
GGenericPluginManager::~GGenericPluginManager(void)
{
}
