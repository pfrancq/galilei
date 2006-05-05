/*

	GALILEI Research Project

	GGALILEIApp.cpp

	Application handling a GALILEI Session - Implementing.

	Copyright 2006 by the Université libre de Bruxelles.

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
// include files for R
#include <rxmlstruct.h>
#include <rxmlfile.h>
using namespace R;
using namespace std;


//------------------------------------------------------------------------------
// include files for GALILEI
#include <ggalileiapp.h>
#include <gpluginmanagers.h>
#include <gconfig.h>
#include <gsession.h>
using namespace GALILEI;



//------------------------------------------------------------------------------
//
// class GGALILEIApp
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GGALILEIApp::GGALILEIApp(int argc, char *argv[])
	: Log(0), Debug(0), PlugInsPath(10,5), HasReadConfig(false), HasInitApp(false), Session(0)
{
}


//------------------------------------------------------------------------------
void GGALILEIApp::ReadConfig(const R::RString config)
{
	RXMLTag* Tag;
	RXMLTag* PlugIns;
	RXMLStruct Config;

	// Open the file
	RXMLFile ConfigFile(config,&Config);
	ConfigFile.Open(RIO::Read);

	// Read configuration part linked to the plug-ins
	Tag=Config.GetTag("plugins");
	if(!Tag)
		throw GException("Problems with the configure file "+config);
	RCursor<RXMLTag> Plugins(Tag->GetNodes());
	for(Plugins.Start();!Plugins.End();Plugins.Next())
	{
		if((Plugins()->GetName()=="search")&&(Plugins()->IsAttrDefined("dir")))
			PlugInsPath.InsertPtr(new RString(Plugins()->GetAttrValue("dir")));
		if(Plugins()->GetName()=="config")
			PlugIns=Plugins();
	}
	if(!PlugIns)
		throw GException("Problems with the configure file '/etc/galilei/galilei.conf'");
	PlugInsConfigName=PlugIns->GetAttrValue("file");

	// Read configuration part linked to the log file
	LogFileName=Config.GetTagAttrValue("log","file");

	// Read configuration part linked to the debug file
	DebugFileName=Config.GetTagAttrValue("debug","file");

	// Config OK
	HasReadConfig=true;
}


//------------------------------------------------------------------------------
void GGALILEIApp::WriteConfig(const R::RString config)
{
	RXMLStruct Config;

	RXMLTag* Top=new RXMLTag("GALILEI");
	Config.AddTag(0,Top);
	RXMLTag* Tag=new RXMLTag("log");
	Tag->InsertAttr("file",LogFileName);
	Config.AddTag(Top,Tag);
	Tag=new RXMLTag("debug");
	Tag->InsertAttr("file",DebugFileName);
	Config.AddTag(Top,Tag);
	Tag=new RXMLTag("plugins");
	Config.AddTag(Top,Tag);
	RXMLTag* Tag2=new RXMLTag("config");
	Tag2->InsertAttr("file",PlugInsConfigName);
	Config.AddTag(Tag,Tag2);
	RCursor<RString> Path(PlugInsPath);
	for(Path.Start();!Path.End();Path.Next())
	{
		Tag2=new RXMLTag("search");
		Tag2->InsertAttr("dir",*Path());
		Config.AddTag(Tag,Tag2);
	}

	RXMLFile File(config,&Config);
	File.Open(RIO::Create);
}


//------------------------------------------------------------------------------
void GGALILEIApp::Init(bool dlg)
{
	if(!HasReadConfig)
		throw GException("Configuration was not readed");

	// Create (if necessary) Log file
	if(LogFileName!=RString::Null)
	{
		cout<<"Create log file "<<LogFileName<<"...";
		cout.flush();
		Log=new GSlotLog(LogFileName);
		cout<<"OK"<<endl;
	}


	// Create (if necessary) the debug file
	if(DebugFileName!=RString::Null)
		Debug=new RDebugXML(DebugFileName);

	//------------------------------------------------------------------------------
	// Load plug-ins
	cout<<"Load and config plug-ins ...";
	cout.flush();
	GPluginManagers::PluginManagers.Load(PlugInsPath,dlg);
	GConfig Conf(PlugInsConfigName);
	Conf.Load();
	cout<<"OK"<<endl;

	// Init OK
	HasInitApp=true;
}


//------------------------------------------------------------------------------
GSession* GGALILEIApp::CreateSession(void)
{
	if(!HasReadConfig)
		throw GException("Configuration was not readed");
	if(!HasInitApp)
		throw GException("Application not initialized");

	// Init Session
	Session=new GSession(Log,Debug);
	if(Log)
		Log->WriteLog("Session created");

	//connect plugins
	GPluginManagers::PluginManagers.Connect(Session);
	if(Log)
		Log->WriteLog("Plugins connected to session");

	return(Session);
}


//------------------------------------------------------------------------------
GGALILEIApp::~GGALILEIApp(void)
{
	// Write PlugIns config
	GConfig Conf(PlugInsConfigName);
	Conf.Save();

	delete Session;
	if(Log)
		Log->WriteLog("Session deleted");
	delete Debug;
	delete Log;
}
