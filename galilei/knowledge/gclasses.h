/*

	GALILEI Research Project

	GClasses.h

	Classes - Header.

	Copyright 2009-2014 by Pascal Francq (pascal@francq.info).

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
class GClasses : private R::RTree<GClasses,GClass,false>, public GObjects<GClass,hClasses>
{
	using GObjects<GClass,hClasses>::InsertObj;
	using GObjects<GClass,hClasses>::DeleteObj;
	using R::RTree<GClasses,GClass,false>::InsertNode;
	using R::RTree<GClasses,GClass,false>::DeleteNode;
	using R::RTree<GClasses,GClass,false>::Clear;

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

   /**
	* Clear the classes.
	*/
	virtual void Clear(void);

   /**
	* Clear the classes.
   * @param obj             pseudo-parameter.
	*/
	virtual void Clear(const GClass* obj);

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
};


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
