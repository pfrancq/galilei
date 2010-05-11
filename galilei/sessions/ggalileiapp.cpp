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
#include <gsubject.h>
#include <ggalileiprg.h>
#include <gdoc.h>
using namespace GALILEI;


//------------------------------------------------------------------------------
// include files for GALILEI (PlugIns)
#include <glang.h>
#include <glinkcalc.h>
#include <gdocanalyze.h>
#include <gfilter.h>
#include <gengine.h>
#include <gmetaengine.h>
#include <gcommunitycalc.h>
#include <ggroupprofiles.h>
#include <gtopiccalc.h>
#include <ggroupdocs.h>
#include <gprofilecalc.h>
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
	  RDownload(), Sessions(10), LoadDialogs(dlg), PlugInsPath(10), MIMES(50,25),
	  Exts(50,25), Log("/var/log/galilei/galilei.log")
{
	Log.Open(RIO::Append);

	// Create the managers of plug-ins
	InsertPtr(Storages=new GPlugInManager("Storage",GPlugInManager::ptSelect));
	InsertPtr(new GPlugInManager("LinkCalc",GPlugInManager::ptSelect));
	InsertPtr(new GPlugInManager("DocAnalyze",GPlugInManager::ptSelect));
	InsertPtr(new GPlugInManager("Filter",GPlugInManager::ptList));
	InsertPtr(new GPlugInManager("Engine",GPlugInManager::ptList));
	InsertPtr(new GPlugInManager("MetaEngine",GPlugInManager::ptSelect));
	InsertPtr(new GPlugInManager("CommunityCalc",GPlugInManager::ptSelect));
	InsertPtr(new GPlugInManager("GroupProfiles",GPlugInManager::ptSelect));
	InsertPtr(new GPlugInManager("ProfileCalc",GPlugInManager::ptSelect));
	InsertPtr(new GPlugInManager("StatsCalc",GPlugInManager::ptList));
	InsertPtr(new GPlugInManager("Measures",GPlugInManager::ptListSelect));
	InsertPtr(Langs=new GPlugInManager("Lang",GPlugInManager::ptList));
	InsertPtr(new GPlugInManager("Tool",GPlugInManager::ptList));
	InsertPtr(new GPlugInManager("TopicCalc",GPlugInManager::ptSelect));
	InsertPtr(new GPlugInManager("GroupDocs",GPlugInManager::ptSelect));
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
	Config.InsertParam(new RParamList("PlugIns Path"));
	Config.InsertParam(new RParamValue("IndexDir","/var/galilei"));
}


//------------------------------------------------------------------------------
void GGALILEIApp::Init(void)
{
	// Parent initialization
	RApplication::Init();

	// Get the parameters linked to the GALILEI library parameters
	IndexDir=Config.Get("IndexDir");
	RCursor<RString> Cur(Config.GetList("PlugIns Path"));
	for(Cur.Start();!Cur.End();Cur.Next())
		PlugInsPath.InsertPtr(new RString(*Cur()));

	// Load the plug-in factories.
	Load(PlugInsPath,LoadDialogs);

	// Re-load the config (to take the factories into account).
	Config.Load(true);
}


//------------------------------------------------------------------------------
GSession* GGALILEIApp::GetSession(const R::RString& name,bool created)
{
	// If the session exist -> return it
	GSession* Session(Sessions.GetPtr(name));
	if(Session||(!created))
		return(Session);

	// A new session must be created and its configuration read
	Session=new GSession(Sessions.GetNb(),name);
	RCursor<GPlugInManager> Cur(*this);
	for(Cur.Start();!Cur.End();Cur.Next())
		Cur()->Create(Session);
	Session->Init();
	Log.WriteLog("Session '"+name+"' created");

	// Initialize the storage and load the ontology
	Storages->InitPlugIns(Session);
	GStorage* Storage(Storages->GetCurrentPlugIn<GStorage>());
	Session->Storage=Storage;
	Storage->LoadConceptTypes();
	Storage->LoadConcepts();
	Storage->LoadPredicates();
	Storage->LoadStatements();
	Log.WriteLog("Storages for session '"+name+"' created");

	// Initialize the languages
	Langs->InitPlugIns(Session);
	RCursor<GConceptType> Types(Session->ConceptTypes);
	for(Types.Start();!Types.End();Types.Next())
	{
		RString code(Types()->Name.Mid(0,2));
		Types()->Lang=Langs->GetPlugIn<GLang>(code,false);
	}
	Log.WriteLog("Languages for session '"+name+"' created");

	// Initialize the rest of the plug-ins
	for(Cur.Start();!Cur.End();Cur.Next())
		if((Cur()!=Storages)&&(Cur()!=Langs))
			Cur()->InitPlugIns(Session);
	Log.WriteLog("Plug-ins connected to session '"+name+"'");

	// Create the storage and initialize session.
/*	GPlugInFactory* fac(GetCurrentFactory("Storage"));
	if(!fac)
		ThrowGException("No current storage");
	fac->Create(0);
	GStorage* Storage(fac->GetPlugIn<GStorage>());
	Storage->ApplyConfig();
	Storage->Init();

	// Connect the current storage to the session and load the different elements
	GPlugInManager* Mng(Storage->GetFactory()->GetMng());
	Storage->LoadConceptTypes();
	Storage->LoadConcepts();
	Storage->LoadPredicates();
	Storage->LoadStatements();
	Log.WriteLog("Session '"+name+"' created");

	// Connect the other plug-ins to the session
	GPlugInManager* Langs(GetPtr("Lang"));
	Langs->Create(Session);  // First connect the languages.
	Session->AssignLangs();
	RCursor<GPlugInManager> Cur(*this);
	for(Cur.Start();!Cur.End();Cur.Next())
		if((Cur()!=Mng)&&(Cur()!=Langs))
			Cur()->Create(Session);
	Log.WriteLog("Plug-ins connected to session '"+name+"'");*/

	return(Session);
}


//------------------------------------------------------------------------------
void GGALILEIApp::DeleteSession(GSession* session)
{
	if(!session)
		return;
	Sessions.DeletePtr(*session);
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
	Config.Set("IndexDir",IndexDir);
	Config.Reset("PlugIns Path");
	RCursor<RString> Cur(PlugInsPath);
	for(Cur.Start();!Cur.End();Cur.Next())
		Config.AddToList("PlugIns Path",*Cur());
}

//------------------------------------------------------------------------------
void GGALILEIApp::RunPrg(GSlot* rec,const RString& filename)
{
	try
	{
		Log.WriteLog("Running program '"+filename+"'");
		GGALILEIPrg(rec).Run(filename);
		Log.WriteLog("End program '"+filename+"'");
	 }
	catch(GException& e)
	{
		Log.WriteLog(RString("Error: ")+e.GetMsg());
		ThrowGException(RString("Error: ")+e.GetMsg());
	}
	catch(RException& e)
	{
		Log.WriteLog(RString("Error: ")+e.GetMsg());
		ThrowGException(RString("Error: ")+e.GetMsg());
	}
	catch(std::exception& e)
	{
		Log.WriteLog(RString("Error: ")+e.what());
		ThrowGException(RString("Error: ")+e.what());
	}
	catch(...)
	{
		Log.WriteLog("Error while processing");
		ThrowGException("Error while processing");
	}
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
GGALILEIApp::~GGALILEIApp(void)
{
	// Save the configuration structures
	Apply();

	// No more Application
	GALILEIApp=0;
}
