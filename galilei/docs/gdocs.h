/*

	GALILEI Research Project

	GDocs.h

	Documents managed by a session - Header.

	Copyright 2001-2003 by the Université Libre de Bruxelles.

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



//------------------------------------------------------------------------------
#ifndef GDocsH
#define GDocsH


//------------------------------------------------------------------------------
// include files for GALILEI
#include <sessions/galilei.h>


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* The GDocs class provides a representation for all the documents managed by a
* GALILEI session.
*
* Several containers store the documents :
* - A container of all documents ordered by identificator.
* - A container of all documents ordered by URL.
* - Language-specific containers where the documents are ordered by
*   identificator.
* @author Pascal Francq
* @short Session Documents.
*/
class GDocs : protected R::RContainer<GDoc,unsigned int,true,true>
{
	class GDocsLang;
	class GDocRefURL;

	/**
	* Documents ordered by language and identificator.
	*/
	R::RContainer<GDocsLang,unsigned int,true,true> DocsLang;

	/**
	* Documents ordered by URL.
	*/
	R::RContainer<GDocRefURL,unsigned int,true,true> DocsRefUrl;

public:

	/**
	* Constructor.
	* @param nb             Maximal number of documents to allocate initially.
	*/
	GDocs(unsigned int nb) throw(std::bad_alloc);

	/**
	* Get a cursor on all the documents.
	* @return GDocCursor.
	*/
	GDocCursor& GetDocsCursor(void);

	/**
	* Get a cursor on the documents of a given langage.
	* @param lang            Language of the documents.
	* @return GDocCursor.
	*/
	GDocCursor& GetDocsCursor(GLang* lang) throw(GException);

	/**
	* Fill a given array with all the documents of a given language. The array
	* must be created and must be large enough to hold all the documents.
	* @see This method is used in GSubjects to create assessments for
	*      profiles during a simulation of a real system.
	* @param docs            Pointer to the array.
	* @param lang            Language of the documents.
	* @returns Number of documents contained in the array.
	*/
	unsigned int FillDocs(GDoc** docs,GLang* lang) throw(GException,std::bad_alloc);

	/**
	* Get the number of documents handled.
	* @returns Number of documents.
	*/
	unsigned int GetNbDocs(void) const {return(NbPtr);}

	/**
	* Get the number of documents handled for a given langage.
	* @param lang          Langage of the documents
	* @returns Number of documents of this language.
	*/
	unsigned int GetNbDocs(GLang* lang) const;

	/**
	* Get an identificator that can be assigned to a new document.
	* @return unsigned int
	*/
	unsigned int GetNewId(void) const;

	/**
	* Insert a document. The document is stored in the different containers.
	* @param d               Pointer to the document.
	*/
	void InsertDoc(GDoc* d) throw(std::bad_alloc);

	/**
	* Move a document from the container holding all the documents with an
	* unknow language to the container corresponding to the language. This
	* method supposes that the documents was previously in the unkown container.
	* @param d               Pointer to the document.
	*/
	void MoveDoc(GDoc* d) throw(std::bad_alloc);

	/**
	* Get a document corresponding to a given identificator.
	* @param id         Identificator of the document.
	* @apram Pointer to GDoc.
	*/
	GDoc* GetDoc(unsigned int id) throw(std::bad_alloc, GException);

	/**
	* Get a document corresponding to a given URL.
	* @param url        URL of the document.
	* @apram Pointer to GDoc or null if the document does no exist.
	*/
	GDoc* GetDoc(const char* url) throw(std::bad_alloc);

	/**
	* Clear all the documents.
	*/
	void Clear(void);

	/**
	* Destructor of the documents.
	*/
	virtual ~GDocs(void);
};


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif

