/*

	GALILEI Research Project

	GPlugin.cpp

	Generic Plug-In - Implementation.

	Copyright 2003-2004 by the Université libre de Bruxelles.

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
#include <sessions/gplugin.h>
using namespace GALILEI;
using namespace R;
using namespace std;



//------------------------------------------------------------------------------
//
// GPlugin Manager class
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Global variables
RContainer<GPluginManager,false,true> GPluginManager::Managers(10,5);


//------------------------------------------------------------------------------
GPluginManager::GPluginManager(RString name,RContainer<RString, true, false>* paths)
	: Name(name),Paths(paths)
{
	Managers.InsertPtr(this);
}


//-----------------------------------------------------------------------------
void GPluginManager::Connect(GSession*)
{
}


//-----------------------------------------------------------------------------
void GPluginManager::Disconnect(GSession*)
{
}


//-----------------------------------------------------------------------------
void GPluginManager::ReadConfig(R::RXMLTag*)
{
}


//-----------------------------------------------------------------------------
void GPluginManager::SaveConfig(R::RXMLStruct*,R::RXMLTag*)
{
}


//-----------------------------------------------------------------------------
int GPluginManager::Compare(const GPluginManager& pm) const
{
	return(Name.Compare(pm.GetName()));
}


//-----------------------------------------------------------------------------
int GPluginManager::Compare(const GPluginManager* pm) const
{
	return(Name.Compare(pm->GetName()));
}


//-----------------------------------------------------------------------------
int GPluginManager::Compare(const RString& name) const
{
	return(Name.Compare(name));
}


//-----------------------------------------------------------------------------
GPluginManager* GPluginManager::GetManager(RString name)
{
	GPluginManager* res;
	res=GPluginManager::Managers.GetPtr(name);
	return res;
}


//-----------------------------------------------------------------------------
RCursor<GPluginManager> GPluginManager::GetCursor(void)
{
	RCursor<GPluginManager> cur(GPluginManager::Managers);
	return(cur);
}


//-----------------------------------------------------------------------------
GPluginManager::~GPluginManager(void)
{
	Managers.DeletePtr(this);
}



//------------------------------------------------------------------------------
//
// General function
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void GALILEI::FindPlugins(const RString dir,RContainer<RString,true,false>& plugins,RContainer<RString,true,false>& dlgs)
{
	DIR* dp;
	struct dirent* ep;
	RString Path(dir);
	RString Name;

	dp=opendir(Path);
	Path+=RTextFile::GetDirSeparator();
	if(!dp) return;
	while((ep=readdir(dp)))
	{
		// Name og the 'file"
		Name=ep->d_name;

		// Look if it is a directoy
		if(ep->d_type==DT_DIR)
		{
			// If not '.' and '..' -> goes though it
			if((Name!=".")&&(Name!=".."))
				FindPlugins(Path+Name,plugins,dlgs);
			continue;
		}

		// Must be a regular file
		if(ep->d_type!=DT_REG)
			continue;

		// Must be a library finishing with '.so'
		if(Name.GetLen()<3)
			continue;
		if(Name.FindStr(".so",-1)==-1) continue;

		// Is it a dialog plug-in?
		if(Name.FindStr("_dlg.so",-1)!=-1)
		{
			dlgs.InsertPtr(new RString(Path+Name));
			continue;
		}

		// Must be the main plug-in
		plugins.InsertPtr(new RString(Path+Name));
	}
	closedir(dp);
}


//------------------------------------------------------------------------------
RString GALILEI::FindPlugin(const RString plugin,const RContainer<R::RString,true,false>& plugins)
{
	RString ret;

	// Go through the dialog plug-ins to see if a a plugin has this name
	RCursor<RString> Cur(plugins);
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		if(Cur()->FindStr(plugin,-plugin.GetLen())!=-1)
			return(*Cur());
	}
	return(ret);
}
