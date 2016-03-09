/*

	GALILEI Research Project

	GPromRank.h

	Promethee Ranking Method - Header.

   Copyright 2015-2015 by Pascal Francq.
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
#ifndef GPromRankH
#define GPromRankH


//-----------------------------------------------------------------------------
// include files for R
#include <robject.h>
using namespace std;
using namespace R;


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <gcomputerank.h>
using namespace GALILEI;


//------------------------------------------------------------------------------
/**
* The GNoRank class provides a representation for a ranking method that does
* nothing.
* @short No Ranking Method.
*/
class GPromRank : public RObject, public GComputeRank
{
	class cIff;
	class cRef;

	/**
	* Parameter for the Tf/Idf criterion.
	*/
	RParamStruct* TfIdf;

	/**
	* Parameter for the Type.
	*/
	RParamStruct* Type;

	/**
	* Parameter for the Distance criterion.
	*/
	RParamStruct* Distance;

	/**
	* Parameter for the Specificity criterion.
	*/
	RParamStruct* Specificity;

	/**
	* Parameter for the TfIff criterion.
	*/
	RParamStruct* TfIff;

	/**
	* Parameter for the occurrence criterion.
	*/
	RParamStruct* Occurrence;

	/**
	 * Container of the iff factor that weights the occurrences of a given
	 * concept in the fragments associated to same parent concept.
	 */
	RContainer<cIff,true,false> Iffs;

	/**
	 * Are the iff factors dirty ?
	 */
	bool IffsDirty;

	/**
	 * Must the iff factors be saved.
	 */
	bool SaveIffs;

	/**
	 * Temporary identifiers.
	 */
	RContainer<cRef,true,false> TmpRefs;

	/**
	 * Weighting method used.
    */
	GMeasure* Weighting;

	/**
	 * Query used to make the search.
	 */
	GSearchQuery* Query;

public:

	/**
	* Constructor.
	* @param session         Session.
	* @param fac             Factory.
	*/
	GPromRank(GSession* session,GPlugInFactory* fac);

	/**
	 * This method is called each time a session is opened. In particular, it
	 * sets the Weighting variable and insert it as observer for
	 * GALILEI::hCurrentPlugIn notification.
    */
	virtual void Init(void);

	/**
	 * This method is called each time a session is closed.
    */
	virtual void Done(void);

	/**
	 * Handle the GALILEI;;hCurrentPlugIn notification. In practice, it sets the
	 * Weighting variable.
    * @param notification
    */
	void HandleCurrentPlugIn(const R::RNotification& notification);

	/**
	 * This method handles the notifications that imply that the tf/idf factors
	 * must be updated by adding the references of the sender.
	 * @param notification   Notification received.
	 */
	void HandleAddDoc(const R::RNotification& notification);

	/**
	 * This method handles the notifications that imply that the tf/idf factors
	 * must be updated by removing the references of the sender.
	 * @param notification   Notification received.
	 */
	void HandleDelDoc(const R::RNotification& notification);

	/**
	 * Reset the engine.
    */
	virtual void Reset(void);

	/**
	 * Handle a notification that an object type must be reinitialised.
    * @param notification   Notification.
    */
	void HandleReInit(const R::RNotification& notification);

	/**
	 * Rank a set of document fragments.
	 * @param engine         Engine which results must be ranked.
	 * @param query          Query.
	 * @param caller         Identifier of the caller (for example a thread).
	 */
	virtual void Rank(GEngine* engine,GSearchQuery* query,size_t caller);

	/**
	* Create the parameters.
	*/
	virtual void CreateConfig(void);

	/**
	* Configurations were applied from the factory.
	*/
	virtual void ApplyConfig(void);

private:

	/**
	 * Build the references for a given node set.
	 * @param nodes          Nodes to update
    */
	void BuildRefs(RNodeCursor<GConceptTree,GConceptNode>& nodes);

	/**
	 * Get a reference to the IFF entry of a given concept.
	 * @param conceptid      Concept identifier.
    */
	cIff* GetIff(size_t conceptid);

	/**
	 * Update the references of a given tree.
	 * @param tree           Tree to update
	 * @param add            Add the references or not.
    */
	void UpdateRefs(const GConceptTree* tree,bool add);

	/**
	 * Recompute the references of concepts in fragments.
    */
	void RecomputeRefs(void);

public:

	/**
	 * Compute the iff factor of a given concept in a given context (fragments
	 * represented by a same parent concept). It is the same factor as idf by
	 * where the computation is made on the fragment level rather than on the
	 * document level.
	 * @param conceptid      Identifier of the concept.
	 * @param parentid       Identifier of the parent (may be cNoRef).
	 * @return the iff factor.
	 */
	double GetIff(size_t conceptid,size_t parentid);

	friend class GProm;
};


//-----------------------------------------------------------------------------
#endif
