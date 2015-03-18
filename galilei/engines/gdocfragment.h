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
* In practice, a fragment is centred at a given position (for example the
* position of word) and is defined by a window (such as a sentence).
*
* What extactly a window is depends from the type of document and the search
* engine. In particular, the GFilter class provides a method that builds a
* string representation of a given fragment.
*
* A fragment can be associated with a selected concept that defines it (such as
* a XML tag) and by a list of child concepts that select the main concept. In
* particular, a fragment can define a whole document.
*
* Two document fragments are considered as identical if they are related to the
* same document and if they start at the same position
* @short Document Fragment.
*/
class GDocFragment
{
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
		* Constructor.
		* @param docid           Document identifier.
		* @param pos             Position in the document.
		*/
		Search(size_t docid,size_t pos);
	};

	/**
	* Reference to the document.
	*/
	GDoc* Doc;

	/**
	 * Concept node.
	 */
	GConceptNode* Node;

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
	 * Child nodes used by the query to select the node.
    */
	R::RContainer<GConceptNode,false,false> Children;

public:

	/**
	* Constructor of a document fragment.
	* @param doc             Document.
	* @param node            Concept node.
	* @param pos             Position in the document.
	* @param first           Beginning position of the window.
	* @param end             End position of the window.
	* @param ranking         Ranking of the fragment.
	* @param info            Information.
	*/
	GDocFragment(GDoc* doc,GConceptNode* node,size_t pos,size_t begin,size_t end,double ranking=0.0,const R::RString& info=R::RString::Null,const R::RDate& proposed=R::RDate::Null);

	/**
	* Constructor of a document fragment representing the whole document.
	* @param doc             Document.
	* @param node            Concept node.
	* @param ranking         Ranking of the fragment.
	* @param info            Information.
	*/
	GDocFragment(GDoc* doc,GConceptNode* node,double ranking=0.0,const R::RString& info=R::RString::Null,const R::RDate& proposed=R::RDate::Null);

	/**
	* Method to compare document fragments.
	* @param d               Document retrieved to compare with.
	*/
	int Compare(const GDocFragment& d) const;

	/**
	* Method to compare a document fragment and a document.
	* @param doc           Document.
	*/
	int Compare(const GDoc* d) const;

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
	* Get the selected concept node corresponding to the fragment.
	* @return a pointer to a GConceptNode
	* @warning The pointer may be null if the fragment corresponds to the whole
	* document or if the structure trees are not built during the analysis.
	*/
	GConceptNode* GetNode(void) const {return(Node);}

	/**
    * @return the number of children.
    */
	inline size_t GetNbChildren(void) const {return(Children.GetNb());}

	/**
    * @return a cursor over the children.
    */
	R::RCursor<GConceptNode> GetChildren(void) const;

	/**
	 * @return the date of the suggestion.
	 */
	R::RDate GetProposed(void) const {return(Proposed);}

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
	 * @return the information associated with a suggestion.
	 */
	R::RString GetInfo(void) const {return(Info);}

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
	 * Add a child node to the document fragment.
    * @param child          Concept node to add.
    */
	void AddChild(GConceptNode* child);

	/**
	 * Look if two fragments overlaps, i.e. they have children of a common node
	 * that are not too much remote.
	 * @param fragment       Fragment to compare with.
	 * @return true if overlap.
	 */
	bool Overlap(const GDocFragment* fragment) const;

	/**
	 * Merge the children of a fragment.
    * @param fragment       Fragment to compare with.
    */
	void Merge(const GDocFragment* fragment);

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
