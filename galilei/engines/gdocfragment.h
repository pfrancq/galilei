/*

	GALILEI Research Project

	GDocFragment.h

	Relevant Document for a Search - Header.

	Copyright 2003-2015 by Pascal Francq (pascal@francq.info).
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
#include <gdocfragmentrank.h>


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* The GDocFragment class provides a representation for a fragment of a document.
* In practice, a fragment starts at a given position (for example the position
* of word) and is defined by a window (such as a sentence). What extactly a
* window is depends from the type of document. Therefore, the GFilter class
* provides a method that build a string representation of a given fragment.
*
* Two document fragments are considered as identical if they are related to the
* same document and if they start at the same position
* @short Document Fragment.
*/
class GDocFragment
{
	/*
	 * Search structure needed to compare to document fragment.
	 */
	struct Search
	{
		/**
		* Identifier of the document.
		*/
		size_t DocId;

		/**
		* Position of the fragment.
		*/
		size_t Pos;

		/**
		* Constructor.
		* @param docid           Document identifier.
		* @param pos             Position in the document.
		*/
		Search(size_t docid,size_t pos);
	};

	/**
	* Reference to the document.
	*/
	GDocRef* Doc;

	/**
	* The fragment.
	*/
	R::RString Fragment;

	/**
	 * Position of the fragment.
	 */
	size_t Pos;

	/**
	 * Beginning position of the fragment window.
	 */
	size_t Begin;

	/**
	 * End position of the fragment window.
	 */
	size_t End;

	/**
	* The global ranking for the document fragment.
	*/
	double Ranking;

	/**
	* Container of all rankings associated to this document fragment.
	*/
	R::RContainer<GDocFragmentRank,true,false> Rankings;

public:

	/**
	* Constructor of a document fragment retrieved.
	* @param owner           Meta-engine.
	* @param doc             Document.
	* @param pos             Position in the document.
	* @param first           Beginning position of the window.
	* @param last            End position of the window.
	* @param engine          Name of the engine that retrieves it.
	*/
	GDocFragment(GDocRef* doc,size_t pos,size_t begin,size_t end,double ranking,const R::RString& engine);

	/**
	* Method to compare document fragments.
	* @param d               Document retrieved to compare with.
	*/
	int Compare(const GDocFragment& d) const;

	/**
	* Method to compare a document fragment and a document fragment signature.
	* @param search          Search.
	*/
	int Compare(const Search& search) const;

	/**
	 * Get the the document. If it is null, the URI is considered
	 * as unknown in the session
	 * @return the pointer to the document.
	 */
	GDocRef* GetDoc(void) const {return(Doc);}

	/**
	 * Get the starting position of the fragment.
	 * @return a size_t.
	 */
	size_t GetPos(void) const {return(Pos);}

	/**
	 * Get the beginning of the window fragment.
	 * @return a size_t.
	 */
	size_t GetBegin(void) const {return(Begin);}

	/**
	 * Get the end of the window fragment.
	 * @return a size_t.
	 */
	size_t GetEnd(void) const {return(End);}

	/**
	* Get the text fragment. If necessary, it is extracted from the file.
	* @return a R::RString.
	*/
	R::RString GetFragment(void);

	/**
	 * Set the gloabl ranking of the document fragment.
    * @param ranking        Value to assign.
    */
	void SetRanking(double ranking);

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
	R::RCursor<GDocFragmentRank> GetRankings(void) const;

	/**
	* Destruct.
	*/
	virtual ~GDocFragment(void);

	friend class GDocRef;
};


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
