/*

	GALILEI Research Project

	GObjects.h

	Object List - Header.

	Copyright 2011 by Pascal Francq (pascal@francq.info).

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
#ifndef GObjects_H
#define GObjects_H


//------------------------------------------------------------------------------
// include files for R Project
#include <rindexfile.h>
#include <rconfig.h>
#include <rdir.h>
#include <robjectcontainer.h>


//------------------------------------------------------------------------------
// include files for GALILEI
#include <gkb.h>
#include <gstorage.h>
#include <ggalileiapp.h>
#include <gdescriptionobject.h>
#include <gconcepttree.h>


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
 * This class represents a set of objects (class C). It aims is to encapsulate
 * all the storage aspects of the management.
 * @tparam C                  The class of the objects managed.
 *
 * To manage correctly the objects, the list uses two static member functions of
 * the class representing the elements (class C):
 * @code
 * static inline bool C::HasDesc(void);
 * static inline bool C::HasStruct(void);
 * @endcode
 *
 * The first one specifies if the corresponding objects have descriptions (such
 * as documents) or not (such as concept types). The second one specifies if
 * the objects have a structure (documents) or not (all the other objects a
 * priori).
 *
 * The fist parameter of several methods is a pseudo-parameter. It is used to
 * differentiate methods with identical names. Several null pointer constants
 * called pDoc, pProfile,etc. are defined and can be used for this parameter. In
 * practice, these methods are called as followed:
 * @code
 * size_t NbDocs(List.GetNbObjs(pDoc);
 * size_t MaxProfileId(List.GetMaxObjId(pProfile);
 * R::RCursor<GDoc> Docs(List.GetObjs(pDoc);
 * GCommunity* Community(List.GetObj(pCommunity,4,true,true));
 * @endcode
 * The reason for this trick is that C++ does not managed methods with the same
 * name, the same parameters but different return types.
 * @author Pascal Francq
 * @short Stored Objects.
 */
template<class C>
	class GObjects : virtual GKB, protected R::RObjectContainer<C,true>
{
public:

	/**
	* Index type.
	*/
	enum tIndexType
	{
		itNoIndex                /** No index. */,
		itIndex                  /** Simple index. */,
		itFullIndex              /** Full index (including the occurrences of
		                             each concept.*/
	};

protected:

	/**
	 * Name of the objects.
	 */
	R::RString ObjName;

	/**
	 * Type of the objects.
	 */
	tObjType Type;

	/**
	 * Type of the index.
	 */
	tIndexType IndexType;

	/**
	 * Description of the objects.
	 */
	R::RIndexFile* Desc;

	/**
	 * Index of the objects.
	 */
	R::RIndexFile* Index;

	/**
	 * Structures of the objects.
	 */
	R::RIndexFile* Struct;

	/**
	 * Occurrences of the concepts in the objects.
	 */
	R::RIndexFile* Occurs;

	/**
	 * Temporary vector of references.
	 */
	R::RNumContainer<size_t,true> tmpRefs;

	/**
	 * Are the objects loaded.
	 */
	bool Loaded;

	/**
	 * Maximum objects to managed in memory.
	 */
	size_t MaxObjs;

public:

	/**
	 * Constructor of a set of stored objects.
	 * @param size           Initial size of the list.
	 * @param name           Name of the objects.
	 * @param type           Type of the objects.
	 */
	GObjects(size_t size,const R::RString& name,tObjType type);

	/**
	 * Initialize the parameters.
	 * @param config         Configuration structure.
	 */
	void Init(R::RConfig* config);

	/**
	 * Open the files in a given directory.
	 * @param config         Configuration structure.
	 * @param subdir         Sub-directory where the files will be stored.
	 */
	void OpenFiles(R::RConfig* config,R::RString subdir);

	/**
	* Insert an object.
	* @param obj             object to insert.
	* @param parent          Parent object. This parameter is only used by
	*                        GClasses.
	*/
	virtual void InsertObj(C* obj,C* parent=0);

	/**
	* Delete an object.
	* @param obj             object to delete.
	*/
	virtual void DeleteObj(C* obj);

	/**
	 * Clear the list.
	 * @param obj            Pseudo-parameter.
	 */
	virtual void Clear(const C* obj);

   /**
    * Look if an index is managed.
    * @param obj            Pseudo-parameter.
    * @return true if an index exists.
    */
   inline bool HasIndex(const C* obj) const;

   /**
    * Look if the structures of the objects are managed.
    * @param obj            Pseudo-parameter.
    * @return true if the structure exist.
    */
   inline bool HasStruct(const C* obj) const;

	/**
	 * Get the number of objects of a given type.
	 * @param obj            Pseudo-parameter.
	 */
	inline size_t GetNbObjs(const C* obj) const;

	/**
	 * Get the highest identifier of the objects of a given type.
	 * @param obj            Pseudo-parameter.
	 */
	inline size_t GetMaxObjId(const C* obj) const;

	/**
	 * Get the objects of a given type.
	 * @param obj            Pseudo-parameter.
	 * @return a cursor over the objects.
	 */
	inline R::RCursor<C> GetObjs(const C* obj) const;

	/**
	 * Fill an array of objects of a given type.
	 * @param tab            Array that will be filled.
	 * @param alloc          Must the array be allocated ?
	 * @return number of objects in the array (including null pointers).
	 */
	inline size_t GetObjs(C** &tab,bool alloc);

	/**
	 * Get an object of a given type based on its identifier.
	 * @param obj            Pseudo-parameter.
	 * @param id             Identifier of the object.
	 * @param load           If set to true, the object is eventually loaded
	 *                       into memory.
	 * @param null           If set to true, if the object does not exist,
	 *                       return 0, else an exception is generated.
	 * @return a pointer to a object.
	 */
	C* GetObj(const C* obj,size_t id,bool load=true,bool null=false);

	/**
	 * Get an object of a given type based on its name.
	 * @param obj            Pseudo-parameter.
	 * @param name           Name of the object.
	 * @param load           If set to true, the object is eventually loaded
	 *                       into memory.
	 * @param null           If set to true, if the object does not exist,
	 *                       return 0, else an exception is generated.
	 * @return a pointer to a object.
	 */
	C* GetObj(const C* obj,const R::RString& name,bool load=true,bool null=false);

	/**
	* Assign an identifier to an object.
	* @param obj             Object.
	*/
	void AssignId(C* obj);

	/**
	* Load all the objects of a given type.
	* @param obj             Pseudo-parameter.
	*/
	void LoadObjs(const C* obj);

	/**
	* Load the description of a given object.
	* @param obj             Pseudo-parameter.
	* @param vectors         Vectors.
	* @param blockid         Identifier of the block of the object.
	* @param id              Identifier of the object.
	*/
	void LoadDesc(const C* obj,R::RContainer<GVector,true,true>* &vectors,size_t blockid,size_t id);

	/**
	* Save the description of a given object.
	* @param obj             Pseudo-parameter.
	* @param vectors         Vectors.
	* @param blockid         Identifier of the block of the object (0 means the block will be found).
	* @param id              Identifier of the object.
	*/
	void SaveDesc(const C* obj,const R::RContainer<GVector,true,true>& vectors,size_t& blockid,size_t id);

	/**
	 * Flush the descriptions of the objects of a given type.
	 * @param obj            Pseudo-parameter.
	 * The reason for this trick is that C++ does not managed methods with the
	 * same name, the same parameters but different return types.
	 */
	void FlushDesc(const C* obj);

	/**
	 * Load the identifiers of all objects of a given type described by a
	 * specific concept.
	 * @param obj            Pseudo-parameter.
	 * @param concept        Concept to search for.
	 * @param refs           Identifiers of the objects.
	 */
	void LoadIndex(const C* obj,GConcept* concept,R::RNumContainer<size_t,true>& refs);

	/**
	 * Update the index of a given object.
	 * @param obj            Pseudo-parameter.
	 * @param vectors        Vectors.
	 * @param id             Identifier of the object.
	 * @param add            Object must be added or removed from the index.
	 */
	void UpdateIndex(const C* obj,const R::RContainer<GVector,true,true>& vectors,size_t id,bool add);

	/**
	 * Build the idex of all the objects of a given type from scratch.
	 * @param obj            Pseudo-parameter.
	 */
	void BuildIndex(const C* obj);

	/**
	 * Load the structure of a given object.
	 * @param obj            Pseudo-parameter.
	 * @param docstruct      Structure. If null,it is created.
	 * @param blockid        Identifier of the block of the object.
	 * @param id             Identifier of the object.
	 */
	void LoadStruct(const C* obj,GConceptTree* &docstruct,size_t blockid,size_t id);

	/**
	 * Save the structure of a given object.
	 * @param obj            Pseudo-parameter.
	 * @param docstruct      Structure of the document.
	 * @param blockid        Identifier of the block of the object (0 means the
	 *                       block will be found).
	 * @param id             Identifier of the object.
	 */
	void SaveStruct(const C* obj,GConceptTree* docstruct,size_t& blockid,size_t id);

	/**
	 * Destructor.
	 */
	virtual ~GObjects(void);
};


//-----------------------------------------------------------------------------
// Template implementation
#include <gobjects.hh>


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
