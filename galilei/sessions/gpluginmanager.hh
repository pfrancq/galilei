/*

	GALILEI Research Project

	GPlugins.h

	Generic Plug-In Managers - Inline Implementation.

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
// class GPluginManager
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
template<class mng,class factory,class plugin>
	GPluginManager<mng,factory,plugin>::GPluginManager(R::RString name,R::RString version,tPluginsType type)
		: GGenericPluginManager(name,version,type), R::RContainer<factory,true,true>(20,10),
		  R::RContainer<plugin,false,true>(20,10), Current(0)
{
			  if(Name=="Documents-Profiles Similarities")
				  std::cout<<Name<<std::endl;
}


//------------------------------------------------------------------------------
template<class mng,class factory,class plugin>
	int GPluginManager<mng,factory,plugin>::Compare(const GPluginManager& pmng) const
{
	return(Name.Compare(pmng.Name));
}


//------------------------------------------------------------------------------
template<class mng,class factory,class plugin>
	int GPluginManager<mng,factory,plugin>::Compare(const R::RString& name) const
{
	return(Name.Compare(name));
}


//------------------------------------------------------------------------------
template<class mng,class factory,class plugin>
	void GPluginManager<mng,factory,plugin>::ReOrder(void)
{
	// Re-order the container of factories
	R::RContainer<factory,true,true>::ReOrder();
	R::RContainer<plugin,false,true>::ReOrder();
}


//-----------------------------------------------------------------------------
template<class mng,class factory,class plugin>
	void GPluginManager<mng,factory,plugin>::Load(const R::RString& dll,void* handle,void* handleDlg)
{
	typedef factory* FactoryInit(mng*,const char*);

	char* error;

	// Try to create the factory
	FactoryInit* initFac=reinterpret_cast<FactoryInit*>(reinterpret_cast<size_t>(dlsym(handle,"FactoryCreate")));
	error=dlerror();
	if(error)
	{
		std::cerr<<error<<std::endl;
		return;
	}
	factory *myfactory= initFac(dynamic_cast<mng*>(this),dll);

	// Verify the versions of the factory and the session
	if(Version.Compare(myfactory->GetAPIVersion()))
		return;

	// Register main plug-in
	RegisterFactory(myfactory);

	// Try to create the dialogs if necessary
	if(!handleDlg)
		return;
	About_t about=reinterpret_cast<About_t>(reinterpret_cast<size_t>(dlsym(handleDlg,"About")));
	error=dlerror();
	if(!error)
		myfactory->SetAbout(about);
	void* config= dlsym(handleDlg,"Configure");
	error=dlerror();
	if(!error)
		myfactory->SetConfig(config);
}


//-----------------------------------------------------------------------------
template<class mng,class factory,class plugin>
	void GPluginManager<mng,factory,plugin>::Connect(GSession* session)
{
	R::RCursor<factory> Cur;
	plugin* calc;

	Cur.Set(*this);
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		calc=Cur()->GetPlugin();
		if(calc)
			calc->Connect(session);
	}
}


//-----------------------------------------------------------------------------
template<class mng,class factory,class plugin>
	void GPluginManager<mng,factory,plugin>::Disconnect(GSession* session)
{
	R::RCursor<factory> Cur;
	plugin* calc;

	Cur.Set(*this);
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		calc=Cur()->GetPlugin();
		if(calc)
			calc->Disconnect(session);
	}
}


//------------------------------------------------------------------------------
template<class mng,class factory,class plugin>
	void GPluginManager<mng,factory,plugin>::CreateConfig(R::RConfig* config)
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

		default:
			break;
	}
}


//------------------------------------------------------------------------------
template<class mng,class factory,class plugin>
	void GPluginManager<mng,factory,plugin>::ReadConfig(R::RConfig* config)
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
				factory* fac=R::RContainer<factory,true,true>::GetPtr(*Cur(),false);
				if(fac)
					fac->SetLevel(pos);
			}
			break;
			ReOrder();
		}

		case ptSelect:
			SetCurrentMethod(config->Get(Name,"Plugins"),false);
			break;

		default:
			break;
	}
}


//------------------------------------------------------------------------------
template<class mng,class factory,class plugin>
	void GPluginManager<mng,factory,plugin>::SaveConfig(R::RConfig* config)
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
				R::RCursor<factory> Cur(*this);
				for(Cur.Start();!Cur.End();Cur.Next())
					param->Insert(Cur()->GetName());
			}
			break;
		}

		case ptSelect:
		{
			R::RString Default;
			if(Current)
				Default=Current->GetName();
			else
				Default="None";
			config->Set(Name,Default,"Plugins");
			break;
		}

		default:
			break;
	}
}


//-----------------------------------------------------------------------------
template<class mng,class factory,class plugin>
	void GPluginManager<mng,factory,plugin>::RegisterFactory(factory* fac)
{
	fac->Load(false);
	if(PluginsType==ptOrdered)
	{
		R::RConfig* config=R::App->GetConfig();
		config->InsertParam(new R::RParamList(Name),"Plugins");
		fac->SetLevel(static_cast<int>(R::RContainer<factory,true,true>::GetNb()));
	}
	if(fac->GetBool("Enable"))
		fac->Create();

	fac->Apply();
	R::RContainer<factory,true,true>::InsertPtr(fac);
}


//-----------------------------------------------------------------------------
template<class mng,class factory,class plugin>
	factory* GPluginManager<mng,factory,plugin>::GetFactory(const R::RString& name,bool need) const
{
	factory* fac;
	fac=R::RContainer<factory,true,true>::GetPtr(name,PluginsType!=ptOrdered);
	if((!fac)&need)
		throw GException(R::RString("No plug-in '")+name+"' availaible for "+Name+".");
	return(fac);
}


//-----------------------------------------------------------------------------
template<class mng,class factory,class plugin>
	R::RCursor<factory> GPluginManager<mng,factory,plugin>::GetFactories(void) const
{
	return(R::RCursor<factory>(*this));
}


//-----------------------------------------------------------------------------
template<class mng,class factory,class plugin>
	void GPluginManager<mng,factory,plugin>::EnablePlugIn(plugin* plug)
{
	R::RContainer<plugin,false,true>::InsertPtr(plug);
	R::RConfig* config=plug->GetFactory();
	R::RParamValue* param=config->FindParam<R::RParamValue>("Enable");
	if(param)
		param->SetBool(true);
}


//-----------------------------------------------------------------------------
template<class mng,class factory,class plugin>
	void GPluginManager<mng,factory,plugin>::DisablePlugIn(plugin* plug)
{
	R::RContainer<plugin,false,true>::DeletePtr(*plug);
	R::RConfig* config=plug->GetFactory();
	R::RParamValue* param=config->FindParam<R::RParamValue>("Enable");
	if(param)
		param->SetBool(false);
}


//-----------------------------------------------------------------------------
template<class mng,class factory,class plugin>
	plugin* GPluginManager<mng,factory,plugin>::GetPlugIn(const R::RString& name,bool need) const
{
	factory* fac=GetFactory(name,need);
	if(fac)
		return(fac->GetPlugin());
	if(need)
		throw GException(R::RString("No plug-in '")+name+"' availaible for "+Name+".");
	return(0);
}


//-----------------------------------------------------------------------------
template<class mng,class factory,class plugin>
	void GPluginManager<mng,factory,plugin>::ApplyConfig(R::RConfig* config)
{
	factory* Factory=dynamic_cast<factory*>(config);
	if(!Factory)
		throw GException("No valid configuration structure");
	Factory->Apply();
}


//-----------------------------------------------------------------------------
template<class mng,class factory,class plugin>
	R::RCursor<plugin> GPluginManager<mng,factory,plugin>::GetPlugIns(void) const
{
	return(R::RCursor<plugin>(*this));
}


//------------------------------------------------------------------------------
template<class mng,class factory,class plugin>
	void GPluginManager<mng,factory,plugin>::SetCurrentMethod(const R::RString& name,bool need)
{
	if(PluginsType!=ptSelect)
		return;
	Current=GetFactory(name,need);
	if((!Current)&&need)
		throw GException(R::RString("No plug-in '")+name+"' availaible for "+Name+".");
}


//------------------------------------------------------------------------------
template<class mng,class factory,class plugin>
	plugin* GPluginManager<mng,factory,plugin>::GetCurrentMethod(bool need) const
{
	if((!Current)&&need)
		throw GException("No current plug-in availaible for "+Name+".");
	if(!Current)
		return(0);
	plugin* plug=Current->GetPlugin();
	if((!plug)&&need)
		throw GException("No current plug-in availaible for "+Name+".");
	return(plug);
}


//------------------------------------------------------------------------------
template<class mng,class factory,class plugin>
	factory* GPluginManager<mng,factory,plugin>::GetCurrentFactory(bool need) const
{
	if((!Current)&&need)
		throw GException("No current plug-in availaible for "+Name+".");
	return(Current);
}


//------------------------------------------------------------------------------
template<class mng,class factory,class plugin>
	plugin* GPluginManager<mng,factory,plugin>::GetCurrentPlugIn(bool need) const
{
	if((!Current)&&need)
		throw GException("No current plug-in availaible for "+Name+".");
	if(!Current)
		return(0);
	return(Current->GetPlugin());
}
