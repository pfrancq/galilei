/*

	GALILEI Research Project

	GFilterManager.h

	Generic filter manager handling URL - Header.

	Copyright 2001-2004 by the Université libre de Bruxelles.

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



//------------------------------------------------------------------------------
#ifndef GFilterManagerH
#define GFilterManagerH


//------------------------------------------------------------------------------
// include files for GALILEI
#include <sessions/galilei.h>
#include <sessions/gplugins.h>
#include <docs/gfilter.h>


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* The GFilterManager class provides a representation for a generic manager that
* handles :
* - Plugins of filters.
* - A list of association between a MIME type and a filter.
* - URL, by downloading the file and making a local copy available (this copy
*   has to be remove after the analysis).
* @author Pascal Francq
* @short Generic URL Manager.
*/
class GFilterManager : public GPluginManager<GFilterManager,GFactoryFilter,GFactoryFilterInit,GFilter>
{
protected:

	class GMIMEFilter;
	class GMIMEExt;

	/**
	* List of all pairs (MIME type,filter) available.
	*/
	R::RContainer<GMIMEFilter,true,true> MIMES;

	/**
	* List of all pairs (extension, MIME type) available.
	*/
	R::RContainer<GMIMEExt,true,true> Exts;

public:

	/**
	* Construct the filter manager.
	*/
	GFilterManager(void);

protected:

	/**
	* Download and store locally a document given by an URL.
	* @param URL            URL of the document.
	* @param tmpFile        Temporary file created.
	*/
	virtual void Download(const char* URL,R::RString& tmpFile);

public:

	/**
	* Try to guess the MIME types of a temporary file. By default, this method
	* looks in the list of association between the file extension and a MIME
	* type.
	* @param tmpfile        Temporary file created.
	* @return Name fo of the MIME type.
	*/
	virtual const char* DetermineMIMEType(const char* tmpfile);

protected:

	/**
	* Delete a temporary copy of a file created by the manager. This method is
	* only called if a temporary file was really created.
	* @param tmpFile        Temporary file to delete.
	*/
	virtual void Delete(R::RString& tmpFile);

public:

	/**
	* Transform a file into a GDocXML document. Try to find the MIME type of the
	* document if not specified.
	* @param doc            Document to analyze
	* Return Pointer to a GDocXML.
	*/
	GDocXML* CreateDocXML(GDoc* doc);

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
	* Get the name of a filter associated with a given MIME type.
	* @param mime           Name of the MIME type.
	* @return C string containing the name of the filter (or null if no filter
	*         was found).
	*/
	const char* GetMIMEType(const char* mime) const;

	/**
	* Read config of the manager
	*/
	virtual void ReadConfig(RXMLTag* t);

	/**
	* Store config of the manager
	*/
	virtual void SaveConfig(R::RXMLStruct* xml,R::RXMLTag* t);

	/**
	* Destructor of filter manager.
	*/
	virtual ~GFilterManager(void);
};


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
