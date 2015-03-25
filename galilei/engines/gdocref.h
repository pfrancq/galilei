/*

	GALILEI Research Project

	GDocRef.h

	Document Reference - Header.

	Copyright 2008-2015 by Pascal Francq (pascal@francq.info).

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



//------------------------------------------------------------------------------
#ifndef GDocRefH
#define GDocRefH


//-----------------------------------------------------------------------------
// include files for GALILEI Project
#include <galilei.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
 * The GDocRef class provides a reference on a document. It manages a container
 * of GDocFragment.
 * @short Document Reference.
 */
class GDocRef
{
	/**
	 * Document referenced.
	 */
	GDoc* Doc;

	/**
	 * List of fragments.
	 */
	R::RContainer<GDocFragment,true,true> Fragments;

public:

	/**
	 * Construct a reference to a document.
    * @param doc            Document referenced.
    */
	GDocRef(GDoc* doc);

	/**
	 * Compare two document references.
    * @param ref            Document reference to compare with.
    * @return -1,0 or 1 depending on the comparison of the document identifiers.
    */
	int Compare(const GDocRef& ref) const;

	/**
	 * Compare a document reference and a document.
    * @param doc            Document to compare with.
    * @return -1,0 or 1 depending on the comparison of the document identifiers.
    */
	int Compare(const GDoc* doc) const;

	/**
	 * Get the document referenced.
    * @return a pointer to a GDoc.
    */
	GDoc* GetDoc(void) const {return(Doc);}

	/**
	 * Clear the fragments.
    */
	void Clear(void);

	/**
	 * Add a fragment related to a given node.
	 * @param node           Concept node associated to the fragment.
	 * @param pos            Position of the concept node that identifies the
	 *                       fragment.
	 * @param spos           Syntactic position of the fragment centre.
	 * @param begin          Beginning of the fragment.
	 * @param end            Ending of the fragment.
	 * @param children       Must all the children be considered as selecting the
	 *                       fragment (true) or not (false).
	 * @param exist          Set by the method to specify if the fragment already
	 *                       exists (true) or was created (false).
	 * @return a pointer to a GDocFragment.
    */
	GDocFragment* AddFragment(const GConceptNode* node,size_t pos,size_t spos,size_t begin,size_t end,bool children,bool& exist);

	/**
	 * Add a fragment related to a given node. The fragment is limited to the
	 * concept node.
	 * @param node           Concept node associated to the fragment.
	 * @return a pointer to a GDocFragment.
    */
	GDocFragment* AddFragment(const GConceptNode* node);

	/**
	 * Add a fragment selected by a child node.
	 * @param node           Concept node associated to the fragment.
	 * @oaram child          Child concept node use to select the concept node.
	 * @param pos            Position of the concept node that identifies the
	 *                       fragment.
	 * @param spos           Syntactic position of the fragment centre.
	 * @param begin          Beginning of the fragment.
	 * @param end            Ending of the fragment.
	 * @param children       Must all the children be considered as selecting the
	 *                       fragment (true) or not (false).
	 * @param exist          Set by the method to specify if the fragment already
	 *                       exists (true) or was created (false).
	 * @return a pointer to a GDocFragment.
    */
	GDocFragment* AddFragment(const GConceptNode* node,const GConceptNode* child,size_t pos,size_t spos,size_t begin,size_t end,bool children,bool& exist);

	/**
	 * Add a fragment selected by a child node. The fragment is limited to the
	 * concept node and the child node.
	 * @param node           Concept node associated to the fragment.
	 * @param child          Child concept node use to select the concept node.
	 * @param exist          Set by the method to specify if the fragment already
	 *                       exists (true) or was created (false).
	 * @return a pointer to a GDocFragment.
    */
	GDocFragment* AddFragment(const GConceptNode* node,const GConceptNode* child,bool& exist);

	/**
	 * Add a document that represents the whole document.
    * @param doc            Document.
	 * @param child          Child concept node use to select the concept node.
	 * @param begin          Beginning of the fragment.
	 * @param end            Ending of the fragment.
 	 * @param exist          Set by the method to specify if the fragment already
	 *                       exists (true) or was created (false).
    * @return a pointer to a GDocFragment.
    */
	GDocFragment* AddFragment(GDoc* doc,const GConceptNode* child,size_t begin,size_t end,bool& exist);

	/**
	 * Copy a fragment into the container.
    * @param tocopy         Fragment to copy.
	 * @param exist          Set by the method to specify if the fragment already
	 *                       exists (true) or was created (false).
    * @return a pointer to a GDocFragment.
    */
	GDocFragment* CopyFragment(const GDocFragment* tocopy,bool& exist);

	/**
	 * Copy a set of fragments into the container.
    * @param tocopy         Fragments to copy.
    * @return
    */
	void CopyFragments(const GDocRef* tocopy);

	/**
    * @return the number of fragment.
    */
	size_t GetNbFragments(void) const {return(Fragments.GetNb());}

	/**
	 * Get all the fragments.
    * @return a R::RCursor of GDocFragment.
    */
	R::RCursor<GDocFragment> GetFragments(void) const;

	/**
	 * Look if a document fragment has a sibling fragment in the container.
	 * @param fragment       Fragment to analyse.
	 * @return true if it is the case.
	 */
	bool FindSibling(GDocFragment* fragment) const;

	/**
	 * Look if a document fragment has a child fragment in the container.
	 * @param fragment       Fragment to analyse.
	 * @return true if it is the case.
	 */
	bool FindChild(GDocFragment* fragment) const;

	/**
	 * Reduce the fragments.
	 * @param force          Force to merge the fragments sharing the same node.
	 *                       It should be true in the case of an AND operator,
	 *                       and false in the case of an OR operator.
    */
	void Reduce(bool force);

	/**
	 * Print the information of each fragment.
	 */
	void Print(void);
};


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
