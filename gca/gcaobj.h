/*

	Genetic Community Algorithm

	GCAObj.h

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
#ifndef GCAObjH
#define GCAObjH


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>
using namespace GALILEI;


//-----------------------------------------------------------------------------
/**
* The GCAObj class provides a representation of subprofile to to place in a
* group.
* @author Pascal Francq
* @short IR Object.
*/
class GCAObj
{
protected:

	/**
	* The identificator of the object.
	*/
	size_t Id;

	/**
	* Pointer to the element.
	*/
	void* Element;

	/**
	 * Identifier of the element.
	 */
	size_t ElementId;

	/**
	 * Can the element be grouped in a group alone?
	 */
	bool Social;

	/**
	 * Identifier of a common parent between elements that could prevent to group them together.
	 */
	size_t ParentId;

	/**
	* Sum of all the "probabilities" of the documents.
	*/
	double SumPjk;

public:

	/**
	* Construct the object.
	* @param id             Identificator.
	* @param d              Corresponding document.
	*/
	GCAObj(size_t id,GDoc* d);

	/**
	* Construct the object.
	* @param id             Identificator.
	* @param p              Corresponding profile.
	*/
	GCAObj(size_t id,GProfile* p);

	/**
	* Copy Construct.
	* @param obj            Source used.
	*/
	GCAObj(const GCAObj* obj);

	/**
	* Comparaison function
	*/
	int Compare(const size_t id) const;

	/**
	* Comparaison function
	*/
	int Compare(const GCAObj& obj) const;

	/**
	* Comparaison function
	*/
	int Compare(const GCAObj* obj) const;

	/**
	* Return the identificator of the object.
	*/
	size_t GetId(void) const {return(Id);}

	/**
	* Return a pointer to the element.
	*/
	inline void* GetElement(void) const {return(Element);}

	/**
	 * Return the identifier of the element.
	 */
	inline size_t GetElementId(void) const {return(ElementId);}

	/**
	 * See if the element is social or not.
	 */
	inline bool IsSocial(void) const {return(Social);}

	/**
	 * Get the identifier of the parent or 0 if there is no parent.
	 */
	inline size_t GetParentId(void) const {return(ParentId);}

	/**
	* Destruct the object.
	*/
	virtual ~GCAObj(void);

	// friend classes
	friend class GCAGroup;
};


//-----------------------------------------------------------------------------
#endif
