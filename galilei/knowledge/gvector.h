/*

	GALILEI Research Project

	GVector.h

	Concept Reference Vector - Header.

	Copyright 2002-2011 by Pascal Francq (pascal@francq.info).
	Copyright 2002-2008 by the Université Libre de Bruxelles (ULB).

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
#ifndef GVectorH
#define GVectorH


//------------------------------------------------------------------------------
// include files for R
#include <rsparsevector.h>


//------------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>
#include <gconceptref.h>


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* The GVector provides a representation for a (sparse) vector of concept
* references associated with a given concept. Each element of the vector is
* represented by a GConceptRef class. The vector can also be seen as a set of
* sub-vectors, where each sub-vector corresponds to one concepts space.
*
* In practice, several vectors are used to describe the object (document,
* profile, community or class) in the extended vector Model.
* @author Pascal Francq
* @short Concept Reference Vector.
*/
class GVector : private R::RContainer<GConceptRef,true,true>
{
	/**
	 * Concept associated to the vector.
	 */
	GConcept* Concept;

   /**
    * Weight of a vector.
    */
   double Weight;

	/**
	 * The maximal weight of each sub-vector.
	 */
	double MaxWeight;

	/**
	 * The maximal absolute weight of each sub-vector.
	 */
	double MaxAbsWeight;

public:

	/**
	* Constructor of a vector.
	* @param concept         Concept associated with the vector.
   * @param weight          Weight associated with the vector.
	*/
	GVector(GConcept* concept,double weight=1);

	/**
	* Constructor of a vector.
	* @param concept         Concept associated with the vector.
	* @param max             Initial number of maximal references.
   * @param weight          Weight associated with the vector.
	*/
	GVector(GConcept* concept,size_t max,double weight=1);

	/**
	* Copy constructor of a vector.
	* @param vector          Vector.
	*/
	GVector(const GVector& vector);

   /**
    * @return the weight associated with the vector.
    */
   double GetWeight(void) const {return(Weight);}

	/**
	* Static function used to order the information entities by weights.
	* @param a              Pointer to the first object.
	* @param b              Pointer to the second object.
	*/
	static int SortOrder(const void* a,const void* b);

protected:

	/**
	* Assignment operator for vectors.
	* @param vector          Source vector.
   * @param weight          Weight associated with the vector.
	*/
	void Copy(const R::RContainer<GConceptRef,false,true>& vector,double weight=1);

public:

	/**
	* Compare two vectors related to their concept.
	* @see R::RContainer
	* @param vector          Vector.
	* @return int
	*/
	int Compare(const GVector& vector) const;

	/**
	* Compare a vector and a concept.
	* @see R::RContainer
	* @param concept         Concept.
	* @return int
	*/
	int Compare(const GConcept& concept) const;

	/**
	* Compare a vector and a concept.
	* @see R::RContainer
	* @param concept         Concept.
	* @return int
	*/
	int Compare(const GConcept* concept) const;

	/**
	* Get a cursor on the concept references.
	* @return a cursor of GConceptRef.
	*/
	inline R::RCursor<GConceptRef> GetRefs(void) const {return(R::RCursor<GConceptRef>(*this));}

	/**
	 * Get the concept associated with the vector.
	 * @return a pointer to a GConcept.
	 */
	inline GConcept* GetConcept(void) const {return(Concept);}

   /**
	 * @return the concept identifier associated with the vector.
	 */
	inline size_t GetConceptId(void) const {return(Concept->GetId());}

	/**
	 * Get a concept reference based on a given one. If it is not found, it is
	 * created with a null weight.
	 * @param ref            Concept reference.
	 * @return a pointer to a GConceptRef.
	 */
	GConceptRef* GetRef(const GConceptRef* ref);

	/**
	 * Get a pointer to a concept reference based on a given concept. If it is
	 * not found, it is created with a null weight.
	 * @param concept        Concept.
	 * @return a pointer to a GConceptRef.
	 */
	GConceptRef* GetRef(GConcept* concept);

	/**
	* Insert a concept reference.
	* @param ref             Concept reference.
	*/
	void InsertRef(GConceptRef* ref);

	/**
	* Delete a concept reference.
	* @param ref             Concept reference.
	*/
	void DeleteRef(GConceptRef* ref);

	/**
	 * Delete an reference to a given concept.
	 * @param concept        Concept.
	 */
	void DeleteRef(const GConcept* concept);

	/**
	 * Verify if a given concept is in the vector.
	 * @param concept        Concept.
	 */
	bool IsIn(const GConcept* concept) const {return(R::RContainer<GConceptRef,true,true>::IsIn(concept));}

	/**
	 * Fill an array with the concept references.
	 * @param tab            Array to fill.
	 * @return the number of elements copied.
	 */
	size_t GetTab(const GConceptRef** tab) const {return(R::RContainer<GConceptRef,true,true>::GetTab(tab));}

	/**
	* Clear the vector and extend it if necessary.
	* @param size            Expected size of the vector.
	*/
	void Clear(size_t size=0) {R::RContainer<GConceptRef,true,true>::Clear(size);}

	 /**
 	* Get the number of elements in the container.
 	* @return size_t.
 	*/
 	size_t GetNb(void) const {return(R::RContainer<GConceptRef,true,true>::GetNb());}

	/**
	 * Verifies if the vector is defined, i.e. if it has at least one element.
	 * @return true if the vector is defined.
	 */
	inline bool IsDefined(void) const {return(GetNb());}

private:

	/**
	 * Analyze the weights to compute the maximum weights for the different
	 * types.
	 */
	inline void AnalyzeWeights(void) const;

public:

	/**
	* Compute the maximal weight of the concept references in the vector.
	* @return double.
	*/
	double GetMaxWeight(void) const;

	/**
	* Compute the maximal absolute weight of the concept references in the
   * vector.
	* @return double.
	*/
	double GetMaxAbsWeight(void) const;

	/**
	 * Extract all concepts related to a given type an put them into a given
	 * vector that will be emptied.
	 * @param vector         Destination vectors.
	 * @param type           Type selected.
	 */
	void Extract(GVector& vector,const GConceptType* type);

	/**
	* Compute the Tf*Idf weights for all the elements of the vector. In
	* practice, the classical formula is applied to each sub-vector.
	* @param ObjType         Type of the reference (documents, profiles, etc.).
	*/
	void ComputeTfIdf(tObjType ObjType);

	/**
	 * Print the vector to the standard output.
	 * @param msg            Header message.
	 */
	void Print(R::RString msg);

	/**
	 * Build the intersection two vectors. In practice, the weight of the
	 * concept references in both vectors are added, the rest is deleted.
	 * @param vector         Vector to search for the intersection.
	 */
	void Intersection(const GVector& vector);

	/**
	 * Addition operator for the vectors.
	 * @param vector         Vector to add.
	 */
	GVector& operator+=(const GVector& vector);

	/**
	 * Subtraction operator for the vectors.
	 * @param vector         Vector to subtract.
	 */
	GVector& operator-=(const GVector& vector);

	/**
	 * Multiplication operator for the vectors.
	 * @param nb             Number to multiply by.
	 */
	GVector& operator*=(const double nb);

	/**
	 * Division operator for the vectors.
	 * @param nb             Number to multiply by.
	 */
	GVector& operator/=(const double nb);

	/**
	* Destruct the vector.
	*/
	virtual ~GVector(void);

	friend class GDescription;
	friend class GDescriptionObject<GDoc>;
	friend class GDescriptionObject<GClass>;
	friend class GDescriptionObject<GTopic>;
	friend class GDescriptionObject<GUser>;
	friend class GDescriptionObject<GProfile>;
	friend class GDescriptionObject<GCommunity>;
	friend class GDoc;
	friend class GObjects<GDoc>;
	friend class GObjects<GClass>;
	friend class GObjects<GTopic>;
	friend class GObjects<GUser>;
	friend class GObjects<GProfile>;
	friend class GObjects<GCommunity>;
	friend class GSession;
};


//------------------------------------------------------------------------------
// General methods

/**
 * Addition operation for two vectors.
 * @param arg1               First vector.
 * @param arg2               Second vector.
 */
GVector operator+(const GVector &arg1,const GVector &arg2);

/**
 * Subtraction operation for two vectors.
 * @param arg1               First vector.
 * @param arg2               Second vector.
 */
GVector operator-(const GVector &arg1,const GVector &arg2);

/**
 * Multiplication operation for a vector and a number.
 * @param arg1               Number.
 * @param arg2               Vector.
 */
GVector operator*(const double arg1,const GVector &arg2);

/**
 * Multiplication operation for a vector and a number.
 * @param arg1               Vector.
 * @param arg2               Number.
 */
GVector operator*(const GVector& arg1,const double arg2);

/**
 * Division operation for a vector and a number.
 * @param arg1               Vector.
 * @param arg2               Number.
 */
GVector operator/(const GVector& arg1,const double arg2);


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
