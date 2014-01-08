/*

	GALILEI Research Project

	GConcept.h

	Concept - Header.

	Copyright 2006-2014 by Pascal Francq (pascal@francq.info).
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


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <gobject.h>


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
class GConcept : public GObject
{
protected:

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
	 * Identifier of the block in the inverted file for the documents.
	 */
	size_t IndexDocs;

	/**
	* Number of references in profiles.
	*/
	size_t NbRefProfiles;

	/**
	 * Inverse factor for profiles.
	 */
	double IfProfiles;

	/**
	 * Identifier of the block in the inverted file for the profiles.
	 */
	size_t IndexProfiles;

	/**
	* Number of references in communities.
	*/
	size_t NbRefCommunities;

	/**
	 * Inverse factor for communities.
	 */
	double IfCommunities;

	/**
	 * Identifier of the block in the inverted file for the communities.
	 */
	size_t IndexCommunities;

	/**
	 * Number of references in topics.
	 */
	size_t NbRefTopics;

	/**
	 * Inverse factor for topics.
	 */
	double IfTopics;

	/**
	 * Identifier of the block in the inverted file for the topics.
	 */
	size_t IndexTopics;

	/**
	 * Number of references in classes.
	 */
	size_t NbRefClasses;

	/**
	 * Inverse factor for classes.
	 */
	double IfClasses;

	/**
	 * Identifier of the block in the inverted file for the classes.
	 */
	size_t IndexClasses;

public:

	/**
	 * Copy constructor.
	 * @param concept        Original concept.
	 */
	GConcept(const GConcept* concept);

	/**
	* Constructor of a generic concept.
	* @param session         Session.
	* @param name            Name of the concept.
	* @param type            Type of the concept.
	*/
	GConcept(GSession* session,const R::RString& name,GConceptType* type);

	/**
	* Constructor of a generic concept.
	* @param session         Session.
	* @param id              Identifier.
	* @param name            Name of the concept.
	* @param type            Type of the concept.
	* @param refdocs         Number of documents referenced.
	* @param idxdocs         Identifier of the block the inverted file related
	*                        to the documents.
	* @param refprofiles     Number of profiles referenced.
	* @param idxprofiles     Identifier of the block the inverted file related
	*                        to the profiles.
	* @param refcommunities  Number of communities referenced.
	* @param idxcommunities  Identifier of the block the inverted file related
	*                        to the communities.
	* @param reftopics       Number of topics referenced.
	* @param idxtopics       Identifier of the block the inverted file related
	*                        to the topics.
	* @param refclasses      Number of classes referenced.
	* @param idxclasses      Identifier of the block the inverted file related
	*                        to the classes.
	*/
	GConcept(GSession* session,size_t id, const R::RString& name, GConceptType* type,
			size_t refdocs, size_t idxdocs,
			size_t refprofiles, size_t idxprofiles,
			size_t refcommunities, size_t idxcommunities,
			size_t reftopics, size_t idxtopics,
			size_t refclasses,size_t idxclasses);

	/**
    * @return the class name.
    */
	virtual R::RCString GetClassName(void) const {return("GConcept");}

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
	* @param idx             Index of the hash index.
	*/
	virtual size_t HashIndex(size_t idx) const
		{return(GetName().HashIndex(idx));}

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
	 * @return the block index of the concept for a given object type.
	 * @param ObjType        Type of the reference.
	 */
	size_t GetIndex(tObjType ObjType) const;

	/**
	 * Set the block index of the concept for a given object type.
	 * @param ObjType        Type of the reference.
	 * @param index          Index to set.
	 */
	void SetIndex(tObjType ObjType,size_t index);

	/**
	 * Clear the block index of the concept for a given object type.
    * @param ObjType        Type of the reference.
    */
	void ClearIndex(tObjType type);

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
	virtual void ClearRefs(tObjType ObjType);

	/**
	* Get the number of references on this concept for a given object type.
	* @param ObjType        Type of the reference. If otAnyType, the method
	*                       returns the number of references of all the
	*                       objects.
	* @returns size_t.
	*/
	size_t GetRef(tObjType ObjType) const;

	/**
	 * Compute the inverse factor of the concept for a given object type (such
	 * as the 'idf' for documents). In practice, the factor is computed once.
	 * @return Inverse factor
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

	friend class GConceptRef;
	friend class GVector;
	friend class GConceptType;
	friend class GSession;
	friend class GKB;
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
