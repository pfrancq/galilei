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
#include <rxmltag.h>
#include <rxmlstruct.h>

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
#include <galilei.h>
#include <gparams.h>


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
* The GPlugin class provides a template for a generic plug-in.
* @author Pascal Francq
* @short Generic Plug-in.
*/
template<class factory>
	class GPlugin
{
protected:

	/**
	* Pointer to the factory.
	*/
	factory* Factory;

	/**
	* Session.
	*/
	GSession* Session;

public:

	/**
	* Constructor of the plugin.
	* @param fac            Factory.
	*/
	GPlugin(factory* fac);

	/**
	* Connect to the session.
	* @param session         Pointer to the session.
	*/
	virtual void Connect(GSession* session);

	/**
	* Disconnect to the session.
	* @param session         Pointer to the session.
	*/
	virtual void Disconnect(GSession* session);

	/**
	* Configurations were applied from the factory.
	*/
	virtual void ApplyConfig(void);

	/**
	* Get the factory of the plugin.
	*/
	factory* GetFactory(void) const {return(Factory);}

	/**
	* Compararison method needed by R::RContainer.
	* @param plugin          Plugin to compare.
	*/
	int Compare(const GPlugin& plugin) const;

	/**
	* Get the name of the Plug-in
	*/
	R::RString GetPlugInName(void) const;

	/**
	* Destructor of the plugin.
	*/
	virtual ~GPlugin(void);
};


//-----------------------------------------------------------------------------
/**
* The GFactoryPlugin class provides a template for a generic plugin factory. A
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

	/**
	* Specific order of the plug-in in the list.
	*/
	int Level;

public:

	/**
	* Constructor.
	* @param m               Manager of the plugin.
	* @param n               Name of the Factory/Plugin.
	* @param f               Lib of the Factory/Plugin.
	*/
	GFactoryPlugin(mng* m,const char* n,const char* f);

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
	* Comparison method needed by R::RContainer.
	* @param f               Factory to compare.
	*/
	int Compare(const factory& f) const;

	/**
	* Comparison method needed by R::RContainer.
	* @param name            Name to compare.
	*/
	int Compare(const RString& name) const;

	/**
	* Comparison method needed by R::RContainer.
	* @param level           Level to compare.
	*/
	int Compare(const unsigned int level) const;

	/**
	* Get the level of the plug-in.
	* @return int.
	*/
	int GetLevel(void) const {return(Level);}

	/**
	* Create a new plug-in and eventually connect it to the session.
	*/
	virtual plugin* NewPlugIn(void)=0;

	/**
	* Delete a plug-in and eventually disconnect it to the session.
	*/
	virtual void DeletePlugIn(plugin* plug)=0;

	/**
	* Create a plugin.
	*/
	virtual void Create(void) throw(GException)
	{
		if(Plugin) return;
		Plugin=NewPlugIn();
		Plugin->ApplyConfig();
		Mng->EnablePlugIn(Plugin);
	}

	/**
	* Create a plugin.
	*/
	virtual void Delete(void) throw(GException)
	{
		if(!Plugin) return;
		Mng->DisablePlugIn(Plugin);
		DeletePlugIn(Plugin);
		Plugin=0;
	}

	/**
	* Create a plugin.
	*/
	virtual void Create(GSession* session) throw(GException)
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

	/**
	* Create a plugin.
	*/
	virtual void Delete(GSession* session) throw(GException)
	{
		if(!Plugin) return;
		if(session)
			Plugin->Disconnect(session);
		Mng->DisablePlugIn(Plugin);
		DeletePlugIn(Plugin);
		Plugin=0;
	}

	/**
	* Show 'about' information.
	*/
	void About(void);

	/**
	* Configure the parameters.
	*/
	void Configure(void);

	/**
	* Apply the configuration eventually to the plugin.
	*/
	void Apply(void);

	/**
	* Specify if an about box exist.
	* @return bool
	*/
	bool HasAbout(void) const {return(AboutDlg);}

	/**
	* Specify if a configure box exist.
	* @return bool
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
	void ReadConfig(R::RXMLTag* parent);

	/**
	* Save a configuration to a XML Tag.
	* this tag.
	* @param xml             XML Structure.
	* @param parent          Parent tag.
	*/
	void SaveConfig(R::RXMLStruct* xml,R::RXMLTag* parent);

	/**
	* Destructor.
	*/
	virtual ~GFactoryPlugin(void);
};


//-------------------------------------------------------------------------------
#define CREATE_FACTORY(manager,factory,genericplugin,plugin,lib,API,name)      \
	class TheFactory : public factory                                          \
{                                                                              \
	static factory* Inst;                                                      \
	TheFactory(manager* mng,const char* l) : factory(mng,name,l)               \
	{                                                                          \
		plugin::CreateParams(this);                                            \
	}                                                                          \
	virtual ~TheFactory(void) {}                                               \
public:                                                                        \
	static factory* CreateInst(manager* mng,const char* l)                     \
	{                                                                          \
		if(!Inst)                                                              \
			Inst = new TheFactory(mng,l);                                      \
		return(Inst);                                                          \
	}                                                                          \
	virtual const char* GetAPIVersion(void)      const {return(API);}          \
		                                                                       \
	virtual genericplugin* NewPlugIn(void)                                     \
	{                                                                          \
		return(new plugin(this));                                              \
	}                                                                          \
	virtual void DeletePlugIn(genericplugin* plug)                             \
	{                                                                          \
		delete plug;                                                           \
	}                                                                          \
};                                                                             \
factory* TheFactory::Inst = 0;                                                 \
                                                                               \
extern "C" factory* FactoryCreate(manager* mng,const char* l)                  \
{                                                                              \
	return(TheFactory::CreateInst(mng,l));                                     \
}                                                                              \
extern "C" const char* LibType(void)                                           \
{                                                                              \
	return(lib);                                                               \
}


//-----------------------------------------------------------------------------
// Template implementation
#include <gplugin.hh>


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
