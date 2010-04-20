/*

	GALILEI Research Project

	GGALILEIApp.cpp

	Application handling a GALILEI Session - Implementing.

	Copyright 2006-2010 by Pascal Francq (pascal@francq.info).
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
#include <fnmatch.h>
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
#include <gsubject.h>
#include <ggalileiprg.h>
#include <gdoc.h>
using namespace GALILEI;


//------------------------------------------------------------------------------
// include files for GALILEI (PlugIns)
#include <glang.h>
#include <glinkcalc.h>
#include <gdocanalyze.h>
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
#include <gsimulator.h>
#include <gtool.h>
#include <gcomputesugs.h>
#include <gcomputetrust.h>


//------------------------------------------------------------------------------
// Global variables
GGALILEIApp* GALILEI::GALILEIApp=0;



//------------------------------------------------------------------------------
//
// class GGALILEIApp::GMIMEFilter
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
class GGALILEIApp::GMIMEFilter
{
public:
	RString Name;
	GFilter* Filter;

	GMIMEFilter(const char* n,GFilter* f) : Name(n), Filter(f) {}
	int Compare(const GMIMEFilter* f) const {return(Name.Compare(f->Name));}
	int Compare(const GMIMEFilter& f) const {return(Name.Compare(f.Name));}
	int Compare(const R::RString& t) const {return(Name.Compare(t));}
};



//------------------------------------------------------------------------------
//
// class GGALILEIApp::GMIMEExt
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
class GGALILEIApp::GMIMEExt
{
public:
	RString Name;
	RString Ext;

	GMIMEExt(const RString& n,const RString& e) : Name(n), Ext(e) {}
	int Compare(const GMIMEExt* f) const {return(Name.Compare(f->Ext));}
	int Compare(const GMIMEExt& f) const {return(Name.Compare(f.Ext));}
	int Compare(const R::RString& e) const {return(Name.Compare(e));}
	int Compare(const char* e) const {return(Name.Compare(e));}
};




//------------------------------------------------------------------------------
//
// class GGALILEIApp
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GGALILEIApp::GGALILEIApp(const RString& name,int argc, char *argv[],bool dlg)
	: RApplication(name,argc,argv), RContainer<GPlugInManager,true,false>(20,10),
	  RDownload(), Log(0), Debug(0), Session(0), LoadDialogs(dlg), PlugInsPath(10),
	  GALILEIConfig(), SessionConfig(0), MIMES(50,25), Exts(50,25)
{
	// Create the managers of plug-ins
	InsertPtr(new GPlugInManager("Storage",GPlugInManager::ptSelect));
	InsertPtr(new GPlugInManager("LinkCalc",GPlugInManager::ptSelect));
	InsertPtr(new GPlugInManager("DocAnalyze",GPlugInManager::ptSelect));
	InsertPtr(new GPlugInManager("PostDoc",GPlugInManager::ptOrdered));
	InsertPtr(new GPlugInManager("Filter",GPlugInManager::ptList));
	InsertPtr(new GPlugInManager("Engine",GPlugInManager::ptList));
	InsertPtr(new GPlugInManager("MetaEngine",GPlugInManager::ptSelect));
	InsertPtr(new GPlugInManager("CommunityCalc",GPlugInManager::ptSelect));
	InsertPtr(new GPlugInManager("GroupProfiles",GPlugInManager::ptSelect));
	InsertPtr(new GPlugInManager("PostCommunity",GPlugInManager::ptOrdered));
	InsertPtr(new GPlugInManager("ProfileCalc",GPlugInManager::ptSelect));
	InsertPtr(new GPlugInManager("PreProfile",GPlugInManager::ptOrdered));
	InsertPtr(new GPlugInManager("PostProfile",GPlugInManager::ptOrdered));
	InsertPtr(new GPlugInManager("StatsCalc",GPlugInManager::ptList));
	InsertPtr(new GPlugInManager("Measures",GPlugInManager::ptListSelect));
	InsertPtr(new GPlugInManager("Lang",GPlugInManager::ptList));
	InsertPtr(new GPlugInManager("Tool",GPlugInManager::ptList));
	InsertPtr(new GPlugInManager("TopicCalc",GPlugInManager::ptSelect));
	InsertPtr(new GPlugInManager("GroupDocs",GPlugInManager::ptSelect));
	InsertPtr(new GPlugInManager("PostTopic",GPlugInManager::ptOrdered));
	InsertPtr(new GPlugInManager("ComputeSugs",GPlugInManager::ptOrdered));
	InsertPtr(new GPlugInManager("ComputeTrust",GPlugInManager::ptOrdered));

	// Try to open list of MIME types
	try
	{
		RXMLStruct xml;
		RXMLFile File("/etc/galilei/galilei.mimes",&xml);
		File.Open(R::RIO::Read);
		// Go trough all MIME types
		RXMLTag* Types=xml.GetTag("mimeTypes");
		if(!Types)
			ThrowGException("MIME type file \"/etc/galilei/galilei.mimes\" is invalid");
		R::RCursor<RXMLTag> Cur(Types->GetNodes());
		for(Cur.Start();!Cur.End();Cur.Next())
		{
			RString MIME(Cur()->GetAttrValue("code"));
			// Go through all file extension
			R::RCursor<RXMLTag> Cur2(Cur()->GetNodes());
			for(Cur2.Start();!Cur2.End();Cur2.Next())
				Exts.InsertPtr(new GMIMEExt(MIME,Cur2()->GetAttrValue("ext")));
		}
	}
	catch(...)
	{
		//cerr<<"Cannot load '/etc/galilei/galilei.mimes'"<<endl;
	}

	GALILEIApp=this;
}


//-----------------------------------------------------------------------------
void GGALILEIApp::CreateConfig(void)
{
	// Parameter of the application
	Config.InsertParam(new RParamValue("GALILEI Config","GALILEI"));
	Config.InsertParam(new RParamValue("Session",""));

	// Parameters associated to GALILEI library
	GALILEIConfig.InsertParam(new RParamList("PlugIns Path"));
	GALILEIConfig.InsertParam(new RParamValue("IndexDir","/var/galilei"));
}


//------------------------------------------------------------------------------
void GGALILEIApp::Init(void)
{
	// Parent initialization
	RApplication::Init();

	// Get the parameters linked to the GALILEI library parameters
	GALILEIConfigName=Config.Get("GALILEI Config");
	GALILEIConfig.SetConfigInfos("lib/galilei",GALILEIConfigName);
	GALILEIConfig.Load();
	IndexDir=GALILEIConfig.Get("IndexDir");
	RCursor<RString> Cur(GALILEIConfig.GetList("PlugIns Path"));
	for(Cur.Start();!Cur.End();Cur.Next())
		PlugInsPath.InsertPtr(new RString(*Cur()));

	// Load the plug-ins.
	Load(PlugInsPath,LoadDialogs);

	// Try to change the session name.
	RString name(Config.Get("Session"));
	ChangeSessionName(name);

	// Initialize is OK
	HasInitApp=true;
}


//------------------------------------------------------------------------------
void GGALILEIApp::DeleteSessionConfig(void)
{
	if(!SessionConfig)
		return;

	SessionConfig->Set("Log File",LogFileName);
	SessionConfig->Set("Debug File",DebugFileName);
	RCursor<GPlugInManager> Cur(*this);
	for(Cur.Start();!Cur.End();Cur.Next())
		Cur()->SaveConfig();
	SessionConfig->Save();
	delete SessionConfig;
	SessionConfig=0;
}


//------------------------------------------------------------------------------
void GGALILEIApp::ChangeSessionName(const R::RString& name)
{
	if(Session)
		ThrowGException("Cannot change the session name while another session one is opened");

	// If empty name -> Nothing to do.
	if((name.IsEmpty())||(name==SessionName))
		return;

	// Delete and re-create the SessionConfig
	DeleteSessionConfig();
	SessionName=name;
	SessionConfig=new RConfig("lib/galilei/sessions",SessionName);

	// Create the parameters
	SessionConfig->InsertParam(new RParamValue("Log File","/var/log/galilei/galilei.log"));
	SessionConfig->InsertParam(new RParamValue("Debug File","/home/pfrancq/debug-galilei.xml"));
	GSimulator::CreateConfig(SessionConfig);
	GIndexer::CreateConfig(SessionConfig);
	R::RCursor<GPlugInManager> Managers(*this);
	for(Managers.Start();!Managers.End();Managers.Next())
		Managers()->CreateConfig();

	// Load the file and the parameters
	SessionConfig->Load(false);
	LogFileName=SessionConfig->Get("Log File");
	DebugFileName=SessionConfig->Get("Debug File");
	for(Managers.Start();!Managers.End();Managers.Next())
		Managers()->ReadConfig();
}


//------------------------------------------------------------------------------
GSession* GGALILEIApp::CreateSession(void)
{
	if(!HasInitApp)
		ThrowGException("Application not initialized");
	if(Session)
		ThrowGException("A session is already created");
	if(!SessionConfig)
		ThrowGException("Name '"+SessionName+"' is not a valid session name");

	// Create the log file and debug file
	try
	{
		// Create (if necessary) Log file
		if(LogFileName!=RString::Null)
		{
			//cout<<"Create log file "<<LogFileName<<"...";
			cout.flush();
			Log=new GSlotLog(LogFileName);
			//cout<<"OK"<<endl;
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

	// Create the storage and initialize session.
	GPlugInFactory* fac(GetCurrentFactory("Storage"));
	if(!fac)
		ThrowGException("No current storage");
	fac->Create(0);
	GStorage* Storage(fac->GetPlugIn<GStorage>());
	Storage->ApplyConfig();
	Storage->Init();
	Session=new GSession(SessionConfig,Storage,Log,Debug);
	Session->ApplyConfig();

	// Connect the current storage to the session and load the different elements
	GPlugInManager* Mng(Storage->GetFactory()->GetMng());
	Storage->LoadConceptTypes();
	Storage->LoadConcepts();
	Storage->LoadPredicates();
	Storage->LoadStatements();
	WriteLog("Session created");

	// Connect the other plug-ins to the session
	RCursor<GPlugInManager> Cur(*this);
	for(Cur.Start();!Cur.End();Cur.Next())
		if(Cur()!=Mng)
			Cur()->Create(Session);
	WriteLog("Plug-ins connected to session");

	return(Session);
}


//------------------------------------------------------------------------------
void GGALILEIApp::DeleteSession(void)
{
	if(!Session)
		return;
	RCursor<GPlugInManager> Cur(*this);
	for(Cur.Start();!Cur.End();Cur.Next())
		Cur()->Delete();
	delete Session;
	Session=0;
	if(Log)
		Log->WriteLog("Session deleted");
	delete Debug;
	Debug=0;
	delete Log;
	Log=0;
}

//------------------------------------------------------------------------------
void GGALILEIApp::SetLogFileName(const R::RString& name)
{
	LogFileName=name;
	if(Log)
	{
		delete Log;
		Log=0;
	}
	if(LogFileName.IsEmpty())
		return;
	try
	{
		Log=new GSlotLog(LogFileName);
	}
	catch(...)
	{
		std::cerr<<"Error: Cannot create log file "<<LogFileName<<std::endl;
	}
}


//------------------------------------------------------------------------------
void GGALILEIApp::SetDebugFileName(const RString& name)
{
	DebugFileName=name;
	if(Debug)
	{
		delete Debug;
		Debug=0;
	}
	if(DebugFileName.IsEmpty())
		return;
	try
	{
		Debug=new RDebugXML(DebugFileName);
		if(Session)
			Session->SetDebug(Debug);
	}
	catch(...)
	{
		std::cerr<<"Error: Cannot create debug file "<<DebugFileName<<std::endl;
	}
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

	// Find all the plug-ins, i.e. the dynamic libraries finishing by:
	// - 'so' -> Stored in PlugIns
	// -'_dlg.so' or 'kde.so' -> Stored in Dlgs.
	R::RCursor<RString> Path(dirs);
	for(Path.Start();!Path.End();Path.Next())
		FindPlugins(*Path(),PlugIns,Dlgs);

	// Analyze the plug-ins category by category
	RCursor<GPlugInManager> Mngs(*this);
	for(Mngs.Start();!Mngs.End();Mngs.Next())
	{
		R::RCursor<RString> Cur(PlugIns);
		for(Cur.Start();!Cur.End();Cur.Next())
		{
			// Get a handle on the dynamic library
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
			GPlugInManager* Manager=GetPtr(Lib);
			if((!Manager)||(Manager!=Mngs()))
				continue;

			// Is there a dialog plug-in?
			RString Dlg;
			RString Short=Cur()->Mid(Cur()->Find(RTextFile::GetDirSeparator(),-1)+1);
			if(dlg)
			{
				// Go through the dialog plug-ins to see if a a plug-in has this name
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
RCursor<GPlugInManager> GGALILEIApp::GetManagers(void)
{
	return(RCursor<GPlugInManager>(*this));
}


//-----------------------------------------------------------------------------
GPlugInManager* GGALILEIApp::GetManager(const R::RString& mng) const
{
	return(GetPtr(mng));
}


//-----------------------------------------------------------------------------
RCursor<GPlugInFactory> GGALILEIApp::GetFactories(const R::RString& mng,const R::RString& list) const
{
	GPlugInManager* ptr(GetPtr(mng));
	if(!ptr)
		ThrowGException("'"+mng+"' is not a valid plug-ins manager");
	return(ptr->GetFactories(list));
}


//-----------------------------------------------------------------------------
GPlugInFactory* GGALILEIApp::GetFactory(const R::RString& mng,const R::RString& name,const R::RString& list,int need) const
{
	GPlugInManager* ptr(GetPtr(mng));
	if(!ptr)
		ThrowGException("'"+mng+"' is not a valid plug-ins manager");
	return(ptr->GetFactory(name,list,need));
}


//------------------------------------------------------------------------------
GPlugInFactory* GGALILEIApp::GetCurrentFactory(const R::RString& mng,const R::RString& list,int need) const
{
	GPlugInManager* ptr(GetPtr(mng));
	if(!ptr)
		ThrowGException("'"+mng+"' is not a valid plug-ins manager");
	return(ptr->GetCurrentFactory(list,need));
}


//------------------------------------------------------------------------------
void GGALILEIApp::Apply(void)
{
	Config.Set("GALILEI Config",GALILEIConfigName);
	Config.Set("Session",SessionName);
	GALILEIConfig.SetConfigInfos("lib/galilei",GALILEIConfigName);
	if(SessionConfig)
	{
		SessionConfig->Set("Log File",LogFileName);
		SessionConfig->Set("Debug File",DebugFileName);
	}
	GALILEIConfig.Set("IndexDir",IndexDir);
	GALILEIConfig.Reset("PlugIns Path");
	RCursor<RString> Cur(PlugInsPath);
	for(Cur.Start();!Cur.End();Cur.Next())
		GALILEIConfig.AddToList("PlugIns Path",*Cur());
}

//------------------------------------------------------------------------------
void GGALILEIApp::RunPrg(GSlot* rec,const RString& filename)
{
	GGALILEIPrg(rec).Run(filename);
}


//------------------------------------------------------------------------------
void GGALILEIApp::FindMIMEType(void)
{
	// If MIME type already exist -> return
	RString MIME(Doc->GetMIMEType());
	if(!MIME.IsEmpty())
		return;

	// Goes through all defined MIME types
	RCursor<GMIMEExt> Cur(Exts);
	for(Cur.Start();!Cur.End();Cur.Next())
		if(fnmatch(Cur()->Ext,Doc->GetURL()(),0)!=FNM_NOMATCH)
		{
			Doc->SetMIMEType(Cur()->Name);
			return;
		}
}


//------------------------------------------------------------------------------
bool GGALILEIApp::IsValidContent(const R::RString& MIME)
{
	if(Doc->GetMIMEType().IsEmpty())
		Doc->SetMIMEType(MIME);
	GMIMEFilter* ptr(MIMES.GetPtr(Doc->GetMIMEType()));
	if(!ptr)
		return(false);
	Filter=ptr->Filter;
	return(true);
}


//------------------------------------------------------------------------------
GFilter* GGALILEIApp::FindMIMEType(GDoc* doc,RURI& uri,RIO::RSmartTempFile& tmp)
{
	// Init Part;
	Doc=doc;
	Filter=0;
	uri=Doc->GetURL();

	// Guess the MIME type if necessary
	FindMIMEType();

	// If it is known to be a XML file -> file can directly analyzed.
	if((Doc->GetMIMEType()=="application/xml")||(Doc->GetMIMEType()=="text/xml"))
		return(0);

	// The file is perhaps not a XML file -> Try to transform it into DocXML

	// If it is not a local	file -> Download it
	if(Doc->GetURL().GetScheme()!="file")
	{
		uri=tmp.GetName();
		DownloadFile(Doc->GetURL(),uri);

		// Perhaps the server holding the file has provide a MIME type which can be XML
		if((Doc->GetMIMEType()=="application/xml")||(Doc->GetMIMEType()=="text/xml"))
			return(0);
	}

	// If no MIME type -> Exception
	if(Doc->GetMIMEType().IsEmpty())
		throw GException("Cannot find MIME type for "+doc->GetURL()());

	// If no filter -> Exception
	GMIMEFilter* ptr=MIMES.GetPtr(Doc->GetMIMEType());
	if(ptr)
		Filter=ptr->Filter;
	if(!Filter)
		ThrowGException("Cannot treat the MIME type '"+doc->GetMIMEType()+"'");
	return(Filter);
}


//------------------------------------------------------------------------------
void GGALILEIApp::AddMIME(const char* mime,GFilter* f)
{
	MIMES.InsertPtr(new GMIMEFilter(mime,f));
}


//------------------------------------------------------------------------------
void GGALILEIApp::DelMIMES(GFilter* f)
{
	RContainer<GMIMEFilter,false,false> Rem(5,5);

	// Find All MIMES types to deleted
	RCursor<GMIMEFilter> Cur(MIMES);
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		if(Cur()->Filter==f)
			Rem.InsertPtr(Cur());
	}

	// Delete all MIMES
	Cur.Set(Rem);
	for(Cur.Start();!Cur.End();Cur.Next())
		MIMES.DeletePtr(Cur());
}


//------------------------------------------------------------------------------
void GGALILEIApp::RunTool(const R::RString& name,GSlot* slot,bool need)
{
	GTool* Tool(GetPlugIn<GTool>("Tool",name));
	if((!Tool)&&need)
		ThrowGException("Tool '"+name+"' does not exist");
	Tool->Run(slot);
}


//------------------------------------------------------------------------------
GGALILEIApp::~GGALILEIApp(void)
{
	// Delete the session if necessary
	DeleteSession();

	// Save the configuration structures
	Apply();
	DeleteSessionConfig(); // Force to save the session configuration if necessary
	GALILEIConfig.Save();

	// No more Application
	GALILEIApp=0;
}
