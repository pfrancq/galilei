/*

	GALILEI Research Project

	GDocRetrieved.h

	Relevant Document for a Search - Header.

	Copyright 2003-2012 by Pascal Francq.
	Copyright 2003-2004 by Valery Vandaele.
	Copyright 2003-2008 Université Libre de Bruxelles (ULB).

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
#ifndef GDocRetrievedH
#define GDocRetrievedH


//------------------------------------------------------------------------------
// include files for ANSI C/C++
#include <stdexcept>


//------------------------------------------------------------------------------
// include files for R library
#include <rstd.h>
#include <rstring.h>
#include <rcontainer.h>


//------------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>
#include <gdocranking.h>


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* The GDocRetrieved class provides a representation for a document proposed by a
* search engine (with its ranking).
* @short Document Retrieved
*/
class GDocRetrieved
{
private:

	/**
	* The identifier of the document (may be null for an unknown document by the
	 * system).
	*/
	size_t DocId;

	/**
	 * URI of the document.
	 */
	R::RString URI;

	/**
	 * Title of the document.
	 */
	R::RString Title;

	/**
	* The description of the resulting page.
	*/
	R::RString Description;

	/**
	* The global ranking for the current document
	*/
	double Ranking;

	/**
	* Container of all rankings associated to this document.
	*/
	R::RContainer<GDocRanking,true,false> Rankings;

public:

	/**
	* Constructor of a document retrieved.
	* @param docid           Identifier of the document.
	* @param uri             URI of the document.
	* @param title           Title of the document.
	* @param description     Description of the document.
	* @param ranking         Ranking of the document.
	* @param engine          Name of the engine that retrieves it.
	*/
	GDocRetrieved(size_t docid,const R::RString uri,const R::RString title,const R::RString description,double ranking,const R::RString engine);

	/**
	* Method to compare document retrieved based on their URI.
	* @param d               Document retrieved to compare with.
	*/
	int Compare(const GDocRetrieved& d) const;

	/**
	* Method to a document retrieved with an URI.
	* @param uri             URI to compare with.
	*/
	int Compare(const R::RString& uri) const;

	/**
	 * Get the identifier of the document. If it is null, the URI is considered
	 * as unknown in the session
	 * @return the identifier of the document.
	 */
	size_t GetDocId(void) const {return(DocId);}

	/**
    * @return the document URI.
    */
	R::RString GetURI(void) const {return(URI);}

	/**
    * @return the document title.
    */
	R::RString GetTitle(void) const {return(Title);}

	/**
	* Get the description of the document
	*/
	R::RString GetDescription(void) const {return(Description);}

	/**
	* Get the global ranking associated to this document.
	*/
	double GetRanking(void) const {return(Ranking);}

	/**
	* Add new ranking for the current document retrieved.
	* @param ranking         Ranking
	* @param engine          Engine
	*/
	void AddRanking(double ranking,const R::RString engine);

	/**
	* @return a cursor on the rankings of the document retrieved.
	*/
	R::RCursor<GDocRanking> GetRankings(void) const;

	/**
	* Destruct.
	*/
	virtual ~GDocRetrieved(void);

	friend class GMetaEngine;
};


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
