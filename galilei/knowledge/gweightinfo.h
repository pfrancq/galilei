/*

	GALILEI Research Project

	GWeightInfo.h

	Weighted information entity - Header.

	Copyright 2002-2009 by Pascal Francq (pascal@francq.info).
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
#ifndef GWeightInfoH
#define GWeightInfoH


//------------------------------------------------------------------------------
// include files for GALILEI
#include <gconcept.h>
#include <gconcepttype.h>


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* The GWeightInfo class provides a representation for an information entity
* associated with a weight.
* @author Pascal Francq
* @short Weighted Information Entity.
*/
class GWeightInfo
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
	* Constructor of a weighed information entity.
	* @param concept         Identifier of the information entity.
	*/
	GWeightInfo(GConcept* concept);

	/**
	* Constructor of a weighed information entity.
	* @param concept         Identifier of the information entity.
	* @param w               Weight of the information entity.
	*/
	GWeightInfo(GConcept* concept,double w);

	/**
	* Copy constructor of a weighed information entity.
	* @param w              Pointer to a weighted information entity.
	*/
	GWeightInfo(const GWeightInfo& w);

	/**
	* Compare two weighted information entities by comparing their
	* identifier and type.
	* @see R::RContainer
	* @param calc           Weighted information entity.
	* @return int
	*/
	int Compare(const GWeightInfo& calc) const;

	/**
	* Compare a weighted information entity and a concept by comparing their
	* identifier and type.
	* @see R::RContainer
	* @param concept         Concept.
	* @return int
	*/
	int Compare(const GConcept& concept) const;

	/**
	* Compare a weighted information entity and a concept by comparing their
	* identifier and type.
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
	* Get the weight of the word.
	* @return Weight as double.
	*/
	double GetWeight(void) const {return(Weight);}

	/**
	* Set the weight of the word.
	* @param w              Weight.
	*/
	void SetWeight(double w);

	/**
	* Assignment operator for weighted information entities.
	* @param i               Weighted information entity.
	*/
	GWeightInfo& operator=(const GWeightInfo& i);

	/**
	* Add a weight to the current one.
	* @param w              Weight to add.
	*/
	GWeightInfo& operator+=(double w);

	/**
	* Subtract a weight to the current one.
	* @param w              Weight to subtract.
	*/
	GWeightInfo& operator-=(double w);

	/**
	* Multiply a weight to the current one.
	* @param w              Weight to multiply.
	*/
	GWeightInfo& operator*=(double w);

	/**
	* Divide a weight to the current one.
	* @param w              Weight to divide.
	*/
	GWeightInfo& operator/=(double w);

	/**
	* Compute the transformed weight of a given information entity using a
	* inverse frequency factor of the object type (idf,isf or ivf). The inverse
	* Frequency of an information entity is linked to a specific language.
	* @param ObjType        Type of the object.
	* @param max            Maximal value of the vector containing the word.
	* @return double.
	*/
	double GetQueryWeight(tObjType ObjType,double max) const;

	/**
	* Destructor of a weighted information entity.
	*/
	virtual ~GWeightInfo(void);

	friend class GIndexer;
};


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
