/*

	GALILEI Research Project

	GInfo.h

	Information entity - Header.

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
#ifndef GInfoH
#define GInfoH


//------------------------------------------------------------------------------
// include files for GALILEI
#include <sessions/galilei.h>


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* The GInfo class provides a representation for a generic information entity.
* Each information entity is represented by :
* - an unique identificator in a space ;
* - a type representing the kind of information represented.
* @author Pascal Francq.
* @short Information Entity.
*/
class GInfo
{
protected:

	/**
	* Identificator of the information entity.
	*/
	unsigned int Id;

	/**
	* Type of the information entity.
	*/
	GInfoType Type;

public:

	/**
	* Constructor of an information entity.
	* @param id              Identificator of the information entity.
	* @param type            Type of the information entity.
	*/
	GInfo(unsigned int id,GInfoType type=infoWord) throw(bad_alloc);

	/**
	* Copy constructor for an information entity.
	* @param i               Information entity.
	*/
	GInfo(const GInfo& i) throw(bad_alloc);

	/**
	* Copy constructor for an information entity.
	* @param i               Pointer to an nformation entity.
	*/
	GInfo(const GInfo* i) throw(bad_alloc);

	/**
	* Compare two information entities by comparing their identificator.
	* @see R::RContainer
	* @param i               Information entity.
	* @return int
	*/
	virtual int Compare(const GInfo& i) const;

	/**
	* Compare two information entities by comparing their identificator.
	* @see R::RContainer
	* @param i               Pointer an information entity.
	* @return int
	*/
	virtual int Compare(const GInfo* i) const;

	/**
	* Assignment operator for information entities.
	* @param i               Information entity.
	*/
	GInfo& operator=(const GInfo& i) throw(bad_alloc);

	/**
	* Compute a similarity between two information entities. In the default
	* implementation, the method compares the identificators.
	* @param i               Information entity.
	* @return a value between 0 and 1.
	*/
	virtual float Similarity(const GInfo& i) const;

	/**
	* Compute a similarity between two information entities. In the default
	* implementation, the method compares the identificators.
	* @param i               Pointer an information entity.
	* @return a value between 0 and 1.
	*/
	virtual float Similarity(const GInfo* i) const;

	/**
	* Compute a dissimilarity between two information entities. In the default
	* implementation, the method compares the identificators.
	* @param i               Information entity.
	* @return a value between 0 and 1.
	*/
	virtual float DisSimilarity(const GInfo& i) const;

	/**
	* Compute a dissimilarity between two information entities. In the default
	* implementation, the method compares the identificators.
	* @param i               Pointer an information entity.
	* @return a value between 0 and 1.
	*/
	virtual float DisSimilarity(const GInfo* i) const;

	/**
	* Get the identificator of the information entity.
	* @return unsigned int.
	*/
	unsigned int GetId(void) const {return(Id);}

	/**
	* Get the type of the information.
	* @return GInfoType.
	*/
	GInfoType InfoType(void) const {return(Type);}

	/**
	* Verifies if two information entities have the same type.
	* @param i               Information entity.
	* @return true if the two entities are from the same type, false else.
	*/
	virtual bool SameType(const GInfo& i) const {return(InfoType()==i.InfoType());}

	/**
	* Verifies if two information entities have the same type.
	* @param i               Pointer an information entity.
	* @return true if the two entities are from the same type, false else.
	*/
	virtual bool SameType(const GInfo* i) const {return(InfoType()==i->InfoType());}

	/**
	* Destructor of a generic information entity.
	*/
	virtual ~GInfo(void);
};


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
