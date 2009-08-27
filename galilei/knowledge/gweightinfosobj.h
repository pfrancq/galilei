/*

	GALILEI Research Project

	GWeightInfosObj.h

	Object represented by a list of weighted information entities - Header.

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
#ifndef GWeightInfosObjH
#define GWeightInfosObjH


//------------------------------------------------------------------------------
// include files for GALILEI
#include <gweightinfos.h>
#include <gobject.h>


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
 * The GWeightInfosObj provides a generic class for a object of a given type
 * with a specific identifier that is represented by a vector of information
 * entities.
 * @param cObj               Destination class.
 * @param type               Type of the object.
 * @author Pascal Francq
 * @short Generic Vector Object
 */
class GWeightInfosObj : public GObject
{
protected:

	/**
	* State of the object.
	*/
	tObjState State;

private:

	/**
	 * Vector of the object. It is not allocated until GetVector() is called.
	 */
	GWeightInfos* Vector;

	/**
	 * Identifier of the block containing the description of the object. If
	 * null, the object has no description.
	 */
	size_t BlockId;

public:

	/**
	 * Construct a object.
	 * @param id             Identifier of the object.
	 * @param blockid        Identifier of the block.
	 * @param objtype        ObjType.
	 * @param name           Name of the object.
	 * @param state          State of the object.
	 */
	GWeightInfosObj(size_t id,size_t blockid,tObjType objtype,const R::RString& name,tObjState state);

	/**
	* @return True if the object is defined.
	*/
	bool IsDefined(void) const {return(BlockId);}

	/**
	 * @return Get the vector associated to the object. It is loaded if
	 * necessary.
	 */
	const GWeightInfos& GetVector(void) const;

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

protected:

	/**
	* Copy the information to the object.
	* @param infos           Source list of weighted information entities.
	*/
	void CopyInfos(const R::RContainer<GWeightInfo,false,true>& infos);

	/**
	 * Clear the vector corresponding to the object. The corresponding file is
	 * not removed from the disk.
	 * @param disk            Clear the reference to the corresponding block on
	 *                        disk.
	 */
	void Clear(bool disk=false);

	/**
	* Add the references for the information entities of the object type in a
	* information entity space (language). This information is used for the
	* inverse frequency factors.
	* @param ObjType        Type of the reference.
	*/
	inline void AddRefs(tObjType ObjType) const {GetVector().AddRefs(ObjType);}

	/**
	* Delete the references for the information entities of the object type in a
	* information entity space (language). This information is used for the
	* inverse frequency factors.
	* @param ObjType        Type of the reference.
	*/
	inline void DelRefs(tObjType ObjType) const {GetVector().DelRefs(ObjType);}

	/**
	 * Transfer the contain of a vector to the current object.
	 * @param info           Original vector.
	 */
	void Transfer(GWeightInfos& info);

public:

	/**
	 * Destruct the object.
	 */
	virtual ~GWeightInfosObj(void);

	friend class GSession;
	friend class GIndexer;
	friend class GSimulator;
};


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
