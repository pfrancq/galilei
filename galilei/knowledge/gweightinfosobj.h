/*

	GALILEI Research Project

	GWeightInfosObj.h

	Object represented by a list of weighted information entities - Header.

	Copyright 2008-2009 by Pascal Francq (pascal@francq.info).

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
#include <gsession.h>


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
template<class cObj,tObjType type>
	class GWeightInfosObj
{
protected:

	/**
	 * Identifier of the object.
	 */
	size_t Id;

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
	 * Size of the vector. This information must be store with the object
	 * information. It is used to known if the object is defined (Size>0) or
	 * not (Size=0).
	 */
	size_t Size;

public:

	/**
	 * Construct a object.
	 * @param id             Identifier.
	 * @param size           Size of the vector.
	 * @param state          State of the object.
	 */
	GWeightInfosObj(size_t id,size_t size,tObjState state);

	/**
	 * @return The size of the vector the last time the object was computed
	 */
	size_t GetSize(void) const {return(Size);}

	/**
	* @return True if the object is defined.
	*/
	bool IsDefined(void) const {return(Size);}

	/**
	 * @return Get the vector associated to the object. It is loaded if
	 * necessary.
	 */
	const GWeightInfos* GetVector(void) const;

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
	size_t GetId(void) const {return(Id);}

	/**
	* Set the identifier of the document.
	* @param id              Identifier.
	*/
	void SetId(size_t id);

protected:

	/**
	* Copy the information to the object.
	* @param infos           Source list of weighted information entities.
	*/
	void CopyInfos(const R::RContainer<GWeightInfo,false,true>& infos);

	/**
	 * Clear the vector corresponding to the object. The corresponding file is
	 * not removed from the disk.
	 */
	void Clear(void);

	/**
	 * Set the new size of the object.
	 * @param size
	 */
	inline void SetSize(size_t size);

	/**
	* Add the references for the information entities of the object type in a
	* information entity space (language). This information is used for the
	* inverse frequency factors.
	* @param ObjType        Type of the reference.
	*/
	inline void AddRefs(tObjType ObjType) const;

	/**
	* Delete the references for the information entities of the object type in a
	* information entity space (language). This information is used for the
	* inverse frequency factors.
	* @param ObjType        Type of the reference.
	*/
	inline void DelRefs(tObjType ObjType) const;

	/**
	 * Transfer the contain of a vector to the current object.
	 * @param info           Original vector.
	 */
	inline void Transfer(GWeightInfos& info);

public:

	/**
	 * Destruct the object.
	 */
	virtual ~GWeightInfosObj(void);
};


//-----------------------------------------------------------------------------
// Template implementation
#include <gweightinfosobj.hh>


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
