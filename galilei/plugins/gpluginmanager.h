/*

	GALILEI Research Project

	GPlugIns.h

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
#ifndef GPlugInsH
#define GPlugInsH


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


/**
 * The GPlugInList manages a given list of plug-ins. It is manage by an
 * instance of a GPlugInManager.
 * @author Pascal Francq
 * @short List of plug-ins.
 */
class GPlugInList
{
	/**
	* The name of the list.
	*/
	R::RString Name;

	/**
	 * Factories.
	 */
	R::RContainer<GPlugInFactory,true,true> Factories;

	/**
	 * Enabled plug-ins.
	 */
	R::RContainer<GPlugIn,false,true> Plugins;

	/**
	* Current selected plug-in.
	*/
	GPlugInFactory* Current;

public:

	/**
	* Constructor for the list of plug-ins.
	* @param name            Name of the list.
	*/
	GPlugInList(R::RString name);

	/**
	* Get the name of the plug-in list.
	*/
	R::RString GetName(void) const {return(Name);}

	/**
	* Comparison method needed by R::RContainer.
	* @param list            Plug-ins list to compare.
	*/
	int Compare(const GPlugInList& list) const;

	/**
	* Comparison method needed by R::RContainer.
	* @param name            Name to compare.
	*/
	int Compare(const R::RString& name) const;

	/**
	* Get the number of factories registered.
	*/
	size_t GetNbFactories(void) const {return(Factories.GetNb());}

	/**
	* Get a cursor over the registered factories.
	*/
	R::RCursor<GPlugInFactory> GetFactories(void) const
	{
		return(R::RCursor<GPlugInFactory>(Factories));
	}

	/**
	* Get the number of enabled plug-ins.
	*/
	size_t GetNbPlugIns(void) const {return(Plugins.GetNb());}

	/**
	* Get a cursor over the enabled plug-ins.
	*/
	template<class plugin> R::RCastCursor<GPlugIn,plugin> GetPlugIns(void) const
	{
		return(R::RCastCursor<GPlugIn,plugin>(Plugins));
	}

private:

	/**
	* Connect to the session.
	* @param session         Pointer to the session.
	*/
	void Connect(GSession* session);

	/**
	* Disconnect to the session.
	* @param session         Pointer to the session.
	*/
	void Disconnect(GSession* session);

	friend class GPlugInManager;
};


//-----------------------------------------------------------------------------
/**
* The GPlugInManager class provides a plug-in manager.
* @author Pascal Francq
* @short Plug-ins Manager.
*/
class GPlugInManager
{
public:
	/**
	* Types of plug-ins.
	*/
	enum tPluginsType
	{
		ptSelect              /** Only one plug-in can be selected.*/,
		ptOrdered             /** Plug-ins must be treated in a given order.*/,
		ptList                /** Simple list of plug-ins.*/,
		ptListSelect          /** Multiple lists of plug-ins where one plug-in
		                          can be selected in a given list. */
	};

protected:

	/**
	* Name of the manager.
	*/
	R::RString Name;

	/**
	* Version of the manager.
	*/
	R::RString Version;

	/**
	* Types of plug-ins handle by the manager.
	*/
	tPluginsType PluginsType;

	/**
	 * Plug-ins managed.
	 */
	union
	{
		GPlugInList* List;
		R::RContainer<GPlugInList,true,true>* Lists;
	} Data;

public:

	/**
	* Constructor for the manager of plug-ins manager.
	* @param name            Name of the manager.
	* @param type            Type of the manager.
	*/
	GPlugInManager(R::RString name,tPluginsType type);

	/**
	* Get the name of the current Manager.
	*/
	R::RString GetName(void) const {return(Name);}

	/**
	* Get the version of the current Manager.
	*/
	R::RString GetVersion(void) const {return(Version);}

	/**
	* Get the type of the plug-ins.
	*/
	tPluginsType GetPlugInType(void) const {return(PluginsType);}

	/**
	* Comparison method needed by R::RContainer.
	* @param pmng            Plug-ins manager to compare.
	*/
	int Compare(const GPlugInManager& pmng) const;

	/**
	* Comparison method needed by R::RContainer.
	* @param name            Name to compare.
	*/
	int Compare(const R::RString& name) const;

	/**
	* ReOrder the containers of the manager. This method must be called if the
	* level of a plug-in changes, and works only with an ordered list of plug-ins.
	*/
	void ReOrder(void);

	/**
	* Load a plug-in and its dialog boxes.
	* @param dll             Name of the dynamic link library.
	* @param handle          Handle to the library of the plug-in.
	* @param handleDlg       Handle to the library of the dialogs.
	* @param config          Configuration structure.
	*/
	void Load(const R::RString& dll,void* handle,void* handleDlg,R::RConfig* config);

	/**
	* Connect to the session.
	* @param session         Pointer to the session.
	*/
	void Connect(GSession* session);

	/**
	* Disconnect to the session.
	* @param session         Pointer to the session.
	*/
	void Disconnect(GSession* session);

	/**
	* Create the configuration parameters.
	* @param config          Configuration structure.
	*/
	void CreateConfig(R::RConfig* config);

	/**
	* Read the configuration parameters of the manager.
	* @param config          Configuration structure.
	*/
	void ReadConfig(R::RConfig* config);

	/**
	* @param config          Configuration structure.
	*/
	void SaveConfig(R::RConfig* config);

	/**
	* Register a factory of a plug-in.
	* @param fac             Factory to register.
	* @param config          Configuration structure.
	*/
	void RegisterFactory(GPlugInFactory* fac,R::RConfig* config);

	/**
	 * @return List of the plug-ins list if supported, or an empty cursor.
	 */
	R::RCursor<GPlugInList> GetPlugInLists(void) const;

	/**
	 * @return a given plug-ins list.
	 * @param list            List.
	 * @param need            If the parameter is non-null and the plug-in
     *                        doesn't exist, generate an exception.
	 */
	GPlugInList* GetPlugInList(const R::RString& list,int need=1) const;

	/**
	* Get a pointer to a given GPlugInFactory.
	* @param name            Name of the GPlugInFactory.
	* @param list            List.
	* @param need            If the parameter is non-null and the plug-in
    *                        doesn't exist, generate an exception.
	* @return Pointer to the GPlugInFactory, or null/exception if the GPlugInFactory does not
	* exist.
	*/
	GPlugInFactory* GetFactory(const R::RString& name,const R::RString& list,int need=1) const;

	/**
	* Get a pointer to a given GPlugInFactory.
	* @param name            Name of the GPlugInFactory.
	* @param need            If the parameter is non-null and the plug-in does
	*                        doesn't exist, generate an exception.
	* @return Pointer to the GPlugInFactory, or null/exception if the GPlugInFactory does not
	* exist.
	*/
	GPlugInFactory* GetFactory(const R::RString& name,int need=1) const
	{
		return(GetFactory(name,R::RString::Null,need));
	}

	/**
	* Get the number of factories registered.
	* @param list            List.
	*/
	size_t GetNbFactories(const R::RString& list=R::RString::Null) const;

	/**
	* Get a cursor over the registered factories.
	* @param list            List.
	*/
	R::RCursor<GPlugInFactory> GetFactories(const R::RString& list=R::RString::Null) const;

	/**
	* Signal that a plug-in was enabled.
	* @param plug            Plug-in enabled.
	*/
	void EnablePlugIn(GPlugIn* plug);

	/**
	* Signal that a plug-in was disabled.
	* @param plug            Plug-in disabled.
	*/
	void DisablePlugIn(GPlugIn* plug);

	/**
	* Get the number of enabled plug-ins.
	* @param list            List.
	*/
	size_t GetNbPlugIns(const R::RString& list=R::RString::Null) const;

	/**
	* Get a pointer to a given plug-in.
	* @param name            Name of the plug-in.
	* @param list            List.
	* @param need            If the parameter is not-null and the plug-in
	*                        doesn't exist, generate an exception.
	* @return Pointer to the plug-in, or null/exception if the name does not
	* exist.
	*/
	template<class plugin> plugin* GetPlugIn(const R::RString& name,const R::RString& list,int need=1) const
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

		plugin* ptr(dynamic_cast<plugin*>(List->Plugins.GetPtr(name,PluginsType!=ptOrdered)));
		if((!ptr)&need)
			ThrowGException(R::RString("No enabled plug-in '")+name+"' available for '"+Name+"'");
		return(ptr);
	}

	/**
	* Get a pointer to a given plug-in.
	* @param name            Name of the plug-in.
	* @param need            If the parameter is non-null and the plug-in
	*                        doesn't exist, generate an exception.
	* @return Pointer to the plug-in, or null/exception if the name does not
	* exist.
	*/
	template<class plugin> plugin* GetPlugIn(const R::RString& name,int need=1) const
	{
		return(GetPlugIn<plugin>(name,R::RString::Null,need));
	}

	/**
	* Get a cursor over the enabled plug-ins.
	* @param list            List.
	*/
	template<class plugin> R::RCastCursor<GPlugIn,plugin> GetPlugIns(const R::RString& list=R::RString::Null) const
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
		return(R::RCastCursor<GPlugIn,plugin>(List->Plugins));
	}

	/**
	* Set the current method if the plug-ins must have a selected one.
	* @param name            Name of the method.
	* @param list            List.
	* @param need            If the parameter is non-null and the plug-in
	*                        doesn't exist, generate an exception.
	*/
	void SetCurrentPlugIn(const R::RString& name,const R::RString& list,int need=1);

	/**
	* Set the current method if the plug-ins must have a selected one.
	* @param name            Name of the method.
	* @param need            If the parameter is non-null and the plug-in
	*                        doesn't exist, generate an exception.
	*/
	void SetCurrentPlugIn(const R::RString& name,int need=1)
	{
		SetCurrentPlugIn(name,R::RString::Null,need);
	}

	/**
	* Get the current method.
	* @param list            List.
	* @param need            If the parameter is non-null and the plug-in
	*                        doesn't exist, generate an exception.
	* @return Pointer to the GPlugIn, or null/exception if no plug-in is
	* selected or if the list does not need to select one.
	*/
	template<class plugin> plugin* GetCurrentPlugIn(const R::RString& list,int need=1) const
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
		if(!List->Current)
			return(0);
		plugin* plug=List->Current->GetPlugIn<plugin>();
		if((!plug)&&need)
			ThrowGException("No active plug-in available for '"+Name+"'");
		return(plug);
	}

	/**
	* Get the current method.
	* @param need            If the parameter is non-null and the plug-in
	*                        doesn't exist, generate an exception.
	* @return Pointer to the GPlugIn, or null/exception if no plug-in is
	* selected or if the list does not need to select one.
	*/
	template<class plugin> plugin* GetCurrentPlugIn(int need=1) const
	{
		return(GetCurrentPlugIn<plugin>(R::RString::Null,need));
	}

	/**
	* Get the current GPlugInFactory.
	* @param list            List.
	* @param need            If the parameter is non-null and the plug-in
	*                        doesn't exist, generate an exception.
	* @return Pointer to the GPlugInFactory, or null/exception if no plug-in is
	* selected or if the list does not need to select one.
	*/
	GPlugInFactory* GetCurrentFactory(const R::RString& list,int need=1) const;

	/**
	* Get the current GPlugInFactory.
	* @param need            If the parameter is non-null and the plug-in
	*                        doesn't exist, generate an exception.
	* @return Pointer to the GPlugInFactory, or null/exception if no plug-in is
	* selected or if the list does not need to select one.
	*/
	GPlugInFactory* GetCurrentFactory(int need=1) const
	{
		return(GetCurrentFactory(R::RString::Null,need));
	}

	/**
	 *  Destructor the plug-ins manager.
	 */
	virtual ~GPlugInManager(void);
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
