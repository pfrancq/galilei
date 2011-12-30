/*

	GALILEI Research Project

	GClass.h

	Class regrouping concepts - Header.

	Copyright 2009-2011 by Pascal Francq (pascal@francq.info).

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
#include <rnode.h>


//------------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>
#include <gdescriptionobject.h>


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
class GClass : public R::RNode<GClasses,GClass,false>, public GDescriptionObject<GClass>
{
    using R::RNode<GClasses,GClass,false>::Clear;

	/**
	 * Method used to correctly instantiate some template methods.
	 */
	void PrivateInit(void);

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

	/**
	* Assign a description to the class.
	* @param desc            Description.
	* \warning The vectors are cleared by this method.
	*/
	void Update(GDescription& desc);

	/**
	 * Get the cost of an Up operation of the current node. The method adds a
	 * cost computed for each vector. The cost of one vector is :
	 * -# The number of concept references if the class has no parent.
	 * -# The difference of the number of concept references between the parent
	 *    and the current class.
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


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
