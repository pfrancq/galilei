/*

	GALILEI Research Project

	GObjects.h

	Object List - Header.

	Copyright 2011-2015 by Pascal Francq (pascal@francq.info).

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
#include <rkeys.h>
#include <rconfig.h>
#include <rdir.h>
#include <robjectcontainer.h>
#include <rnodecursor.h>
#include <rrwlock.h>


//------------------------------------------------------------------------------
// include files for GALILEI
#include <gkb.h>
#include <gstorage.h>
#include <ggalileiapp.h>
#include <gdescriptionobject.h>
#include <gconceptnode.h>
#include <gconcepttree.h>
#include <gdocfragment.h>


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
 * This class represents a set of objects (class C). It aims is to encapsulate
 * all the storage aspects of the management.
 * @tparam C                  The class of the objects managed.
 * @tparam hEvents            Array of notification to post concerning an
 *                            object.
 *
 * To manage correctly the objects, the list uses two static member functions of
 * the class representing the elements (class C):
 * @code
 * static inline bool C::HasDesc(void);
 * static inline bool C::HasTree(void);
 * @endcode
 *
 * The first one specifies if the corresponding objects have descriptions (such
 * as documents) or not (such as users). The second one specifies if the objects
 * have a structure (documents) or not (all the other objects a priori).
 *
 * The first parameter of several methods is a pseudo-parameter. It is used to
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
 *
 * For the storage of trees (to store the full description if documents), each
 * node is stored as a record (GConceptRecord). The nodes are stored with using
 * a depth-first approach: the first root node is stored, followed by its first
 * child node, followed by its first grandchild nodes, etc.
 * @short Stored Objects.
 */
template<class C,const R::hNotification* hEvents>
	class GObjects : virtual GKB, R::RObjectContainer<C,true>
{
	// Explicit use of methods from GKB
	using GKB::InsertObj;
	using GKB::GetObj;
	using GKB::GetObjs;
	using GKB::GetNbObjs;
	using GKB::GetMaxObjId;

	/**
	 * Name of the objects.
	 */
	R::RString ObjName;

	/**
	 * Type of the objects.
	 */
	tObjType Type;

	/**
	 * Maximum objects to managed in memory.
	 */
	size_t MaxObjs;

	/**
	 * Description of the objects.
	 */
	R::RKeyValueFile<R::RIntKey>* Desc;

	/**
	 * Create the index ?
	 */
	bool CreateIndex;

	/**
	 * Index of the objects.
	 */
	R::RKeyValueFile<R::RIntKey>* Index;

	/**
	 * Create the tree concepts?
	 */
	bool CreateTree;

	/**
	 * Tree concepts of the objects.
	 */
	R::RKeyValueFile<R::RIntKey>* Tree;

	/**
	 * Are the objects loaded.
	 */
	bool Loaded;

	/**
	 * Temporary vector of references.
	 */
	R::RNumContainer<size_t,true> tmpRefs;

	/**
	 * Reader/writer lock on the objects.
	 */
	R::RRWLock lObjects;

	/**
	 * Reader/writer lock on the descriptions.
	 */
	R::RRWLock lDesc;

	/**
	 * Reader/writer lock on the index.
	 */
	R::RRWLock lIndex;

	/**
	 * Reader/writer lock on the trees.
	 */
	R::RRWLock lTree;

public:

	/**
	 * Constructor of a set of stored objects.
	 * @param session        Session.
	 * @param size           Initial size of the list.
	 * @param name           Name of the objects.
	 * @param type           Type of the objects.
	 */
	GObjects(GSession* session,size_t size,const R::RString& name,tObjType type);

	/**
	* Insert an object.
	* @param obj             object to insert.
	* @param selected        Specify if the object is selected or not.
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
	 * Get the number of objects.
	 * @param obj            Pseudo-parameter.
	 */
	inline size_t GetNbObjs(const C* obj) const;

	/**
	 * Get the highest identifier of the objects.
	 * @param obj            Pseudo-parameter.
	 */
	inline size_t GetMaxObjId(const C* obj) const;

	/**
	 * Get the highest position occupied by an object.
	 * @param obj            Pseudo-parameter.
	 */
	inline size_t GetMaxObjPos(const C* obj) const;

	/**
	 * Get the objects.
	 * @param obj            Pseudo-parameter.
	 * @return a cursor over the objects.
	 */
	inline R::RCursor<C> GetObjs(const C* obj) const;

	/**
	 * Fill an array of objects.
	 * @param tab            Array that will be filled.
	 * @param alloc          Must the array be allocated ?
	 * @return number of objects in the array (including null pointers).
	 */
	inline size_t GetObjs(C** &tab,bool alloc);

	/**
	 * Get an object based on its identifier.
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
	 * Get an object based on its name.
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
	 * Load the identifiers of all objects described by a specific concept.
	 * @param obj            Pseudo-parameter.
	 * @param concept        Concept to search for.
	 * @param refs           Identifiers of the objects.
	 */
	void LoadIndex(const C* obj,GConcept* concept,R::RNumContainer<size_t,true>& refs);

	/**
	 * Build the index of all the objects from scratch. Since only the object
	 * descriptions are used, no occurrence information is available.
	 * @param obj            Pseudo-parameter.
	 */
	void BuildIndex(const C* obj);

	/**
	 * Load the concept tree of a given object. This method generates an
	 * exception for all objects excepted documents.
	 * @param obj            Pseudo-parameter.
	 * @param tree           Concept tree. If null,it is created.
	 * @param blockid        Identifier of the block of the document.
	 * @param id             Identifier of the document.
	 */
	void LoadTree(const C* obj,GConceptTree* &tree,size_t blockid,size_t id);

	/**
	 * Load the tree as a container of records.
	 * @param obj            Obj for which the tree must be loaded.
	 * @param records        Container of records (modified by the method).
	 * @param nbrecords      Number of records used for the document tree
	 *                       (modified by the method).
	 * @param nbtoprecords   Number of top records (modified by the method).
	 * @param nbrefs         Number of concepts referenced (modified by the
	 *                       method).
    */
	void LoadTree(const C* obj,R::RContainer<GConceptRecord,false,true>& records,size_t& nbrecords,size_t& nbtoprecords,size_t& nbrefs);

private:

	/**
	 * Initialize the parameters.
	 * @param config         Configuration structure.
	 */
	void Init(R::RConfig* config);

	/**
	 * Clear the list.
	 * @param obj            Pseudo-parameter.
	 * @param list           Specify if the objects must be deleted or just their
	 *                       descriptions.
	 */
	virtual void Clear(const C* obj,bool del);

	/**
	 * Open the files in a given directory.
	 * @param config         Configuration structure.
	 * @param subdir         Sub-directory where the files will be stored.
	 */
	void OpenFiles(R::RConfig* config,R::RString subdir);

	/**
	* @return if an index is create.
	* @param obj            Pseudo-parameter.
	*/
	inline bool DoCreateIndex(const C* obj) const;

	 /**
	* @return if the concept trees are created.
	* @param obj            Pseudo-parameter.
	*/
	inline bool DoCreateTree(const C* obj) const;

	/**
	* Assign an identifier to an object.
	* @param obj             Object.
	*/
	void AssignId(C* obj);

	/**
	* Load all the objects.
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
	 * Flush the descriptions of the objects.
	 * @param obj            Pseudo-parameter.
	 */
	void FlushDesc(const C* obj);

   /**
	 * Update the index of a given object that is only described by a
	 * description.
	 * @param obj            Pseudo-parameter.
	 * @param desc           Description.
	 * @param id             Identifier of the object.
	 * @param add            Object must be added or removed from the index.
	 */
	void UpdateIndex(const C* obj,const GDescription& desc,size_t id,bool add);

	/**
	 * Load the next node in the file into a given tree. The method loads first
	 * the node information and then the method is called for its children.
    * @param tree           Tree that will contain the new node.
    * @param parent         Parent node of the one to load.
    */
	void LoadNode(GConceptTree* tree,GConceptNode* parent);

	/**
	 * Save a record in the file. The method saves first the node information and
	 * then calls the method for its children.
    * @param node           Node to save.
    */
	void SaveNode(GConceptRecord* rec);

	/**
	 * Save the concept tree of a given object. This method generates an
	 * exception except for documents.
	 * @param obj            Pseudo-parameter.
	 * @param records        Container of records.
	 * @param nbrecords      Number of records used for the document tree.
	 * @param nbtoprecords   Number of top records.
	 * @param nbrefs         Number of concepts referenced.
	 * @param blockid        Identifier of the block of the document (0 means the
	 *                       block will be found).
	 * @param id             Identifier of the document.
	 */
	void SaveTree(const C* obj,const R::RContainer<GConceptRecord,false,true>& records,size_t nbrecords,size_t nbtoprecords,size_t nbrefs,size_t& blockid,size_t id);

	/**
	 * Flush the file storing the concept trees of the objects.
	 * @param obj            Pseudo-parameter.
	 */
	void FlushTree(const C* obj);

	/**
	 * Seek the store to the beginning of a record of an object.
    * @param obj            Pointer to the object.
    * @param idx            Index of the object.
    */
	void SeekRecord(const C* obj,size_t idx);

public:

	/**
	 * Find the first record that correspond to a given object starting from a
	 * given index.
	 *
	 * The first record is not necessary the highest in the hierarchy, since a
	 * depth-first search is used.
    * @param obj            Pointer to the object.
    * @param rec            Record that will be filled with the result. The
	 *                       concept to be searched must be set.
	 * @param idx            Index where to start the search.
    * @return true of a record was found.
    */
	bool FindDepthFirstRecord(const C* obj,GConceptRecord& rec,size_t idx);

	/**
	 * Find a record at a given syntactic position.
    * @param obj            Pointer to the object.
    * @param rec            Record that will be filled with the result.
    * @param synpos         Syntactic position.
    * @return true of a record was found.
    */
	bool FindRecord(const C* obj,GConceptRecord& rec,size_t synpos);

	/**
	 * Find the parent record of a given record.
    * @param obj            Pointer to the object.
    * @param search         Record that contains the record for which the
	 *                       parent must be found.
	 * @param rec            Record that will be filled with the result.
    * @return true of a parent record was found.
    */
	bool FindParentRecord(const C* obj,const GConceptRecord& search,GConceptRecord& rec);

	/**
	 * Find the first child record that correspond to a given record and starting
	 * from a given index.
	 *
    * @param obj            Pointer to the object.
    * @param search         Record that contains the record for which the
	 *                       parent must be found.
	 * @param rec            Record that will be filled with the result.
	 * @param idx            Index where to start the search.
    * @return true of a record was found.
    */
	bool FindFirstChildRecord(const C* obj,const GConceptRecord& search,GConceptRecord& rec,size_t idx);

	/**
	 * Find the nearest record at a given syntactic position. In practice, the
	 * method searches alternatively before and after the given position.
    * @param obj            Pointer to the object.
    * @param rec            Record that will be filled with the result.
    * @param synpos         Syntactic position.
    * @return true of a record was found.
    */
	bool FindNearestRecord(const C* obj,GConceptRecord& rec,size_t synpos);

	/**
	 * Find the nearest record at a given syntactic position.  In practice, the
	 * method searches the first record after or before the given position. If no
	 * record is found, it searches in the order direction.
    * @param obj            Pointer to the object.
    * @param rec            Record that will be filled with the result.
    * @param synpos         Syntactic position.
	 * @param after          Search after the position (true) or before it
	 *                       (false)?
    * @return true if a record was found.
    */
	bool FindNearestRecord(const C* obj,GConceptRecord& rec,size_t synpos,bool after);

	/**
	 * Find the root record (the most common highest record) of two records. If
	 * at least one of the node doesn't have a parent, no root node can be found.
    * @param obj            Pointer to the object.
    * @param rec1           First record.
    * @param rec2           Second record.
    * @param rec            Record that will be filled with the result.
	 * @return true if a root node could be found.
    */
	bool FindRootRecord(const C* obj,const GConceptRecord& rec1,const GConceptRecord& rec2,GConceptRecord& rec);

	/**
	 * Find the minimal position centred on a given record and a number of
	 * records before.
    * @param obj            Pointer to the object.
    * @param rec            Record that contains the record for which the
	 *                       parent must be found.
    * @param nbbefore       Maximum number of concept node before.
    * @return a position.
    */
	size_t GetMinPosRecord(const C* obj,const GConceptRecord& rec,size_t nbbefore);

	/**
	 * Find the maximal position centred on a given record and a number of
	 * records after.
    * @param obj            Pointer to the object.
    * @param rec            Record that contains the record for which the
	 *                       parent must be found.
    * @param nbafter        Maximum number of record after.
    * @return a position.
    */
	size_t GetMaxPosRecord(const C* obj,const GConceptRecord& rec,size_t nbafter);

	/**
	 * Compute the maximal position occupied by a record in an object.
	 * @param obj            Pointer to the object.
    * @return a position.
    */
	size_t GetMaxPosRecord(const C* obj);

	/**
	 * Compute the maximal syntactic position occupied by a record in an object.
	 * @param obj            Pointer to the object.
    * @return a syntactic position.
    */
	size_t GetMaxSyntacticPosRecord(const C* obj);

	/**
	 * Get the total number of child records (including all the children) of a
	 * given record.
    * @param obj            Pointer to the object.
    * @param rec            Record that contains the record for which the
	 *                       number must be found.
    * @return a number.
    */
	size_t GetNbTotalChildRecords(const C* obj,const GConceptRecord& rec);

	/**
	 * Destructor.
	 */
	virtual ~GObjects(void);

	friend class GSession;
	friend class GClasses;
};


//-----------------------------------------------------------------------------
// Template implementation
#include <gobjects.hh>


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
