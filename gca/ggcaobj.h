/*

	Genetic Community Algorithm

	GGCAObj.h

	Objects to group - Header.

	Copyright 2002-2007 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, write to the Free Software
	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

*/



//-----------------------------------------------------------------------------
#ifndef GGCAObjH
#define GGCAObjH


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>
using namespace GALILEI;


//-----------------------------------------------------------------------------
/**
* The GGCAObj class provides a representation of subprofile to to place in a
* group.
* @author Pascal Francq
* @short IR Object.
*/
class GGCAObj
{
protected:

	/**
	* The identificator of the object.
	*/
	unsigned int Id;

	/**
	* Pointer to a subprofile.
	*/
	GProfile* Profile;

	/**
	* Sum of all the "probabilities" of the documents.
	*/
	double SumPjk;

public:

	/**
	* Construct the object.
	* @param id             Identificator.
	* @param p              Corresponding profile.
	*/
	GGCAObj(const unsigned int id,GProfile* p);

	/**
	* Copy Construct.
	* @param obj            Source used.
	*/
	GGCAObj(const GGCAObj* obj);

	/**
	* Comparaison function
	*/
	int Compare(const unsigned int &id) const;

	/**
	* Comparaison function
	*/
	int Compare(const GGCAObj& obj) const;

	/**
	* Comparaison function
	*/
	int Compare(const GGCAObj* obj) const;

	/**
	* Return the identificator of the object.
	*/
	unsigned int GetId(void) const {return(Id);}

	/**
	* Return the subprofile of the object.
	*/
	GProfile* GetProfile(void) const {return(Profile);}

	/**
	* Destruct the object.
	*/
	virtual ~GGCAObj(void);

	// friend classes
	friend class GGCAGroup;
};


//-----------------------------------------------------------------------------
#endif
