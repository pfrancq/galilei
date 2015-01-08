/*

	GALILEI Research Project

	GDescriptionSet.h

	Description Set - Header.

	Copyright 2006-2015 by Pascal Francq (pascal@francq.info).
	Copyright 2006-2008 by the Université Libre de Bruxelles (ULB).

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
#ifndef GDescriptionSet_H
#define GDescriptionSet_H


//-----------------------------------------------------------------------------
// include files for R Project
#include <rnumcontainer.h>
#include <rboolvector.h>


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <gdescription.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/**
 * The GDescriptionSetData provides a representation for a piece of data
 * associated to a GDescriptionSet. In practice, methods that compute some
 * data based on a description set create their own class. This inherited class
 * is responsible to set the member variable Dirty to false once a computation
 * is done.
 * @short Description Set Data
 */
class GDescriptionSetData
{
protected:

   /**
    * Must the data be recomputed.
    */
   bool Dirty;

public:
	/**
	 * Construct the data.
    * @param session        Session.
    */
	GDescriptionSetData(GSession* session);

	 /**
	  * Define if the data is dirty or not.
	  */
	 bool IsDirty(void) const {return(Dirty);}

	/**
	 * Destructor.
    */
	virtual ~GDescriptionSetData(void);

	friend class GDescriptionSet;
};


//-----------------------------------------------------------------------------
/**
 * The GDescriptionSet provides a representation for a set of descriptions. It
 * is useful to compute the inverse object for a given subset.
 * @author Pascal Francq
 * @short Description Set
 */
class GDescriptionSet
{
private:
   /**
    * The session.
    */
   GSession* Session;

   /**
    * The set of description.
    */
   R::RContainer<const GDescription,false,false> Set;

	/**
	 * Data associated to a description set.
	 */
	GDescriptionSetData* Data;

public:

    /**
     * Constructor of the description set.
     * @param session       Session.
     */
    GDescriptionSet(GSession* session);

    /**
     * @return the number of descriptions.
     */
    size_t GetNb(void) const{return(Set.GetNb());}

    /**
     * Clear the description set.
     */
    void Clear(void);

    /**
     * Add a description to the set.
     * @param desc          Description to add.
     */
    void InsertDescription(const GDescription* desc);

	 /**
	  * Get the data associated with the set.
     * @return a pointer to the data (or null if no data is assigned).
     */
	 GDescriptionSetData* GetData(void) const {return(Data);}

	 /**
	  * Assign a data with the set. The description set is responsible for the
	  * deallocation of the last data assigned.
     * @param data          Data to assign.
     */
	 void SetData(GDescriptionSetData* data);

	 /**
	  * Get a cursor over the descriptions.
	  */
	 R::RCursor<const GDescription> GetDescriptions(void) const;

	 /**
	  * Destructor. If a data is associated to the set, it is deleted.
     */
	 ~GDescriptionSet(void);
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
