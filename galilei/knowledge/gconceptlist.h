/*

	GALILEI Research Project

	GConceptList.h

	Concept List - Header.

	Copyright 2007-2015 by Pascal Francq (pascal@francq.info).
	Copyright 2007-2008 by the Université Libre de Bruxelles (ULB).

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



//-----------------------------------------------------------------------------
#ifndef GConceptList_H
#define GConceptList_H


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <gconcept.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
* The GConceptList class provides a representation for a concept described as a
* list of concept. Its "definition" is given by:
* @code
* stem1_id:stem2_id:stem3_id
* @endcode
* @author Pascal Francq
* @short Concept List
*/
class GConceptList : public GConcept
{
	/**
	 * Concepts.
	 */
	R::RContainer<GConcept,false,true> Concepts;

public:

	/**
	 * Copy constructor.
	 * @param list           Original list.
	 */
	GConceptList(const GConceptList* list);

	/**
	* Constructor of a concept list. The full definition of the concept is
	* deduced from its definition.
	* @param session         Session.
	* @param name            Name of the concept.
	* @param type            Type of the concept.
	* @param def             Definition of the concept.
	*/
	GConceptList(GSession* session,const R::RString& name,GConceptType* type,const R::RString& def);

	/**
	 * Constructor of a concept list.
	 * @param session        Session.
	 * @param type           Type of the concept.
	 */
	GConceptList(GSession* session,GConceptType* type);

	/**
	 * Constructor of a concept list.
	 * @param session        Session.
	 * @param type           Type of the concept.
	 * @param name           Name of the concept. If name is empty, the name is
	 *                       associated to the definition.
	 * @param concepts       Concepts.
	 */
	GConceptList(GSession* session,GConceptType* type,const R::RString& name,R::RContainer<GConcept,false,true>& concepts);

	/**
	* Constructor of a XML Index. The full definition of the concept is deduced
	* from its name.
	* @param session         Session.
	* @param id              Identifier.
	* @param name            Name of the concept.
	* @param type            Type of the concept.
	* @param def             Definition of the concept.
	* @param idxdocs         Identifier of the block the inverted file related
	*                        to the documents.
	* @param idxprofiles     Identifier of the block the inverted file related
	*                        to the profiles.
	* @param idxcommunities  Identifier of the block the inverted file related
	*                        to the communities.
	* @param idxtopics       Identifier of the block the inverted file related
	*                        to the topics.
	* @param idxclasses      Identifier of the block the inverted file related
	*                        to the classes.
	*/
	GConceptList(GSession* session,size_t id, const R::RString& name, GConceptType* type,const R::RString& def,
			size_t idxdocs, size_t idxprofiles, size_t idxcommunities, size_t idxtopics, size_t idxclasses);

	/**
    * @return the class name.
    */
	virtual R::RCString GetClassName(void) const {return("GConceptList");}

protected:

	/**
	 * Build the full definition of the XML index based on its name.
	 * @param def             Definition of the concept.
	 */
	void BuildFromDef(const R::RString& def);

public:

	/**
	 * Build the definition of the list by parsing all of its concepts.
	 * @return the definition of the list.
	 */
	R::RString GetDef(void) const;

	/**
	 * Do a deep copy of the current concept.
	 * @return Pointer to a new element created.
	 */
	virtual GConcept* DeepCopy(void) const;

	/**
	* Clear all the information of the data.
	*/
	virtual void Clear(void);

	/**
	 * Get the concepts.
	 */
	R::RCursor<GConcept> GetConcepts(void) const;

	/**
	 * Verify if the index is empty.
	 */
	inline bool IsEmpty(void) const {return(!Concepts.GetNb());}

	/**
	 * Get the number of concepts in the list.
	 */
	inline size_t GetNbConcepts(void) const {return(!Concepts.GetNb());}

	/**
	 * Add a concept.
	 * @param concept        Concept to add.
	 */
	void AddConcept(GConcept* concept);

	/**
	 * This method computes a similarity between two concept lists. A raw
	 * comparison is done between the two lists of stems.
	 * @param list           Second list.
	 * @returns Number between 0 (nothing in common) and 1 (same list).
	 */
	double GetSimilarity(const GConceptList* list) const;

	/**
	* Destruct the concept list.
	*/
	virtual ~GConceptList(void);
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
