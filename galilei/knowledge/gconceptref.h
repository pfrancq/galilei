/*

	GALILEI Research Project

	GConceptRef.h

	Concept Reference - Header.

	Copyright 2002-2014 by Pascal Francq (pascal@francq.info).
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
#ifndef GConceptRefH
#define GConceptRefH


//------------------------------------------------------------------------------
// include files for GALILEI
#include <gconcepttype.h>
#include <gconcept.h>


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* The GConceptRef class provides a representation for reference of a given
* concept in a vector. In practice, it is composed from a concept and an
* associated weight.
* @author Pascal Francq
* @short Concept Reference.
*/
class GConceptRef
{
protected:

	/**
	 * Concept.
	 */
	GConcept* Concept;

	/**
	* Weight associated with the concept.
	*/
	double Weight;

public:

	/**
	* Constructor of a concept reference.
	* @param concept         Concept.
	*/
	GConceptRef(GConcept* concept);

	/**
	* Constructor of a concept reference.
	* @param concept         Concept.
	* @param w               Weight associated with the reference.
	*/
	GConceptRef(GConcept* concept,double w);

	/**
	* Copy constructor of a concept reference.
	* @param ref             Concept reference.
	*/
	GConceptRef(const GConceptRef& ref);

	/**
	* Compare two concept references related to their concept.
	* @see R::RContainer
	* @param ref             Concept reference.
	* @return int
	*/
	int Compare(const GConceptRef& ref) const;

	/**
	* Compare a concept reference and a concept.
	* @see R::RContainer
	* @param concept         Concept.
	* @return int
	*/
	int Compare(const GConcept& concept) const;

	/**
	* Compare a concept reference and a concept.
	* @see R::RContainer
	* @param concept         Concept.
	* @return int
	*/
	int Compare(const GConcept* concept) const;

	/**
	* Get the identifier of the concept.
	*/
	inline size_t GetId(void) const {return(Concept->GetId());}

	/**
	* Get the concept.
	*/
	inline GConcept* GetConcept(void) const {return(Concept);}

	/**
	* Get the type of the concept.
	*/
	inline GConceptType* GetType(void) const {return(Concept->GetType());}

	/**
	* Get the identifier of the type of the concept.
	*/
	inline size_t GetTypeId(void) const {return(Concept->GetType()->GetId());}

	/**
	* Get the weight of the concept reference.
	* @return Weight as double.
	*/
	double GetWeight(void) const {return(Weight);}

	/**
	* Set the weight of the concept reference.
	* @param w              Weight.
	*/
	void SetWeight(double w);

	/**
	* Assignment operator for concept references.
	* @param ref             Concept reference.
	*/
	GConceptRef& operator=(const GConceptRef& ref);

	/**
	* Add a weight to the current one.
	* @param w              Weight to add.
	*/
	GConceptRef& operator+=(double w);

	/**
	* Subtract a weight to the current one.
	* @param w              Weight to subtract.
	*/
	GConceptRef& operator-=(double w);

	/**
	* Multiply a weight to the current one.
	* @param w              Weight to multiply.
	*/
	GConceptRef& operator*=(double w);

	/**
	* Divide a weight to the current one.
	* @param w              Weight to divide.
	*/
	GConceptRef& operator/=(double w);

	/**
	* Compute the transformed weight of a given concept reference using a
	* inverse frequency factor of the object type (idf,isf or ivf).
	* @param ObjType        Type of the object.
	* @param max            Maximal weight of the corresponding vector.
	* @return a query weight computed for the concept reference.
	*/
	double GetQueryWeight(tObjType ObjType,double max) const;

	/**
	* Destructor of a concept reference.
	*/
	virtual ~GConceptRef(void);

	friend class GSession;
	friend class GVector;
};


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
