/*

	GALILEI Research Project

	GDescriptionObject.h

	Object represented by a list of weighted information entities - Header.

	Copyright 2009-2015 by Pascal Francq (pascal@francq.info).

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
 *
 * Each object can be either selected (the default) or unselected. When
 * unselected, the object is not taken into account for computation. For
 * example, the document clustering treats only the selected documents.
 * @tparam C                 Class of the object.
 * @author Pascal Francq
 * @short Generic Vector Object
 */
template<class C>
	class GDescriptionObject : public GObject, private GDescription
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
	 */
	GDescriptionObject(GSession* session,size_t id,size_t blockid,tObjType objtype,const R::RString& name);

protected:

	/**
	 * Load the vectors from the disk.
	 * @return true if the vectors was loaded.
	 */
	virtual bool LoadVectors(void);

public:

	/**
	 * Defines if the objects have descriptions.
	 * @return true.
	 */
	static inline bool HasDesc(void) {return(true);}

	 /**
	  * Release the vectors from memory.
	*/
	 void ReleaseVectors(void);

	/**
	* @return a const pointer to the description.
	*/
	inline const GDescription& operator()(void) const
	{
		if(!Vectors)
		{
			if(!const_cast<GDescriptionObject<C>*>(this)->LoadVectors())
				const_cast<GDescriptionObject<C>*>(this)->Vectors=new R::RContainer<GVector,true,true>(5,20);
		}
		return(*this);
	}

	/**
	 * Get the vectors associated to the object. The whole description is loaded
	 * if necessary.
	 * @return a cursor of GVector.
	 */
	inline R::RConstCursor<GVector> GetVectors(void) const {return(GDescription::GetVectors());}

	/**
	 * Get the vector associated to a meta-concept. The whole description is
	 * loaded if necessary.
	 * @param metaconcept    Meta-concept.
	 * @return a pointer to a GVector (may be null if the meta-concept isn't
	 * found).
	 */
	inline const GVector* GetVector(GConcept* metaconcept) const {return(GDescription::GetVector(metaconcept));}

	/**
	 * Verify if a given concept is in one of the vectors.
	 * @param concept        Concept.
	 * @returns true if the concept is contained.
	 */
	inline bool IsIn(const GConcept* concept) const {return(GDescription::IsIn(concept));}

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
	* Get the identifier of the document.
	*/
	inline size_t GetId(void) const {return(Id);}

	/**
	 * Get the identifier of the block containing the description. If null, it
	 * means that the object is not defined.
	 * @return Identifier of the block.
	 */
	inline size_t GetBlockId(void) const {return(BlockId);}

private:

	/**
	 * Clear the vectors corresponding to the object. The corresponding file is
	 * not removed from the disk.
	 * @param disk            Clear the reference to the corresponding block on
	 *                        disk.
	 */
	void Clear(bool disk=false);

public:

	/**
	 * Destruct the object.
	 */
	virtual ~GDescriptionObject(void);

	friend class GDoc;
	friend class GObjects<GDoc,hDocs>;
	friend class GClass;
	friend class GObjects<GClass,hClasses>;
	friend class GGroup<GDoc,GTopic,otTopic>;
	friend class GTopic;
	friend class GObjects<GTopic,hTopics>;
	friend class GProfile;
	friend class GObjects<GProfile,hProfiles>;
	friend class GGroup<GProfile,GCommunity,otCommunity>;
	friend class GCommunity;
	friend class GObjects<GCommunity,hCommunities>;
	friend class GSession;
	friend class GDocAnalyze;
};


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
