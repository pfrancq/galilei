/*

	GALILEI Research Project

	GPluginManagers.cpp

	Manager of Plug-Ins Managers - Implementation.

	Copyright 2005 by the Université libre de Bruxelles.

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



//-----------------------------------------------------------------------------
// include files for ANSI C/C++
#include <ctype.h>
#include <stdexcept>
#include <dirent.h>
using namespace std;



//-----------------------------------------------------------------------------
// include file for dlopen
#include <dlfcn.h>


//------------------------------------------------------------------------------
// include files for GALILEI
#include <gpluginmanagers.h>
#include <glang.h>
#include <glinkcalc.h>
#include <gdocanalyse.h>
#include <gpostdoc.h>
#include <gfiltermanagercurl.h>
#include <gengine.h>
#include <gmetaengine.h>
#include <ggroupcalc.h>
#include <ggrouping.h>
#include <gpostgroup.h>
#include <gprofilecalc.h>
#include <gpostprofile.h>
#include <gpreprofile.h>
#include <gstatscalc.h>
#include <gprofilessims.h>
#include <gprofilesdocssims.h>
#include <ggroupsdocssims.h>
#include <gprofilesgroupssims.h>
using namespace GALILEI;
using namespace R;



//------------------------------------------------------------------------------
//
// GGPluginManagers::PluginManagers
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GPluginManagers::GPluginManagers(void)
	: RContainer<GGenericPluginManager,true,true>(20,10)
{
	InsertPtr(new GLangManager());
	InsertPtr(new GLinkCalcManager());
	InsertPtr(new GDocAnalyseManager());
	InsertPtr(new GPostDocManager());
	InsertPtr(new GFilterManagerCURL());
	InsertPtr(new GEngineManager());
	InsertPtr(new GMetaEngineManager());
	InsertPtr(new GGroupCalcManager());
	InsertPtr(new GGroupingManager());
	InsertPtr(new GPostGroupManager());
	InsertPtr(new GProfileCalcManager());
	InsertPtr(new GPreProfileManager());
	InsertPtr(new GPostProfileManager());
	InsertPtr(new GStatsCalcManager());
	InsertPtr(new GProfilesSimsManager());
	InsertPtr(new GProfilesDocsSimsManager());
	InsertPtr(new GGroupsDocsSimsManager());
	InsertPtr(new GProfilesGroupsSimsManager());
}


//-----------------------------------------------------------------------------
void GPluginManagers::Connect(GSession* session)
{
	RCursor<GGenericPluginManager> Cur(*this);
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		Cur()->Connect(session);
	}
}


//-----------------------------------------------------------------------------
void GPluginManagers::Disconnect(GSession* session)
{
	RCursor<GGenericPluginManager> Cur(*this);
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		Cur()->Disconnect(session);
	}
}


//------------------------------------------------------------------------------
void GPluginManagers::FindPlugins(const RString dir,RContainer<RString,true,false>& plugins,RContainer<RString,true,false>& dlgs)
{
	DIR* dp;
	struct dirent* ep;
	RString Path(dir);
	RString Name;

	dp=opendir(Path);
	Path+=RFile::GetDirSeparator();
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
		if(Name.FindStr(".so",-1)==-1)
			continue;

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


//-----------------------------------------------------------------------------
void GPluginManagers::Load(const R::RContainer<R::RString,true,false>& dirs,bool dlg)
{
	typedef const char* LibTypeFc(void); // Internal Type
	R::RContainer<R::RString,true,false> PlugIns(50,25);
	R::RContainer<R::RString,true,false> Dlgs(50,25);
	char* error;

	// Find the plugins
	R::RCursor<RString> Path(dirs);
	for(Path.Start();!Path.End();Path.Next())
		FindPlugins(*Path(),PlugIns,Dlgs);

	// Analyse the plugins
	R::RCursor<RString> Cur(PlugIns);
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		// Get a hangle
		void *handle=dlopen(Cur()->Latin1(),RTLD_LAZY);
		if(!handle)
		{
			cerr<<dlerror()<<endl;
			continue;
		}

		// Find the category and the manager
		LibTypeFc* LibType=(LibTypeFc*)dlsym(handle,"LibType");
		error=dlerror();
		if(error)
		{
			cerr<<error<<endl;
			continue;
		}
		const char* Lib=LibType();
		GGenericPluginManager* Manager=GetPtr(Lib);
		if(!Manager)
			continue;

		// Is there a dialog plug-in?
		RString Dlg;
		RString Short=Cur()->Mid(Cur()->Find(RTextFile::GetDirSeparator(),-1)+1);
		if(dlg)
		{
			// Go through the dialog plug-ins to see if a a plugin has this name
			RString search=Short.Mid(0,Short.GetLen()-3)+"_dlg.so";
			RCursor<RString> Cur2(Dlgs);
			for(Cur2.Start();!Cur2.End();Cur2.Next())
			{
				if(Cur2()->FindStr(search,-1)!=-1)
				{
					Dlg=(*Cur2());
					break;
				}
			}
		}
		void* handleDlg=dlopen(Dlg.Latin1(),RTLD_LAZY);

		Manager->Load(Short,handle,handleDlg);
	}
}


//-----------------------------------------------------------------------------
RCursor<GGenericPluginManager> GPluginManagers::GetManagers(void)
{
	return(RCursor<GGenericPluginManager>(*this));
}


//-----------------------------------------------------------------------------
GPluginManagers::~GPluginManagers(void)
{
}
