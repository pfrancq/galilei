/*

	GALILEI Research Project

	GPlugin.h

	Generic Plug-In - Header.

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
#ifndef GPluginH
#define GPluginH


//------------------------------------------------------------------------------
// include file for R
#include <rstd/rxmltag.h>
#include <rstd/rxmlstruct.h>

//-----------------------------------------------------------------------------
// include files for ANSI C/C++
#include <ctype.h>
#include <stdexcept>
#include <dirent.h>
using namespace std;
using namespace R;


//-----------------------------------------------------------------------------
// include file for dlopen
#include <dlfcn.h>


//-----------------------------------------------------------------------------
// include file for GALILEI
#include <sessions/galilei.h>
#include <sessions/gparams.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/**
* Type of a function used to show the about box of a plugin.
*/
typedef void (*About_t)();
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/**
* The GPluginManager class provides a template for a generic plug-in.
* @author Pascal Francq
* @short Plug-ins manager.
*/
class GGenericPluginManager
{
protected:

	/**
	* The name of the manager
	*/
	R::RString Name;

	/**
	* Version of the manager
	*/
	R::RString Version;

public :
	/**
	* Constructor for the manager of plug-ins manager
	*/
	GGenericPluginManager(R::RString name,R::RString version);

	/**
	* Connect to the session.
	*/
	virtual void Connect(GSession* sess)=0;

	/**
	* Disconnect to the session.
	*/
	virtual void Disconnect(GSession* sess)=0;

	/**
	* Load a plug-in and its dialog boxes.
	* @param dll             Name of the dynamic link library.
	* @param handle          Handle to the library contaioning the plug-in.
	* @param handleDlg       Handle to the library contaioning the dialogs.
	*/
	virtual void Load(const R::RString& dll,void* handle,void* handleDlg)=0;

	/**
	* Read Config for the current manager.
	*/
	virtual void ReadConfig(R::RXMLTag* t)=0;

	/**
	* Save Config for the current manager.
	*/
	virtual void SaveConfig(R::RXMLStruct* xml,R::RXMLTag* t)=0;

	/**
	* Method used to compare element from a Container.
	*/
	int Compare(const GGenericPluginManager& pm) const;

	/**
	* Method used to compare element from a Container.
	*/
	int Compare(const R::RString& name) const;

	/**
	* Get the name of the current Manager.
	*/
	R::RString GetName(void) const {return(Name);}

	/**
	* Get the name of the current Manager.
	*/
	R::RString GetVersion(void) const {return(Version);}

	/**
	* The destructor.
	*/
	virtual ~GGenericPluginManager(void);
};


//-----------------------------------------------------------------------------
template<class mng,class factory,class factoryInit,class plugin>
	class GPluginManager : public GGenericPluginManager, public R::RContainer<factory,true,true>
{
public:

	/**
	* Constructor for the manager of plug-ins manager
	*/
	GPluginManager(R::RString name,R::RString version)
		: GGenericPluginManager(name,version), R::RContainer<factory,true,true>(20,10)
		 {}

	virtual void Load(const R::RString& dll,void* handle,void* handleDlg)
	{
		char* error;

		// Try to create the factory
		factoryInit* initFac= (factoryInit*) dlsym(handle,"FactoryCreate");
		error=dlerror();
		if(error)
		{
			cerr<<error<<endl;
			return;
		}
		factory *myfactory= initFac(dynamic_cast<mng*>(this),dll);

		// Verify the versions of the factory and the session
		if(strcmp(Version,myfactory->GetAPIVersion()))
			return;

		// Register main plugin
		InsertPtr(myfactory);

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

	virtual void Connect(GSession* session)
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

	virtual void Disconnect(GSession* session)
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

	R::RCursor<factory> GetFactories(void) const
	{
		return(R::RCursor<factory>(*this));
	}
};


//-----------------------------------------------------------------------------
/**
* The GPLugin class provides a template for a generic plugin.
* @author Pascal Francq
* @short Generic Plugin.
*/
template<class factory>
	class GPlugin
{
protected:

	/**
	* Pointer to the factory.
	*/
	factory* Factory;

public:

	/**
	* Constructor of the plugin.
	* @param fac            Factory.
	*/
	GPlugin(factory* fac) : Factory(fac) {}

	/**
	* Connect to the session.
	*/
	virtual void Connect(GSession*) {};

	/**
	* Disconnect to the session.
	*/
	virtual void Disconnect(GSession*) {};

	/**
	* Configurations were applied from the factory.
	*/
	virtual void ApplyConfig(void) {}

	/**
	* Get the factory of the plugin.
	*/
	factory* GetFactory(void) const {return(Factory);}

	/**
	* Destructor of the plugin.
	*/
	virtual ~GPlugin(void) {}
};



//-----------------------------------------------------------------------------
/**
* The GPLuginPlugin class provides a template for a generic plugin factory. A
* factory handles the loading of the dynamic library containing the plugin.
* @author Pascal Francq
* @short Generic Plugin Factory.
*/
template<class factory,class plugin,class mng>
	class GFactoryPlugin : public GParams
{
public:
	/**
	* Type of a function used to show dialog box of a plugin.
	*/
	typedef void* (*Configure_t)(GFactoryPlugin*);

protected:

	/**
	* Plug-in Manager associated.
	*/
	mng* Mng;

	/**
	* Pointer to the plugin.
	*/
	plugin* Plugin;

	/**
	* Name of the library.
	*/
	R::RString Lib;

	/**
	* Pointer to a function showing the about box.
	*/
	About_t AboutDlg;

	/**
	* Pointer to a function showing the configure box.
	* @param f               Factory of the pluging to configure.
	*/
	void* ConfigDlg;

	/**
	* The Handle(void *) to maintain the (dlopen)handler for the opened library
	*/
	void* Handle;

	/**
	* The Handle(void *) to maintain the (dlopen)handler for the opened library
	*/
	void* HandleDlg;

public:

	/**
	* Constructor.
	* @param m               Manager of the plugin.
	* @param n               Name of the Factory/Plugin.
	* @param f               Lib of the Factory/Plugin.
	*/
	GFactoryPlugin(mng* m,const char* n,const char* f)
		: GParams(n), Mng(m), Plugin(0), Lib(f), AboutDlg(0), ConfigDlg(0), Handle(0), HandleDlg(0) {}

	/**
	* Get the manager of the factory.
	* @return Pointer to the manager.
	*/
	mng* GetMng(void) const {return(Mng);}

	/**
	* Set the about box.
	* @param dlg             Param to the function.
	*/
	void SetAbout(About_t dlg) {AboutDlg=dlg;}

	/**
	* Set the configure box.
	* @param dlg             Param to the function.
	*/
	void SetConfig(void* dlg) {ConfigDlg=dlg;}

	/**
	* Set the handler of the library.
	* @param handle         Handle of the library.
	*/

	void SetHandler(void* handle) {Handle=handle;}

	/**
	* Set the Handler of the Dlg library.
	* @param handleDlg      Handle of the dialog library
	*/
	void SetDlgHandler(void* handleDlg) {HandleDlg=handleDlg;}

	/**
	* Method needed by R::RContainer.
	*/
	int Compare(const factory& f) const {return(Name.Compare(f.Name));}

	/**
	* Method needed by R::RContainer.
	*/
	int Compare(const char* n) const {return(Name.Compare(n));}

	/**
	* Create a plugin.
	*/
	virtual void Create(void) throw(GException)= 0;

	/**
	* Create a plugin.
	*/
	virtual void Delete(void) throw(GException)= 0;

	/**
	* Create a plugin.
	*/
	virtual void Create(GSession*) throw(GException)= 0;

	/**
	* Create a plugin.
	*/
	virtual void Delete(GSession*) throw(GException)= 0;

	/**
	* Show 'about' information.
	*/
	void About(void)
	{
		if(AboutDlg)
			AboutDlg();
	}

	/**
	* Configure the parameters.
	*/
	void Configure(void)
	{
		if(ConfigDlg)
		{
			Configure_t config= (Configure_t) ConfigDlg ;
			config(dynamic_cast<factory*>(this));
		}
	}

	/**
	* Apply the configuration eventually to the plugin.
	*/
	void Apply(void) {if(Plugin) Plugin->ApplyConfig();}

	/**
	* Specify if an about box exist.
	*/
	bool HasAbout(void) const {return(AboutDlg);}

	/**
	* Specify if a configure box exist.
	*/
	bool HasConfigure(void) const {return(ConfigDlg);}

	/**
	* Get the API Version of the plugin.
	* @return C String.
	*/
	virtual const char* GetAPIVersion(void) const=0;

	/**
	* Return the manager of the plugin.
	*/
	mng* GetMng(void) {return(Mng);}

	/**
	* Get the plugin of this factory.
	*/
	plugin* GetPlugin(void) const {return(Plugin);}

	/**
	* Get the library of the plugin.
	*/
	RString GetLib(void) const {return(Lib);}

	/**
	* Return the version of the plugin.
	*/
	const char* GetVersion(void) const {return(this->Version);}

	/**
	* Read a configuration from a XML Tag.
	* @param parent          Parent Tag.
	*/
	void ReadConfig(R::RXMLTag* parent)
	{
		R::RCursor<R::RXMLTag> Cur(parent->GetNodes());
		R::RXMLTag* tag=0;

		// Find Tag
		for(Cur.Start();!Cur.End();Cur.Next())
		{
			if((Cur()->GetName()=="plugin")&&(Cur()->GetAttrValue("name")==Name))
			{
				tag=Cur();
				break;
			}
		}
		if(!tag) return;

		// Read Info
		if(tag->GetAttrValue("enable")=="True")
			Create();
		GParams::ReadConfig(tag);

		// Apply the information.
		Apply();
	}

	/**
	* Save a configuration to a XML Tag.
	* this tag.
	* @param xml             XML Structure.
	* @param parent          Parent tag.
	*/
	void SaveConfig(R::RXMLStruct* xml,R::RXMLTag* parent)
	{
		R::RXMLTag* tag=new R::RXMLTag("plugin");
		tag->InsertAttr("name",Name);
		xml->AddTag(parent,tag);
		if(Plugin)
			tag->InsertAttr("enable","True");
		else
			tag->InsertAttr("enable","False");
		GParams::SaveConfig(xml,tag);
	}

	/**
	* Destructor.
	*/
	virtual ~GFactoryPlugin(void)
	{
		if(Plugin)
			delete Plugin;

		if (Handle)
			dlclose(Handle);

		if (HandleDlg)
			dlclose(HandleDlg);
	}
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
