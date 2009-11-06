/*

	GALILEI Research Project

	GPlugin.h

	Generic Plug-In - Implementation.

	Copyright 2003-2009 by Pascal Francq (pascal@francq.info).
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



//-----------------------------------------------------------------------------
// include file for GALILEI
#include <gplugin.h>
#include <gpluginmanager.h>
using namespace GALILEI;
using namespace R;



//-----------------------------------------------------------------------------
//
// class GPlugin
//
//-----------------------------------------------------------------------------

//------------------------------------------------------------------------------
GPlugin::GPlugin(GPluginFactory* fac)
  	: Factory(fac), Session(0)
{
	if(!fac)
		std::cout<<"Big Problem"<<std::endl;
}


//------------------------------------------------------------------------------
void GPlugin::Connect(GSession* session)
{
	Session=session;
}


//------------------------------------------------------------------------------
void GPlugin::Disconnect(GSession*)
{
	Session=0;
}


//------------------------------------------------------------------------------
void GPlugin::ApplyConfig(void)
{
}


//------------------------------------------------------------------------------
int GPlugin::Compare(const GPlugin& plugin) const
{
	return(Factory->Compare(*plugin.GetFactory()));
}


//------------------------------------------------------------------------------
int GPlugin::Compare(const RString& plugin) const
{
	return(Factory->Compare(plugin));
}


//------------------------------------------------------------------------------
RString GPlugin::GetName(void) const
{
	return(Factory->GPluginFactory::GetName());
}


//------------------------------------------------------------------------------
RString GPlugin::GetDesc(void) const
{
	return(Factory->GPluginFactory::GetDesc());
}


//------------------------------------------------------------------------------
GPlugin::~GPlugin(void)
{
}



//-----------------------------------------------------------------------------
//
// class GPluginFactory
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GPluginFactory::GPluginFactory(GPluginManager* mng,const R::RString& name,const R::RString& desc,const R::RString& lib,const R::RString& list)
		: Name(name), Desc(desc),Level(0), Mng(mng), Plugin(0), Lib(lib),
	      AboutDlg(0), ConfigDlg(0), Handle(0), HandleDlg(0), List(list)
{
	RString Cat(mng->GetName());
	Cat.Replace('/','-');

	if(mng->GetPluginsType()==GPluginManager::ptListSelect)
	{
		RString ListName(List);
		ListName.Replace('/','-');
		SetConfigInfos("lib/galilei/plugins/"+Cat+"/"+ListName,lib.Mid(0,lib.GetLen()-3));
	}
	else
	{
		SetConfigInfos("lib/galilei/plugins/"+Cat,lib.Mid(0,lib.GetLen()-3));
		InsertParam(new R::RParamValue("Enable",false));
	}
}


//-----------------------------------------------------------------------------
int GPluginFactory::Compare(const GPluginFactory& f) const
{
	int a=Level-f.Level;
	if(!a)
		return(Name.Compare(f.Name));
	return(a);
}


//-----------------------------------------------------------------------------
int GPluginFactory::Compare(const R::RString& name) const
{
	return(Name.Compare(name));
}


//-----------------------------------------------------------------------------
int GPluginFactory::Compare(const size_t level) const
{
	return(Level-level);
}


//-----------------------------------------------------------------------------
void GPluginFactory::Create(void)
{
	if(Plugin) return;
	Plugin=NewPlugIn();
	Plugin->ApplyConfig();
	Mng->EnablePlugIn(Plugin);
}


//-----------------------------------------------------------------------------
void GPluginFactory::Delete(void)
{
	if(!Plugin) return;
	Mng->DisablePlugIn(Plugin);
	DeletePlugIn(Plugin);
	Plugin=0;
}

//-----------------------------------------------------------------------------
void GPluginFactory::Create(GSession* session)
{
	if(!Plugin)
	{
		Plugin=NewPlugIn();
		Plugin->ApplyConfig();
		Mng->EnablePlugIn(Plugin);
	}
	if(session)
		Plugin->Connect(session);
}


//-----------------------------------------------------------------------------
void GPluginFactory::Delete(GSession* session)
{
	if(!Plugin) return;
	if(session)
		Plugin->Disconnect(session);
	Mng->DisablePlugIn(Plugin);
	DeletePlugIn(Plugin);
	Plugin=0;
}


//-----------------------------------------------------------------------------
void GPluginFactory::About(void)
{
	if(AboutDlg)
		AboutDlg();
}


//-----------------------------------------------------------------------------
void GPluginFactory::Configure(void)
{
	if(ConfigDlg)
	{
		Configure_t config=reinterpret_cast<Configure_t>(reinterpret_cast<size_t>(ConfigDlg));
		config(this);
	}
}


//-----------------------------------------------------------------------------
void GPluginFactory::Apply(void)
{
	if(Plugin)
		Plugin->ApplyConfig();
}


//-----------------------------------------------------------------------------
GPluginFactory::~GPluginFactory(void)
{
	// Save configuration file
	Save();

	delete Plugin;

	if (Handle)
		dlclose(Handle);

	if (HandleDlg)
		dlclose(HandleDlg);
}
