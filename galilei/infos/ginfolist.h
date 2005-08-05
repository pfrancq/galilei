/*

	GALILEI Research Project

	GInfoList.h

	List of information entities - Header.

	Copyright 2001-2003 by the Universit�Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).

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
#include <ginfo.h>


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* The GInfoList class provides a representation for a list of information
* entities. In particular, the list is also considered as a information entity.
* @author Pascal Francq
* @short Information Entity List.
*/
class GInfoList : public GInfo, public R::RContainer<GInfo,true,true>
{
public:

	/**
	* Constructor of a list of information entities.
	* @param id              Identificator of the information entity.
	* @param type            Type of the information entity.
	*/
	GInfoList(unsigned int id,tInfoType type=infoWordList) throw(std::bad_alloc);

	/**
	* Copy constructor for a list of information entities.
	* @param i               Information entity representing a list.
	*/
	GInfoList(const GInfoList& i) throw(std::bad_alloc);

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
	* Get the identificator of the information entity.
	* @return unsigned int.
	*/
	unsigned int GetId(void) const {return(Id);}

	/**
	* Assignment operator for lists of information entities.
	* @param i               Information entity representing a list.
	*/
	GInfoList& operator=(const GInfoList& i) throw(std::bad_alloc);

	/**
	* The method verifies if two lists contain exactly the same information
	* entities.
	* @param i               Information entity representing a list.
	*/
	bool IsSame(const GInfoList& i) const;

	/**
	* Desctuctor of a list of information entities.
	*/
	virtual ~GInfoList(void);
};


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
