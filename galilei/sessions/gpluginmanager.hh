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
	FactoryInit* initFac= (FactoryInit*) dlsym(handle,"FactoryCreate");
	error=dlerror();
	if(error)
	{
		cerr<<error<<endl;
		return;
	}
	factory *myfactory= initFac(dynamic_cast<mng*>(this),dll);

	// Verify the versions of the factory and the session
	if(Version.Compare(myfactory->GetAPIVersion()))
		return;

	// Register main plugin
	RegisterFactory(myfactory);

	// Try to create the dialogs if necessary
	if(!handleDlg)
		return;
	About_t about = (About_t) dlsym(handleDlg,"About");
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
	void GPluginManager<mng,factory,plugin>::ReadConfig(RXMLTag* t)
{
	if(!t) return;
	R::RCursor<factory> Cur(*this);
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		Cur()->ReadConfig(t);
	}
	if(PluginsType!=ptSelect)
		return;
	try
	{
		SetCurrentMethod(t->GetAttrValue("current"));
	}
	catch(GException)
	{
	}
}


//------------------------------------------------------------------------------
template<class mng,class factory,class plugin>
	void GPluginManager<mng,factory,plugin>::SaveConfig(RXMLStruct* xml,RXMLTag* t)
{
	R::RCursor<factory> Cur(*this);
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		Cur()->SaveConfig(xml,t);
	}
	if(PluginsType!=ptSelect)
		return;
	if(Current)
		t->InsertAttr("current",Current->GetFactory()->GetName());
	else
		t->InsertAttr("current","None");
}


//-----------------------------------------------------------------------------
template<class mng,class factory,class plugin>
	void GPluginManager<mng,factory,plugin>::RegisterFactory(factory* fac)
{
	R::RContainer<factory,true,true>::InsertPtr(fac);
	if(PluginsType==ptOrdered)
		fac->InsertPtr(new GParamInt("Level",R::RContainer<factory,true,true>::GetNb()));
}


//-----------------------------------------------------------------------------
template<class mng,class factory,class plugin>
	factory* GPluginManager<mng,factory,plugin>::GetFactory(const R::RString& name) const
{
	return(R::RContainer<factory,true,true>::GetPtr(name,PluginsType!=ptOrdered));
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
}


//-----------------------------------------------------------------------------
template<class mng,class factory,class plugin>
	void GPluginManager<mng,factory,plugin>::DisablePlugIn(plugin* plug)
{
	R::RContainer<plugin,false,true>::DeletePtr(*plug);
}


//-----------------------------------------------------------------------------
template<class mng,class factory,class plugin>
	plugin* GPluginManager<mng,factory,plugin>::GetPlugIn(const R::RString& name) const
{
	factory* fac=GetFactory(name);
	if(fac)
		return(fac->GetPlugin());
	return(0);
}


//-----------------------------------------------------------------------------
template<class mng,class factory,class plugin>
	R::RCursor<plugin> GPluginManager<mng,factory,plugin>::GetPlugIns(void) const
{
	return(R::RCursor<plugin>(*this));
}


//------------------------------------------------------------------------------
template<class mng,class factory,class plugin>
	void GPluginManager<mng,factory,plugin>::SetCurrentMethod(const R::RString& name)
{
	if(PluginsType!=ptSelect)
		return;
	factory* fac=GetFactory(name);
	if(fac)
		Current=fac->GetPlugin();
	else
		Current=0;
	if(!Current)
		throw GException(RString("No plug-in '")+name+"' availaible for "+Name+".");
}