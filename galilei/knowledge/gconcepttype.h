/*

	GALILEI Research Project

	GConceptType.h

	Concept Type - Header.

	Copyright 2006-2008 by the Université Libre de Bruxelles.

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
* The GDict provides a representation for a given type of concepts. Each
* type can be universal (independent of a given language such as names) or
* language-dependent (such as words or stems).
*
* It also manage a dictionary of concepts, each concept having its own
* identifier and name.
* @param author Pascal Francq
* @param short Concept Type.
*/
class GConceptType : public GDebugObject, protected R::RDblHashContainer<GConcept,true>
{
	/**
	* Identifier of the concept type.
	*/
	char Id;

	/**
	 * Session.
	 */
	GSession* Session;

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
	* Array of concepts ordered by identifier.
	*/
	GConcept** Direct;

	/**
	* Highest identifier that the dictionary can handle.
	*/
	size_t MaxId;

	/**
	* Highest identifier of the concept stored.
	*/
	size_t UsedId;

	/**
	* State of the dictionary.
	*/
	bool Loaded;

	/**
	* Number of references in documents.
	*/
	size_t NbRefDocs;

	/**
	* Number of references in profiles.
	*/
	size_t NbRefProfiles;

	/**
	* Number of references in groups.
	*/
	size_t NbRefGroups;

	/**
	 * Number of references in topics.
	 */
	size_t NbRefTopics;

public:

	/**
	* Construct a concept type.
	* @param id              Identifier of the type.
	* @param session         Session.
	* @param name            Name of the type.
	* @param desc            Short description.
	* @param lang            Language eventually associated to the concept type.
	* @param s               Initial number of concepts.
	* @param s2              Size of the second hash table.
	*/
	GConceptType(char id,GSession* session,const R::RString& name,const R::RString& desc,GLang* lang,size_t s=20000,size_t s2=5000);

	/**
	* Set the references of a given language. This method is called when
	* connecting to and disconnecting from a session.
	* @param refdocs         Number of documents referenced.
	* @param refprofiles     Number of profiles referenced.
	* @param refgroups       Number of groups referenced.
	* @param reftopics       Number of topics referenced.
	*/
	void SetReferences(size_t refdocs,size_t refprofiles,size_t refgroups,size_t reftopics);

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
	* Compare a concept with an identifier.
	* @param id              Identifier used.
	* @see R::RContainer.
	*/
	int Compare(char id) const;

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
	 * Is the dictionary of concepts loaded.
	 */
	bool IsLoaded(void) const {return(Loaded);}

	/**
	 * Load the concepts if necessary.
	 */
	void Load(void) const;

	/**
	 * Build some debugging information.
	 * @param info           Description of the information needed ("idf","itf"
	 *                       ,"ipf" or "icf" are allowed).
	 */
	virtual void DebugInfo(const R::RString& info);

	/**
	* Clear the dictionary.
	*/
	void Clear(void);

private:

	/**
	* Put a concept in direct.
	* @param concept         Pointer to the concept to insert.
	*/
	void PutDirect(GConcept* concept);

public:

	/**
	* Insert a concept in the dictionary. In practice, it is a copy of the
	* concept which is inserted.
	* @param concept         Concept to insert.
	* @return Pointer to the concept inserted in the dictionary.
	*/
	GConcept* InsertConcept(const GConcept* concept);

	/**
	* Delete a given concept from the dictionary.
	* @param concept         Concept to delete.
	*/
	void DeleteConcept(GConcept* concept);

	/**
	* Get the concept with a specific identifier.
	* @param id             Identifier.
	* @return Pointer to a GConcept.
	*/
	GConcept* GetConcept(size_t id) const;

	/**
	* Get the highest identifier of the concept stored by the dictionary.
	* @return size_t
	*/
	size_t GetConceptMaxId(void) const {return(UsedId);}

	/**
	* Get the list of the concepts currently defined in the dictionary.
	* @returns const Pointer to array of GConcept*.
	*/
	const GConcept** GetConcepts(void) const {return(const_cast<const GConcept**>(Direct));}

	/**
	* Look if a given concept is in the dictionary.
	* @param name            Name fo the concept to look for.
	* @return true if the concept is in the dictionary.
	*/
	bool IsIn(const R::RString& name) const;

	/**
	 * Rename a concept.
	 * @param concept        Concept to rename.
	 * @param name           New name.
	 * @return Pointer to the correct concept (may be an existing one).
	 */
	GConcept* RenameConcept(GConcept* concept,const R::RString& name);

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
	 * Get the 'Inverse Frequency' factor of a given concept type.
	* @param id             Identifier of the concept.
	* @param ObjType        Type of the object.
	*/
	double GetIF(size_t id,tObjType ObjType);

	/**
	* Increase the number of objects of a given type that make a reference to a
	* concept.
	* @param id             Identifier of the concept.
	* @param ObjType        Type of the object.
	*/
	void IncRef(size_t id,tObjType ObjType);

	/**
	* Decrease the number of objects of a given type that make a reference to a
	* concept.
	* @param id             Identifier of the concept.
	* @param ObjType        Type of the object.
	*/
	void DecRef(size_t id,tObjType ObjType);

	/**
	* Get the number of objects of a given type that make a reference to a
	* concept.
	* @param id             Identifier of the concept.
	* @param ObjType        Type of the object.
	* @return size_t.
	*/
	size_t GetRef(size_t id,tObjType ObjType);

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
	* @param ObjType        Type of the object.
	*/
	size_t GetRef(tObjType ObjType) const;

	/**
	* Clear the number of objects of a given type using the dictionary.
	* @param ObjType        Type of the object.
	*/
	void ClearRef(tObjType ObjType);

	/**
	 * Destruct.
	 */
	virtual ~GConceptType(void);

	friend class GConcept;
	friend class GWeightInfo;
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
