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
* It maintains:
* - A list of concept types (GConceptType).
* - A list of concepts (GConcept).
* - A list of statements that are stored (GStatement).
*
* @warning Some methods use a reinterpret_cast<GSession*>(this) expression.
* @short Knowledge Base.
*/
class GKB
{
protected:

	/**
	 * Session.
	 */
	GSession* Session;

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
	 * Default type for tokens.
	 */
	GConceptType* TokenType;

	/**
	 * Default type for the Dublin Core Metadata Initiative.
	 */
	GConceptType* DCMIType;

	/**
	 * Default type for named entities.
    */
	GConceptType* NamedEntityType;

	/**
	 * Default type for URI.
	 */
	GConceptType* URIType;

	/**
	 * Concept type that represents relations of taxonomies.
	 */
	GConceptType* TaxonomyType;

	/**
    * Meta-concept representing the default textual content.
    */
   GConcept* Text;

	/**
	 * Concepts related to named entities.
	 */
	GConcept* NamedEntity;

	/**
	 * Concepts related to the default URI content.
	 */
	GConcept* URI;

	/**
	 * Concept that represents a "IsA" relation.
    */
	GConcept* IsA;

	/**
	 * Concept that represents a "synonym" relation.
    */
	GConcept* Synonym;

	/**
	 * Concept that represents a "part of" relation.
	 */
	GConcept* PartOf;

	/**
	 *  Types of Concepts.
	 */
	R::RContainer<GConceptType,true,true> ConceptTypes;

	/**
	 *  Types of Concepts (accessed by identifiers).
	 */
	R::RContainer<GConceptType,false,false> ConceptTypesByIds;

	/**
	* Concepts (accessed by identifier).
	*/
	R::RContainer<GConcept,true,false> ConceptsByIds;

	/**
	 * Statements (accessed by identifier).
	 */
	R::RContainer<GStatement,true,false> StatementsByIds;

	/**
	* Constructor.
	* @param session         Session.
	*/
	GKB(GSession* session);

public:

	/** @name General Methods
	 *  General methods.
	 */
	//@{
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

	//@}

	/**
	 * @name Convenient Methods
    */
	//@{
	/**
	 * Get the default concept type representing tokens.
    * @return a concept type.
    */
	GConceptType* GetTokenType(void) const;

	/**
	 * Get the default concept type representing the DMCI.
    * @return a concept type.
    */
	GConceptType* GetDCMIType(void) const;

	/**
	 * Get the default type for URI.
    * @return a concept type.
    */
	GConceptType* GetURIType(void) const;

	/**
	 * Get the default type for named entities.
    * @return a concept type.
    */
	GConceptType* GetNamedEntityType(void) const;

	/**
	 * Get the default concept type that represents relations of taxonomies.
	 * @return a concept type.
	 */
	GConceptType* GetTaxonomyType(void) const;

	/**
	 * Get the default meta-concept representing the default text content.
    * @return a concept.
    */
	GConcept* GetText(void) const;

	/**
	 * Get the meta-concept representing the default URI content.
    * @return a concept.
    */
	GConcept* GetURI(void) const;

	/**
	 * Get the meta-concept representing the default named entities.
    * @return a concept.
    */
	GConcept* GetNamedEntity(void) const;

	/**
	 * Get the default concept that represents a "IsA" relation.
	 * @return a concept.
    */
	GConcept* GetIsA(void) const;

	/**
	 * Get the default concept that represents a "synonym" relation.
	 * @return a concept.
    */
	GConcept* GetSynonym(void) const;

	/**
	 * Get the default concept that represents a "part of" relation.
	 * @return a concept.
	 */
	GConcept* GetPartOf(void) const;

	//@}

protected:

	/**
	 * Clear the block index of all the concepts for a given object type.
    * @param type           Object type.
    */
	void ClearIndex(tObjType type);

public:

	/** @name Concept Type Methods
	 *  Methods related to the management of concept types.
	 */
	//@{
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
	* Get the a pointer to a type of concept.
	* @param obj             Pseudo-parameter.
	* @param id              Identifier of the type.
	* @param null            If set to true, if the type does not exist,
	*                        return 0, else an exception is generated.
	* @return Pointer to a GConceptType.
	*/
	GConceptType* GetObj(const GConceptType* obj,char id,bool null);

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
	* Get all concept types defined.
	* @param obj             Pseudo-parameter.
	* @return RCursor over GConceptType.
	*/
	inline R::RCursor<GConceptType> GetObjs(const GConceptType* obj) const;

	/**
	* Create a new concept type. If the concept type exists, nothing is done.
	* @param obj             Pseudo-parameter.
   * @param cat             Concept category.
	* @param id              Identifier of the type (if cNoRef, an identifier is
	*                        assigned).
	* @param name            Name of the type.
	* @param desc            Short description
	* @param newone          Is the concept type a new one (set by the method).
	* @return Pointer to a GConceptType.
	*/
	GConceptType* InsertObj(const GConceptType* obj,tConceptCat cat,size_t id,const R::RString& name,const R::RString& desc,bool& newone);

	/**
	* Create a new concept type. If the concept type exists, nothing is done.
	* @param obj             Pseudo-parameter.
   * @param cat             Concept category.
	* @param id              Identifier of the type (if cNoRef, an identifier is
	*                        assigned).
	* @param name            Name of the type.
	* @param desc            Short description
	* @return Pointer to a GConceptType.
	*/
	inline GConceptType* InsertObj(const GConceptType* obj,tConceptCat cat,size_t id,const R::RString& name,const R::RString& desc)
	{
		bool newone;
		return(InsertObj(obj,cat,id,name,desc,newone));
	}

	/**
	* Create a new concept type. If the concept type exists, nothing is done.
	* @param obj             Pseudo-parameter.
   * @param cat             Concept category.
	* @param name            Name of the type.
	* @param desc            Short description
	* @param newone          Is the concept type a new one (set by the method).
	* @return Pointer to a GConceptType.
	*/
	inline GConceptType* InsertObj(const GConceptType* obj,tConceptCat cat,const R::RString& name,const R::RString& desc,bool& newone)
	{
		return(InsertObj(obj,cat,R::cNoRef,name,desc,newone));
	}

	/**
	* Create a new concept type. If the concept type exists, nothing is done.
	* @param obj             Pseudo-parameter.
   * @param cat             Concept category.
	* @param id              Identifier of the type (if cNoRef, an identifier is
	*                        assigned).
	* @param name            Name of the type.
	* @param desc            Short description
	* @return Pointer to a GConceptType.
	*/
	inline GConceptType* InsertObj(const GConceptType* obj,tConceptCat cat,const R::RString& name,const R::RString& desc)
	{
		bool newone;
		return(InsertObj(obj,cat,R::cNoRef,name,desc,newone));
	}

	//@}

	/** @name Concept Methods
	 *  Methods related to the management of concepts.
	 */
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
	* @param newone          Is the concept type a new one (set by the method).
	* @return a pointer to the concept is the dictionary.
	*/
	GConcept* InsertObj(const GConcept* obj,GConceptType* type,const R::RString& concept,bool& newone);

	/**
	* Insert a concept of a given type and name. If the concept already exists,
	* nothing is done.
	* @param obj             Pseudo-parameter.
	* @param type            Type of the concept.
	* @param concept         Name of the concept to insert.
	* @return a pointer to the concept is the dictionary.
	*/
	inline GConcept* InsertObj(const GConcept* obj,GConceptType* type,const R::RString& concept)
	{
		bool newone;
		return(InsertObj(obj,type,concept,newone));
	}

	/**
	* Insert a concept that is exactly the same as a model. If the concept exist,
	* nothing is done.
	* @param concept         Model Concept.
	* @param newone          Is the concept type a new one (set by the method).
	* @return a pointer to the concept is the dictionary.
	*/
	GConcept* InsertObj(const GConcept& concept,bool& newone);

	/**
	* Insert a concept that is exactly the same as a model. If the concept exist,
	* nothing is done.
	* @param concept         Model Concept.
	* @return a pointer to the concept is the dictionary.
	*/
	inline GConcept* InsertObj(const GConcept& concept)
	{
		bool newone;
		return(InsertObj(concept,newone));
	}

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

	//@}

	/** @name Statement Methods
	 *  Methods related to the management of statements.
	 */
	/**
	* Get a statement.
	* @param obj             Pseudo-parameter.
	* @param id              Identifier of the statement.
	* @param null            Can a null pointer be returned or an exception must
	*                        be generated.
	* @return Pointer to a statement.
	*/
	GStatement* GetObj(const GStatement* obj,size_t id,bool null=false);

	/**
	* Get a statement.
	* @param obj             Pseudo-parameter.
	* @param subject         Subject.
	* @param predicate       Predicate.
	* @param value           Value.
	* @param null            Can a null pointer be returned or an exception must
	*                        be generated.
	* @return a pointer to the newly statement created or the existing one.
	*/
	GStatement* GetObj(const GStatement* obj,GObject* subject,GObject* predicate,GObject* value,bool null=false);

	/**
	* Insert a new statement. If it exists, it is not re-created but its weight
	* is updated.
	* @param obj             Pseudo-parameter.
	* @param id              Identifier of the statement. If cNoRef, an
	*                        identifier is assigned.
	* @param subject         Subject.
	* @param predicate       Predicate.
	* @param value           Value.
	* @param weight          Weight of the statement.
	* @param newone          Is the concept type a new one (set by the method).
	* @return a pointer to the newly statement created or the existing one.
	*/
	GStatement* InsertObj(const GStatement* obj,size_t id,GObject* subject,GObject* predicate,GObject* value,double weight,bool& newone);

	/**
	* Insert a new statement. If it exists, it is not re-created but its weight
	* is updated.
	* @param obj             Pseudo-parameter.
	* @param id              Identifier of the statement. If cNoRef, an
	*                        identifier is assigned.
	* @param subject         Subject.
	* @param predicate       Predicate.
	* @param value           Value.
	* @param weight          Weight of the statement.
	* @return a pointer to the newly statement created or the existing one.
	*/
	inline GStatement* InsertObj(const GStatement* obj,size_t id,GObject* subject,GObject* predicate,GObject* value,double weight)
	{
		bool newone;
		return(InsertObj(obj,id,subject,predicate,value,weight,newone));
	}

	//@}

public:

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
