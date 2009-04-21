/*

	GALILEI Research Project

	GOntology.h

	Ontology - Header.

	Copyright 2008-2009 by Pascal Francq (pfrancq@ulb.ac.be).

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
#ifndef GOntology_H
#define GOntology_H



//-----------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>
#include <gbasicsession.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
 * The GOntology class manages the ontology associated to a given session (most
 * of a the time a particular documents collection).
 * @author Pascal Francq
 * @short Ontology.
 */
class GOntology : virtual public GBasicSession
{
	/**
	 *  Types of Concepts.
	 */
	R::RContainer<GConceptType,true,true> ConceptTypes;

	/**
	 *  Types of Concepts (accessed by identifiers).
	 */
	R::RContainer<GConceptType,false,false> ConceptTypesByIds;

	/**
	* Array of concepts ordered.
	*/
	R::RContainer<GConcept,true,false> Concepts;

public:

	/**
	 * Construct the ontology.
	 * @param nbconcepts     Number of concepts actually managed.
	 */
	GOntology(size_t nbconcepts);

	/**
	* Get all concept types defined.
	* @return RCursor over GConceptType.
	*/
	R::RCursor<GConceptType> GetConceptTypes(void) const;

	/**
	* Get the a pointer to a type of concept.
	* @param id              Identifier of the type.
	* @param null            If set to true, if the type does not exist,
	*                        return 0, else an exception is generated.
	* @return Pointer to a GConceptType
	*/
	GConceptType* GetConceptType(char id,bool null);

	/**
	* Get the a pointer to a type of concept.
	* @param name            Name of the type.
	* @param null            If set to true, if the type does not exist,
	*                        return 0, else an exception is generated.
	* @return Pointer to a GConceptType
	*/
	GConceptType* GetConceptType(const R::RString& name,bool null);

	/**
	* Get the a pointer to a type of concept. If the concept type doesn't
	* exist, it is created.
	* @param name            Name of the type.
	* @param desc            Short description
	* @return Pointer to a GConceptType
	*/
	GConceptType* GetInsertConceptType(const R::RString& name,const R::RString& desc);

	/**
	* Insert a new concept type.
	* @param id              Identifier of the type.
	* @param name            Name of the type.
	* @param desc            Short description.
	* @param refdocs         Number of documents referenced.
	* @param refprofiles     Number of profiles referenced.
	* @param refgroups       Number of groups referenced.
	* @param reftopics       Number of topics referenced.
	*/
	void InsertConceptType(char id,const R::RString& name,const R::RString& desc,size_t refdocs,size_t refprofiles,size_t refgroups,size_t reftopics);

	/**
	* Get all concepts defined.
	* @return RCursor over GConcept.
	*/
	R::RCursor<GConcept> GetConcepts(void) const;

	/**
	* Get the concept with a specific identifier.
	* @param id             Identifier.
	* @return Pointer to a GConcept.
	*/
	GConcept* GetConcept(size_t id);

	/**
	* Insert a concept. In practice, it is a copy of the concept which is
	* inserted.
	* @param concept         Concept to insert.
	* @return Pointer to the concept inserted.
	*/
	GConcept* InsertConcept(const GConcept* concept);

	/**
	* Delete a given concept from the dictionary.
	* @param concept         Concept to delete.
	*/
	void DeleteConcept(GConcept* concept);

	/**
	 * Rename a concept.
	 * @param concept        Concept to rename.
	 * @param name           New name.
	 * @return Pointer to the correct concept (may be an existing one).
	 */
	GConcept* RenameConcept(GConcept* concept,const R::RString& name);

	/**
	 * Destruct the ontology.
	 */
	virtual ~GOntology(void);

	friend class GConceptType;
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
