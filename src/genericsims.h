/*

	GALILEI Research Project

	GenericSims.h

	Generic Tensor Space Model Similarity Measure - Header.

	Copyright 2003-2012 by Pascal Francq (pascal@francq.info).
	Copyright 2003 by Valery Vandaele.
	Copyright 2003-2007 by the Université Libre de Bruxelles (ULB).

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
#ifndef GGenericSimsH
#define GGenericSimsH


//------------------------------------------------------------------------------
// include files for ANSI C/C++
#include <math.h>
using namespace std;


//------------------------------------------------------------------------------
// include files for R Project
#include <rtextfile.h>
using namespace R;


//------------------------------------------------------------------------------
// include files for GALILEI
#include <gmatrixmeasure.h>
#include <gconcepttype.h>
#include <gconceptcat.h>
#include <gconceptref.h>
#include <gdescription.h>
#include <glang.h>
#include <gconcept.h>
#include <gconceptref.h>
#include <gprofile.h>
#include <gdoc.h>
#include <gtopic.h>
#include <gcommunity.h>
#include <gsession.h>
using namespace GALILEI;


//------------------------------------------------------------------------------
/**
 * The template class GGenericSims provides a generic implementation of the
 * tensor space model similarity. It can use to compute the similarity between
 * same type of objects (for example documents) or objects of different types
 * (such as a profile and a community of interests).
 * @tparam cObj1            First object class.
 * @tparam cObj1            Second object class.
 * @author Pascal Francq
 * @short Tensor Space Model Similarity
 */
template<class cObj1,class cObj2>
   class GGenericSims : public GMatrixMeasure
{
protected:

   /**
    * Type of the aggregating function used to combine the category
    * similarities.
    */
	enum tSimType
	{
		Undefined            /** Unknown method.*/,
		TextOnly             /** Based only on the text.*/,
		Product              /** Product of the different similarities.*/,
		Sum                  /** Weighted sum of the different similarities.*/,
		Choquet              /** Use the integral of Choquet.*/
	};

   /**
    * Concept categories taken into account to compute the object similarity.
    */
   enum tCat
   {
       Textual             /** Textual content.*/,
       Metadata            /** Metadata content.*/,
       Semantic            /** Semantic rules.*/,
       NbCats              /** Number of different content categories.*/
   };

   /**
    * The Cat structure represents all the information related to a particlar
    * concept category.
    */
   struct Cat
   {
       double Capacity;    // The capacity of the integral of Choquet.
       GConceptCat* Cat;   // The concept category.
       double Sim;         // The similarity for that category.
       double NbConcepts;  // Number of common concepts of two objects in the category.
   };

	/**
	 * Type of similarity:
	 */
	tSimType SimType;

   /**
    * The different categories.
    */
   Cat Cats[NbCats];

	/**
	 * Factor used for the computation of the similarity if the product is the
    * aggregating function.
	 */
	double ProductFactor;

	/**
	 * Capacity of the combination of the textual and semantic criteria
	 * (Choquet).
	 */
	double TextualSemanticCapacity;

	/**
	 * Capacity of the combination of the textual and meta criteria
	 * (Choquet)
	 */
	double TextualMetadataCapacity;

	/**
	 * Capacity of the combination of the semantic and metadata criteria
	 * (Choquet)
	 */
	double SemanticMetadataCapacity;

	/**
	 * First object.
	 */
	cObj1* Desc1;

	/**
	 * Second object.
	 */
	cObj2* Desc2;

public:

	/**
	* Constructor of the similarities between two elements of the same type.
	* @param fac             Factory of the plug-in.
	* @param lines           Type of the elements in the lines.
	* @param cols            Type of the elements in the columns.
	*/
	GGenericSims(GSession* session,GPlugInFactory* fac,tObjType lines,tObjType cols);

	/**
	 * Virtual method inherits from R::RObject and that must be re-implemented
	 * in all child classes.
	 * @return Name of the class.
	 */
	virtual R::RCString GetClassName(void) const {return("GGenericSims");}

	/**
	 * Get the name of the files that will be used. By default, it is the name
	 * of the plug-in.
	 */
	virtual R::RString GetFilesName(void) const;

	/**
	* Configurations were applied from the factory.
	*/
	virtual void ApplyConfig(void);

	/**
	 * Initialize the measure.
	 */
	virtual void Init(void);

	/**
	 * @return the inverse factor of a given concept. If the elements in the
	 * line and in the columns are different, the sum of both types are
	 * computed.
	 * @param concept        Concept.
	 */
	double GetIF(GConcept* concept);

   /**
    * Compute the "metadata" similarity between two vectors. It adds to
    * Cats[Metadata] the weighted similarity and the number of common concept.
    * @param vec1           First vector.
    * @param vec2           Second vector.
    */
   void ComputeMetaSim(GVector* vec1,GVector* vec2);

   /**
    * Compute the adapted vector space similarity between two vectors. It adds
    * to cat the weighted similarity and the number of common concept.
    * @param vec1           First vector.
    * @param vec2           Second vector.
    * @param cat            Concept category.
    */
   void ComputeCosineSim(GVector* vec1,GVector* vec2,Cat& cat);

	/**
	 * Compute the similarity for each category.
    * @param filter         Define a filter to limit the similarities to
    *                       compute. If null, all the similarities are computed.
	 * @return true if something could be computed.
	 */
	bool ComputeSims(GConceptCat* filter=0);

	/**
	 * Compute a similarity between description using the integral of Choquet as
    * aggregating function.
	 */
	double SimilarityChoquet(void);

	/**
	 * Compute a similarity between description using the sum as aggregating
    * function.
	 */
	double SimilaritySum(void);

	/**
	 * Compute a similarity between description using the product as aggregating
    * function.
	 */
	double SimilarityProduct(void);

	/**
	 * Compute a similarity between description using only the textual content.
	 */
	double SimilarityTextOnly(void);

	/**
	 * Compute the similarity between two objects that must inherits from the
	 * class GDescription.
	 */
	virtual double Compute(GObject* obj1,GObject* obj2);

	/**
	 * Get the identifier of an object of a line or a column.
	 * @param obj            Pointer to the object.
	 * @param line           Object in a line ?
	 * @return the identifier.
	 */
	virtual size_t GetId(void* obj,bool line);

	/**
	 * Create the parameters.
	 */
	virtual void CreateConfig(void);
};


//-----------------------------------------------------------------------------
// Template implementation
#include <genericsims.hh>


//------------------------------------------------------------------------------
#endif
