/*

	GALILEI Research Project

	GKB.h

	Generic GALILEI Session - Header.

	Copyright 2001-2015 by Pascal Francq (pascal@francq.info).
	Copyright 2001-2004 by Julien Lamoral.
	Copyright 2001-2004 by Valery Vandaele.
	Copyright 2001-2004 by David Wartel.
	Copyright 2001-2008 by the Université Libre de Bruxelles (ULB).

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
#ifndef GKB_H
#define GKB_H


//------------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>
#include <gconcept.h>
#include <gconcepttype.h>


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* The GKB represents the knowledge base. It is the basic building block for a
* session. In fact, all the methods defined here can only be called through a
* session instance.
*
* By default, four concept categories are always created: Text, Metadata,
* @warning Some methods use a reinterpret_cast<GSession*>(this) expression.
* @author GALILEI Team
* @short Knowledge Base.
*/
class GKB
{
protected:

	/**
	* State of the knowledge base.
	*/
	tObjState State;

	/**
	 * Storage manager.
	 */
	GStorage* Storage;

	/**
	 *  Must the results be saved after computed ?
	 */
	bool SaveResults;

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

	/**
	 * Predicates.
	 */
	R::RContainer<GPredicate,false,true> Predicates;

	/**
	 * Predicates (accessed by identifiers).
	 */
	R::RContainer<GPredicate,true,false> PredicatesByIds;

	/**
	 * Array of statements ordered.
	 */
	R::RContainer<GStatement,true,false> Statements;

public:

	/**
	* Constructor.
	*/
	GKB(void);

	/**
	 * The knowlegde base may be in two states only :
	 * - osOnDemand if some information is loaded from the disk.
	 * - osLatest if the latest information was loaded to the disk.
	 *
	 * When the knowledge base is in the osOnDemand state, it is not necessary to
	 * save something to the disk since an information is currently be loaded.
    * @return the state.
    */
	inline tObjState GetState(void) const {return(State);}

	/**
	* @return Pointer to storage manager.
	*/
	inline GStorage* GetStorage(void) const {return(Storage);}

	/**
	* Get the a pointer to a type of concept.
	* @param obj             Pseudo-parameter.
	* @param id              Identifier of the type.
	* @param null            If set to true, if the type does not exist,
	*                        return 0, else an exception is generated.
	* @return Pointer to a GConceptType.
	*/
	GConceptType* GetObj(const GConceptType* obj,char id,bool null);

	/**
	* Get the a pointer to a type of concept. If the concept type doesn't
	* exist, it is created.
	* @param obj             Pseudo-parameter.
   * @param cat             Concept category.
	* @param name            Name of the type.
	* @param desc            Short description
	* @return Pointer to a GConceptType.
	*/
	GConceptType* GetObj(const GConceptType* obj,tConceptCat cat,const R::RString& name,const R::RString& desc);

	/**
	* Get the a pointer to a type of concept.
	* @param obj             Pseudo-parameter.
	* @param name            Name of the type.
	* @param null            If set to true, if the type does not exist,
	*                        return 0, else an exception is generated.
	* @return Pointer to a GConceptType.
	*/
	GConceptType* GetObj(const GConceptType* obj,const R::RString& name,bool null);

	/**
	 * Get the number of concept types.
	 * @param obj            Pseudo-parameter.
	 */
	inline size_t GetNbObjs(const GConceptType* obj) const;

	/**
	 * Get the highest identifier of the concept types.
	 * @param obj            Pseudo-parameter.
	 */
	inline size_t GetMaxObjId(const GConceptType* obj) const;

	/**
	* Get all concept types defined.
	* @param obj             Pseudo-parameter.
	* @return RCursor over GConceptType.
	*/
	inline R::RCursor<GConceptType> GetObjs(const GConceptType* obj) const;

	/**
	* Insert a new concept type.
	* @param obj             Pseudo-parameter.
   * @param cat             Concept category.
	* @param id              Identifier of the type.
	* @param name            Name of the type.
	* @param desc            Short description.
	*/
	void InsertObj(const GConceptType* obj,tConceptCat cat,char id,const R::RString& name,const R::RString& desc);

	/**
	 * Get the number of concepts.
	 * @param obj            Pseudo-parameter.
	 */
	inline size_t GetNbObjs(const GConcept* obj) const;

	/**
	 * Get the highest identifier of the concepts.
	 * @param obj            Pseudo-parameter.
	 */
	inline size_t GetMaxObjId(const GConcept* obj) const;

	/**
	* Get all concept defined.
	* @param obj             Pseudo-parameter.
	* @return RCursor over GConcept.
	*/
	inline R::RCursor<GConcept> GetObjs(const GConcept* obj) const;

	/**
	* Get the concept with a specific identifier.
	* @param id             Identifier.
	* @return a pointer to a GConcept.
	*/
	GConcept* GetObj(const GConcept* obj,size_t id);

	/**
	* Get a pointer to a concept.
	* @param obj             Pseudo-parameter.
	* @param type            Type of the concept.
	* @param concept         Name of the concept to insert.
	* @param null            If set to true, if the object does not exist,
	*                        return 0, else an exception is generated.
	*/
	GConcept* GetObj(const GConcept* obj,GConceptType* type,const R::RString& concept,bool null=false);

	/**
	* Insert a concept of a given type and name. If the concept already exists,
	* nothing is done.
	* @param obj             Pseudo-parameter.
	* @param type            Type of the concept.
	* @param concept         Name of the concept to insert.
	* @return a pointer to the concept is the dictionary.
	*/
	GConcept* InsertObj(const GConcept* obj,GConceptType* type,const R::RString& concept);

	/**
	* Insert a concept that is exactly the same as a model. If the concept exist,
	* nothing is done.
	* @param concept         Model Concept.
	* @return a pointer to the concept is the dictionary.
	*/
	GConcept* InsertObj(const GConcept& concept);

	/**
	 * Rename a concept.
	 * @param concept        Concept to rename.
	 * @param name           New name.
	 * @return a pointer to the correct concept (may be an existing one).
	 */
	GConcept* RenameObj(GConcept* concept,const R::RString& name);

	/**
	* Delete a given concept from the dictionary.
	* @param concept         Concept to delete.
	*/
	void DeleteObj(GConcept* concept);

protected:

	/**
	 * Clear the block index of all the concepts for a given object type.
    * @param type           Object type.
    */
	void ClearIndex(tObjType type);

public:

	/**
	* Get a predicate.
	* @param obj             Pseudo-parameter.
	* @param id              Identifier of the type.
	* @param null            If set to true, if the type does not exist,
	*                        return 0, else an exception is generated.
	* @return a pointer to a predicate
	*/
	GPredicate* GetObj(const GPredicate* obj,size_t id,bool null);

	/**
	* Get a predicate.
	* @param obj             Pseudo-parameter.
	* @param name            Name of the type.
	* @param null            If set to true, if the type does not exist,
	*                        return 0, else an exception is generated.
	* @return a pointer to a predicate
	*/
	GPredicate* GetObj(const GPredicate* obj,const R::RString& name,bool null);

	/**
	* Get all predicates defined.
	* @param obj             Pseudo-parameter.
	*/
	R::RCursor<GPredicate> GetObjs(const GPredicate* obj) const;

	/**
	* Insert a new predicate.
	* @param obj             Pseudo-parameter.
	* @param id              Identifier of the type.
	* @param name            Name of the type.
	* @param desc            Short description.
	* @return a pointer to the predicate created.
	*/
	GPredicate* InsertObj(const GPredicate* obj,size_t id,const R::RString& name,const R::RString& desc);

	/**
	* Get a statement.
	* @param obj             Pseudo-parameter.
	* @param id              Identifier of the statement.
	* @return Pointer to a statement.
	*/
	GStatement* GetObj(const GStatement* obj,size_t id);

	/**
	* Insert a new statement.
	* @param obj             Pseudo-parameter.
	* @param id              Identifier of the statement.
	* @param predicate       Identifier of the predicate.
	* @param xi              Identifier of the \f$x_i\f$.
	* @param xitype          Type of the \f$x_i\f$.
	* @param xj              Identifier of the \f$x_j\f$.
	* @param xjtype          Type of the \f$x_j\f$.
	* @param weight          Weight of the statement.
	*/
	void InsertObj(const GStatement* obj,size_t id,size_t predicate,size_t xi,tObjType xitype,size_t xj,tObjType xjtype,double weight);

	/**
	* Destruct the session.
	*/
	virtual ~GKB(void)=0;

	friend class GConceptType;
};


//------------------------------------------------------------------------------
// some inline implementations
# include <gkb.hh>


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
