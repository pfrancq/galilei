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
	 * Add a fragment.
	 * @param pos            Position to the fragment to extract.
	 * @param first          First concept found.
	 * @param last           Last concept found.
	 * @param ranking        Ranking of the document given by the engine
	 *                       (\f$0\leq ranking \leq 1\f$).
	 * @param info           Information about how the fragment was computed.
    */
	void AddFragment(size_t pos,size_t first,size_t last,double ranking,const R::RString& info);

	/**
	 * Get all the fragments.
    * @return a R::RCursor of GDocFragment.
    */
	R::RCursor<GDocFragment> GetFragments(void) const;
};

}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
