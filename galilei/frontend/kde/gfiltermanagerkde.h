/*

	GALILEI Research Project

	GFilterManagerKDE.h

	Manager to handle URL file in a KDE environment - Header.

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



//-----------------------------------------------------------------------------
#ifndef GFilterManagerKDEH
#define GFilterManagerKDEH


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <docs/gfiltermanager.h>



//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
* The GFilterManagerKDE class provides a representation for a manager in a KDE
* environment
* @author Pascal Francq
* @short KDE URL Manager.
*/
class GFilterManagerKDE : public GFilterManager
{
public:

	/**
	* Construct a URL manager.
	* @param path            Path to the plugins.
	* @param dlg             Should the dialog box be loaded.
	*/
	GFilterManagerKDE(R::RContainer<RString, true, false>* paths,bool dlg=true) throw(std::bad_alloc,GException);

protected:

	/**
	* Download and store locally a document given by an URL.
	* @param URL            URL of the document.
	* @param tmpFile        Temporary file created.
	*/
	virtual void Download(const char* URL,R::RString& tmpFile) throw(GException);

public:

	/**
	* Look for the mime types of a temporary file.
	* @param tmpfile        Temporary file created.
	* @return Pointer to a GMIMEFilter.
	*/
	virtual const char* DetermineMIMEType(const char* tmpfile) throw(GException);

protected:

	/**
	* Delete the file locally.
	* @param tmpFile        Temporary file to delete.
	*/
	virtual void Delete(R::RString& tmpFile) throw(GException);

public:

	/**
	* Destructor of URL manager.
	*/
	virtual ~GFilterManagerKDE(void);
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
