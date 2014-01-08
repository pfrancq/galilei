/*

	GALILEI Research Project

	GKB.h

	Generic GALILEI Session - Header.

	Copyright 2001-2014 by Pascal Francq (pascal@francq.info).
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
    * @return the state of the session.
    */
	inline tObjState GetState(void) const {return(State);}

	/**
	* @return Pointer to storage manager.
	*/
	inline GStorage* GetStorage(void) const {return(Storage);}

   /**
	* Insert a new concept category.
	* @param id              Identifier of the category.
	* @param name            Name of the category.
	*/
	void InsertConceptCat(char id,const R::RString& name);

	/**
	* Get the a pointer to a type of concept.
	* @param id              Identifier of the type.
	* @param null            If set to true, if the type does not exist,
	*                        return 0, else an exception is generated.
	* @return Pointer to a GConceptType.
	*/
	GConceptType* GetConceptType(char id,bool null);

	/**
	* Get the a pointer to a type of concept. If the concept type doesn't
	* exist, it is created.
   * @param cat             Concept category.
	* @param name            Name of the type.
	* @param desc            Short description
	* @return Pointer to a GConceptType.
	*/
	GConceptType* GetInsertConceptType(tConceptCat cat,const R::RString& name,const R::RString& desc);

	/**
	* Get the a pointer to a type of concept.
	* @param name            Name of the type.
	* @param null            If set to true, if the type does not exist,
	*                        return 0, else an exception is generated.
	* @return Pointer to a GConceptType.
	*/
	GConceptType* GetConceptType(const R::RString& name,bool null);

	/**
	 * Get the number of concept types created in the system.
    *
    * In fact, it returns the highest identifier of a concept type.
	 * @return the number of concept types.
	 */
	inline size_t GetNbConceptTypes(void) const
   {
       if(!ConceptTypes.GetNb())
           return(0);
       return(ConceptTypesByIds[ConceptTypesByIds.GetMaxPos()]->Id);
   }

	/**
	* Get all concept types defined.
	* @return RCursor over GConceptType.
	*/
	inline R::RCursor<GConceptType> GetConceptTypes(void) const {return(R::RCursor<GConceptType>(ConceptTypes));}

	/**
	* Insert a new concept type.
   * @param cat             Concept category.
	* @param id              Identifier of the type.
	* @param name            Name of the type.
	* @param desc            Short description.
	* @param refdocs         Number of documents referenced.
	* @param refprofiles     Number of profiles referenced.
	* @param refgroups       Number of communities referenced.
	* @param reftopics       Number of topics referenced.
	* @param refclasses      Number of classes referenced.
	*/
	void InsertConceptType(tConceptCat cat,char id,const R::RString& name,const R::RString& desc,size_t refdocs,size_t refprofiles,size_t refgroups,size_t reftopics,size_t refclasses);

  	/**
	 * Get the number of concept created in the system.
    *
    * In fact, it returns the highest identifier of a concept.
	 * @return the number of concepts.
	 */
	inline size_t GetNbConcepts(void) const
   {
       if(!Concepts.GetNb())
           return(0);
       return(Concepts[Concepts.GetMaxPos()]->Id);
   }

	/**
	* Get the concept with a specific identifier.
	* @param id             Identifier.
	* @return Pointer to a GConcept.
	*/
	GConcept* GetConcept(size_t id);

	/**
	* Get all concepts defined.
	* @return RCursor over GConcept.
	*/
	R::RCursor<GConcept> GetConcepts(void) const;

	/**
	* Get a pointer to a concept.
	* @param type            Type of the concept.
	* @param concept         Name of the concept to insert.
	* @param null            If set to true, if the object does not exist,
	*                        return 0, else an exception is generated.
	*/
	GConcept* GetConcept(GConceptType* type,const R::RString& concept,bool null=false);

	/**
	* Insert a concept.
	* @param type            Type of the concept.
	* @param concept         Name of the concept to insert.
	* @return Pointer to the concept inserted.
	*/
	GConcept* InsertConcept(GConceptType* type,const R::RString& concept);

	/**
	* Insert a concept. In practice, it is a copy of the concept which is
	* inserted.
	* @param concept         Concept to insert.
	* @return Pointer to the concept inserted.
	*/
	GConcept* InsertConcept(const GConcept* concept);

	/**
	 * Rename a concept.
	 * @param concept        Concept to rename.
	 * @param name           New name.
	 * @return a pointer to the correct concept (may be an existing one).
	 */
	GConcept* RenameConcept(GConcept* concept,const R::RString& name);

	/**
	* Delete a given concept from the dictionary.
	* @param concept         Concept to delete.
	*/
	void DeleteConcept(GConcept* concept);

	/**
	 * Clear the block index of all the concepts for a given object type.
	 * @param session        Session.
    * @param type           Object type.
    */
	void ClearIndex(GSession* session,tObjType type);

	/**
	 * Clear the references of all the concepts and concept types for a given
	 * object type.
	 * @param session        Session.
    * @param type           Object type.
    */
	void ClearRefs(GSession* session,tObjType type);

	/**
	* Get a predicate.
	* @param id              Identifier of the type.
	* @param null            If set to true, if the type does not exist,
	*                        return 0, else an exception is generated.
	* @return a pointer to a predicate
	*/
	GPredicate* GetPredicate(size_t id,bool null);

	/**
	* Get a predicate.
	* @param name            Name of the type.
	* @param null            If set to true, if the type does not exist,
	*                        return 0, else an exception is generated.
	* @return a pointer to a predicate
	*/
	GPredicate* GetPredicate(const R::RString& name,bool null);

	/**
	* Get all predicates defined.
	*/
	R::RCursor<GPredicate> GetPredicates(void) const;

	/**
	* Insert a new predicate.
	* @param id              Identifier of the type.
	* @param name            Name of the type.
	* @param desc            Short description.
	*/
	GPredicate* InsertPredicate(size_t id,const R::RString& name,const R::RString& desc);

	/**
	* Get a statement.
	* @param id              Identifier of the statement.
	* @return Pointer to a statement.
	*/
	GStatement* GetStatement(size_t id);

	/**
	* Insert a new statement.
	* @param id              Identifier of the statement.
	* @param predicate       Identifier of the predicate.
	* @param xi              Identifier of the \f$x_i\f$.
	* @param xitype          Type of the \f$x_i\f$.
	* @param xj              Identifier of the \f$x_j\f$.
	* @param xjtype          Type of the \f$x_j\f$.
	* @param weight          Weight of the statement.
	*/
	void InsertStatement(size_t id,size_t predicate,size_t xi,tObjType xitype,size_t xj,tObjType xjtype,double weight);

	/**
	* Destruct the session.
	*/
	virtual ~GKB(void)=0;

	friend class GConceptType;
};


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
