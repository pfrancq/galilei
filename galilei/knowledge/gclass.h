/*

	GALILEI Research Project

	GClass.h

	Class regrouping concepts - Header.

	Copyright 2009-2010 by Pascal Francq (pascal@francq.info).

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
#ifndef GClassH
#define GClassH


//------------------------------------------------------------------------------
// include files for R
#include <rtree.h>


//------------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>
#include <gweightinfosobj.h>


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* The GClass provides a representation for a group of concepts. All the classes
* are part of tree.
* @author Pascal Francq
* @short Concepts Class.
*/
class GClass : public R::RNode<GClasses,GClass,true>, public GWeightInfosObj
{
public:

	/**
	* Construct a new class.
	* @param session         Session.
	* @param name            Name of the class.
	*/
	GClass(GSession* session,const R::RString& name);

	/**
	* Construct a class with a specific identifier.
	* @param session         Session.
	* @param id              Identifier.
	* @param blockid         Identifier of the block.
	* @param name            Name of the class.
	*/
	GClass(GSession* session,size_t id,size_t blockid,const R::RString& name);

	/**
	 * Clear the node.
	 */
	virtual void Clear(void);

	/**
	* Compare two groups by comparing their identifier.
	* @see R::RContainer
	* @param grp             Group.
	* @return int
	*/
	int Compare(const GClass& grp) const;

	/**
	* Compare two groups by comparing their identifier.
	* @see R::RContainer
	* @param grp             Pointer to a group.
	* @return int
	*/
	int Compare(const GClass* grp) const;

	/**
	* Compare the identifier of a group with another one.
	* @see R::RContainer
	* @param id              Identifier.
	* @return int
	*/
	int Compare(const size_t id) const;

private:

	/**
	* Assign a new description to the class.
	* @param session         Session.
	* @param infos           Pointer to the information.
	* \warning The container infos is cleared by this method.
	*/
	void Update(GSession* session,GWeightInfos& infos);

public:

	/**
	 * Get the cost of an Up operation of the current node. By default, the
	 * cost equals to 1.
	 *
	 * In their paper <em>TreeRank: A Similarity Measure for Nearest Neighbor
	 * Searching in Phylogenetic Databases</em>, Wang, Shan, Shasha and Piel
	 * define the up operation as the operation that moves a token from one
	 * node to its parent.
	 * @return Cost of the up operation.
	 */
	virtual double GetUpOperationCost(void) const;

	/**
	* Destruct the group.
	*/
	virtual ~GClass(void);

	friend class GOntlogy;
	friend class GSession;
};


//------------------------------------------------------------------------------
/**
 * The GClasses provides just a tree of GClass.
 * @author Pascal Francq.
 * @short Classes
 */
class GClasses : public R::RTree<GClasses,GClass,true>
{
public:

	/**
	* Construct a tree of classes.
	* @param max            Initial size of the array of top nodes.
	* @param inc            Increment size of the array.
	*/
	GClasses(size_t max,size_t inc);
};


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
