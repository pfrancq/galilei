/*

	GALILEI Research Project

	GDocsLang.h

	List of SubProfiles for a given Language - Implementation.

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
#ifndef GDocLangH
#define GDocLangH


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/**
* The GDocsLang class provides a representation to maintain all the documents for the specific langage
* This class represents a list of GDocs. One GDocs fo each langage.
* @author Pascal Francq , Valery Vandaele
* @short SubProfiles.
*/
class GDocsLang : public RStd::RContainer<GDoc,unsigned int,false,true>
{
protected:

	/**
	* Language of the documents.
	*/
	GLang* Lang;

public:

	/**
	* Constructor of Users
	* @param lang               The langage of all the documents contained in the list
	*/
	GDocsLang(GLang* lang) throw(bad_alloc);


	/**
	* Insert a Document in the container.
	* @param s              Pointer to the document to add.
	*/
	void InsertDoc(GDoc* p) throw(bad_alloc);

	/**
	* Get a cursor over the profiles used in the system.
	*/
	GDocCursor& GetDocsLangCursor();

	/**
	* Get a cursor over the documents of the system.
	*/
	GLang* GetLang(void) const {return Lang;}

	/**
	* Compare function needed by RStd::RContainer
	* @param doc             Documents used for the comparaison.
	*/
	int Compare(const GDocsLang& docLang) const;

	/**
	* Compare function needed by RStd::RContainer.
	* @param doc            Pointer to the document used for the comparaison.
	*/
	int Compare(const GDocsLang* docLang) const;

	/**
	* Compare function needed by RStd::RContainer.
	* @param lang           Language used for the comparaison.
	*/
	int Compare(const GLang*) const;

public:

	/**
	* Destructor.
	*/
	virtual ~GDocsLang(void);
};


//-----------------------------------------------------------------------------
/**
* The GDocLangCursor class provides a way to go trough a set of profilesSim
* of a given language.
* @short ProfilesSims Cursor.
*/
//CLASSCURSOR(GDocsLangCursor,GDocsLang,unsigned int)


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
 
