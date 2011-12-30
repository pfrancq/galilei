/*

	GALILEI Research Project

	GDescription.h

	Description - Header.

	Copyright 2009-2011 by Pascal Francq (pascal@francq.info).

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
#ifndef GDescriptionH
#define GDescriptionH


//------------------------------------------------------------------------------
// include files for GALILEI
#include <gvector.h>
#include <gobject.h>


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
 * The GDescription provides a generic class for a description, i.e. a set of
 * vectors.
 * @author Pascal Francq
 * @short Description.
 */
class GDescription
{
protected:

	/**
	 * Vectors.
	 */
	R::RContainer<GVector,true,true>* Vectors;

public:

	/**
	 * Construct a description.
	 */
	GDescription(void);

   /**
	 * Copy constructor.
    * @param src            Source description.
	 */
	GDescription(const GDescription& src);

protected:

	/**
	 * Load the vectors from the disk. By default, this method does nothing. It
	 * must be overloaded by objects that are represented with a description.
	 * @return true if the vectors was loaded.
	 */
	virtual bool LoadVectors(void);

public:

    /**
     * Compare method used by R::RContainer.
     * @param desc          Description to compare with.
     * @return always -1.
     */
    int Compare(const GDescription&) const {return(-1);}

	/**
	 * Get the vectors associated to the object. The whole description is loaded
	 * if necessary.
	 * @return a cursor of GVector.
	 */
	R::RCursor<GVector> GetVectors(void) const;

	/**
	 * Get the vector associated to the object and associated to a particular
	 * concept. The whole description is loaded if necessary.
	 * @param concept        Concept.
	 * @return a pointer to a GVector (may be null if the concept is not found).
	 */
	const GVector* GetVector(GConcept* concept) const;

	/**
	 * Get the vector associated to the object and associated to a particular
	 * concept. The whole description is loaded if necessary.
	 * @param concept        Concept.
	 * @return a pointer to a GVector (may be null if the concept is not found).
	 */
	GVector* GetVector(GConcept* concept);

	/**
	 * Get the vector associated to the object and associated to a particular
	 * concept. When not existing, the vector is created. The whole description
	 * is loaded if necessary.
	 * @param concept        Concept.
	 * @return a pointer to a GVector.
	 */
	GVector* GetInsertVector(GConcept* concept);

   /**
    * Look if the description is defined : at least one of the vectors must be
    * defined.
    * @return true if defined.
    */
   bool IsDefined(void) const;

  	/**
	 * Clear the vectors corresponding to the object.
	 */
	void Clear(void);

protected:

	/**
	* Copy a list of object references to the object for a given concept.
	* @param concept         Concept
	* @param list             List of concept references.
	*/
	void Copy(GConcept* concept,const R::RContainer<GConceptRef,false,true>& list);

	/**
	* Add the references for the concepts for a given object type. This
	* information is used for the inverse frequency factors.
	* @param session         Session.
	* @param ObjType         Object type.
	*/
	void AddRefs(GSession* session,tObjType ObjType) const;

	/**
	* Delete the references for the concepts for a given object type. This
	* information is used for the inverse frequency factors.
	* @param session         Session.
	* @param ObjType         Object type.
	*/
	void DelRefs(GSession* session,tObjType ObjType) const;

public:

    /**
     * Normalize the vectors.
     */
    void Normalize(void);

   /**
    * Multiply each weight by its inverse frequency factor based on a given set.
    * @param set            Document Set.
    */
    void MultiplyIF(GDescriptionSet& set);

   /**
	 * Assignment operator for the descriptions.
	 * @param desc           Description to assign.
	 */
	GDescription& operator=(const GDescription& desc);

	/**
	 * Addition operator for the descriptions.
	 * @param desc           Description to add.
	 */
	GDescription& operator+=(const GDescription& desc);

	/**
	 * Subtraction operator for the descriptions.
	 * @param desc           Description to subtract.
	 */
	GDescription& operator-=(const GDescription& desc);

	/**
	 * Multiplication operator for the descriptions.
	 * @param nb             Number to multiply by.
	 */
	GDescription& operator*=(const double nb);

	/**
	 * Division operator for the descriptions.
	 * @param nb             Number to multiply by.
	 */
	GDescription& operator/=(const double nb);

	/**
	 * Destruct the object.
	 */
	virtual ~GDescription(void);
};


//------------------------------------------------------------------------------
// General methods

/**
 * Addition operation for two descriptions.
 * @param arg1               First description.
 * @param arg2               Second description.
 */
GDescription operator+(const GDescription &arg1,const GDescription &arg2);

/**
 * Subtraction operation for two descriptions.
 * @param arg1               First description.
 * @param arg2               Second description.
 */
GDescription operator-(const GDescription &arg1,const GDescription &arg2);

/**
 * Multiplication operation for a description and a number.
 * @param arg1               Number.
 * @param arg2               description.
 */
GDescription operator*(const double arg1,const GDescription &arg2);

/**
 * Multiplication operation for a description and a number.
 * @param arg1               description.
 * @param arg2               Number.
 */
GDescription operator*(const GDescription& arg1,const double arg2);

/**
 * Division operation for a description and a number.
 * @param arg1               description.
 * @param arg2               Number.
 */
GDescription operator/(const GDescription& arg1,const double arg2);


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
