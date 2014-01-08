/*

	GALILEI Research Project

	GObjects.h

	Object List - Header.

	Copyright 2011-2014 by Pascal Francq (pascal@francq.info).

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
#include <rnodecursor.h>


//------------------------------------------------------------------------------
// include files for GALILEI
#include <gkb.h>
#include <gstorage.h>
#include <ggalileiapp.h>
#include <gdescriptionobject.h>
#include <gconceptnode.h>
#include <gconcepttree.h>


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
 * @author Pascal Francq
 * @short Stored Objects.
 */
template<class C,const R::hNotification* hEvents>
	class GObjects : virtual GKB, protected R::RObjectContainer<C,true>
{
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
	 * Maximum objects to managed in memory.
	 */
	size_t MaxObjs;

	/**
	 * Description of the objects.
	 */
	R::RIndexFile* Desc;

	/**
	 * Create the index ?
	 */
	bool CreateIndex;

	/**
	 * Index of the objects.
	 */
	R::RIndexFile* Index;

	/**
	 * Create the tree concepts?
	 */
	bool CreateTree;

	/**
	 * Tree concepts of the objects.
	 */
	R::RIndexFile* Tree;

	/**
	 * Are the objects loaded.
	 */
	bool Loaded;

	/**
	* Selected objects.
	*/
	R::RContainer<C,false,true> Selected;

	/**
	 * Temporary vector of references.
	 */
	R::RNumContainer<size_t,true> tmpRefs;

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
	 * Clear the list.
	 * @param obj            Pseudo-parameter.
	 */
	virtual void Clear(const C* obj);

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

protected:

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

public:

	/**
	 * Load the identifiers of all objects of a given type described by a
	 * specific concept.
	 * @param obj            Pseudo-parameter.
	 * @param concept        Concept to search for.
	 * @param refs           Identifiers of the objects.
	 */
	void LoadIndex(const C* obj,GConcept* concept,R::RNumContainer<size_t,true>& refs);

protected:

   /**
	 * Update the index of a given object that is only described by a
	 * description. The methods generates an exception for documents.
	 * @param obj            Pseudo-parameter.
	 * @param desc           Description.
	 * @param id             Identifier of the object.
	 * @param add            Object must be added or removed from the index.
	 */
	void UpdateIndex(const C* obj,const GDescription& desc,size_t id,bool add);

public:

	/**
	 * Build the index of all the objects of a given type from scratch. Since
	 * only the object descriptions are used, no occurrence information is
	 * available. The methods generates an exception for documents.
	 * The methods generates an exception for documents.
	 * @param obj            Pseudo-parameter.
	 */
	void BuildIndex(const C* obj);

private:

	/**
	 * Load the next node in the file into a given tree. The method loads first
	 * the node information and then the method is called for its children.
    * @param tree           Tree that will contain the new node.
    * @param parent         Parent node of the one to load.
    */
	void LoadNode(GConceptTree* tree,GConceptNode* parent);

	/**
	 * Save a node in the file. The method saves first the node information and
	 * then calls the method for its children.
    * @param node           Node to save.
    */
	void SaveNode(GConceptNode* node);

public:

	/**
	 * Load the concept tree of a given document.
	 * @param obj            Pseudo-parameter.
	 * @param tree           Concept tree. If null,it is created.
	 * @param blockid        Identifier of the block of the document.
	 * @param id             Identifier of the document.
	 */
	void LoadTree(const C* obj,GConceptTree* &tree,size_t blockid,size_t id);

protected:

	/**
	 * Save the concept tree of a given document.
	 * @param obj            Pseudo-parameter.
	 * @param tree           Concept tree to save.
	 * @param blockid        Identifier of the block of the document (0 means the
	 *                       block will be found).
	 * @param id             Identifier of the document.
	 */
	void SaveTree(const C* obj,const GConceptTree& tree,size_t& blockid,size_t id);

	/**
	 * Flush the file storing the concept trees of the documents.
	 * @param obj            Pseudo-parameter.
	 */
	void FlushTree(const C* obj);

public:

	/**
	 * Destructor.
	 */
	virtual ~GObjects(void);

	friend class GDocAnalyze;
	friend class GDoc;
	friend class GClass;
	friend class GTopic;
	friend class GProfile;
	friend class GCommunity;
};


//-----------------------------------------------------------------------------
// Template implementation
#include <gobjects.hh>


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
