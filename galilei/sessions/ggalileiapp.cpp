/*

	GALILEI Research Project

	GGALILEIApp.cpp

	Application handling a GALILEI Session - Implementing.

	Copyright 2006-2009 by Pascal Francq (pascal@francq.info).
	Copyright 2006-2008 by the Université Libre de Bruxelles (ULB).

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
#include <ctype.h>
#include <sys/stat.h>
#ifdef _BSD_SOURCE
	#include <unistd.h>
#else
	#include <io.h>
#endif
#include <fcntl.h>
using namespace std;


//------------------------------------------------------------------------------
// include files for R
#include <rxmlstruct.h>
#include <rxmlfile.h>
using namespace R;
using namespace std;


//------------------------------------------------------------------------------
// include files for GALILEI
#include <ggalileiapp.h>
#include <gsession.h>
#include <gsubjects.h>
#include <ggalileiprg.h>
using namespace GALILEI;


//------------------------------------------------------------------------------
// include files for GALILEI (PlugIns)
#include <glang.h>
#include <glinkcalc.h>
#include <gdocanalyse.h>
#include <gpostdoc.h>
#include <gfilter.h>
#include <gengine.h>
#include <gmetaengine.h>
#include <gcommunitycalc.h>
#include <ggroupprofiles.h>
#include <gpostcommunity.h>
#include <gtopiccalc.h>
#include <ggroupdocs.h>
#include <gposttopic.h>
#include <gprofilecalc.h>
#include <gpostprofile.h>
#include <gpreprofile.h>
#include <gstatscalc.h>
#include <gstorage.h>
#include <gmeasure.h>
#include <gtool.h>


//------------------------------------------------------------------------------
// Extern variables
GGALILEIApp* GALILEI::GALILEIApp=0;



//------------------------------------------------------------------------------
//
// class GGALILEIApp
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GGALILEIApp::GGALILEIApp(const RString& name,int argc, char *argv[],bool dlg)
	: RApplication(name,argc,argv), RContainer<GGenericPluginManager,true,false>(20,10), Log(0), Debug(0), Session(0), LoadDialogs(dlg),
	  PlugInsPath(10), GALILEIConfig()
{
	InsertPtr(new GStorageManager());
	InsertPtr(new GLinkCalcManager());
	InsertPtr(new GDocAnalyseManager());
	InsertPtr(new GPostDocManager());
	InsertPtr(new GFilterManager());
	InsertPtr(new GEngineManager());
	InsertPtr(new GMetaEngineManager());
	InsertPtr(new GCommunityCalcManager());
	InsertPtr(new GGroupProfilesManager());
	InsertPtr(new GPostCommunityManager());
	InsertPtr(new GProfileCalcManager());
	InsertPtr(new GPreProfileManager());
	InsertPtr(new GPostProfileManager());
	InsertPtr(new GStatsCalcManager());
	InsertPtr(new GMeasureManager());
	InsertPtr(new GLangManager());
	InsertPtr(new GToolManager());
	InsertPtr(new GTopicCalcManager());
	InsertPtr(new GGroupDocsManager());
	InsertPtr(new GPostTopicManager());
	GALILEIApp=this;
}


//-----------------------------------------------------------------------------
void GGALILEIApp::CreateConfig(void)
{
	// General parameters
	Config.InsertParam(new RParamValue("PlugIns Config","default"));
	GALILEIConfig.InsertParam(new RParamValue("Log File","/var/log/galilei/galilei.log"));
	GALILEIConfig.InsertParam(new RParamValue("Debug File","/home/pfrancq/debug-galilei.xml"));
	GALILEIConfig.InsertParam(new RParamValue("IndexDir","/var/galilei"));
	GALILEIConfig.InsertParam(new RParamValue("IndexRamSize",512));
	GALILEIConfig.InsertParam(new RParamList("PlugIns Path"));

	// Plug-ins manager parameters
	R::RCursor<GGenericPluginManager> Managers(*this);
	for(Managers.Start();!Managers.End();Managers.Next())
		Managers()->CreateConfig(&GALILEIConfig);
}


//------------------------------------------------------------------------------
void GGALILEIApp::Init(void)
{
	// Parent initialization
	RApplication::Init();
	PlugInsConfigName=Config.Get("PlugIns Config");
	GALILEIConfig.SetConfigInfos("lib/galilei",PlugInsConfigName);

	// Get the parameters
	GALILEIConfig.Load();
	LogFileName=GALILEIConfig.Get("Log File");
	DebugFileName=GALILEIConfig.Get("Debug File");
	IndexDir=GALILEIConfig.Get("IndexDir");
	IndexRamSize=GALILEIConfig.GetUInt("IndexRamSize");
	RContainer<RParam,false,false> Paths(15);
	RCursor<RString> Cur(GALILEIConfig.GetList("PlugIns Path"));
	for(Cur.Start();!Cur.End();Cur.Next())
		PlugInsPath.InsertPtr(new RString(*Cur()));

	try
	{
		// Create (if necessary) Log file
		if(LogFileName!=RString::Null)
		{
			cout<<"Create log file "<<LogFileName<<"...";
			cout.flush();
			Log=new GSlotLog(LogFileName);
			cout<<"OK"<<endl;
		}
	}
	catch(...)
	{
		std::cerr<<"Error: Cannot create log file "<<LogFileName<<std::endl;
	}

	try
	{
		// Create (if necessary) the debug file
		if(DebugFileName!=RString::Null)
			Debug=new RDebugXML(DebugFileName);
	}
	catch(...)
	{
		std::cerr<<"Error: Cannot create debug file "<<DebugFileName<<std::endl;
	}

	//------------------------------------------------------------------------------
	// Load plug-ins
	cout<<"Load and config plug-ins ...";
	cout.flush();
	Load(PlugInsPath,LoadDialogs);
	R::RCursor<GGenericPluginManager> Managers(*this);
	for(Managers.Start();!Managers.End();Managers.Next())
		Managers()->ReadConfig(&GALILEIConfig);
	cout<<"OK"<<endl;

	// Init OK
	HasInitApp=true;
}


//------------------------------------------------------------------------------
GSession* GGALILEIApp::CreateSession(void)
{
	if(!HasInitApp)
		throw GException("Application not initialized");
	if(Session)
		throw GException("A session is alreary created");

	// Init Session
	Session=new GSession(Log,Debug);
	Session->Apply();
	GStorageManager* Mng=GetManager<GStorageManager>("Storage");
	GStorage* fac=Mng->GetCurrentPlugIn();
	fac->Connect(Session);
	fac->LoadConceptTypes();
	fac->LoadRelationTypes();
//	fac->GetPlugin()->LoadRelations();
	WriteLog("Session created");

	// Connect plugins
	RCursor<GGenericPluginManager> Cur(*this);
	for(Cur.Start();!Cur.End();Cur.Next())
		if(Cur()!=Mng)
			Cur()->Connect(Session);
	WriteLog("Plugins connected to session");

	return(Session);
}


//------------------------------------------------------------------------------
void GGALILEIApp::DeleteSession(void)
{
	if(!Session)
		return;
	RCursor<GGenericPluginManager> Cur(*this);
	for(Cur.Start();!Cur.End();Cur.Next())
		Cur()->Disconnect(Session);
	delete Session;
	Session=0;
}


//------------------------------------------------------------------------------
void GGALILEIApp::WriteLog(const RString& str)
{
	if(Log)
		Log->WriteLog(str);
}


//------------------------------------------------------------------------------
void GGALILEIApp::FindPlugins(const RString dir,RContainer<RString,true,false>& plugins,RContainer<RString,true,false>& dlgs)
{
	DIR* dp;
	struct dirent* ep;
	RString Path(dir);
	RString Name;
	struct stat statbuf;

	dp=opendir(Path);
	Path+=RFile::GetDirSeparator();
	if(!dp) return;
	while((ep=readdir(dp)))
	{
		// Name og the 'file"
		Name=ep->d_name;

		// Open file
		lstat(Path+Name, &statbuf);

		// Look if it is a directory
		if(S_ISDIR(statbuf.st_mode))
		{
			// If not '.' and '..' -> goes though it
			if((Name!=".")&&(Name!=".."))
				FindPlugins(Path+Name,plugins,dlgs);
			continue;
		}

		// Must be a regular file
		if(!S_ISREG(statbuf.st_mode))
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
		if(Name.FindStr("kde.so",-1)!=-1)
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
void GGALILEIApp::Load(const R::RContainer<R::RString,true,false>& dirs,bool dlg)
{
	typedef const char* LibTypeFc(void); // Internal Type
	R::RContainer<R::RString,true,false> PlugIns(50,25);
	R::RContainer<R::RString,true,false> Dlgs(50,25);
	char* error;

	// Find the plug-ins
	R::RCursor<RString> Path(dirs);
	for(Path.Start();!Path.End();Path.Next())
		FindPlugins(*Path(),PlugIns,Dlgs);

	// Analyze the plug-ins category by category
	RCursor<GGenericPluginManager> Mngs(*this);
	for(Mngs.Start();!Mngs.End();Mngs.Next())
	{

		R::RCursor<RString> Cur(PlugIns);
		for(Cur.Start();!Cur.End();Cur.Next())
		{
			// Get a handle
			void *handle=dlopen(Cur()->Latin1(),RTLD_NOW);
			if(!handle)
			{
				cerr<<dlerror()<<endl;
				continue;
			}

			// Find the category and the manager
			LibTypeFc* LibType=reinterpret_cast<LibTypeFc*>(reinterpret_cast<size_t>(dlsym(handle,"LibType")));

			error=dlerror();
			if(error)
			{
				cerr<<error<<endl;
				continue;
			}
			const char* Lib=LibType();
			GGenericPluginManager* Manager=GetPtr(Lib);
			if((!Manager)||(Manager!=Mngs()))
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
				if(Dlg.IsEmpty())
				{
					RString search=Short.Mid(0,Short.GetLen()-3)+"kde.so";
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
			}
			void* handleDlg=dlopen(Dlg.Latin1(),RTLD_NOW);

			Manager->Load(Short,handle,handleDlg);
		}
	}
}


//-----------------------------------------------------------------------------
RCursor<GGenericPluginManager> GGALILEIApp::GetManagers(void)
{
	return(RCursor<GGenericPluginManager>(*this));
}


//------------------------------------------------------------------------------
void GGALILEIApp::Apply(void)
{
	Config.Set("PlugIns Config",PlugInsConfigName);
	GALILEIConfig.Set("lib/galilei",PlugInsConfigName);
	GALILEIConfig.Set("Log File",LogFileName);
	GALILEIConfig.Set("Debug File",DebugFileName);
	GALILEIConfig.Set("IndexDir",IndexDir);
	GALILEIConfig.SetUInt("IndexRamSize",IndexRamSize);
	GALILEIConfig.Reset("PlugIns Path");
	RCursor<RString> Cur(PlugInsPath);
	for(Cur.Start();!Cur.End();Cur.Next())
		GALILEIConfig.AddToList("PlugIns Path",*Cur());
	if(Session)
	{
		GSubjects* sub=Session->GetSubjects();
		if(sub)
			sub->Apply();
	}
}

//------------------------------------------------------------------------------
void GGALILEIApp::RunPrg(GSlot* rec,const RString& filename)
{
	GGALILEIPrg(rec).Run(filename);
}


//------------------------------------------------------------------------------
GGALILEIApp::~GGALILEIApp(void)
{
	// No more App
	GALILEIApp=0;

	// Delete the session
	DeleteSession();

	// Get the parameters back
	Apply();
	R::RCursor<GGenericPluginManager> Managers(*this);
	for(Managers.Start();!Managers.End();Managers.Next())
		Managers()->SaveConfig(&GALILEIConfig);
	GALILEIConfig.Save();

	if(Log)
		Log->WriteLog("Session deleted");
	delete Debug;
	delete Log;
}
