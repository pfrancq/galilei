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
using namespace GALILEI;
using namespace R;
using namespace std;



//-----------------------------------------------------------------------------
//
// class GPlugInList
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GPlugInList::GPlugInList(R::RString name)
	: Name(name), Factories(20,10), Plugins(20,10), Current(0)
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
void GPlugInList::Connect(GSession* session)
{
	R::RCursor<GPlugIn> Cur(Plugins);
	for(Cur.Start();!Cur.End();Cur.Next())
		Cur()->Connect(session);
}


//-----------------------------------------------------------------------------
void GPlugInList::Disconnect(GSession* session)
{
	R::RCursor<GPlugIn> Cur(Plugins);
	for(Cur.Start();!Cur.End();Cur.Next())
		Cur()->Disconnect(session);
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
		Data.List=new GPlugInList(RString::Null);
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
void GPlugInManager::Load(const R::RString& dll,void* handle,void* handleDlg,R::RConfig* config)
{
	typedef GPlugInFactory* FactoryInit(GPlugInManager*,const char*);

	char* error;

	// Try to create the GPlugInFactory
	FactoryInit* initFac=reinterpret_cast<FactoryInit*>(reinterpret_cast<size_t>(dlsym(handle,"FactoryCreate")));
	error=dlerror();
	if(error)
	{
		std::cerr<<error<<std::endl;
		return;
	}
	GPlugInFactory *myGPlugInFactory= initFac(this,dll);

	// Verify the versions of the GPlugInFactory and the session
	if(Version.Compare(myGPlugInFactory->GetAPIVersion()))
		return;

	// Register main plug-in
	RegisterFactory(myGPlugInFactory,config);

	// Try to create the dialogs if necessary
	if(!handleDlg)
		return;
	About_t about=reinterpret_cast<About_t>(reinterpret_cast<size_t>(dlsym(handleDlg,"About")));
	error=dlerror();
	if(!error)
		myGPlugInFactory->SetAbout(about);
	void* configdll= dlsym(handleDlg,"Configure");
	error=dlerror();
	if(!error)
		myGPlugInFactory->SetConfig(configdll);
}


//-----------------------------------------------------------------------------
void GPlugInManager::Connect(GSession* session)
{
	if(PluginsType==ptListSelect)
	{
		RCursor<GPlugInList> Cur(*Data.Lists);
		for(Cur.Start();!Cur.End();Cur.Next())
			Cur()->Connect(session);
	}
	else
		Data.List->Connect(session);
}


//-----------------------------------------------------------------------------
void GPlugInManager::Disconnect(GSession* session)
{
	if(PluginsType==ptListSelect)
	{
		RCursor<GPlugInList> Cur(*Data.Lists);
		for(Cur.Start();!Cur.End();Cur.Next())
			Cur()->Disconnect(session);
	}
	else
		Data.List->Disconnect(session);
}


//------------------------------------------------------------------------------
void GPlugInManager::CreateConfig(R::RConfig* config)
{
	// For list and ordered plug-ins -> do nothing
	switch(PluginsType)
	{
		case ptOrdered:
			config->InsertParam(new R::RParamList(Name),"Plugins");
			break;

		case ptSelect:
			config->InsertParam(new R::RParamValue(Name,"None"),"Plugins");
			break;

		case ptListSelect:
			config->InsertParam(new RParamStruct(Name),"Plugins");
			break;

		default:
			break;
	}
}


//------------------------------------------------------------------------------
void GPlugInManager::ReadConfig(R::RConfig* config)
{
	// For list and ordered plug-ins -> do nothing
	switch(PluginsType)
	{
		case ptOrdered:
		{
			R::RParamList* param=config->FindParam<R::RParamList>(Name,"Plugins");
			int pos;
			R::RCursor<R::RString> Cur(param->GetList());
			for(Cur.Start(),pos=0;!Cur.End();Cur.Next(),pos++)
			{
				GPlugInFactory* fac(Data.List->Factories.GetPtr(*Cur(),false));
				if(fac)
					fac->SetLevel(pos);
			}
			ReOrder();
			break;
		}

		case ptSelect:
			SetCurrentPlugIn(config->Get(Name,"Plugins"),RString::Null,false);
			break;

		case ptListSelect:
		{
			RParamStruct* ptr(config->FindParam<RParamStruct>("Measures","Plugins"));
			RCursor<RParam> Cur(ptr->GetStruct());
			for(Cur.Start();!Cur.End();Cur.Next())
				SetCurrentPlugIn(dynamic_cast<RParamValue*>(Cur())->Get(),Cur()->GetName(),false);
			break;
		}

		default:
			break;
	}
}


//------------------------------------------------------------------------------
void GPlugInManager::SaveConfig(R::RConfig* config)
{
	// For list and ordered plug-ins -> do nothing
	switch(PluginsType)
	{
		case ptOrdered:
		{
			R::RParamList* param=config->FindParam<R::RParamList>(Name,"Plugins");
			if(param)
			{
				param->Reset();
				R::RCursor<GPlugInFactory> Cur(Data.List->Factories);
				for(Cur.Start();!Cur.End();Cur.Next())
					param->Insert(Cur()->GetName());
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
			config->Set(Name,Default,"Plugins");
			break;
		}

		case ptListSelect:
		{
			RParamStruct* ptr=config->FindParam<RParamStruct>("Measures","Plugins");
			RCursor<GPlugInList> Cur(*Data.Lists);
			for(Cur.Start();!Cur.End();Cur.Next())
			{
				RParamValue* param=ptr->Get<RParamValue>(Cur()->GetName());
				RString Default;
				if(Cur()->Current)
					Default=Cur()->Current->GetName();
				else
					Default="None";
				param->Set(Default);
			}
			break;
		}

		default:
			break;
	}
}


//-----------------------------------------------------------------------------
void GPlugInManager::RegisterFactory(GPlugInFactory* fac,R::RConfig* config)
{
	// Find the correct list
	GPlugInList* List;
	if(PluginsType==ptListSelect)
	{
		List=Data.Lists->GetInsertPtr(fac->GetList());

		// Look if a param exist in the config structure
		RParamStruct* param(config->FindParam<RParamStruct>("Measures","Plugins"));
		RParam* Exist(param->Get<RParam>(fac->GetList()));
		if(!Exist)
			param->Insert(new RParamValue(fac->GetList(),"None"));
	}
	else
		List=Data.List;

	// Load and create the factory
	fac->Load(false);
	if(PluginsType==ptOrdered)
		fac->SetLevel(static_cast<int>(List->GetNbFactories()));

	// Create it
	if(fac->GetBool("Enable"))
		fac->Create();
	else
		fac->Apply();
	List->Factories.InsertPtr(fac);
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
void GPlugInManager::EnablePlugIn(GPlugIn* plug)
{
	GPlugInList* List;
	if(PluginsType==ptListSelect)
	{
		List=Data.Lists->GetPtr(plug->GetFactory()->GetList());
		if(!List)
			ThrowGException("No type '"+plug->GetFactory()->GetList()+"' available for '"+Name+"'");
	}
	else
		List=Data.List;

	List->Plugins.InsertPtr(plug);
	R::RConfig* config(plug->GetFactory());
	R::RParamValue* param(config->FindParam<R::RParamValue>("Enable"));
	if(param)
		param->SetBool(true);
}


//-----------------------------------------------------------------------------
void GPlugInManager::DisablePlugIn(GPlugIn* plug)
{
	GPlugInList* List;
	if(PluginsType==ptListSelect)
	{
		List=Data.Lists->GetPtr(plug->GetFactory()->GetList());
		if(!List)
			ThrowGException("No type '"+plug->GetFactory()->GetList()+"' available for '"+Name+"'");
	}
	else
		List=Data.List;

	List->Plugins.DeletePtr(*plug);
	R::RConfig* config(plug->GetFactory());
	R::RParamValue* param(config->FindParam<R::RParamValue>("Enable"));
	if(param)
		param->SetBool(false);
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
