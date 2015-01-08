/*

	GALILEI Research Project

	GClass.h

	Class regrouping concepts - Header.

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
	* Date of last class computation.
	*/
	R::RDate Computed;

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
	* @param c               Date of the last computation.
	*/
	GClass(GSession* session,size_t id,size_t blockid,const R::RString& name,const R::RDate& c);

	/**
    * @return the class name.
    */
	virtual R::RCString GetClassName(void) const {return("GClass");}

	/**
	 * Give the type of the object.
    * @return otClass
    */
	static inline tObjType GetType(void) {return(otClass);}

private:

	/**
	 * Clear the class.
	 * @param disk            Clear the reference to the corresponding block on
	 *                        disk.
	 */
	virtual void Clear(bool disk);

public:

	/**
	* Compare two classes by comparing their identifier.
	* @see R::RContainer
	* @param theclass        Class.
	* @return int
	*/
	int Compare(const GClass& theclass) const;

	/**
	* Compare two classes by comparing their identifier.
	* @see R::RContainer
	* @param theclass        Class.
	* @return int
	*/
	int Compare(const GClass* theclass) const;

	/**
	* Compare the identifier of a class with another one.
	* @see R::RContainer
	* @param id              Identifier.
	* @return int
	*/
	int Compare(const size_t id) const;

	/**
	* Compare the name of a class with another one.
	* @see R::RContainer
	* @param id              Identifier.
	* @return int
	*/
	int Compare(const R::RString& name) const;

	/**
	* Get the date of the last analysis of the document.
	* @return the date.
	*/
	R::RDate GetComputed(void) const {return(Computed);}

	/**
	* Update the representation of the class once a computation was done. The
	* computed date and the status are updated.
	*
	* If the class is an internal one, the following steps are done:
	* * -# An 'hClasses[oeAboutToBeUpdated]' notification is send.
	* -# The references are updated.
	* -# Existing in memory description is replaced.
	* -# If necessary, the class and its description are saved.
	* -# An 'hClasses[oeUpdated]' notification is send.
	*
	* If the topic is an external one, its description is replaced.
	* @param desc            Description to assign.
	* \warning The description is cleared by this method.
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
	friend class GClasses;
	friend class R::RNodeCursor<GClasses,GClass>;
	friend class R::RTree<GClasses,GClass,false>;
	friend class GObjects<GClass,hClasses>;
};


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
