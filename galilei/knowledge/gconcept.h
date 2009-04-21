/*

	GALILEI Research Project

	GConcept.h

	Concept - Header.

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
#ifndef GConcept_H
#define GConcept_H


//------------------------------------------------------------------------------
// include files for R
#include <rvectorint.h>


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
* The GConcept class provides a representation for a generic concept. A concept
* is a "representation" of a given piece of knowledge.
* @author Pascal Francq
* @short Concept
*/
class GConcept
{
protected:

	/**
	* Identifier of the concept.
	*/
	size_t Id;

	/**
	* Name of the concept.
	*/
	R::RString Name;

	/**
	* Type of the concept.
	*/
	GConceptType* Type;

	/**
	* Number of references in documents.
	*/
	size_t NbRefDocs;

	/**
	 * Inverse factor for documents.
	 */
	double IfDocs;

	/**
	 * Position in the inverted file corresponding to the documents.
	 */
	off_t IndexDocs;

	/**
	* Number of references in profiles.
	*/
	size_t NbRefProfiles;

	/**
	 * Inverse factor for profiles.
	 */
	double IfProfiles;

	/**
	* Number of references in communities.
	*/
	size_t NbRefCommunities;

	/**
	 * Inverse factor for communities.
	 */
	double IfCommunities;

	/**
	 * Number of references in topics.
	 */
	size_t NbRefTopics;

	/**
	 * Inverse factor for topics.
	 */
	double IfTopics;

public:

	/**
	 * Copy constructor.
	 * @param concept        Original concept.
	 */
	GConcept(const GConcept* concept);

	/**
	* Constructor of a generic concept.
	* @param name            Name of the concept.
	* @param type            Type of the concept.
	*/
	GConcept(const R::RString& name,GConceptType* type);

	/**
	* Constructor of a generic concept.
	* @param id              Identifier.
	* @param name            Name of the concept.
	* @param type            Type of the concept.
	* @param refdocs         Number of documents referenced.
	* @param indexdocs       Position in the inverted file related to the
	*                        documents.
	* @param refprofiles     Number of profiles referenced.
	* @param refcommunities  Number of groups referenced.
	* @param reftopics       Number of topics referenced.
	*/
	GConcept(size_t id,const R::RString& name,GConceptType* type,size_t refdocs,off_t indexdocs,size_t refprofiles,size_t refcommunities,size_t reftopics);

	/**
	* Compare two concepts.
	* @param c               Concept used.
	* @see R::RContainer
	*/
	int Compare(const GConcept& c) const;

	/**
	* Compare a concept with a given name.
	* @param name            Name.
	* @see R::RContainer
	*/
	int Compare(const R::RString& name) const;

	/**
	 * Do a deep copy of the current concept.
	 * @return Pointer to a new element created.
	 */
	virtual GConcept* DeepCopy(void) const;

	/**
	* Compute the first hash index of the name of a concept.
	* @see R::RHashContainer
	* @param c               Concept.
	*/
	virtual size_t HashIndex(size_t idx) const
		{return(Name.HashIndex(idx));}

	/**
	* Get the type of the concept.
	*/
	GConceptType* GetType(void) const {return(Type);}

	/**
	* Set the Identifier of the concept.
	* @param id              Identifier.
	*/
	void SetId(size_t id);

	/**
	* @return Identifier of the concept.
	*/
	size_t GetId(void) const {return(Id);}

	/**
	* @return A string representing the concept.
	*/
	virtual R::RString GetName(void) const {return(Name);}

	/**
	 * Set the name of the concept. This method works only if the Id==cNoRef.
	 * The references are cleared.
	 * @param name
	 */
	void SetName(const R::RString& name);

private:

	/**
	* Increase the number of references on this concept for a given object
	* type.
	* @param ObjType        Type of the reference.
	*/
	size_t IncRef(tObjType ObjType);

	/**
	* Decrease the number of references on this concept for a given object
	* type.
	* @param ObjType        Type of the reference.
	*/
	size_t DecRef(tObjType ObjType);

	/**
	* Clear the information of the references of the concept linked to a
	* specific object.
	* @param ObjType        Type of the object.
	*/
	virtual void ClearRef(tObjType ObjType);

public:

	/**
	* Get the number of references on this concept for a given object type.
	* @param ObjType        Type of the reference.
	* @returns size_t.
	*/
	size_t GetRef(tObjType ObjType) const;

	/**
	 * @return The inverse factor of the concept for a given object type (such
	 * as the idf factor for documents.
	* @param ObjType        Type of the reference.
	 */
	double GetIF(tObjType ObjType) const;

	/**
	* Clear all the information of the concept.
	*/
	virtual void Clear(void);

	/**
	* Test if the concept is considered as empty. By default, every concept
	* defined by a non-empty name, is also considered as not empty.
	*/
	virtual bool IsEmpty(void) const;

	/**
	* Destruct the concept.
	*/
	virtual ~GConcept(void);

	friend class GWeightInfo;
	friend class GConceptType;
	friend class GIndexer;
	friend class GOntology;
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
