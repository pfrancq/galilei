/*

	GALILEI Research Project

	GDocs.h

	List of documents - Implementation.

	Copyright 2001 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).

	Version $Revision$

	Last Modify: $Date$

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
#ifndef GDocsH
#define GDocsH


//-----------------------------------------------------------------------------
// include files for R Project
#include <rstd/rcontainer.h>
#include <rstd/rstring.h>



//-----------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// forward class declaration
class GSession;
class GDoc;
class GDocCursor;


//-----------------------------------------------------------------------------
/**
* The GDocs class provides a representation for a list of documents.
* @author Pascal Francq
* @short List of documents.
*/
class GDocs : public RStd::RContainer<GDoc,unsigned,true,true>
{

	/**
	* State of the documents.
	*/
	bool bDocs;

public:

	/**
	* Constructor.
	* @param nb             Maximal number of documents.
	*/
	GDocs(unsigned int nb) throw(bad_alloc);

	/**
	* Get a cursor over the profiles used in the system.
	*/
	GDocCursor& GetDocsCursor(void);

	/**
	* Load the documents.
	*/
	virtual void LoadDocs(void) throw(bad_alloc,GException)=0;

	/**
	* Create a new document.
	* @param url        URL of the document.
	* @param name       Name of the document.
	* @param mime       MIME Type of the document
	* @returns Pointer to a new created document.
	*/
	virtual GDoc* NewDoc(const char* url,const char* name,const char* mime) throw(GException)=0;

	/**
	* Save a document.
	* @param doc        Document to save.
	*/
	virtual void SaveDoc(GDoc* doc) throw(GException)=0;

	/**
	* Get the number of documents treated by the system.
	* @returns Number of documents.
	*/
	unsigned int GetNbDocs(void) const
		{return(NbPtr);}


	/**
	* Verify if the documents are loaded.
	* @returns true, if loaded.
	*/
	bool IsDocsLoad(void) const {return(bDocs);}

	/**
	* Load the documents.
	*/
	void InitDocs(void) throw(bad_alloc,GException);

	/**
	* Insert a document in the container.
	*/
	void InsertDoc(GDoc* d) throw(bad_alloc);

	/**
	* Get a document from the container.
	* @param id         Identificator of the document.
	*/
	GDoc* GetDoc(unsigned int id) throw(bad_alloc);

	/**
	* Destructor.
	*/
	virtual ~GDocs(void);
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
