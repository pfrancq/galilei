/*

	GALILEI Research Project

	GClasses.h

	Classes - Header.

	Copyright 2009-2015 by Pascal Francq (pascal@francq.info).

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
#ifndef GClassesH
#define GClassesH


//------------------------------------------------------------------------------
// include files for R
#include <rtree.h>


//------------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>
#include <gobjects.h>
#include <gclass.h>


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
 * The GClasses provides just a tree of GClass.
 * @author Pascal Francq.
 * @short Classes
 */
class GClasses : private R::RTree<GClasses,GClass,false>, private GObjects<GClass,hClasses>
{
	using GObjects<GClass,hClasses>::InsertObj;
	using GObjects<GClass,hClasses>::DeleteObj;
	using R::RTree<GClasses,GClass,false>::InsertNode;
	using R::RTree<GClasses,GClass,false>::DeleteNode;
	using R::RTree<GClasses,GClass,false>::Clear;
	using GObjects<GClass,hClasses>::Clear;

public:

	/**
	* Construct a tree of classes.
	* @param max             Initial size of the container.
	*/
	GClasses(size_t max);

	/**
	 * @return Pointer to the class.
	 */
	GClasses* GetClassTree(void);

	/**
    * @return the first root class. If null, the tree is empty.
    */
	inline GClass* GetFirst(void) const {return(R::RTree<GClasses,GClass,false>::GetFirst());}

	/**
    * @return the last root class. If null, the tree is empty.
    */
	inline  GClass* GetLast(void) const {return(R::RTree<GClasses,GClass,false>::GetLast());}

	/**
	 * @return the depth of the tree. If necessary, the depth is recomputed.
	 */
	inline size_t GetMaxDepth(void) const {return(R::RTree<GClasses,GClass,false>::GetMaxDepth());}

	/**
	* @return the total number of classes in the tree.
	*/
	inline size_t GetNbNodes(void) const {return(R::RTree<GClasses,GClass,false>::GetNbNodes());}

	/**
	* @return the total number of top classes in the tree.
	*/
	inline size_t GetNbTopNodes(void) const {return(R::RTree<GClasses,GClass,false>::GetNbTopNodes());}

	/**
	 * Copy the classes of a class into a temporary array. This array must have
	 * the right size.
	 * @param tab             Temporary array.
	 * @param node            Parent class. If null, the array contains the top
	 *                        classes.
	 * @param children        Must the children be copied too.
	 * @return Number of the nodes copied in the array
	 */
	inline size_t GetTab(GClass** tab,GClass* node,bool children=false) {return(R::RTree<GClasses,GClass,false>::GetTab(tab,node,children));}

	/**
	 * Get the number of classes.
	 * @param obj            Pseudo-parameter.
	 */
	inline size_t GetNbObjs(const GClass* obj) const {return(GObjects<GClass,hClasses>::GetNbObjs(obj));}

	/**
	 * Get the highest identifier of the classes.
	 * @param obj            Pseudo-parameter.
	 */
	inline size_t GetMaxObjId(const GClass* obj) const {return(GObjects<GClass,hClasses>::GetMaxObjId(obj));}

	/**
	 * Get the highest position occupied by a clas.
	 * @param obj            Pseudo-parameter.
	 */
	inline size_t GetMaxObjPos(const GClass* obj) const {return(GObjects<GClass,hClasses>::GetMaxObjPos(obj));}

	/**
	 * Get a cursor to the classes.
    * @param obj            Pseudo-parameter.
    */
	inline R::RCursor<GClass> GetObjs(const GClass* obj) const {return(GObjects<GClass,hClasses>::GetObjs(obj));}

	/**
	 * Fill an array of classes.
	 * @param tab            Array that will be filled.
	 * @param alloc          Must the array be allocated ?
	 * @return number of objects in the array (including null pointers).
	 */
	inline size_t GetObjs(GClass** &tab,bool alloc) {return(GObjects<GClass,hClasses>::GetObjs(tab,alloc));}

	/**
	 * Get an class based on its identifier.
	 * @param obj            Pseudo-parameter.
	 * @param id             Identifier of the class.
	 * @param load           If set to true, the class is eventually loaded
	 *                       into memory.
	 * @param null           If set to true, if the class does not exist,
	 *                       return 0, else an exception is generated.
	 * @return a pointer to a class.
	 */
	inline GClass* GetObj(const GClass* obj,size_t id,bool load=true,bool null=false) {return(GObjects<GClass,hClasses>::GetObj(obj,id,load,null));}

	/**
	 * Get an class based on its name.
	 * @param obj            Pseudo-parameter.
	 * @param name           Name of the class.
	 * @param load           If set to true, the class is eventually loaded
	 *                       into memory.
	 * @param null           If set to true, if the class does not exist,
	 *                       return 0, else an exception is generated.
	 * @return a pointer to a class.
	 */
	inline GClass* GetObj(const GClass* obj,const R::RString& name,bool load=true,bool null=false) {return(GObjects<GClass,hClasses>::GetObj(obj,name,load,null));}

	/**
	 * Load the identifiers of all classes described by a specific concept.
	 * @param obj            Pseudo-parameter.
	 * @param concept        Concept to search for.
	 * @param refs           Identifiers of the classes.
	 */
	inline void LoadIndex(const GClass* obj,GConcept* concept,R::RNumContainer<size_t,true>& refs) {GObjects<GClass,hClasses>::LoadIndex(obj,concept,refs);}

	/**
	 * Build the index of all the classes from scratch. Since only the class
	 * descriptions are used, no occurrence information is* available.
	 * @param obj            Pseudo-parameter.
	 */
	inline void BuildIndex(const GClass* obj) {GObjects<GClass,hClasses>::BuildIndex(obj);}

	/**
	* Insert a class.
	* @param parent          Parent class.
	* @param obj             object to insert.
	*/
	virtual void InsertObj(GClass* parent,GClass* obj);

	/**
	* Delete an object.
	* @param obj             object to delete.
	*/
	virtual void DeleteObj(GClass* obj);

private:

	/**
	 * Initialize the parameters.
	 * @param config         Configuration structure.
	 */
	inline void Init(R::RConfig* config) {GObjects<GClass,hClasses>::Init(config);}

	/**
	 * Open the files in a given directory.
	 * @param config         Configuration structure.
	 * @param subdir         Sub-directory where the files will be stored.
	 */
	inline void OpenFiles(R::RConfig* config,R::RString subdir) {GObjects<GClass,hClasses>::OpenFiles(config,subdir);}

	/**
	* @return if an index is create.
	* @param obj            Pseudo-parameter.
	*/
	inline bool DoCreateIndex(const GClass* obj) const {return(GObjects<GClass,hClasses>::DoCreateIndex(obj));}

	 /**
	* @return if the concept trees are created.
	* @param obj            Pseudo-parameter.
	*/
	inline bool DoCreateTree(const GClass* obj) const {return(GObjects<GClass,hClasses>::DoCreateTree(obj));}

	/**
	* Assign an identifier to an class.
	* @param obj             Class.
	*/
	inline void AssignId(GClass* obj) {GObjects<GClass,hClasses>::AssignId(obj);}

	/**
	* Load all the classes.
	* @param obj             Pseudo-parameter.
	*/
	inline void LoadObjs(const GClass* obj) {GObjects<GClass,hClasses>::LoadObjs(obj);}

	/**
	* Load the description of a given class.
	* @param obj             Pseudo-parameter.
	* @param vectors         Vectors.
	* @param blockid         Identifier of the block of the class.
	* @param id              Identifier of the class.
	*/
	inline void LoadDesc(const GClass* obj,R::RContainer<GVector,true,true>* &vectors,size_t blockid,size_t id) {GObjects<GClass,hClasses>::LoadDesc(obj,vectors,blockid,id);}

	/**
	* Save the description of a given class.
	* @param obj             Pseudo-parameter.
	* @param vectors         Vectors.
	* @param blockid         Identifier of the block of the class (0 means the block will be found).
	* @param id              Identifier of the class.
	*/
	inline void SaveDesc(const GClass* obj,const R::RContainer<GVector,true,true>& vectors,size_t& blockid,size_t id) {GObjects<GClass,hClasses>::SaveDesc(obj,vectors,blockid,id);}

	/**
	 * Flush the descriptions of the classes.
	 * @param obj            Pseudo-parameter.
	 */
	inline void FlushDesc(const GClass* obj) {GObjects<GClass,hClasses>::FlushDesc(obj);}

   /**
	 * Update the index of a given class that is only described by a
	 * description.
	 * @param obj            Pseudo-parameter.
	 * @param desc           Description.
	 * @param id             Identifier of the class.
	 * @param add            Class must be added or removed from the index.
	 */
	inline void UpdateIndex(const GClass* obj,const GDescription& desc,size_t id,bool add) {GObjects<GClass,hClasses>::UpdateIndex(obj,desc,id,add);}

   /**
	* Clear the classes.
	*/
	virtual void Clear(void);

   /**
	* Clear the classes.
   * @param obj             pseudo-parameter.
	*/
	virtual void Clear(const GClass* obj,bool del);

public:

  	/**
	 * Get the cost of the Up operations to move a token from a node to another
	 * one.
	 *
	 * In their paper <em>TreeRank: A Similarity Measure for Nearest Neighbor
	 * Searching in Phylogenetic Databases</em>, Wang, Shan, Shasha and Piel
	 * define a similarity measure between two trees based on the different
	 * costs of up operations of the nodes of each tree.
	 * @param u              First node.
	 * @param v              Second node.
	 * @return Cost of the up operations.
	 */
	inline double GetUpOperationsCost(const GClass* u,const GClass* v) const {return(R::RTree<GClasses,GClass,false>::GetUpOperationsCost(u,v));}

	friend class R::RNodeCursor<GClasses,GClass>;
	friend class R::RTree<GClasses,GClass,false>;
	friend class GSession;
};


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
