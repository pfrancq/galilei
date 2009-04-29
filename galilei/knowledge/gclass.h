/*

	GALILEI Research Project

	GClass.h

	Class regrouping concepts - Header.

	Copyright 2009 by Pascal Francq (pascal@francq.info).

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
class GClass : public R::RNode<GClass,true>, public GWeightInfosObj
{
public:

	/**
	* Construct a class with a specific identifier.
	* @param id              Identifier.
	* @param name            Name of the class.
	* @param size            Size of the vector.
	*/
	GClass(size_t id,const R::RString& name,size_t size);

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
	* @param infos           Pointer to the information.
	* \warning The container infos is cleared by this method.
	*/
	void Update(R::RContainer<GWeightInfo,false,true>& infos);

public:

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
