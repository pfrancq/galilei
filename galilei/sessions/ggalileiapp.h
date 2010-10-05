/*

	GALILEI Research Project

	GGALILEIApp.h

	Application handling a GALILEI Session - Header.

	Copyright 2006-2010 by Pascal Francq (pascal@francq.info).
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
* @author Pascal Francq
*/
class GGALILEIApp : public R::RApplication, public R::RContainer<GPlugInManager,true,false>, public R::RDownload
{
	class GMIMEFilter;
	class GMIMEExt;

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
	R::RContainer<R::RString,true,false> PlugInsPath;

	/**
	 * Directory where to binary files of the index are stored.
	 */
	R::RString IndexDir;

	/**
	* List of all pairs (MIME type,filter) available.
	*/
	R::RContainer<GMIMEFilter,true,true> MIMES;

	/**
	* List of all pairs (extension, MIME type) available.
	*/
	R::RContainer<GMIMEExt,true,true> Exts;

	/**
	 * Filter that should analyze the file.
	 */
	GFilter* Filter;

	/**
	 * Document to analyze.
	 */
	GDoc* Doc;

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
	*/
	GSession* GetSession(const R::RString& name,bool created=false);

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
	void SetIndexDir(const R::RString& name) {IndexDir=name;}

	/**
	* Get the list of plug-ins paths.
	*/
	R::RCursor<R::RString> GetPlugInsPath(void) const {return(R::RCursor<R::RString>(PlugInsPath));}

	/**
	* Clear the list of plug-ins paths.
	*/
	void ClearPlugInsPath(void) {PlugInsPath.Clear();}

	/**
	* Add a new plug-ins path in the list.
	* @param path            Path to add.
	*/
	void AddPlugInsPath(const R::RString& path) {PlugInsPath.InsertPtr(new R::RString(path));}

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
	* @param mng             Name of the manager to be found.
	* @param list            List.
	*/
	template<class plugin> R::RCastCursor<GPlugIn,plugin> GetPlugIns(const R::RString& mng,const R::RString& list=R::RString::Null) const
	{
		GPlugInManager* ptr(GetPtr(mng));
		if(!ptr)
			ThrowGException("'"+mng+"' is not a valid plug-ins manager");
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
			ThrowGException("'"+mng+"' is not a valid plug-in");
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
			ThrowGException("'"+mng+"' is not a valid plug-in");
		return(Factory->GetPlugIn());
	}

	/**
	* Get a plug-in of a given manager.
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
			ThrowGException("'"+mng+"' is not a valid plug-ins manager");
		return(ptr->GetPlugIn<plugin>(name,list,need));
	}

	/**
	* Get a plug-in of a given manager.
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
				ThrowGException("'"+mng+"' is not a valid plug-ins manager");
			else
				return(0);
		}
		return(ptr->GetCurrentPlugIn<plugin>(list,need));
	}

	/**
	* Get the current plug-in of a given manager.
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
			ThrowGException("'"+mng+"' is not a valid plug-ins manager");
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
	* Apply the configuration.
	*/
	void Apply(void);

	/**
	* Run a "program".
	* @param rec             Slot that receive information.
	* @param filename        Name of the file.
	*/
	void RunPrg(GSlot* rec,const R::RString& filename);


protected:

	/**
	 * Find the MIME type of a file based on its extension (Ex. ".html" gives
	 * "text/html").
	 */
	void FindMIMEType(void);

	/**
	 * If the protocol is HTTP and the server returns a content type for the
	 * downloaded file. This function verifies that a filter exist for the
	 * document to download.
	 * @param MIME           MIME type send by the server.
	 * @return true if the file should be downloaded.
	 */
	virtual bool IsValidContent(const R::RString& MIME);

public:

	/**
	 * Pre-analyze a document to find the correct MIME type.
	 * @return the filter needed to analyze the document, or null if the
	 * document is a XML one.
	 * @param doc             Document to analyze.
	 * @param uri             File to analyze (can be a local copy of the
	 *                        original file).
	 * @param tmp             Necessary if a file must be downloaded.
	 * @exception A GException is generated if the document could not be
	 * analyzed (no MIME type, no filter, ...).
	 */
	GFilter* FindMIMEType(GDoc* doc,R::RURI& uri,R::RIO::RSmartTempFile& tmp);

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
