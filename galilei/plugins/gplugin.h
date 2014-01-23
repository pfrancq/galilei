/*

	GALILEI Research Project

	GPlugIn.h

	Generic Plug-In - Header.

	Copyright 2003-2014 by Pascal Francq (pascal@francq.info).
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
* The GPlugInFactory class provides a template for a generic plug-in factory. A
* factory handles the loading of the dynamic library containing the plug-in.
* @author Pascal Francq
* @short Generic Plug-in Factory.
*/
class GPlugInFactory
{
public:

	/**
	* Type of a function used to show the about box of a plug-in.
	*/
	typedef void (*About_t)();

	/**
	* Type of a function used to show dialog box of a plug-in.
	*/
	typedef bool* (*Configure_t)(GPlugIn*);

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
	* Create the new plug-in.
	* @param session         Session.
	*/
	virtual GPlugIn* NewPlugIn(GSession* session)=0;

	/**
	* Create a plug-in.
	* @param session         Session.
	*/
	void Create(GSession* session);

	/**
	* Delete a plug-in.
	*/
	void Delete(void);

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
	void ApplyConfig(void);

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
	GPlugIn* GetPlugIn(void) const {return(Plugin);}

	/**
	* Get the plug-in of this factory.
	*/
	template<class plugin> plugin* GetPlugIn(void) const {return(dynamic_cast<plugin*>(Plugin));}

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
	friend class GPlugInList;
	friend class GPlugInManager;
};


//-----------------------------------------------------------------------------
/**
 * The GPlugIn class provides a template for a generic plug-in. Plug-ins are
 * managed by GGALILEIApp, but they are used in relation to a particular
 * session.
 *
 * Each plug-in goes through an identical process when a session is created :
 * - The session variable is set.
 * - The method GPlugIn::CreateConfig is called. The plug-in creates its
 *   parameters. This parameters are then automatically read from a
 *   configuration file of the particular session.
 * - The method GPlugIn::ApplyConfig is called. The plug-in can use the
 *   configuration read.
 * - The method GPlugIn::Init is called. The plug-in makes some initialization
 *   of its internal structures. Since all plug-ins are allocated, it is the
 *   right place to make a plug-in an observer of another's plug-in
 *   notifications.
 *
 * When a session is closed, the method GPlugIn::Done is called for each
 * plug-in.
 *
 * The method GPlugin::Reset can be used to force a plug-in to reset itself. In
 * particular, if plug-ins managed important permanent information, it can be in
 * an unknown state (for example after a crash or when a plug-in becomes enabled
 * with an existing session). A user can call this method to force the plug-in
 *  to do the necessary work to be again in a coherent state.
 *
 * It should be understand that a plug-in is create once by GGALILEIApp, but
 * that the methods GPlugIn::Init and GPlugIn::Done are called each time a
 * session is created or deleted. Therefore, if a plug-in manages some internal
 * structures, the developer has two choices :
 * -# The structures are created and deleted each time (by GPlugIn::Init and
 *    GPlugIn::Done).
 * -# The structures are created and deleted once (by the constructor and the
 *    destructor). The methods GPlugIn::Init and GPlugIn::Done are then just
 *    responsible to put the structures updated with the current session.
 *
 * Generally, the best approach is to create internal structures as late as
 * possible.
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

	/**
	 * Identifier of the plug-in.
	 */
	size_t Id;

public:

	/**
	* Constructor of the plug-in.
	* @param session         Session.
	* @param fac             Factory.
	*/
	GPlugIn(GSession* session,GPlugInFactory* fac);

	/**
	* Configuration parameters were changed (or loaded).
	*
	* It is the good place to get the parameters and store them in variables
	* (it is faster than to use the FindParam method each time). Here is an
	* example of an implementation.
	* @code
	* void MyPlugIn::ApplyConfig(void)
	* {
	*   bool a(FindParam<RParamValue>("EnableOption")->GetBool());
	*   int b(FindParam<RParamValue>("IntNumber")->GetInt());
	* }
	* @endcode
	*/
	virtual void ApplyConfig(void);

	/**
	* Declare a new parameter. If it exist, the parameter passed as argument is
	* deleted.
	* @param param           Parameter.
	*/
	void InsertParam(R::RParam* param);

private:

	/**
	* Find a given parameter.
	* @param name            Name of the parameter.
	* @return Pointer or null if not found.
	*/
	R::RParam* GetParam(const R::RString& name);

public:

	/**
	* Find a given parameter (template version).
	* @tparam T              Type of the parameter.
	* @param name            Name of the parameter.
	* @return Pointer or null if not found.
	*/
	template<class T> T* FindParam(const R::RString& name)
	{
		return(dynamic_cast<T*>(GetParam(name)));
	}

	/**
	* @return a cursor over the parameters of the plug-in.
	* @param cat             Category of the parameter.
	*/
	R::RCursor<R::RParam> GetParams(const R::RString& cat=R::RString::Null);

	/**
	* Fill a container with the categories.
	* @param cats            Container that will be filled (it is emptied by
	*                        the method).
	*/
	void GetCategories(R::RContainer<R::RString,true,false>& cats);

	/**
	 * Initialize the plug-in. This method is called each time a session is
	 * opened. A first call to ApplyConfig was already done and the session
	 * dictionaries were loaded.
	 */
	virtual void Init(void);

	/**
	 * Create the configuration (attached to the session).
	 */
	virtual void CreateConfig(void);

	/**
	* The plug-in should be reseted. This method is "manually" called (typically
	* by an user through an application) to ensure that the plug-in state is
	* coherent with a session. This can be useful if after a crash during some
	* tasks done by the plug-in.
	*
	* For example, if the plug-in holds the results of some computations, they
	* should be redone.
	*/
	virtual void Reset(void);

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
	 * The method is called each time a session is closed.
    */
	virtual void Done(void);

	/**
	* Destruct of the plug-in.
	*/
	virtual ~GPlugIn(void);

	friend class GPlugInFactory;
	friend class GPlugInList;
	friend class GPlugInManager;
};


//-------------------------------------------------------------------------------
#define CREATE_FACTORY(base,plugin,type,list,name,desc)                                            \
class TheFactory : public GALILEI::GPlugInFactory                                                  \
{                                                                                                  \
	static GALILEI::GPlugInFactory* Inst;                                                           \
	TheFactory(GALILEI::GPlugInManager* mng,const char* t) : GPlugInFactory(mng,name,desc,t,list)   \
	{                                                                                               \
	}                                                                                               \
	virtual ~TheFactory(void) {}                                                                    \
public:                                                                                            \
	static GALILEI::GPlugInFactory* CreateInst(GALILEI::GPlugInManager* mng,const char* t)          \
	{                                                                                               \
		if(!Inst)                                                                                    \
			Inst = new TheFactory(mng,t);                                                             \
		return(Inst);                                                                                \
	}                                                                                               \
	virtual const char* GetAPIVersion(void) const                                                   \
		{return(API_PLUG_IN_VERSION);}                                                               \
		                                                                                             \
	virtual GALILEI::GPlugIn* NewPlugIn(GSession* session)                                          \
	{                                                                                               \
		base* ptr(new plugin(session,this));                                                         \
		return(ptr);                                                                                 \
	}                                                                                               \
};                                                                                                 \
GALILEI::GPlugInFactory* TheFactory::Inst = 0;                                                     \
                                                                                                   \
extern "C" GALILEI::GPlugInFactory* FactoryCreate(GALILEI::GPlugInManager* mng,const char* t)      \
{                                                                                                  \
	return(TheFactory::CreateInst(mng,t));                                                          \
}                                                                                                  \
extern "C" const char* LibType(void)                                                               \
{                                                                                                  \
	return(type);                                                                                   \
}


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
