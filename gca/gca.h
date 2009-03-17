/*

	Genetic Community Algorithm

	GCA.h

	Main - Header.

	Copyright 2002-2007 by the Université Libre de Bruxelles.

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


//-----------------------------------------------------------------------------
#ifndef GCAH
#define GCAH


//-----------------------------------------------------------------------------
// includes files for R Project
#include <scga.h>
#include <rparam.h>
#include <rgroups.h>
#include <robjsc.h>
using namespace R;


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>
using namespace GALILEI;


//-----------------------------------------------------------------------------
// Forward class declaration
class GCAGroup;
class GCAThreadData;
class GCAChromo;
class GCAInst;
class GCAObj;
class GCAHeuristic;


//-----------------------------------------------------------------------------
// forward declaration
class CGroups;


//-----------------------------------------------------------------------------
/**
 * The Group provides a representation for a group holding GCAObj (used by
 * kMeans).
 * @author Pascal Francq
 * @short kMeans Group
 */
class CGroup : public R::RGroup<CGroup,GCAObj,CGroups>
{
public:

	/**
	 * Constructor.
	 * @param owner          Owner of the group.
	 * @param id             Identifier of the group.
	 */
	CGroup(CGroups* owner,size_t id);
};


//-----------------------------------------------------------------------------
/**
 *
 */
class CGroups : public R::RGroups<CGroup,GCAObj,CGroups>
{
public:
	CGroups(R::RCursor<GCAObj> objs,size_t max);
};


//-----------------------------------------------------------------------------
/**
* The GCAObj class provides a representation of a profile or a document to
* group.
* @author Pascal Francq
* @short IR Object.
*/
class GCAObj : public RObjSC
{
protected:

	/**
	* Pointer to the element.
	*/
	void* Element;

	/**
	 * Identifier of the element.
	 */
	size_t ElementId;

public:

	/**
	* Construct the object.
	* @param id             Identifier.
	* @param d              Corresponding document.
	*/
	GCAObj(size_t id,GDoc* d);

	/**
	* Construct the object.
	* @param id             Identifier.
	* @param p              Corresponding profile.
	*/
	GCAObj(size_t id,GProfile* p);

	/**
	* Return a pointer to the element.
	*/
	inline void* GetElement(void) const {return(Element);}

	/**
	 * Return the identifier of the element.
	 */
	inline size_t GetElementId(void) const {return(ElementId);}

	/**
	* Destruct the object.
	*/
	~GCAObj(void);

	// friend classes
	friend class GCAGroup;
};


//-----------------------------------------------------------------------------
#endif
