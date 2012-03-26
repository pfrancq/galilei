/*

	GALILEI Research Project

	GDocFragment.h

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
#ifndef GDocFragmentH
#define GDocFragmentH


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
* The GDocFragment class provides a representation for a fragment of a document
* proposed by a search engine (with its ranking).
* @short Document Fragment Retrieved
*/
class GDocFragment
{
	/*
	 * Search structure needed to compare to document fragment.
	 */
	struct Search
	{
		/**
		* URI of the document.
		*/
		const R::RURI& URI;

		/**
		* Position of the fragment.
		*/
		size_t Pos;

		/**
		* Position of the first query concept found.
		*/
		size_t First;

		/**
		* Position of the last query concept found.
		 */
		size_t Last;

		/**
		* Constructor.
		* @param uri             URI of the document.
		* @param pos             Position in the document.
		* @param first           First concept found.
		* @param last            Last concept found.
		*/
		Search(const R::RURI& uri,size_t pos,size_t first,size_t last);
	};

	/**
	 * Meta-engine owning this document fragment.
	 */
	GMetaEngine* Owner;

	/**
	* Pointer to the document (may be null for an unknown document by the
	 * system).
	*/
	GDoc* Doc;

	/**
	 * URI of the document.
	 */
	R::RURI URI;

	/**
	 * Title of the document.
	 */
	R::RString Title;

	/**
	* The fragment.
	*/
	R::RString Fragment;

	/**
	 * Starting position of the fragment.
	 */
	size_t Pos;

	/**
	 * Position of the first query concept found.
	 */
	size_t First;

	/**
	 * Position of the last query concept found.
	 */
	size_t Last;

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
	* Constructor of a document fragment retrieved.
	* @param owner           Meta-engine.
	* @param doc             Document.
	* @param pos             Position in the document.
	* @param first           First concept found.
	* @param last            Last concept found.
	* @param engine          Name of the engine that retrieves it.
	*/
	GDocFragment(GMetaEngine* owner,GDoc* doc,size_t pos,size_t first,size_t last,double ranking,const R::RString& engine);

	/**
	* Constructor of a document fragment retrieved.
	* @param owner           Meta-engine.
	* @param uri             URI of the document.
	* @param title           Title of the document.
	* @param fragment        Fragment.
	* @param ranking         Ranking of the document.
	* @param engine          Name of the engine that retrieves it.
	*/
	GDocFragment(GMetaEngine* owner,const R::RString& uri,const R::RString& title,const R::RString& fragment,double ranking,const R::RString& engine);

	/**
	* Method to compare document retrieved based on their URI.
	* @param d               Document retrieved to compare with.
	*/
	int Compare(const GDocFragment& d) const;

	/**
	* Method to a document retrieved with a search structure.
	* @param search          Search.
	*/
	int Compare(const Search& search) const;

	/**
	 * Get the the document. If it is null, the URI is considered
	 * as unknown in the session
	 * @return the pointer to the document.
	 */
	GDoc* GetDoc(void) const {return(Doc);}

	/**
    * @return the document URI.
    */
	R::RURI GetURI(void) const {return(URI);}

	/**
    * @return the document title.
    */
	R::RString GetTitle(void) const {return(Title);}

	/**
	 * @return the starting position of the fragment.
	 */
	size_t GetPos(void) const {return(Pos);}

	/**
	 * @return the position of the first query concept found.
	 */
	size_t GetFirst(void) const {return(First);}

	/**
	 * @return the position of the last query concept found.
	 */
	size_t GetLast(void) const {return(Last);}

	/**
	* @return the fragment of the document.
	*/
	R::RString GetFragment(void);

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
	virtual ~GDocFragment(void);

	friend class GMetaEngine;
};


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
