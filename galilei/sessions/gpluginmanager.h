/*

	GALILEI Research Project

	GPlugins.h

	Generic Plug-In Managers - Header.

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
#ifndef GPluginsH
#define GPluginsH


//------------------------------------------------------------------------------
// include file for R
#include <rxmltag.h>
#include <rxmlstruct.h>
#include <rapplication.h>
#include <rconfig.h>


//-----------------------------------------------------------------------------
// include files for ANSI C/C++
#include <ctype.h>
#include <stdexcept>
#include <dirent.h>


//-----------------------------------------------------------------------------
// include file for dlopen
#include <dlfcn.h>


//-----------------------------------------------------------------------------
// include file for R
#include <rconfig.h>


//-----------------------------------------------------------------------------
// include file for GALILEI
#include <galilei.h>
#include <gplugin.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/*
* The GGenericPluginManager class provides a template for a generic plug-in manager.
* @author Pascal Francq
*/
class GGenericPluginManager
{
public:
	/**
	* Types of plug-ins.
	*/
	enum tPluginsType
	{
		ptSelect              /** Only one plug-in can be selected.*/,
		ptOrdered             /** Plug-ins must be treated in a given order.*/,
		ptList                /** Simple list of plug-ins*/,
		ptLists               /** Multiple lists of plug-ins*/
	};

protected:

	/*
	* The name of the manager
	*/
	R::RString Name;

	/*
	* Version of the manager
	*/
	R::RString Version;

	/*
	* Types of plug-ins handle by the manager.
	*/
	tPluginsType PluginsType;

public :

	/*
	* Constructor for the manager of plug-ins manager.
	* @param name            Name of the manager.
	* @param version         Version of the manager.
	* @param type            Type of the Plug-Ins.
	*/
	GGenericPluginManager(R::RString name,R::RString version,tPluginsType type);

	/**
	* Comparison method needed by R::RContainer.
	* @param pmng            Plug-ins manager to compare.
	*/
	int Compare(const GGenericPluginManager& pmng) const;

	/**
	* Comparison method needed by R::RContainer.
	* @param name            Name to compare.
	*/
	int Compare(const R::RString& name) const;

	/*
	* Load all plug-ins and their dialog boxes.
	* @param dll             Name of the dynamic link library.
	* @param handle          Handle to the library containing the plug-in.
	* @param handleDlg       Handle to the library containing the dialogs.
	*/
	virtual void Load(const R::RString& dll,void* handle,void* handleDlg)=0;

	/*
	* Connect to the session.
	* @param session         Pointer to the session.
	*/
	virtual void Connect(GSession* session)=0;

	/*
	* Disconnect to the session.
	* @param session         Pointer to the session.
	*/
	virtual void Disconnect(GSession* session)=0;

	/*
	* Create the config parameters.
	* @param config          Configuration structure.
	*/
	virtual void CreateConfig(R::RConfig* config)=0;

	/*
	* Read config of the manager.
	* @param config          Configuration structure.
	*/
	virtual void ReadConfig(R::RConfig* config)=0;

	/*
	* @param config          Configuration structure.
	*/
	virtual void SaveConfig(R::RConfig* config)=0;

	/*
	* Get the name of the current Manager.
	*/
	R::RString GetName(void) const {return(Name);}

	/*
	* Get the name of the current Manager.
	*/
	R::RString GetVersion(void) const {return(Version);}

	/*
	* Get the type of the plug-ins.
	*/
	tPluginsType GetPluginsType(void) const {return(PluginsType);}

	/**
	 * Get a pointer to the configuration structure for a specific plug-in.
	 * @param name           Name of the plug-in.
	 * @param type           Type of the factory
	 * @return Pointer to R::RConfig.
	 */
	virtual R::RConfig* GetConfig(const R::RString& name,const R::RString& type=R::RString::Null) const=0;

	/**
	 * Apply the configuration for a plug-in corresponding to a given
	 * R::RConfig.
	 * @param config         Pointer to the configuration.
	 */
	virtual void ApplyConfig(R::RConfig* config)=0;

	/**
	* Set the current method if the plug-ins must have a selected one.
	* @param name            Name of the method.
	* @param need            If the parameter is true and the plug-in does not
	*                        exist, generate an exception.
	*/
	virtual void SetCurrentMethod(const R::RString& name,bool need=true)=0;

	/*
	* Destruct.
	*/
	virtual ~GGenericPluginManager(void);
};


//-----------------------------------------------------------------------------
/**
* The GPluginManager class provides a template for a plug-in manager.
* @param mng                 Class that implements a given manager.
* @param factory             Class representing a given factory.
* @param plugin              Class representing a given plug-in.
* @author Pascal Francq
* @short Template for a plug-ins manager.
*/
template<class mng,class factory,class plugin>
	class GPluginManager : public GGenericPluginManager, private R::RContainer<factory,true,true>, private R::RContainer<plugin,false,true>
{
protected:

	/**
	* Current selected plug-in.
	*/
	factory* Current;

public:

	/**
	* Constructor for the manager of plug-ins manager.
	* @param name            Name of the manager.
	* @param version         Version of the manager.
	* @param type            Type of the Plug-Ins.
	*/
	GPluginManager(R::RString name,R::RString version,tPluginsType type);

	/**
	* Comparison method needed by R::RContainer.
	* @param pmng            Plug-ins manager to compare.
	*/
	int Compare(const GPluginManager& pmng) const;

	/**
	* Comparison method needed by R::RContainer.
	* @param name            Name to compare.
	*/
	int Compare(const R::RString& name) const;

	/**
	* ReOrder the containers of the manager. This method must be called if the
	* level of a plug-in changes in an ordered list of plug-ins.
	*/
	void ReOrder(void);

	/**
	* Load a plug-in and its dialog boxes.
	* @param dll             Name of the dynamic link library.
	* @param handle          Handle to the library of the plug-in.
	* @param handleDlg       Handle to the library of the dialogs.
	*/
	virtual void Load(const R::RString& dll,void* handle,void* handleDlg);

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
	* Create the configuration parameters.
	* @param config          Configuration structure.
	*/
	virtual void CreateConfig(R::RConfig* config);

	/**
	* Read the configuration parameters of the manager.
	* @param config          Configuration structure.
	*/
	virtual void ReadConfig(R::RConfig* config);

	/**
	* @param config          Configuration structure.
	*/
	virtual void SaveConfig(R::RConfig* config);

	/**
	* Register a factory of a plug-in.
	* @param fac             Factory to register.
	*/
	void RegisterFactory(factory* fac);

	/**
	* Get a pointer to a given factory.
	* @param name            Name of the factory.
	* @param need            If the parameter is true and the plug-in does not
	*                        exist, generate an exception.
	* @return Pointer to the factory, or null/exception if the factory does not
	* exist.
	*/
	factory* GetFactory(const R::RString& name,bool need=true) const;

	/**
	* Get the number of factories registered.
	*/
	size_t GetNbFactories(void) const {return(R::RContainer<factory,true,true>::GetNb());}

	/**
	* Get a cursor over the registered factories.
	*/
	R::RCursor<factory> GetFactories(void) const;

	/**
	* Signal that a plug-in was enabled.
	* @param plug            Plug-in enabled.
	*/
	virtual void EnablePlugIn(plugin* plug);

	/**
	* Signal that a plug-in was disabled.
	* @param plug            Plug-in disabled.
	*/
	virtual void DisablePlugIn(plugin* plug);

	/**
	* Get the number of enabled plug-ins.
	*/
	size_t GetNbPlugIns(void) const {return(R::RContainer<plugin,false,true>::GetNb());}

	/**
	* Get a pointer to a given plug-in.
	* @param name            Name of the plug-in.
	* @param need            If the parameter is true and the plug-in does not
	*                        exist, generate an exception.
	* @return Pointer to the plug-in, or null/exception if the name does not
	* exist.
	*/
	plugin* GetPlugIn(const R::RString& name,bool need=true) const;

	/**
	 * Get a pointer to the configuration structure for a specific plug-in.
	 * @param name           Name of the plug-in.
	 * @param type           Type of the factory. By default, it is not used.
	 * @return Pointer to R::RConfig.
	 */
	virtual R::RConfig* GetConfig(const R::RString& name,const R::RString& type) const;

	/**
	 * Apply the configuration for a plug-in corresponding to a given
	 * R::RConfig.
	 * @param config         Pointer to the configuration.
	 */
	virtual void ApplyConfig(R::RConfig* config);

	/**
	* Get a cursor over the enabled plug-ins.
	*/
	R::RCursor<plugin> GetPlugIns(void) const;

	/**
	* Set the current method if the plug-ins must have a selected one.
	* @param name            Name of the method.
	* @param need            If the parameter is true and the plug-in does not
	*                        exist, generate an exception.
	*/
	virtual void SetCurrentMethod(const R::RString& name,bool need=true);

	/**
	* Get the current method.
	* @param need            If the parameter is true and the plug-in does not
	*                        exist, generate an exception.
	* @return Pointer to the plugin, or null/exception if no plug-in is
	* selected or if the list does not need to select one.
	*/
	virtual plugin* GetCurrentMethod(bool need=true) const;

	/**
	* Get the current factory.
	* @param need            If the parameter is true and the plug-in does not
	*                        exist, generate an exception.
	* @return Pointer to the factory, or null/exception if no plug-in is
	* selected or if the list does not need to select one.
	*/
	virtual factory* GetCurrentFactory(bool need=true) const;

	/**
	* Get the current plug-in.
	* @param need            If the parameter is true and the plug-in does not
	*                        exist, generate an exception.
	* @return Pointer to the plug-in, or null/exception if no plug-in is
	* selected or if the list does not need to select one.
	*/
	virtual plugin* GetCurrentPlugIn(bool need=true) const;
};


//-----------------------------------------------------------------------------
// Template implementation
#include <gpluginmanager.hh>


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
