/*

	GALILEI Research Project

	GGALILEIApp.h

	Application handling a GALILEI Session - Header.

	Copyright 2006-2015 by Pascal Francq (pascal@francq.info).
	Copyright 2006-2008 by the Université Libre de Bruxelles (ULB).

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



//------------------------------------------------------------------------------
#ifndef GGALILEIAppH
#define GGALILEIAppH


//------------------------------------------------------------------------------
// include files for R
#include <rapplication.h>
#include <rdownload.h>


//------------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>
#include <gslotlog.h>
#include <gpluginmanager.h>


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* The GGALILEIApp class represents a generic application running a session.
*
* Each application inheriting from GGALILEIApp manages three configuration
* files:
* -# Its own configuration which defines at least two parameters. The "GALILEI
*    Config" parameter gives the name of the configuration file for the GALILEI
*    library ("GALILEI" by default). The "Session" parameter defines the
*    current session name.
* -# A configuration file corresponding to the GALILEI library storing some
*    global parameters (such as the directories of the plug-ins). Several
*    applications may share this configuration file.
* -# The configuration file corresponding to the current session (storing the
*    parameters for the plug-ins, the log file, etc.). All the applications
*    having the same current session share this configuration file.
* @short Generic GALILEI Application
*/
class GGALILEIApp : public R::RApplication, public R::RContainer<GPlugInManager,true,false>
{
	class cFilter;
	class cExt;

protected:

	/**
	* The Sessions.
	*/
	R::RContainer<GSession,true,true> Sessions;

	/**
	 * The storages.
	 */
	GPlugInManager* Storages;

	/**
	 * The languages.
	 */
	GPlugInManager* Langs;

	/**
	* Load the dialog boxes
	*/
	bool LoadDialogs;

	/**
	* Path where to search for plug-ins
	*/
	R::RContainer<R::RString,true,false> PlugInsPaths;

	/**
	 * Directory where to binary files of the index are stored.
	 */
	R::RString IndexDir;

	/**
	* List of all pairs (MIME type,filter) available.
	*/
	R::RContainer<cFilter,true,true> MIMES;

	/**
	* List of all pairs (extension, MIME type) available.
	*/
	R::RContainer<cExt,true,true> Exts;

	/**
	* List of all pairs (Schemes,filter) available.
	*/
	R::RContainer<cFilter,true,true> Schemes;

	/**
     * Log file for the application.
     */
	 R::RTextFile Log;

public:

	/**
	* Construct the application.
	* @param name            Name of the application.
	* @param argc            Number of arguments.
	* @param argv            Arguments.
	* @param dlg             Dialog boxes must be loaded too?
	*/
	GGALILEIApp(const R::RString& name,int argc, char *argv[],bool dlg=false);

protected:

	/**
	* Create the configuration structure. New parameters can be added by
	* defining a new method.
	*/
	virtual void CreateConfig(void);

	/**
	* Initialization of the application.
	*/
	virtual void Init(void);

public:

	/**
	* Get the session with a given name.
	* @param name            Name of the session.
	* @param created         Determine if the session must be created if not
	*                        existing.
	* @param slot            Slot to show progress.
	*/
	GSession* GetSession(const R::RString& name,bool created=false,GSlot* slot=0);

	/**
	* Delete a session.
	* @param session         Session.
	*/
	void DeleteSession(GSession* session);

	/**
	* @return the name of the index directory.
	*/
	R::RString GetIndexDir(void) const {return(IndexDir);}

	/**
	* Set the name of the index directory.
	* @param name            Name.
	*/
	void SetIndexDir(const R::RString& name);

	/**
	* Get the list of plug-ins paths.
	*/
	R::RCursor<R::RString> GetPlugInsPaths(void) const {return(R::RCursor<R::RString>(PlugInsPaths));}

	/**
	* Set the list of plug-ins paths.
	* @âram paths            Paths to set.
	*/
	void SetPlugInsPaths(const R::iRContainer<R::RString>& paths);

protected:

	/**
	* Starting from a directory, this function looks for all shared libraries in the
	* sub-directories and add them in the containers.
	* @param dir                 Root directory to scan.
	* @param plugins             Strings for each main plug-ins.
	* @param dlgs                Strings for each dialog plug-ins.
	*/
	void FindPlugins(const R::RString dir,R::RContainer<R::RString,true,false>& plugins,R::RContainer<R::RString,true,false>& dlgs);

	/**
	* Load all plug-ins and their dialog boxes.
	* @param dirs            Directories to search in.
	* @param dlg             Should the dialog boxes be loaded?
	*/
	void Load(const R::RContainer<R::RString,true,false>& dirs,bool dlg=true);

	/**
	 * Create a valid storage. The method is call when a session is opened for
	 * which a current storage doesn't exist. By default, this method does
	 * nothing.
	 * @param session        Session for which the stoarge must be created.
    * @return a valid pointer to a GStorage.
    */
	virtual GStorage* CreateStorage(GSession* session);

public:

	/**
	* Get a cursor over all the managers.
	* @return GPlugInManager A cursor on the managers
	*/
	R::RCursor<GPlugInManager> GetManagers(void);

	/**
	* Get the manager associated to the "name".
	* @param mng             Name of the manager to be found.
	*/
	GPlugInManager* GetManager(const R::RString& mng) const;

	/**
	* Get the factories of a given manager.
	* @param mng             Name of the manager to be found.
	* @param list            List.
	*/
	R::RCursor<GPlugInFactory> GetFactories(const R::RString& mng,const R::RString& list=R::RString::Null) const;

	/**
	* Get the plug-ins of a given manager.
	* @tparam plugin         Class of the plug-in.
	* @param mng             Name of the manager to be found.
	* @param list            List.
	*/
	template<class plugin> R::RCastCursor<GPlugIn,plugin> GetPlugIns(const R::RString& mng,const R::RString& list=R::RString::Null) const
	{
		GPlugInManager* ptr(GetPtr(mng));
		if(!ptr)
			mThrowGException("'"+mng+"' is not a valid plug-ins manager");
		return(ptr->GetPlugIns<plugin>(list));
	}

	/**
	* Get a factory of a given manager.
	* @param mng             Name of the manager to be found.
	* @param name            Name of the plug-in.
	* @param list            List.
	* @param need            If the parameter is non-null and the plug-in
	*                        doesn't exist, generate an exception.
	*/
	GPlugInFactory* GetFactory(const R::RString& mng,const R::RString& name,const R::RString& list,int need=1) const;

	/**
	* Get a factory of a given manager.
	* @param mng             Name of the manager to be found.
	* @param name            Name of the plug-in.
	* @param need            If the parameter is non-null and the plug-in
	*                        doesn't exist, generate an exception.
	*/
	GPlugInFactory* GetFactory(const R::RString& mng,const R::RString& name,int need=1) const
	{
		return(GetFactory(mng,name,R::RString::Null,need));
	}

	/**
	* Get a plug-in of a given manager.
	* @param mng             Name of the manager to be found.
	* @param name            Name of the plug-in.
	* @param list            List.
	* @param need            If the parameter is non-null and the plug-in
	*                        doesn't exist, generate an exception.
	*/
	GPlugIn* GetPlugIn(const R::RString& mng,const R::RString& name,const R::RString& list,int need=1) const
	{
		GPlugInFactory* Factory(GetFactory(mng,name,list,need));
		if(!Factory)
			mThrowGException("'"+mng+"' is not a valid plug-in");
		return(Factory->GetPlugIn());
	}

	/**
	* Get a plug-in of a given manager.
	* @param mng             Name of the manager to be found.
	* @param name            Name of the plug-in.
	* @param need            If the parameter is non-null and the plug-in
	*                        doesn't exist, generate an exception.
	*/
	GPlugIn* GetPlugIn(const R::RString& mng,const R::RString& name,int need=1) const
	{
		GPlugInFactory* Factory(GetFactory(mng,name,need));
		if(!Factory)
			mThrowGException("'"+mng+"' is not a valid plug-in");
		return(Factory->GetPlugIn());
	}

	/**
	* Get a plug-in of a given manager.
	* @tparam plugin         Class of the plug-in.
	* @param mng             Name of the manager to be found.
	* @param name            Name of the plug-in.
	* @param list            List.
	* @param need            If the parameter is non-null and the plug-in
	*                        doesn't exist, generate an exception.
	*/
	template<class plugin> plugin* GetPlugIn(const R::RString& mng,const R::RString& name,const R::RString& list,int need=1) const
	{
		GPlugInManager* ptr(GetPtr(mng));
		if(!ptr)
			mThrowGException("'"+mng+"' is not a valid plug-ins manager");
		return(ptr->GetPlugIn<plugin>(name,list,need));
	}

	/**
	* Get a plug-in of a given manager.
	* @tparam plugin         Class of the plug-in.
	* @param mng             Name of the manager to be found.
	* @param name            Name of the plug-in.
	* @param need            If the parameter is non-null and the plug-in
	*                        doesn't exist, generate an exception.
	*/
	template<class plugin> plugin* GetPlugIn(const R::RString& mng,const R::RString& name,int need=1) const
	{
		return(GetPlugIn<plugin>(mng,name,R::RString::Null,need));
	}

	/**
	* Get the current factory of a given manager.
	* @param mng             Name of the manager to be found.
	* @param list            List.
	* @param need            If the parameter is non-null and the plug-in
	*                        doesn't exist, generate an exception.
	*/
	GPlugInFactory* GetCurrentFactory(const R::RString& mng,const R::RString& list,int need=1) const;

	/**
	* Get the current factory of a given manager.
	* @param mng             Name of the manager to be found.
	* @param need            If the parameter is non-null and the plug-in
	*                        doesn't exist, generate an exception.
	*/
	GPlugInFactory* GetCurrentFactory(const R::RString& mng,int need=1) const
	{
		return(GetCurrentFactory(mng,R::RString::Null,need));
	}

	/**
	* Get the current plug-in of a given manager.
	* @tparam plugin         Class of the plug-in.
	* @param mng             Name of the manager to be found.
	* @param list            List.
	* @param need            If the parameter is non-null and the plug-in
	*                        doesn't exist, generate an exception.
	*/
	template<class plugin> plugin* GetCurrentPlugIn(const R::RString& mng,const R::RString& list,int need=1) const
	{
		GPlugInManager* ptr(GetPtr(mng));
		if(!ptr)
		{
			if(need)
				mThrowGException("'"+mng+"' is not a valid plug-ins manager");
			else
				return(0);
		}
		return(ptr->GetCurrentPlugIn<plugin>(list,need));
	}

	/**
	* Get the current plug-in of a given manager.
	* @tparam plugin         Class of the plug-in.
	* @param mng             Name of the manager to be found.
	* @param need            If the parameter is non-null and the plug-in
	*                        doesn't exist, generate an exception.
	*/
	template<class plugin> plugin* GetCurrentPlugIn(const R::RString& mng,int need=1) const
	{
		return(GetCurrentPlugIn<plugin>(mng,R::RString::Null,need));
	}

	/**
	* Set the current plug-in for a given manager.
	* @param mng             Name of the manager to be found.
	* @param name            Name of the method.
	* @param list            List.
	* @param need            If the parameter is non-null and the plug-in
	*                        doesn't exist, generate an exception.
	*/
	void SetCurrentPlugIn(const R::RString& mng,const R::RString& name,const R::RString& list,int need=1)
	{
		GPlugInManager* ptr(GetPtr(mng));
		if(!ptr)
			mThrowGException("'"+mng+"' is not a valid plug-ins manager");
		ptr->SetCurrentPlugIn(name,list,need);
	}

	/**
	* Set the current plug-in for a given manager.
	* @param mng             Name of the manager to be found.
	* @param name            Name of the method.
	* @param need            If the parameter is non-null and the plug-in
	*                        doesn't exist, generate an exception.
	*/
	void SetCurrentPlugIn(const R::RString& mng,const R::RString& name,int need=1)
	{
		SetCurrentPlugIn(mng,name,R::RString::Null,need);
	}

	/**
	* Run a "program".
	* @param rec             Slot that receive information.
	* @param filename        Name of the file.
	*/
	void RunPrg(GSlot* rec,const R::RString& filename);

	/**
	 * Find the filter corresponding to a document based on the MIME type.
	 *
	 * An exception is generated if no filter was found (no MIME type, no
	 * corresponding filter, ...).
	 * @param doc             Document to analyse.
	 * @return the filter needed to analyse the document.
	 */
	GFilter* FindMIMEType(GDoc* doc) const;

	/**
	 * Find the filter corresponding to a given scheme. This allows GALILEI to
	 * manage exotic schemes that CURL cannot handle.
	 *
	 * @param doc             Document with a given scheme.
	 * @return the filter or a null pointer if no filter is found (hoping that
	 * CURL can handle the protocol).
	 */
	GFilter* FindScheme(GDoc* doc) const;

	/**
	* Add a pair (Scheme,filter).
	* @param scheùe         Name of the scheme.
	* @param f              Pointer to the filter.
	*/
	void AddScheme(const char* mime,GFilter* f);

	/**
	* Add a pair (MIME type,filter).
	* @param mime           Name of the MIME type.
	* @param f              Pointer to the filter.
	*/
	void AddMIME(const char* mime,GFilter* f);

	/**
	* Delete all the MIME type associated with a filter.
	* @param f              Pointer to the filter.
	*/
	void DelMIMES(GFilter* f);

	/**
	* Destruct the application.
	*/
	virtual ~GGALILEIApp(void);

	friend class GPlugInFactory;
};


//------------------------------------------------------------------------------
// Global variables
/**
* Pointer to the current application running a GALILEI session.
*/
extern GGALILEIApp* GALILEIApp;


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
