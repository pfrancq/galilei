/*

	GALILEI Research Project

	GPlugin.h

	Generic Plug-In - Inline Implementation.

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



//-----------------------------------------------------------------------------
//
// class GPlugin
//
//-----------------------------------------------------------------------------

//------------------------------------------------------------------------------
template<class factory>
	GPlugin<factory>::GPlugin(factory* fac)
	  	: Factory(fac), Session(0)
{
	if(!fac)
		std::cout<<"Big Problem"<<std::endl;
}


//------------------------------------------------------------------------------
template<class factory>
	void GPlugin<factory>::Connect(GSession* session)
{
	Session=session;
}


//------------------------------------------------------------------------------
template<class factory>
	void GPlugin<factory>::Disconnect(GSession*)
{
	Session=0;
}


//------------------------------------------------------------------------------
template<class factory>
	void GPlugin<factory>::ApplyConfig(void)
{
}


//------------------------------------------------------------------------------
template<class factory>
	int GPlugin<factory>::Compare(const GPlugin& plugin) const
{
	return(Factory->Compare(*plugin.GetFactory()));
}


//------------------------------------------------------------------------------
template<class factory>
	R::RString GPlugin<factory>::GetPlugInName(void) const
{
	return(Factory->factory::GetName());
}


//------------------------------------------------------------------------------
template<class factory>
	GPlugin<factory>::~GPlugin(void)
{
}



//-----------------------------------------------------------------------------
//
// class GFactoryPlugin
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
template<class factory,class plugin,class mng>
	GFactoryPlugin<factory,plugin,mng>::GFactoryPlugin(mng* m,const R::RString& n,const R::RString& f)
		: RConfig("lib/galilei/plugins/"+m->GetName(),f.Mid(0,f.GetLen()-3)), Mng(m), Plugin(0), Lib(f), Name(n),
	      AboutDlg(0), ConfigDlg(0), Handle(0), HandleDlg(0), Level(0)
{
	InsertParam(new R::RParamValue("Enable",false));
}


//-----------------------------------------------------------------------------
template<class factory,class plugin,class mng>
	int GFactoryPlugin<factory,plugin,mng>::Compare(const factory& f) const
{
	int a=Level-f.Level;
	if(!a)
		return(Name.Compare(f.Name));
	return(a);
}


//-----------------------------------------------------------------------------
template<class factory,class plugin,class mng>
	int GFactoryPlugin<factory,plugin,mng>::Compare(const R::RString& name) const
{
	return(Name.Compare(name));
}


//-----------------------------------------------------------------------------
template<class factory,class plugin,class mng>
	int GFactoryPlugin<factory,plugin,mng>::Compare(const size_t level) const
{
	return(Level-level);
}


//-----------------------------------------------------------------------------
template<class factory,class plugin,class mng>
	void GFactoryPlugin<factory,plugin,mng>::About(void)
{
	if(AboutDlg)
		AboutDlg();
}


//-----------------------------------------------------------------------------
template<class factory,class plugin,class mng>
	void GFactoryPlugin<factory,plugin,mng>::Configure(void)
{
	if(ConfigDlg)
	{
		Configure_t config= (Configure_t) ConfigDlg ;
		config(dynamic_cast<factory*>(this));
	}
}


//-----------------------------------------------------------------------------
template<class factory,class plugin,class mng>
	void GFactoryPlugin<factory,plugin,mng>::Apply(void)
{
	if(Plugin)
		Plugin->ApplyConfig();
}


//-----------------------------------------------------------------------------
template<class factory,class plugin,class mng>
	GFactoryPlugin<factory,plugin,mng>::~GFactoryPlugin(void)
{
	// Save configuration file
	Save();

	delete Plugin;

	if (Handle)
		dlclose(Handle);

	if (HandleDlg)
		dlclose(HandleDlg);
}
