/*

	GALILEI Research Project

	GPlugIn.h

	Generic Plug-In - Implementation.

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



//-----------------------------------------------------------------------------
// include file for GALILEI
#include <gplugin.h>
#include <gpluginmanager.h>
#include <ggalileiapp.h>
using namespace GALILEI;
using namespace R;
using namespace std;



//-----------------------------------------------------------------------------
//
// class GPlugInFactory
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GPlugInFactory::GPlugInFactory(GPlugInManager* mng,const R::RString& name,const R::RString& desc,const R::RString& lib,const R::RString& list)
		: Name(name), Desc(desc),Level(0), Mng(mng), Plugin(0), Lib(lib),
	      AboutDlg(0), ConfigDlg(0), Handle(0), HandleDlg(0), List(list)
{
	RString Cat(mng->GetName());
	Cat.Replace('/','-');
}


//-----------------------------------------------------------------------------
int GPlugInFactory::Compare(const GPlugInFactory& f) const
{
	int a=Level-f.Level;
	if(!a)
		return(Name.Compare(f.Name));
	return(a);
}


//-----------------------------------------------------------------------------
int GPlugInFactory::Compare(const R::RString& name) const
{
	return(Name.Compare(name));
}


//-----------------------------------------------------------------------------
int GPlugInFactory::Compare(const size_t level) const
{
	return(Level-level);
}

//-----------------------------------------------------------------------------
void GPlugInFactory::Create(GSession* session)
{
	if(Plugin)
		return;
	Plugin=NewPlugIn(session);
	Plugin->ApplyConfig();
	Plugin->Init();
	Mng->RegisterPlugIn(Plugin,true);
}


//-----------------------------------------------------------------------------
void GPlugInFactory::Delete(void)
{
	if(!Plugin)
		return;
	Mng->RegisterPlugIn(Plugin,false);
	delete Plugin;
	Plugin=0;
}


//-----------------------------------------------------------------------------
void GPlugInFactory::About(void)
{
	if(AboutDlg)
		AboutDlg();
}


//-----------------------------------------------------------------------------
void GPlugInFactory::Configure(void)
{
	if(ConfigDlg)
	{
		Configure_t config=reinterpret_cast<Configure_t>(reinterpret_cast<size_t>(ConfigDlg));
		if(config(this)&&Plugin)
			Plugin->ApplyConfig();
	}
}


//-----------------------------------------------------------------------------
void GPlugInFactory::CreateConfig(void)
{
}


//-----------------------------------------------------------------------------
void GPlugInFactory::ApplyConfig(void)
{
	if(Plugin)
		Plugin->ApplyConfig();
}


//------------------------------------------------------------------------------
void GPlugInFactory::InsertParam(RParam* param)
{
	// Configuration is the session
	if(!GALILEIApp->GetSessionConfig())
		ThrowGException("No current session");
	if(Mng->GetPlugInType()==GPlugInManager::ptListSelect)
		GALILEIApp->GetSessionConfig()->InsertParam(param,Mng->GetName(),List,Name);
	else
		GALILEIApp->GetSessionConfig()->InsertParam(param,Mng->GetName(),Name);
}


//------------------------------------------------------------------------------
RParam* GPlugInFactory::GetParam(const R::RString& name)
{
	// Configuration is the session
	if(!GALILEIApp->GetSessionConfig())
		ThrowGException("No current session");
	if(Mng->GetPlugInType()==GPlugInManager::ptListSelect)
		return(GALILEIApp->GetSessionConfig()->FindParam<RParam>(name,Mng->GetName(),List,Name));
	else
		return(GALILEIApp->GetSessionConfig()->FindParam<RParam>(name,Mng->GetName(),Name));
}


//-----------------------------------------------------------------------------
GPlugInFactory::~GPlugInFactory(void)
{
	delete Plugin;

	if (Handle)
		dlclose(Handle);

	if (HandleDlg)
		dlclose(HandleDlg);
}



//-----------------------------------------------------------------------------
//
// class GPlugIn
//
//-----------------------------------------------------------------------------

//------------------------------------------------------------------------------
GPlugIn::GPlugIn(GSession* session,GPlugInFactory* fac)
  	: Factory(fac), Session(session)
{
	if(!fac)
		std::cout<<"Big Problem"<<std::endl;
}


//------------------------------------------------------------------------------
int GPlugIn::Compare(const GPlugIn& plugin) const
{
	return(Factory->Compare(*plugin.GetFactory()));
}


//------------------------------------------------------------------------------
int GPlugIn::Compare(const RString& plugin) const
{
	return(Factory->Compare(plugin));
}


//------------------------------------------------------------------------------
RString GPlugIn::GetName(void) const
{
	return(Factory->GPlugInFactory::GetName());
}


//------------------------------------------------------------------------------
RString GPlugIn::GetDesc(void) const
{
	return(Factory->GPlugInFactory::GetDesc());
}


//------------------------------------------------------------------------------
void GPlugIn::ApplyConfig(void)
{
}


//------------------------------------------------------------------------------
void GPlugIn::Init(void)
{
}


//------------------------------------------------------------------------------
GPlugIn::~GPlugIn(void)
{
}
