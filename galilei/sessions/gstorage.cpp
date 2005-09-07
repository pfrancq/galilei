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
	RDate today=RDate::GetToday();
	Filtering=(Filter.GetYear()!=today.GetYear())||(Filter.GetMonth()!=today.GetMonth())||(Filter.GetDay()!=today.GetDay());
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
void GStorage::ExecuteCmd(const R::RXMLTag& inst,void* caller)
{
	GStorageCmd* cmd=Commands.GetPtr(inst.GetName());
	if(!cmd)
		throw GException("Command '"+inst.GetName()+"' not support on storage "+Factory->GetName());
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
	// Register command in commands container
	if (Commands.GetPtr(*cmd))
		return false;
	Commands.InsertPtr(cmd);

	//get current plugin and test if command must be inserted into it
	GStorage* store=GetCurrentMethod();
	if(store && store->GetFactory()->GetName()==cmd->GetStorage())
		store->InsertCmd(cmd);

	return true;
}

//------------------------------------------------------------------------------
GStorageManager::~GStorageManager(void)
{
}
