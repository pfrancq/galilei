/*

	GALILEI Research Project

	GStorage.cpp

	Generic Storage Manager - Implementation.

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
#include <gstorage.h>
using namespace GALILEI;
using namespace R;



//------------------------------------------------------------------------------
//
// GStorageTag
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GStorageTag::GStorageTag(const RString& cmd)
	: RXMLTag("StorageCmd")
{
	InsertAttr("nameCmd",cmd);
}



//------------------------------------------------------------------------------
//
// GStorageCmd
//
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
GStorageCmd::GStorageCmd(const R::RString cmd,const R::RString storage)
	: Cmd(cmd), Storage(storage)
{
}


//------------------------------------------------------------------------------
int GStorageCmd::Compare(const GStorageCmd& cmd) const
{
	int i=Cmd.Compare(cmd.Cmd);
	if(i)
		return(i);
	return(Storage.Compare(cmd.Storage));
}


//------------------------------------------------------------------------------
int GStorageCmd::Compare(const R::RString& cmd) const
{
	return(Cmd.Compare(cmd));
}


//------------------------------------------------------------------------------
RString GStorageCmd::GetName(void) const
{
	return(Cmd);
}


//------------------------------------------------------------------------------
RString GStorageCmd::GetStorage(void) const
{
	return(Storage);
}


//------------------------------------------------------------------------------
void GStorageCmd::Connect(GStorage*)
{
}


//------------------------------------------------------------------------------
void GStorageCmd::Disconnect(GStorage*)
{
}


//------------------------------------------------------------------------------
GStorageCmd::~GStorageCmd(void)
{
}



//------------------------------------------------------------------------------
//
// GStorage
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GStorage::GStorage(GFactoryStorage* fac)
	: GPlugin<GFactoryStorage>(fac), LoadAll(true), Filter(RDate::GetToday()), Filtering(false),
	  AllMemory(true), Commands(30,15)
{
}


//------------------------------------------------------------------------------
void GStorage::ApplyConfig(void)
{
	AllMemory=(LoadAll&&(!Filtering));
}


//------------------------------------------------------------------------------
void GStorage::Connect(GSession* session)
{
	GPlugin<GFactoryStorage>::Connect(session);

	RCursor<GStorageCmd> Cur(Commands);
	for(Cur.Start();!Cur.End();Cur.Next())
		Cur()->Connect(this);
}


//------------------------------------------------------------------------------
void GStorage::Disconnect(GSession* session)
{
	RCursor<GStorageCmd> Cur(Commands);
	for(Cur.Start();!Cur.End();Cur.Next())
		Cur()->Disconnect(this);

	GPlugin<GFactoryStorage>::Disconnect(session);
}


//------------------------------------------------------------------------------
void* GStorage::GetInfos(void)
{
	return(0);
}


//------------------------------------------------------------------------------
bool GStorage::IsAllInMemory(void) const
{
	return(AllMemory);
}


//------------------------------------------------------------------------------
bool GStorage::UseFiltering(void) const
{
	return(Filtering);
}


//------------------------------------------------------------------------------
R::RDate GStorage::GetFilter(void) const
{
	return(Filter);
}


//------------------------------------------------------------------------------
bool GStorage::IsCmdSupported(const R::RString cmd) const
{
	return(Commands.IsIn(cmd));
}


//------------------------------------------------------------------------------
void GStorage::ExecuteCmd(const GStorageTag& inst,void* caller)
{
	GStorageCmd* cmd=Commands.GetPtr(inst.GetAttrValue("nameCmd"));
	if(!cmd)
		throw GException("Command '"+inst.GetAttrValue("nameCmd")+"' not support on storage "+Factory->GetName());
	cmd->Run(this,inst,caller);
}


//------------------------------------------------------------------------------
void GStorage::InsertCmd(GStorageCmd* cmd)
{
	Commands.InsertPtr(cmd);
	cmd->Connect(this);
}


//------------------------------------------------------------------------------
GStorage::~GStorage(void)
{
}


//------------------------------------------------------------------------------
//
// class GFactoryStorage
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GFactoryStorage::GFactoryStorage(GStorageManager* mng,const char* n,const char* f)
	: GFactoryPlugin<GFactoryStorage,GStorage,GStorageManager>(mng,n,f)
{
/*	R::RXMLTag* tag=new R::RXMLTag("plugin");
	xml->AddTag(parent,tag);
	tag->InsertAttr("name",GetName());
	tag->InsertAttr("enable","False");
	GParams::SaveConfig(xml,tag);*/
}


//-----------------------------------------------------------------------------
void GFactoryStorage::Create(void)
{
	if(Plugin) return;
	Plugin=NewPlugIn();
	Plugin->ApplyConfig();
 	Plugin->InitAccess();
	Mng->EnablePlugIn(Plugin);
}


//-----------------------------------------------------------------------------
void GFactoryStorage::Create(GSession* session)
{
	if(session)
		throw GException("Cannot create storage when a session is already created");
	Create();
}



//------------------------------------------------------------------------------
//
// class GStorageManager
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GStorageManager::GStorageManager(void)
	: GPluginManager<GStorageManager,GFactoryStorage,GStorage>("Storage",API_STORAGE_VERSION,ptSelect), Commands(50,50)
{
}


//------------------------------------------------------------------------------
void GStorageManager::EnablePlugIn(GStorage* plug)
{
	// Enable plugin
	GPluginManager<GStorageManager,GFactoryStorage,GStorage>::EnablePlugIn(plug);

	// Dispatch commands for this plugin
	RCursor<GStorageCmd> Cur(Commands);
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		if (Cur()->GetStorage()==plug->GetFactory()->GetName())
			plug->InsertCmd(Cur());
	}
}


//------------------------------------------------------------------------------
void GStorageManager::DisablePlugIn(GStorage* plug)
{
	// Disable plugin
	GPluginManager<GStorageManager,GFactoryStorage,GStorage>::DisablePlugIn(plug);
}


//------------------------------------------------------------------------------
bool GStorageManager::InsertCmd(GStorageCmd* cmd)
{
	// Verify that the command does not exists
	if(Commands.GetPtr(*cmd))
		return(false);
	Commands.InsertPtr(cmd);

	//get current plugin and test if command must be inserted into it
	GStorage* store=GetCurrentMethod();
	if(store && store->GetFactory()->GetName()==cmd->GetStorage())
		store->InsertCmd(cmd);
	return(true);
}


//------------------------------------------------------------------------------
GStorageManager::~GStorageManager(void)
{
}
