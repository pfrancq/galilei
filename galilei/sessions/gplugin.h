/*

	GALILEI Research Project

	GPlugin.h

	Generic Plug-In - Header.

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
#ifndef GPluginH
#define GPluginH


//------------------------------------------------------------------------------
// include file for R
#include <rxmltag.h>
#include <rxmlstruct.h>
#include <rconfig.h>


//-----------------------------------------------------------------------------
// include files for ANSI C/C++
#include <ctype.h>
#include <stdexcept>
#include <dirent.h>


//-----------------------------------------------------------------------------
// include file to manage dynamic libraries
#include <dlfcn.h>


//-----------------------------------------------------------------------------
// include file for GALILEI
#include <galilei.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
* Type of a function used to show the about box of a plug-in.
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
	* Constructor of the plug-in.
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
	* Get the factory of the plug-in.
	*/
	factory* GetFactory(void) const {return(Factory);}

	/**
	* Comparison method needed by R::RContainer.
	* @param plugin          Plug-in to compare.
	*/
	int Compare(const GPlugin& plugin) const;

	/**
	* Get the name of the Plug-in
	*/
	R::RString GetPlugInName(void) const;

	/**
	 * Get the session associated with the plug-in.
	 */
	GSession* GetSession(void) const {return(Session);}

	/**
	* Destruct of the plug-in.
	*/
	virtual ~GPlugin(void);
};


//-----------------------------------------------------------------------------
/**
 * The GPluginConfig class provides a generic configuration for a plug-in. All
 * factories should inherit from this class. Moreover, it provides a unique
 * class to configure all plug-ins.
 * @short Generic PlugIn Configuration
 * @author Pascal Francq
 */
class GPluginConfig : public R::RConfig
{
protected:

	/**
	* Name of the plug-in.
	*/
	R::RString Name;

	/**
	* Specific order of the plug-in in the list.
	*/
	int Level;

public:

	/**
	* Constructor.
	* @param m               Manager of the plug-in.
	* @param n               Name of the Factory/Plug-in ('/' are replaced by
	*                        '-' for the configuration file).
	* @param f               Library of the Factory/Plug-in.
	*/
	GPluginConfig(const R::RString& n) : R::RConfig(), Name(n), Level(0) {}

	/**
	* Get the level of the plug-in.
	*/
	int GetLevel(void) const {return(Level);}

	/**
	* Set the level of the plug-in.
	* @param level           Level of the plug-in.
	*/
	void SetLevel(int level) {Level=level;}

	/**
	* Name of the plug-in.
	*/
	R::RString GetName(void) const {return(Name);}

	/**
	* Create a plug-in.
	*/
	virtual void Create(void)=0;

	/**
	* Create a plug-in.
	*/
	virtual void Delete(void)=0;

	/**
	* Create a plug-in.
	*/
	virtual void Create(GSession* session)=0;

	/**
	* Delete a plug-in.
	*/
	virtual void Delete(GSession* session)=0;

	/**
	* Show 'about' information.
	*/
	virtual void About(void)=0;

	/**
	* Configure the parameters.
	*/
	virtual void Configure(void)=0;

	/**
	* Apply the configuration eventually to the plug-in.
	*/
	virtual void Apply(void)=0;

	/**
	* Specify if an about box exist.
	*/
	virtual bool HasAbout(void) const=0;

	/**
	* Specify if a configure box exist.
	*/
	virtual bool HasConfigure(void) const=0;

	/**
	* Specify if the plug-in is created.
	*/
	virtual bool IsCreated(void) const=0;

	/**
	 * Destruct the generic factory.
	 */
	virtual ~GPluginConfig(void) {}
};


//-----------------------------------------------------------------------------
/**
* The GFactoryPlugin class provides a template for a generic plug-in factory. A
* factory handles the loading of the dynamic library containing the plug-in.
* @author Pascal Francq
* @short Generic Plug-in Factory.
*/
template<class factory,class plugin,class mng>
	class GFactoryPlugin : public GPluginConfig
{
public:

	/**
	* Type of a function used to show dialog box of a plug-in.
	*/
	typedef void* (*Configure_t)(GFactoryPlugin*);

protected:

	/**
	* Plug-in Manager associated.
	*/
	mng* Mng;

	/**
	* Pointer to the plug-in.
	*/
	plugin* Plugin;

	/**
	* File of the library.
	*/
	R::RString Lib;

	/**
	* Pointer to a function showing the about box.
	*/
	About_t AboutDlg;

	/**
	* Pointer to a function showing the configure box.
	* @param f               Factory of the plug-in to configure.
	*/
	void* ConfigDlg;

	/**
	* The Handle(void *) to maintain the handler for the opened library.
	*/
	void* Handle;

	/**
	* The Handle(void *) to maintain the handler for the opened library.
	*/
	void* HandleDlg;

public:

	/**
	* Constructor.
	* @param m               Manager of the plug-in.
	* @param n               Name of the Factory/Plug-in ('/' are replaced by
	*                        '-' for the configuration file).
	* @param f               Library of the Factory/Plug-in.
	*/
	GFactoryPlugin(mng* m,const R::RString& n,const R::RString& f);

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
	* Set the Handler of the library containing the dailog box.
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
	int Compare(const R::RString& name) const;

	/**
	* Comparison method needed by R::RContainer.
	* @param level           Level to compare.
	*/
	int Compare(const size_t level) const;

	/**
	* Create a new plug-in and eventually connect it to the session.
	*/
	virtual plugin* NewPlugIn(void)=0;

	/**
	* Delete a plug-in and eventually disconnect it to the session.
	*/
	virtual void DeletePlugIn(plugin* plug)=0;

	/**
	* Create a plug-in.
	*/
	virtual void Create(void)
	{
		if(Plugin) return;
		Plugin=NewPlugIn();
		Plugin->ApplyConfig();
		Mng->EnablePlugIn(Plugin);
	}

	/**
	* Create a plug-in.
	*/
	virtual void Delete(void)
	{
		if(!Plugin) return;
		Mng->DisablePlugIn(Plugin);
		DeletePlugIn(Plugin);
		Plugin=0;
	}

	/**
	* Create a plug-in.
	*/
	virtual void Create(GSession* session)
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
	* Delete a plug-in.
	*/
	virtual void Delete(GSession* session)
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
	virtual void About(void);

	/**
	* Configure the parameters.
	*/
	virtual void Configure(void);

	/**
	* Apply the configuration eventually to the plug-in.
	*/
	virtual void Apply(void);

	/**
	* Specify if an about box exist.
	*/
	virtual bool HasAbout(void) const {return(AboutDlg);}

	/**
	* Specify if a configure box exist.
	*/
	virtual bool HasConfigure(void) const {return(ConfigDlg);}

	/**
	* Get the API Version of the plug-in.
	* @return C String.
	*/
	virtual const char* GetAPIVersion(void) const=0;

	/**
	* Return the manager of the plug-in.
	*/
	mng* GetMng(void) {return(Mng);}

	/**
	* Get the plug-in of this factory.
	*/
	plugin* GetPlugin(void) const {return(Plugin);}

	/**
	* Specify if the plug-in is created.
	*/
	virtual bool IsCreated(void) const {return(Plugin);}

	/**
	* Get the library of the plug-in.
	*/
	R::RString GetLib(void) const {return(Lib);}

	/**
	* Return the version of the plug-in.
	*/
	const char* GetVersion(void) const {return(this->Version);}

	/**
	* Destruct the factory.
	*/
	virtual ~GFactoryPlugin(void);

	// friend class
	friend class GPlugin<factory>;
};


//-------------------------------------------------------------------------------
#define CREATE_FACTORY(manager,factory,genericplugin,plugin,lib,API,name)      \
class TheFactory : public factory                                              \
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
