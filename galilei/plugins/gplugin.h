/*

	GALILEI Research Project

	GPlugIn.h

	Generic Plug-In - Header.

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
#ifndef GPlugInH
#define GPlugInH


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


//------------------------------------------------------------------------------
// Plug-in API VERSION
#define API_PLUG_IN_VERSION "2.0"


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
* The GPlugIn class provides a template for a generic plug-in.
* @author Pascal Francq
* @short Generic Plug-in.
*/
class GPlugIn
{
protected:

	/**
	* Pointer to the factory.
	*/
	GPlugInFactory* Factory;

	/**
	* Session.
	*/
	GSession* Session;

public:

	/**
	* Constructor of the plug-in.
	* @param fac            Factory.
	*/
	GPlugIn(GPlugInFactory* fac);

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
	GPlugInFactory* GetFactory(void) const {return(Factory);}

	/**
	* Comparison method needed by R::RContainer.
	* @param plugin          Plug-in to compare.
	*/
	int Compare(const GPlugIn& plugin) const;

	/**
	* Comparison method needed by R::RContainer.
	* @param plugin          Plug-in to compare.
	*/
	int Compare(const R::RString& plugin) const;

	/**
	* Get the name of the Plug-in
	*/
	R::RString GetName(void) const;

	/**
	* Get the description of the Plug-in
	*/
	R::RString GetDesc(void) const;

	/**
	 * Get the session associated with the plug-in.
	 */
	GSession* GetSession(void) const {return(Session);}

	/**
	* Destruct of the plug-in.
	*/
	virtual ~GPlugIn(void);
};


//-----------------------------------------------------------------------------
/**
* The GPlugInFactory class provides a template for a generic plug-in factory. A
* factory handles the loading of the dynamic library containing the plug-in.
* @author Pascal Francq
* @short Generic Plug-in Factory.
*/
class GPlugInFactory : public R::RConfig
{
public:

	/**
	* Type of a function used to show dialog box of a plug-in.
	*/
	typedef void* (*Configure_t)(GPlugInFactory*);

protected:

	/**
	* Name of the plug-in.
	*/
	R::RString Name;

	/**
	* Description of the plug-in.
	*/
	R::RString Desc;

	/**
	* Specific order of the plug-in in the list.
	*/
	int Level;

	/**
	* Plug-in Manager associated.
	*/
	GPlugInManager* Mng;

	/**
	* Pointer to the plug-in.
	*/
	GPlugIn* Plugin;

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

	/**
	* Name of the corresponding (if any) list.
	*/
	R::RString List;

public:

	/**
	* Constructor.
	* @param mng             Manager of the plug-in.
	* @param name            Name of the Factory/Plug-in ('/' are replaced by
	*                        '-' for the configuration file).
	* @param desc            Description of the plug-in.
	* @param lib             Library of the Factory/Plug-in.
	* @param list            List of the plug-in. Must be a null string if the
	*                        plug-in is not part of multiple lists.
	*/
	GPlugInFactory(GPlugInManager* mng,const R::RString& name,const R::RString& desc,const R::RString& lib,const R::RString& list);

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
	* Description of the plug-in.
	*/
	R::RString GetDesc(void) const {return(Desc);}

	/**
	* Get the name of the list.
	*/
	R::RString GetList(void) const {return(List);}

	/**
	* Get the manager of the factory.
	* @return Pointer to the manager.
	*/
	GPlugInManager* GetMng(void) const {return(Mng);}

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
	* Set the Handler of the library containing the dialog box.
	* @param handleDlg      Handle of the dialog library
	*/
	void SetDlgHandler(void* handleDlg) {HandleDlg=handleDlg;}

	/**
	* Comparison method needed by R::RContainer.
	* @param f               Factory to compare.
	*/
	int Compare(const GPlugInFactory& f) const;

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
	virtual GPlugIn* NewPlugIn(void)=0;

	/**
	* Delete a plug-in and eventually disconnect it to the session.
	*/
	virtual void DeletePlugIn(GPlugIn* plug)=0;

	/**
	* Create a plug-in.
	*/
	void Create(void);

	/**
	* Create a plug-in.
	*/
	void Delete(void);

	/**
	* Create a plug-in.
	*/
	void Create(GSession* session);

	/**
	* Delete a plug-in.
	*/
	void Delete(GSession* session);

	/**
	* Show 'about' information.
	*/
	void About(void);

	/**
	* Configure the parameters.
	*/
	void Configure(void);

	/**
	* Apply the configuration eventually to the plug-in.
	*/
	void Apply(void);

	/**
	* Specify if an about box exist.
	*/
	bool HasAbout(void) const {return(AboutDlg);}

	/**
	* Specify if a configure box exist.
	*/
	bool HasConfigure(void) const {return(ConfigDlg);}

	/**
	* Get the API Version of the plug-in.
	* @return C String.
	*/
	virtual const char* GetAPIVersion(void) const=0;

	/**
	* Return the manager of the plug-in.
	*/
	GPlugInManager* GetMng(void) {return(Mng);}

	/**
	* Get the plug-in of this factory.
	*/
	template<class plugin> plugin* GetPlugIn(void) const {return(static_cast<plugin*>(Plugin));}

	/**
	* Specify if the plug-in is created.
	*/
	bool IsCreated(void) const {return(Plugin);}

	/**
	* Get the library of the plug-in.
	*/
	R::RString GetLib(void) const {return(Lib);}

	/**
	* Return the version of the plug-in.
	*/
	const char* GetVersion(void) const {return("");}

	/**
	* Destruct the factory.
	*/
	virtual ~GPlugInFactory(void);

	// friend class
	friend class GPlugIn;
};


//-------------------------------------------------------------------------------
#define CREATE_FACTORY(base,plugin,type,list,name,desc)                                            \
class TheFactory : public GALILEI::GPlugInFactory                                                  \
{                                                                                                  \
	static GALILEI::GPlugInFactory* Inst;                                                          \
	TheFactory(GALILEI::GPlugInManager* mng,const char* t) : GPlugInFactory(mng,name,desc,t,list)  \
	{                                                                                              \
		plugin::CreateParams(this);                                                                \
	}                                                                                              \
	virtual ~TheFactory(void) {}                                                                   \
public:                                                                                            \
	static GALILEI::GPlugInFactory* CreateInst(GALILEI::GPlugInManager* mng,const char* t)         \
	{                                                                                              \
		if(!Inst)                                                                                  \
			Inst = new TheFactory(mng,t);                                                          \
		return(Inst);                                                                              \
	}                                                                                              \
	virtual const char* GetAPIVersion(void) const                                                  \
		{return(API_PLUG_IN_VERSION);}                                                             \
		                                                                                           \
	virtual GALILEI::GPlugIn* NewPlugIn(void)                                                      \
	{                                                                                              \
		base* ptr(new plugin(this));                                                               \
		return(ptr);                                                                               \
	}                                                                                              \
	virtual void DeletePlugIn(GALILEI::GPlugIn* plug)                                              \
	{                                                                                              \
		delete plug;                                                                               \
	}                                                                                              \
};                                                                                                 \
GALILEI::GPlugInFactory* TheFactory::Inst = 0;                                                     \
                                                                                                   \
extern "C" GALILEI::GPlugInFactory* FactoryCreate(GALILEI::GPlugInManager* mng,const char* t)      \
{                                                                                                  \
	return(TheFactory::CreateInst(mng,t));                                                         \
}                                                                                                  \
extern "C" const char* LibType(void)                                                               \
{                                                                                                  \
	return(type);                                                                                  \
}


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
