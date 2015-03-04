/*

	GALILEI Research Project

	GEngineXML.h

	XML Search Engine - Header.

	Copyright 2010-2015 by Pascal Francq (pascal@francq.info).
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
#ifndef GEngineXMLH
#define GEngineXMLH


//-----------------------------------------------------------------------------
// include files for R/GALILEI Projects
#include <robject.h>
#include <gengine.h>
using namespace std;
using namespace R;
using namespace GALILEI;


//------------------------------------------------------------------------------
// forward declaration
class GQuery;
class GQueryRes;


//------------------------------------------------------------------------------
/**
* The GEngineXML class provides a search engine over XML documents
* @short XML Search Engine
*/
class GEngineXML : public RObject, public GEngine
{
	class cTreeRef;
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
	* Number of results
	*/
	size_t NbResults;

	/**
	 * Retrieve only documents.
	 */
	bool OnlyDocs;

	/**
	 * Container of the most used document trees.
	 */
	RContainer<cTreeRef,true,true> Trees;

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

public:

	/**
	* Construct XML search engine.
	* @param session         Session.
	* @param fac             Factory.
	*/
	GEngineXML(GSession* session,GPlugInFactory* fac);

	/**
	* Create the parameters.
	*/
	virtual void CreateConfig(void);

	/**
	* Configurations were applied from the factory.
	*/
	virtual void ApplyConfig(void);

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
	 * Handle a notification that an object type must be reinitialized.
    * @param notification   Notification.
    */
	void HandleReInit(const R::RNotification& notification);

	/**
	* Request a query.
	* @param query           Query.
	*/
	virtual void PerformRequest(const RString& query);

private:

	/**
	 * Perform the PROMETHEE method to rank the document fragments.
    * @param req            Query performed.
    * @param res            Fragment selected.
    */
	void RankFragments(GQuery& req,const GQueryRes* res);

	/**
	 * Perform a simple selection of each document for which at least one
	 * fragment was selected.
	 * @param req            Query performed.
    * @param res            Fragment selected.
    */
	void SelectDocs(GQuery& req,const GQueryRes* res);

	/**
	 * Method used to ordered the blocks by descending order of accesses.
	 * @param a              Pointer to a block.
	 * @param b              Pointer to the second block.
	 */
	static int sortOrderAccess(const void* a,const void* b);

public:

	/**
	 * Get a concept tree for a given document. If it is not loaded into memory,
	 * the engine loads it.
	 * @param docid          Identifier of the document.
	 * @return a pointer to the tree.
	 */
	const GConceptTree* GetTree(size_t docid);

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
