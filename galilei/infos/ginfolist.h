/*

	GALILEI Research Project

	GInfoList.h

	Information entity representing a list of words - Header.

	Copyright 2001-2003 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).

	Version $Revision$

	Last Modify: $Date$

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
#ifndef GInfoListH
#define GInfoListH


//------------------------------------------------------------------------------
// include files for GALILEI
#include <infos/ginfo.h>


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* The GInfoList class provides a representation for a list of words. In
* particular, each list has an identificator, which can be used to build
* containers of GInfoList.
* @author Pascal Francq
* @short Words List Information.
*/
class GInfoList : public GInfo, public R::RContainer<GInfo,unsigned,true,true>
{
public:

	/**
	* Constructor of an information entity representing a list.
	*/
	GInfoList(void) throw(bad_alloc);

	/**
	* Compare an identificator with the corresponding one of the list.
	* @param id              Identificator.
	* @see R::RContainer
	* @return int
	*/
	int Compare(const unsigned int& id) const;

	/**
	* Compare the identificators of the list.
	* @param i               Pointer to an information entity representing a
	*                        list.
	* @see R::RContainer
	* @return int
	*/
	int Compare(const GInfoList* i) const;

	/**
	* Compare the identificators of the list.
	* @param i               Information entity representing a list.
	* @see R::RContainer
	* @return int
	*/
	int Compare(const GInfoList& i) const;

	/**
	* Get the name of the class.
	* @return R::String.
	*/
	virtual const R::RString ClassName(void) const;

	/**
	* Get the type of the information.
	* @return GInfoType.
	*/
	virtual const GInfoType InfoType(void) const;

	/**
	* Get the identificator of the list.
	* @return unsigned int.
	*/
	unsigned int GetId(void) const;

	/**
	* Set the identificator of the list.
	* @param id             Identificator to assign.
	*/
	void SetId(unsigned int id);

	/**
	* The method verifies if two lists contain exactly the same information
	* entities.
	* @param i               Information entity representing a list.
	*/
	bool IsSame(const GInfoList& i) const;

	/**
	* Destructor of an information entity representing a list.
	*/
	virtual ~GInfoList(void);
};


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
