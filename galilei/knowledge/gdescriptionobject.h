/*

	GALILEI Research Project

	GDescription.h

	Object represented by a list of weighted information entities - Header.

	Copyright 2009-2012 by Pascal Francq (pascal@francq.info).

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
#ifndef GDescriptionObjectH
#define GDescriptionObjectH


//------------------------------------------------------------------------------
// include files for GALILEI
#include <gdescription.h>
#include <gobject.h>


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
 * The GDescriptionObject provides a generic class for a object of a given type
 * with a specific identifier that is represented by a description.
 * @tparam C                 Class of the object.
 * @author Pascal Francq
 * @short Generic Vector Object
 */
template<class C>
	class GDescriptionObject : public GObject, public GDescription
{
protected:

	/**
	* State of the object.
	*/
	tObjState State;

private:

	/**
	 * Identifier of the block containing the description of the object. If
	 * null, the object has no description.
	 */
	size_t BlockId;

public:

	/**
	 * Construct a object.
	 * @param session        Session.
	 * @param id             Identifier of the object.
	 * @param blockid        Identifier of the block.
	 * @param objtype        ObjType.
	 * @param name           Name of the object.
	 * @param state          State of the object.
	 */
	GDescriptionObject(GSession* session,size_t id,size_t blockid,tObjType objtype,const R::RString& name,tObjState state);

protected:

	/**
	 * Load the vectors from the disk.
	 * @return true if the vectors was loaded.
	 */
	virtual bool LoadVectors(void);

public:

	/**
	 * Defines if the objects have a description.
	 * @return true.
	 */
	static inline bool HasDesc(void) {return(true);}

	/**
    * Verify if the object has a description in memory or on the disk.
	 * @return true if the object is defined.
    */
	inline bool IsDefined(void) const
   {
       // Verify first in memory
       if(GDescription::IsDefined())
           return(true);

       // If not in memory -> on the disk ?
       return(BlockId);
   }

	/**
	* @return the state of the object.
	*/
	inline tObjState GetState(void) const {return(State);}

	/**
	* Set the state of the object. If the object must be delete, its vector is
	* loaded to delete the references.
	* @param state           New state.
	*/
	void SetState(tObjState state);

	/**
	* Get the identifier of the document.
	*/
	inline size_t GetId(void) const {return(Id);}

	/**
	* Set the identifier of the document.
	* @param id              Identifier.
	*/
	void SetId(size_t id);

	/**
	 * Get the identifier of the block containing the description. If null, it
	 * means that the object is not defined.
	 * @return Identifier of the block.
	 */
	inline size_t GetBlockId(void) const {return(BlockId);}

	/**
	* Copy a list of object references to the object for a given concept.
	* @param concept         Concept
	* @param list             List of concept references.
	*/
	void Copy(GConcept* concept,const R::RContainer<GConceptRef,false,true>& list);

	/**
	 * Clear the vectors corresponding to the object. The corresponding file is
	 * not removed from the disk.
	 * @param disk            Clear the reference to the corresponding block on
	 *                        disk.
	 */
	void Clear(bool disk=false);

	/**
	 * Save the description of the object.
	 */
	void SaveDesc(void);

	/**
	 * Destruct the object.
	 */
	virtual ~GDescriptionObject(void);

	friend class GObjects<C>;
	friend class GSession;
	friend class GDocAnalyze;
};


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
