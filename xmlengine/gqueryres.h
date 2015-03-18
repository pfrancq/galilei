/*

	GALILEI Research Project

	GQueryRes.h

	Query Result - Header.

	Copyright 2008-2015 by Pascal Francq (pascal@francq.info).
   Copyright 2004-2005 by Jean-Baptiste Valsamis.
	Copyright 2005-2009 by Faïza Abbaci.

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
#ifndef GQueryResH
#define GQueryResH


//------------------------------------------------------------------------------
// include files for R/GALILEI Projects
#include <galilei.h>
#include <gdocref.h>
#include <gquerynode.h>
using namespace R;
using namespace GALILEI;
using namespace std;


//------------------------------------------------------------------------------
/**
 * The GQueryRes provides a representation from a list of documents
 * corresponding to a given query (or a part of it).
 * @short Query Result.
 */
class GQueryRes : RContainer<GDocRef,true,true>
{
public :

	/**
	 * Construct an empty query result.
    */
	GQueryRes(void);

	/**
	 * Construct a query result.
    * @param size           Initial size of the container.
    */
	GQueryRes(size_t size);

	/**
	 * Add a document to the list containing to a concept. In practice, all the
	 * occurrences of the concept are also added.
	 * @param engine         Engine launching the search.
    * @param doc            Document to add.
	 * @param concept        Concept associated to a document.
    */
	void AddDoc(GEngineXML* engine,GDoc* doc,GConcept* concept);

	/**
	 * Insert a list of concept nodes corresponding to a document.
	 * @param doc            Document to add.
	 */
	void InsertDoc(GDocRef* doc);

	/**
	 * Compare two query results.
    * @return always -1.
    */
	int Compare(const GQueryRes&) const {return(-1);}

	/**
	 * Print the results on the output screen.
	 */
	void Print(void) const;

	/**
	 * @return a list of concept nodes corresponding to a given query.
	 */
	RCursor<GDocRef> GetDocs(void) const;

	/**
	 * @return the number of results.
	 */
	size_t GetNb(void) const {return(RContainer<GDocRef,true,true>::GetNb());}

	/**
	 * Reduce the node sets.
    */
	void Reduce(void);
};


//------------------------------------------------------------------------------
#endif
