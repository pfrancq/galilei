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
	: RApplication(name,argc,argv), RContainer<GPlugInManager,true,false>(20,10), RDownload(), Log(0), Debug(0), Session(0), LoadDialogs(dlg),
	  PlugInsPath(10), GALILEIConfig(), MIMES(50,25), Exts(50,25)
{
	// Create the managers of plug-ins
	InsertPtr(new GPlugInManager("Storage",GPlugInManager::ptSelect));
	InsertPtr(new GPlugInManager("LinkCalc",GPlugInManager::ptSelect));
	InsertPtr(new GPlugInManager("DocAnalyse",GPlugInManager::ptSelect));
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
	// General parameters
	Config.InsertParam(new RParamValue("PlugIns Config","default"));
	GALILEIConfig.InsertParam(new RParamValue("Log File","/var/log/galilei/galilei.log"));
	GALILEIConfig.InsertParam(new RParamValue("Debug File","/home/pfrancq/debug-galilei.xml"));
	GALILEIConfig.InsertParam(new RParamValue("IndexDir","/var/galilei"));
	GALILEIConfig.InsertParam(new RParamList("PlugIns Path"));

	// Plug-ins manager parameters
	R::RCursor<GPlugInManager> Managers(*this);
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
	RContainer<RParam,false,false> Paths(15);
	RCursor<RString> Cur(GALILEIConfig.GetList("PlugIns Path"));
	for(Cur.Start();!Cur.End();Cur.Next())
		PlugInsPath.InsertPtr(new RString(*Cur()));

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

	//------------------------------------------------------------------------------
	// Load plug-ins
	//cout<<"Load and configure plug-ins ...";
	cout.flush();
	Load(PlugInsPath,LoadDialogs);
	R::RCursor<GPlugInManager> Managers(*this);
	for(Managers.Start();!Managers.End();Managers.Next())
		Managers()->ReadConfig(&GALILEIConfig);
	//cout<<"OK"<<endl;

	// Init OK
	HasInitApp=true;
}


//------------------------------------------------------------------------------
GSession* GGALILEIApp::CreateSession(void)
{
	if(!HasInitApp)
		throw GException("Application not initialized");
	if(Session)
		throw GException("A session is already created");

	// Initialize Session
	Session=new GSession(Log,Debug);
	Session->Apply();
	GPlugInManager* Mng(Session->GetStorage()->GetFactory()->GetMng());
	Session->GetStorage()->Connect(Session);
	Session->GetStorage()->LoadConceptTypes();
	Session->GetStorage()->LoadConcepts();
	Session->GetStorage()->LoadPredicates();
	Session->GetStorage()->LoadStatements();
	WriteLog("Session created");

	// Connect plugins
	RCursor<GPlugInManager> Cur(*this);
	for(Cur.Start();!Cur.End();Cur.Next())
		if(Cur()!=Mng)
			Cur()->Connect(Session);
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
	RCursor<GPlugInManager> Mngs(*this);
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
			GPlugInManager* Manager=GetPtr(Lib);
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

			Manager->Load(Short,handle,handleDlg,&GALILEIConfig);
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
	Config.Set("PlugIns Config",PlugInsConfigName);
	GALILEIConfig.Set("lib/galilei",PlugInsConfigName);
	GALILEIConfig.Set("Log File",LogFileName);
	GALILEIConfig.Set("Debug File",DebugFileName);
	GALILEIConfig.Set("IndexDir",IndexDir);
	GALILEIConfig.Reset("PlugIns Path");
	RCursor<RString> Cur(PlugInsPath);
	for(Cur.Start();!Cur.End();Cur.Next())
		GALILEIConfig.AddToList("PlugIns Path",*Cur());
	if(Session)
	{
		GSimulator* sub=Session->GetSimulator();
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
RURI GGALILEIApp::WhatAnalyze(GDoc* doc,RIO::RSmartTempFile& docxml,bool& native)
{
	RIO::RSmartTempFile DwnFile;      // Temporary file containing the downloaded file  (if necessary).
	RURI NonXMLFile;               // Non XML-File file.

	// Init Part;
	Doc=doc;
	Filter=0;
	native=true;       // Suppose real XML file

	// Guess the MIME type if necessary
	FindMIMEType();

	// If it is known to be a XML file -> file can directly analyzed.
	if((Doc->GetMIMEType()=="application/xml")||(Doc->GetMIMEType()=="text/xml"))
		return(Doc->GetURL());

	// The file is perhaps not a XML file -> Try to transform it into DocXML

	// If it is not a local	file -> Download it
	if(Doc->GetURL().GetScheme()!="file")
	{
		NonXMLFile=DwnFile.GetName();
		DownloadFile(Doc->GetURL(),NonXMLFile);

		// Perhaps the server holding the file has provide a MIME type which can be XML
		if((Doc->GetMIMEType()=="application/xml")||(Doc->GetMIMEType()=="text/xml"))
			return(Doc->GetURL());
	}
	else
		NonXMLFile=Doc->GetURL().GetPath();

	// No XML file
	native=false;

	// If no MIME type -> Exception
	if(doc->GetMIMEType().IsEmpty())
		throw GException("Cannot find MIME type for "+doc->GetURL()());

	// If no filter -> Exception
	GMIMEFilter* ptr=MIMES.GetPtr(Doc->GetMIMEType());
	if(ptr)
		Filter=ptr->Filter;
	if(!Filter)
		ThrowGException("Cannot treat the MIME type '"+doc->GetMIMEType()+"'");

	// Analyze the file
	if(!RFile::Exists(NonXMLFile))
		ThrowGException("File '"+NonXMLFile()+"' not exist");

	Filter->Analyze(Doc->GetURL(),NonXMLFile,docxml.GetName());

	// Return file to realy analyze
	return(docxml.GetName());
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
	// No more App
	GALILEIApp=0;

	// Delete the session
	DeleteSession();

	// Get the parameters back
	Apply();
	R::RCursor<GPlugInManager> Managers(*this);
	for(Managers.Start();!Managers.End();Managers.Next())
		Managers()->SaveConfig(&GALILEIConfig);
	GALILEIConfig.Save();

	if(Log)
		Log->WriteLog("Session deleted");
	delete Debug;
	delete Log;
}
