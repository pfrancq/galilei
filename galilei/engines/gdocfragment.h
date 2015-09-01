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
* In practice, a fragment is centred at a given record (for example the
* position of word) and is defined by a window (such as a sentence).
*
* What exactly a window is depends from the type of document and the search
* engine. In particular, the GFilter class provides a method that builds a
* string representation of a given fragment.
*
* A fragment can be associated with a selected record that defines it (such as
* a XML tag) and by a list of child records that select the main concept. But a
* fragment can define a whole document. In this case, the selected record is
* null.
*
* Two document fragments are considered as identical if they are related to the
* same document and if they start at the same position
* @warning The GDdocFragment class manages pointers to GConceptRecord. It is
* never responsible for their deallocation.
* @short Document Fragment.
*/
class GDocFragment
{
public:

	/*
	 * Structure needed to search for a document fragment.
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
		 * Is the fragment a whole document.
       */
		bool WholeDoc;

		/**
		* Constructor.
		* @param docid           Document identifier.
		* @param pos             Position in the document.
		* @param wholedoc        Is the fragment a whole document.
		*/
		Search(size_t docid,size_t pos,bool wholedoc);
	};

private:

	/**
	* Reference to the document.
	*/
	GDoc* Doc;

	/**
	 * Concept record.
	 */
	const GConceptRecord* Rec;

	/**
	* The fragment.
	*/
	R::RString Fragment;

	/**
	 * Position of the fragment.
	 */
	size_t Pos;

	/**
	 * Syntactic position of the fragment.
	 */
	size_t SyntacticPos;

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
	 * Date where the fragment was proposed.
	 */
	R::RDate Proposed;

	/**
	 * An information about the fragment.
	 */
	R::RString Info;

	/**
	 * Does the fragment correspond to the whole document ?
    */
	bool WholeDoc;

	/**
	 * Child records used by the query to select the node.
    */
	R::RContainer<const GConceptRecord,false,false> Children;

public:

	/**
	* Constructor of a document fragment.
	* @param doc             Document.
	* @param rec             Concept record.
	* @param pos             Position in the fragment centre.
	* @param spos            Syntactic position of the fragment centre.
	* @param begin           Beginning position of the window.
	* @param end             End position of the window.
	* @param ranking         Ranking of the fragment.
	* @param info            Information.
	*/
	GDocFragment(GDoc* doc,const GConceptRecord* rec,size_t pos,size_t spos,size_t begin,size_t end,double ranking=0.0,const R::RString& info=R::RString::Null,const R::RDate& proposed=R::RDate::Null);

	/**
	* Constructor of a document fragment representing the whole document. A
	* window must be specified (but it can be an empty one).
	* @param doc             Document.
	* @param begin           Beginning position of the window.
	* @param end             End position of the window.
	* @param ranking         Ranking of the fragment.
	* @param info            Information.
	*/
	GDocFragment(GDoc* doc,size_t begin,size_t end,double ranking=0.0,const R::RString& info=R::RString::Null,const R::RDate& proposed=R::RDate::Null);

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
	GDoc* GetDoc(void) const {return(Doc);}

	/**
	 * Look of the document fragment is a flat one. There are several cases where
	 * it is considered as flat :
	 * -# It has no selected concept node.
	 * -# The selected concept node has no parent.
	 * -# The fragment represents a whole document.
    * @return true if it is flat or false if not.
    */
	bool IsFlat(void) const;

	/**
	* Get the selected concept record corresponding to the fragment.
	* @return a pointer to a GConceptRecord.
	* @warning The pointer may be null if the fragment corresponds to the whole
	* document or if the structure trees are not built during the analysis.
	*/
	const GConceptRecord* GetRecord(void) const {return(Rec);}

	/**
    * @return the number of children.
    */
	inline size_t GetNbChildren(void) const {return(Children.GetNb());}

	/**
    * @return a cursor over the children.
    */
	R::RCursor<const GConceptRecord> GetChildren(void) const;

	/**
	 * @return the date of the suggestion.
	 */
	R::RDate GetProposed(void) const {return(Proposed);}

	/**
	 * Get the position of the fragment centre.
	 * @return a size_t.
	 */
	size_t GetPos(void) const {return(Pos);}

	/**
	 * Get the syntactic position of the fragment centre.
	 * @return a size_t.
	 */

	size_t GetSyntacticPos(void) const {return(SyntacticPos);}

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
	 * @return the information associated with a suggestion.
	 */
	R::RString GetInfo(void) const {return(Info);}

	/**
	 * Set the global ranking of the document fragment.
    * @param ranking        Value to assign.
    */
	void SetRanking(double ranking);

	/**
	* Get the global ranking associated to this document.
	*/
	double GetRanking(void) const {return(Ranking);}

	/**
	 * Add a child record to the document fragment. The interval of the fragment
	 * is adjusted if necessary in order to contain the child (except if the
	 * fragment represents the whole document).
    * @param rec            Concept record to add.
    */
	void AddChild(const GConceptRecord* rec);

	/**
	 * Look if two fragments overlaps. In practice, the method follows different
	 * steps :
	 * -# It looks if at least one fragment represents the whole document.
	 * -# It looks if both fragments have the same selected node or no
	 *    parent nodes nodes (for flat documents).
	 * -# It looks if the two intervals overlap.
	 * @param fragment       Fragment to compare with.
	 * @return true if overlap.
	 */
	bool Overlap(const GDocFragment* fragment) const;

	/**
	 * Merge the children of a fragment. The interval of the fragment is adjusted
	 * if necessary in order to contain all the children  (except if the fragment
	 * represents the whole document).
    * @param fragment       Fragment to compare with.
    */
	void Merge(const GDocFragment* fragment);

	/**
	 * Print some information related to the document fragment.
    */
	void Print(void) const;

	/**
	* Static function used to order the document fragments by ranking (the
	* highest first). This function can be used with the RContainer::ReOrder
	* method.
	* @param a              Pointer to the first object.
	* @param b              Pointer to the second object.
	*/
	static int SortOrderRanking(const void* a,const void* b);

	/**
	* Destruct.
	*/
	virtual ~GDocFragment(void);

	friend class GDocRef;
};


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
