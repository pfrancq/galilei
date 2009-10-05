/*

	GALILEI Research Project

	GConceptType.h

	Concept Type - Header.

	Copyright 2006-2009 by Pascal Francq (pascal@francq.info).
	Copyright 2006-2008 by the Université Libre de Bruxelles (ULB).

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
#ifndef GConceptType_H
#define GConceptType_H


//------------------------------------------------------------------------------
// include files for R Project
#include <rdblhashcontainer.h>


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>
#include <gdebugobject.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
* The GConceptType provides a representation for a given type of concepts. Each
* type can be universal (independent of a given language such as names) or
* language-dependent (such as words or stems).
*
* It also manage a dictionary of concepts, each concept having its own
* identifier and name.
* @param author Pascal Francq
* @param short Concept Type.
*/
class GConceptType : public GDebugObject, protected R::RDblHashContainer<GConcept,false>
{
private:

	/**
	 * Ontology.
	 */
	GOntology* Ontology;

	/**
	* Identifier of the concept type.
	*/
	char Id;

	/**
	* Short description of the type.
	*/
	R::RString Description;

	/**
	 * If the type of concept is universal, i.e. independent of a given
	 * language, it must be null.
	 */
	GLang* Lang;

	/**
	* Number of references in documents.
	*/
	size_t NbRefDocs;

	/**
	* Number of references in profiles.
	*/
	size_t NbRefProfiles;

	/**
	* Number of references in communities.
	*/
	size_t NbRefCommunities;

	/**
	 * Number of references in topics.
	 */
	size_t NbRefTopics;

	/**
	 * Number of references in classes.
	 */
	size_t NbRefClasses;

public:

	/**
	* Construct a concept type.
	* @param id              Identifier of the type.
	* @param ontology        Ontology.
	* @param name            Name of the type.
	* @param desc            Short description.
	* @param lang            Language eventually associated to the concept type.
	* @param s               Size of the second hash table.
	*/
	GConceptType(char id,GOntology* ontology,const R::RString& name,const R::RString& desc,GLang* lang,size_t s);

	/**
	* Set the references of a given language. This method is called when
	* connecting to and disconnecting from a session.
	* @param refdocs         Number of documents referenced.
	* @param refprofiles     Number of profiles referenced.
	* @param refcommunities  Number of communities referenced.
	* @param reftopics       Number of topics referenced.
	* @param refclasses      Number of classes referenced.
	*/
	void SetReferences(size_t refdocs,size_t refprofiles,size_t refcommunities,size_t reftopics,size_t refclasses);

	/**
	* Compare two concepts types.
	* @param type            Concept type used.
	* @see R::RContainer.
	*/
	int Compare(const GConceptType& type) const;

	/**
	* Compare two concepts types.
	* @param type            Concept type used.
	* @see R::RContainer.
	*/
	int Compare(const GConceptType* type) const;

	/**
	* Compare an object with a name.
	* @param name            Name used.
	* @see R::RContainer.
	*/
	int Compare(const R::RString& name) const;

	/**
	* Set the Identifier of the concept type.
	* @param id              Identifier.
	*/
	void SetId(char id);

	/**
	* Get the identifier.
	*/
	char GetId(void) const {return(Id);}

	/**
	* Get the description.
	*/
	R::RString GetDescription(void) const {return(Description);}

	/**
	 * Is the concept type universal.
	 */
	bool IsUniversal(void) const {return(Lang==0);}

	/**
	 * Get the language associated with this concept type. If the concept type
	 * is universal, returns 0.
	 */
	GLang* GetLang(void) const {return(Lang);}

	/**
	 * Build some debugging information.
	 * @param info           Description of the information needed ("idf","itf"
	 *                       ,"ipf" or "icf" are allowed).
	 */
	virtual void DebugInfo(const R::RString& info);

	/**
	* Get a cursor over the main hash table.
	*/
	R::RCursor<Hash> GetConcepts(void) const
	{
		return(R::RCursor<Hash>(GetCursor()));
	}

	/**
	* Look if a given concept is in the dictionary.
	* @param name            Name fo the concept to look for.
	* @return true if the concept is in the dictionary.
	*/
	bool IsIn(const R::RString& name) const;

	/**
	* Get a given concept from the dictionary.
	* @param name            Name of the concept to look for.
	* @return Pointer to the concept.
	*/
	GConcept* GetConcept(const R::RString& name) const;

	/**
	* Get the total number of concepts.
	*/
	size_t GetNbConcepts(void) const {return(GetNb());}

	/**
	* Increase the number of objects of a given type using the dictionary of the
	* language.
	* @param ObjType        Type of the object.
	*/
	void IncRef(tObjType ObjType);

	/**
	* Decrease the number of objects of a given type using the dictionary of the
	* language.
	* @param ObjType        Type of the object.
	*/
	void DecRef(tObjType ObjType);

	/**
	* Get the number of objects of a given type using the dictionary of the
	* language.
	* @param ObjType        Type of the reference. If otAnyType, the method
	*                       returns the number of references of all the
	*                       objects.
	*/
	size_t GetRef(tObjType ObjType) const;

	/**
	* Clear the number of objects of a given type using the dictionary.
	* @param ObjType        Type of the object.
	*/
	void ClearRef(tObjType ObjType);

	/**
	* Increase the number of objects of a given type that make a reference to a
	* concept.
	* @param concept        Concept.
	* @param ObjType        Type of the object.
	*/
	size_t IncRef(GConcept* concept,tObjType ObjType);

	/**
	* Decrease the number of objects of a given type that make a reference to a
	* concept.
	* @param concept        Concept.
	* @param ObjType        Type of the object.
	*/
	size_t DecRef(GConcept* concept,tObjType ObjType);

public:

	/**
	 * Destruct.
	 */
	virtual ~GConceptType(void);

	friend class GConcept;
	friend class GWeightInfo;
	friend class GIndexer;
	friend class GOntology;
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
