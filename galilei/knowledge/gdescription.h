/*

	GALILEI Research Project

	GDescription.h

	Description - Header.

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
#ifndef GDescriptionH
#define GDescriptionH


//------------------------------------------------------------------------------
// include files for GALILEI
#include <gvector.h>


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
 * The GDescription provides a representation for a tensor in the
 * [article Tensor_Space_Model tensor space model]. In practice, it is
 * implemented as a set of [code GALILEI_GVector vectors]. Such a tensor is
 * used for computations or to describe objects.
 *
 * The GDescriptionObject class provides a generic class for an object described
 * as a tensor.
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
	  * Release the vectors from memory.
     */
	 void ReleaseVectors(void);

	/**
	 * Get the vectors associated to the object. The whole description is loaded
	 * if necessary.
	 * @return a cursor of GVector.
	 */
	R::RConstCursor<GVector> GetVectors(void) const;

	/**
	 * Get the vectors associated to the object. The whole description is loaded
	 * if necessary.
	 * @return a cursor of GVector.
	 */
	R::RCursor<GVector> GetVectors(void);

	/**
	 * Get the vector associated to a meta-concept. The whole description is
	 * loaded if necessary.
	 * @param metaconcept    Meta-concept.
	 * @return a pointer to a GVector (may be null if the meta-concept isn't
	 * found).
	 */
	const GVector* GetVector(GConcept* metaconcept) const;

	/**
	 * Get the vector associated to a meta-concept. The whole description is
	 * loaded if necessary.
	 * @param metaconcept    Meta-concept.
	 * @return a pointer to a GVector (may be null if the meta-concept isn't
	 * found).
	 */
	GVector* GetVector(GConcept* metaconcept);

	/**
	 * Get the vector associated to meta-concept. When not existing, the vector
	 * is created. The whole description is loaded if necessary.
	 * @param metaconcept    Meta-concept.
	 * @return a pointer to a GVector.
	 */
	GVector* GetInsertVector(GConcept* metaconcept);

	/**
	 * Delete a vector associated to a meta-concept.
	 * \warning If it is called for a class inheriting from GDescriptionObject,
	 * the references for that given object type are not deleted. The method
	 * should therefore never be called in that context.
    * @param metaconcept    Meta-concept.
    */
	void DeleteVector(GConcept* metaconcept);

   /**
    * Look if the description is defined : at least one of the vectors must be
    * defined.
    * @return true if defined.
    */
   bool IsDefined(void) const;

	/**
	 * Verify if a given concept is in one of the vectors.
	 * @param concept        Concept.
	 * @returns true if the concept is contained.
	 */
	bool IsIn(const GConcept* concept) const;

  	/**
	 * Clear the vectors corresponding to the object.
	 */
	void Clear(void);

protected:

	/**
	* Copy a list of object references to the object for a given meta-concept.
	* @param metaconcept    Meta-concept.
	* @param list             List of concept references.
	*/
	void Copy(GConcept* metaconcept,const R::RContainer<GConceptRef,false,true>& list);

public:

	/**
	* Normalize the vectors.
	*/
	void Normalize(void);

	/**
	 * Print the content of the description.
	 * @tparam S             Stream class that implements the << operator.
	 * @param stream         Stream.
	 * @param name           Name of the vector.
	 * @param format         Format used to print the number. By default, it is "%E".
	 * @param colsize        Size of a column. By default, it is 12.
	 */
	template<class S> void Print(S& stream,const R::RString& name,const char* format="%E",size_t colsize=12) const
	{
		R::RString Spaces;
		for(size_t i=0;i<name.GetLen()+1;i++)
			Spaces+=" ";
		size_t NamePos(Vectors->GetNb()/2);
		size_t i;
		R::RConstCursor<GVector> Vector(GetVectors());
		for(Vector.Start(),i=0;!Vector.End();Vector.Next(),i++)
		{
			if(NamePos==i)
				stream<<name<<"=";
			else
				stream<<Spaces;
			Vector()->Print(stream,"",format,colsize);
		}
	}

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

	friend class GDoc;
	friend class GClass;
	friend class GTopic;
	friend class GProfile;
	friend class GCommunity;
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
