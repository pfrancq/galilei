	/*

	GALILEI Research Project

	GPlugIns.cpp

	Generic Plug-In Managers - Implementation.

	Copyright 2003-2010 by Pascal Francq (pascal@francq.info).
	Copyright 2003-2008 by the Université Libre de Bruxelles (ULB).

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
#include <ggalileiapp.h>
#include <gsession.h>
using namespace GALILEI;
using namespace R;
using namespace std;



//-----------------------------------------------------------------------------
//
// class GPlugInList
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GPlugInList::GPlugInList(GPlugInManager* mng,R::RString name)
	: Name(name), Factories(20,10), Plugins(20,10), Current(0), Mng(mng)
{
}


//------------------------------------------------------------------------------
int GPlugInList::Compare(const GPlugInList& list) const
{
	return(Name.Compare(list.Name));
}


//------------------------------------------------------------------------------
int GPlugInList::Compare(const R::RString& name) const
{
	return(Name.Compare(name));
}


//-----------------------------------------------------------------------------
void GPlugInList::CreateConfig(void)
{
	R::RCursor<GPlugInFactory> Cur(Factories);
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		GPlugIn* PlugIn(Cur()->GetPlugIn());
		if(!PlugIn)
			return;
		PlugIn->CreateConfig();
		if(Mng->PluginsType==GPlugInManager::ptOrdered)
		{
			Cur()->SetLevel(static_cast<int>(GetNbFactories()));
			PlugIn->InsertParam(new RParamValue("Level",static_cast<int>(GetNbFactories()),"Position order of the plug-in"));
		}
	}
	if(Mng->PluginsType==GPlugInManager::ptOrdered)
	{
		Factories.ReOrder();
		Plugins.ReOrder();
	}
}


//-----------------------------------------------------------------------------
void GPlugInList::Create(GSession* session)
{
	R::RCursor<GPlugInFactory> Cur(Factories);
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		bool Enabled;
		if(Cur()->Mng->GetPlugInType()==GPlugInManager::ptListSelect)
			Enabled=GALILEIApp->GetConfig()->GetBool("Enable",Cur()->Mng->GetName(),Cur()->List,Cur()->Name);
		else
			Enabled=GALILEIApp->GetConfig()->GetBool("Enable",Cur()->Mng->GetName(),Cur()->Name);
		if(!Enabled)
			continue;
		Cur()->Create(session);
	}
}


//-----------------------------------------------------------------------------
void GPlugInList::Delete(void)
{
	R::RCursor<GPlugInFactory> Cur(Factories);
	for(Cur.Start();!Cur.End();Cur.Next())
		Cur()->Delete();
}



//-----------------------------------------------------------------------------
//
// class GPlugInManager
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GPlugInManager::GPlugInManager(R::RString name,tPluginsType type)
	: Name(name), Version(API_PLUG_IN_VERSION), PluginsType(type)
{
	if(PluginsType==ptListSelect)
		Data.Lists=new R::RContainer<GPlugInList,true,true>(10,5);
	else
		Data.List=new GPlugInList(this,RString::Null);
}


//------------------------------------------------------------------------------
int GPlugInManager::Compare(const GPlugInManager& pmng) const
{
	return(Name.Compare(pmng.Name));
}


//------------------------------------------------------------------------------
int GPlugInManager::Compare(const R::RString& name) const
{
	return(Name.Compare(name));
}


//------------------------------------------------------------------------------
void GPlugInManager::ReOrder(void)
{
	if(PluginsType!=ptOrdered)
		return;
	Data.List->Factories.ReOrder();
	Data.List->Plugins.ReOrder();
}


//-----------------------------------------------------------------------------
void GPlugInManager::Load(const R::RString& dll,void* handle,void* handleDlg)
{
	typedef GPlugInFactory* FactoryInit(GPlugInManager*,const char*);

	char* error;

	// Try to create the GPlugInFactory
	FactoryInit* initFac=reinterpret_cast<FactoryInit*>(reinterpret_cast<size_t>(dlsym(handle,"FactoryCreate")));
	error=dlerror();
	if(error)
	{
		cerr<<error<<endl;
		return;
	}
	GPlugInFactory* Factory(initFac(this,dll));

	// Verify the versions of the GPlugInFactory and the session
	if(Version.Compare(Factory->GetAPIVersion()))
		return;

	// Insert the factory in the right plug-ins list
	GPlugInList* List(0);
	if(PluginsType==ptListSelect)
	{
		List=Data.Lists->GetPtr(Factory->GetList());
		if(!List)
			Data.Lists->InsertPtr(List=new GPlugInList(this,Factory->GetList()));
	}
	else
	{
		if(!Data.List)
			Data.List=new GPlugInList(this,RString::Null);
		List=Data.List;
	}
	List->Factories.InsertPtr(Factory);
	if(GetPlugInType()==GPlugInManager::ptListSelect)
		GALILEIApp->GetConfig()->InsertParam(new RParamValue("Enable",true,"Is the plug-in enabled?"),GetName(),Factory->List,Factory->Name);
	else
		GALILEIApp->GetConfig()->InsertParam(new RParamValue("Enable",true,"Is the plug-in enabled?"),GetName(),Factory->Name);

	// Try to create the dialogs if necessary
	if(!handleDlg)
		return;
	GPlugInFactory::About_t about=reinterpret_cast<GPlugInFactory::About_t>(reinterpret_cast<size_t>(dlsym(handleDlg,"About")));
	error=dlerror();
	if(!error)
		Factory->SetAbout(about);
	void* configdll= dlsym(handleDlg,"Configure");
	error=dlerror();
	if(!error)
		Factory->SetConfig(configdll);
}


//-----------------------------------------------------------------------------
void GPlugInManager::Create(GSession* session)
{
	if(PluginsType==ptListSelect)
	{
		RCursor<GPlugInList> Cur(*Data.Lists);
		for(Cur.Start();!Cur.End();Cur.Next())
			Cur()->Create(session);
	}
	else
		Data.List->Create(session);
}


//-----------------------------------------------------------------------------
void GPlugInManager::Delete(void)
{
	if(PluginsType==ptListSelect)
	{
		RCursor<GPlugInList> Cur(*Data.Lists);
		for(Cur.Start();!Cur.End();Cur.Next())
			Cur()->Delete();
	}
	else
		Data.List->Delete();
}


//------------------------------------------------------------------------------
void GPlugInManager::CreateConfig(GSession* session)
{
	// For list and ordered plug-ins -> do nothing
	switch(PluginsType)
	{
		case ptList:
		case ptOrdered:
			Data.List->CreateConfig();
			break;

		case ptSelect:
			session->InsertParam(new R::RParamValue(Name,"None","Current plug-in"),Name);
			Data.List->CreateConfig();
			break;

		case ptListSelect:
		{
			RCursor<GPlugInList> Cur(*Data.Lists);
			for(Cur.Start();!Cur.End();Cur.Next())
			{
				session->InsertParam(new R::RParamValue(Cur()->Name,"None","Current plug-in"),Name,Cur()->Name);
				Cur()->CreateConfig();
			}
			break;
		}
	}
}


//------------------------------------------------------------------------------
void GPlugInManager::ReadConfig(GSession* session)
{
	// For list and ordered plug-ins -> do nothing
	switch(PluginsType)
	{
		case ptList:
			break;

		case ptOrdered:
		{
			// Parse each factory to find its level and re-order them after
			RCursor<GPlugInFactory> Factories(Data.List->Factories);
			for(Factories.Start();!Factories.End();Factories.Next())
			{
				if(GetPlugInType()==GPlugInManager::ptListSelect)
					Factories()->SetLevel(session->GetInt("Level",Name,Factories()->List,Factories()->Name));
				else
					Factories()->SetLevel(session->GetInt("Level",Name,Factories()->Name));
			}
			ReOrder();
			break;
		}

		case ptSelect:
			SetCurrentPlugIn(session->Get(Name,Name),RString::Null,false);
			break;

		case ptListSelect:
		{
			// Parse each list to determine the current plug-in
			RCursor<GPlugInList> Lists(*Data.Lists);
			for(Lists.Start();!Lists.End();Lists.Next())
				SetCurrentPlugIn(session->Get(Lists()->Name,Name,Lists()->Name),Lists()->Name,false);
			break;
		}
	}

	// Go trough each plug-in
	if(PluginsType==ptListSelect)
	{
		RCursor<GPlugInList> Lists(*Data.Lists);
		for(Lists.Start();!Lists.End();Lists.Next())
		{
			RCursor<GPlugIn> PlugIns(Lists()->Plugins);
			for(PlugIns.Start();!PlugIns.End();PlugIns.Next())
			{
				PlugIns()->ApplyConfig();
			}
		}
	}
	else
	{
		RCursor<GPlugIn> PlugIns(Data.List->Plugins);
		for(PlugIns.Start();!PlugIns.End();PlugIns.Next())
		{
			PlugIns()->ApplyConfig();
		}
	}
}


//------------------------------------------------------------------------------
void GPlugInManager::SaveConfig(GSession* session)
{
	// For list and ordered plug-ins -> do nothing
	switch(PluginsType)
	{
		case ptList:
			break;

		case ptOrdered:
		{
			// Parse each factory to find its level and re-order them after
			RCursor<GPlugInFactory> Factories(Data.List->Factories);
			for(Factories.Start();!Factories.End();Factories.Next())
			{
				if(GetPlugInType()==GPlugInManager::ptListSelect)
					session->SetInt("Level",Factories()->GetLevel(),Name,Factories()->List,Factories()->Name);
				else
					session->SetInt("Level",Factories()->GetLevel(),Name,Factories()->Name);
			}
			break;
		}

		case ptSelect:
		{
			R::RString Default;
			if(Data.List->Current)
				Default=Data.List->Current->GetName();
			else
				Default="None";
			session->Set(Name,Default,Name);
			break;
		}

		case ptListSelect:
		{
			// Parse each list to determine the current plug-in
			RCursor<GPlugInList> Lists(*Data.Lists);
			for(Lists.Start();!Lists.End();Lists.Next())
			{
				R::RString Default;
				if(Lists()->Current)
					Default=Lists()->Current->GetName();
				else
					Default="None";
				session->Set(Lists()->Name,Default,Name,Lists()->Name);
			}
			break;
		}
	}
}


//------------------------------------------------------------------------------
void GPlugInManager::InitPlugIns(GSession*)
{
	// Go trough each plug-in
	if(PluginsType==ptListSelect)
	{
		RCursor<GPlugInList> Lists(*Data.Lists);
		for(Lists.Start();!Lists.End();Lists.Next())
		{
			RCursor<GPlugIn> PlugIns(Lists()->Plugins);
			for(PlugIns.Start();!PlugIns.End();PlugIns.Next())
				PlugIns()->Init();
		}
	}
	else
	{
		RCursor<GPlugIn> PlugIns(Data.List->Plugins);
		for(PlugIns.Start();!PlugIns.End();PlugIns.Next())
			PlugIns()->Init();
	}
}


//------------------------------------------------------------------------------
void GPlugInManager::RegisterPlugIn(GPlugIn* plugin,bool enable)
{
	// Insert the factory in the right plug-ins list
	GPlugInList* List(0);
	if(PluginsType==ptListSelect)
	{
		List=Data.Lists->GetPtr(plugin->GetFactory()->GetList());
		if(!List)
			ThrowGException("Invalid list of plug-ins '"+plugin->GetFactory()->GetList()+"' in '"+Name+"'");
	}
	else
	{
		if(!Data.List)
			Data.List=new GPlugInList(this,RString::Null);
		List=Data.List;
	}
	if(enable)
		List->Plugins.InsertPtr(plugin);
	else
		List->Plugins.DeletePtr(*plugin);
}


//-----------------------------------------------------------------------------
RCursor<GPlugInList> GPlugInManager::GetPlugInLists(void) const
{
	if(PluginsType==ptListSelect)
		return(RCursor<GPlugInList>(*Data.Lists));
	return(RCursor<GPlugInList>());
}


//-----------------------------------------------------------------------------
GPlugInList* GPlugInManager::GetPlugInList(const R::RString& list,int need) const
{
	if(PluginsType!=ptListSelect)
		ThrowGException("No plug-ins list for manager '"+Name+"'");
	GPlugInList* List(Data.Lists->GetPtr(list));
	if((!List)&&need)
		ThrowGException("No plug-ins list '"+list+"' for manager '"+Name+"'");
	return(List);
}


//-----------------------------------------------------------------------------
GPlugInFactory* GPlugInManager::GetFactory(const R::RString& name,const R::RString& list,int need) const
{
	GPlugInList* List;
	if(PluginsType==ptListSelect)
	{
		List=Data.Lists->GetPtr(list);
		if(!List)
			ThrowGException("No type '"+list+"' available for '"+Name+"'");
	}
	else
		List=Data.List;

	GPlugInFactory* fac(List->Factories.GetPtr(name,PluginsType!=ptOrdered));
	if((!fac)&need)
		throw GException("No plug-in '"+name+"' available for '"+Name+"'");
	return(fac);
}


//-----------------------------------------------------------------------------
size_t GPlugInManager::GetNbFactories(const RString& list) const
{
	GPlugInList* List;
	if(PluginsType==ptListSelect)
	{
		List=Data.Lists->GetPtr(list);
		if(!List)
			ThrowGException("No type '"+list+"' available for '"+Name+"'");
	}
	else
		List=Data.List;

	return(List->GetNbFactories());
}


//-----------------------------------------------------------------------------
R::RCursor<GPlugInFactory> GPlugInManager::GetFactories(const R::RString& list) const
{
	GPlugInList* List;
	if(PluginsType==ptListSelect)
	{
		List=Data.Lists->GetPtr(list);
		if(!List)
			ThrowGException("No type '"+list+"' available for '"+Name+"'");
	}
	else
		List=Data.List;
	return(RCursor<GPlugInFactory>(List->Factories));
}


//-----------------------------------------------------------------------------
size_t GPlugInManager::GetNbPlugIns(const RString& list) const
{
	GPlugInList* List;
	if(PluginsType==ptListSelect)
	{
		List=Data.Lists->GetPtr(list);
		if(!List)
			ThrowGException("No type '"+list+"' available for '"+Name+"'");
	}
	else
		List=Data.List;
	return(List->GetNbPlugIns());
}


//------------------------------------------------------------------------------
void GPlugInManager::SetCurrentPlugIn(const R::RString& name,const R::RString& list,int need)
{
	GPlugInList* List;
	if(PluginsType==ptListSelect)
	{
		List=Data.Lists->GetPtr(list);
		if((!List)&&need)
			ThrowGException("No type '"+list+"' available for '"+Name+"'");
		if(!List)
			return;
	}
	else if(PluginsType==ptSelect)
		List=Data.List;
	else
		return;

	List->Current=List->Factories.GetPtr(name);
	if((!List->Current)&&need)
	{
		if(PluginsType==ptListSelect)
			ThrowGException("No plug-in '"+name+"' available for '"+Name+"' in category '"+list+"'");
		else
			ThrowGException("No plug-in '"+name+"' available for '"+Name+"'");
	}
}


//------------------------------------------------------------------------------
GPlugInFactory* GPlugInManager::GetCurrentFactory(const R::RString& list,int need) const
{
	GPlugInList* List;
	if(PluginsType==ptListSelect)
	{
		List=Data.Lists->GetPtr(list);
		if(!List)
			ThrowGException("No type '"+list+"' available for '"+Name+"'");
	}
	else
		List=Data.List;

	if((!List->Current)&&need)
		ThrowGException("No current plug-in available for '"+Name+"'");
	return(List->Current);
}


//------------------------------------------------------------------------------
GPlugInManager::~GPlugInManager(void)
{
	if(PluginsType==ptListSelect)
		delete Data.Lists;
	else
		delete Data.List;
}
