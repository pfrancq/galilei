/*

	GALILEI Research Project

	GenericSims.h

	Generic Similarity Measure - Header.

	Copyright 2003-2011 by Pascal Francq (pascal@francq.info).
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
// include files for R Project
#include <rtextfile.h>
using namespace R;


//------------------------------------------------------------------------------
// include files for GALILEI
#include <gmatrixmeasure.h>
#include <gconcepttype.h>
#include <gweightinfo.h>
using namespace GALILEI;


//------------------------------------------------------------------------------
class GGenericSims;

//------------------------------------------------------------------------------
class GSimType
{
protected:

	GGenericSims* Owner;
	GConceptType* Type;
	size_t LastNbComps;  // Number of comparisons between information entities
	double Value;

public:
	GSimType(GGenericSims* owner,GConceptType* type);
	virtual void Init(void)=0;
	virtual void Add(GWeightInfo* info1,GWeightInfo* info2,GMeasure* mes)=0;
	virtual void AddObj1(GWeightInfo* info,GMeasure* mes)=0;
	virtual void AddObj2(GWeightInfo* info,GMeasure* mes)=0;
	virtual void Done(GMeasure* mes)=0;
	int Compare(const GSimType& t) const {return(Type->Compare(t.Type));}
	int Compare(const GSimType* t) const {return(Type->Compare(t->Type));}
	int Compare(const GConceptType* t) const {return(Type->Compare(t));}
	inline double GetIF(GConcept* concept,GMeasure* mes)
	{
		double tmp;
		mes->Measure(0,concept->GetId(),&tmp);
		return(tmp);
	}
	virtual ~GSimType(void) {}

	friend class GGenericSims;
};

//------------------------------------------------------------------------------
class GSimTypeCosinus : public GSimType
{
	double Norm1;
	double Norm2;
	double Num;
	double Max1;
	double Max2;
public:
	GSimTypeCosinus(GGenericSims* owner,GConceptType* type) :
		GSimType(owner,type) {}
	virtual void Init(void);
	virtual void Add(GWeightInfo* info1,GWeightInfo* info2,GMeasure* mes);
	virtual void AddObj1(GWeightInfo* info,GMeasure* mes);
	virtual void AddObj2(GWeightInfo* info,GMeasure* mes);
	virtual void Done(GMeasure* mes);
};


//------------------------------------------------------------------------------
class GSimTypeXMLIndex : public GSimType
{
	RContainer<GWeightInfo,false,false> Obj1;
	RContainer<GWeightInfo,false,false> Obj2;

public:
	GSimTypeXMLIndex(GGenericSims* owner,GConceptType* type) :
		GSimType(owner,type), Obj1(100), Obj2(100) {}
	virtual void Init(void);
	virtual void Add(GWeightInfo* info1,GWeightInfo* info2,GMeasure* mes);
	virtual void AddObj1(GWeightInfo* info,GMeasure* mes);
	virtual void AddObj2(GWeightInfo* info,GMeasure* mes);
	virtual void Done(GMeasure* mes);
};


//------------------------------------------------------------------------------
class GGenericSims : public GMatrixMeasure
{
protected:

	enum tSimType
	{
		Undefined            /** Unknown method.*/,
		LanguageOnly         /** Based only on the languages.*/,
		Product              /** Product of the different similarities.*/,
		Sum                  /** Weighted sum of the different similarities.*/,
		Choquet              /** Use the integral of Choquet.*/
	};

	/**
	 * Type of similarity:
	 */
	tSimType SimType;

	/**
	 * Factor used for the computation of the global similarity.
	 */
	double Factor;

	/**
	 * Capacity/Weight of the similarity in the content space.
	 */
	double ContentCapacity;

	/**
	 * Capacity/Weight of the similarity in the structure space.
	 */
	double StructCapacity;

	/**
	 * Capacity/Weight of the similarity in the metadata space.
	 */
	double MetaCapacity;

	/**
	 * Capacity of the combination of the content and structure criteria
	 * (Choquet).
	 */
	double ContentStructCapacity;

	/**
	 * Capacity of the combination of the content and meta criteria
	 * (Choquet)
	 */
	double ContentMetaCapacity;

	/**
	 * Capacity of the combination of the metadata and structure criteria
	 * (Choquet)
	 */
	double MetaStructCapacity;

	/**
	 * Metadata space.
	 */
	GConceptType* MetaSpace;

	/**
	 * Structure space.
	 */
	GConceptType* StructSpace;

	/**
	 * Vector corresponding to the first object.
	 */
	GWeightInfosObj* vec1;

	/**
	 * Vector corresponding to the second object.
	 */
	GWeightInfosObj* vec2;

	/**
	 * Similarity to compute for the different spaces.
	 */
	RContainer<GSimType,true,false> Types;
	RContainer<GSimType,false,false> Valid;

	/**
	 * Current language used for the cosine measure.
	 */
	GConceptType* CurLang;

	/**
	 * Current type used for the cosine measure.
	 */
	GSimType* CurType;

	/**
	 * Similarity in the different spaces:
	 * #- 0=Content.
	 * #- 1=Structure.
	 * #- 2=Metadata.
	 */
	double SimSpaces[5];

	/**
	 * Transform the similarity from [-1,+1] to [0,1].
	 */
	bool Transform;

	/**
	 * Feature evaluation.
	 */
	GMeasure* FeatureEval;

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
	 * Compute the similarity in each space and put them in 'SimSpaces'.
	 * @return true if something could be computed.
	 */
	bool ComputeSimSpace(void);

	/**
	* Compute a similarity between two lists of weighted information entities.
	* A similarity is computed in each space (content,structure and metadata).
	* The different similarities are aggregated with the integral of Choquet.
	*/
	double SimilarityChoquet(void);

	/**
	* Compute a similarity between two lists of weighted information entities.
	* A similarity is computed in each space (content,structure and metadata).
	* The different similarities are aggregated with a weighted sum.
	*/
	double SimilaritySum(void);

	/**
	* Compute a similarity between two lists of weighted information entities.
	* A similarity is computed in each space (content,structure and metadata).
	* The different similarities are aggregated with a product.
	*/
	double SimilarityProduct(void);

	/**
	* Compute a similarity between two lists of weighted information entities
	* based on the content space only.
	*/
	double SimilarityLang(void);

	/**
	 * Compute the similarity between two objects that must inherits from the
	 * class GWeightInfos.
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

	friend class GSimType;
	friend class GSimTypeCosinus;
	friend class GSimTypeXMLIndex;
};


//------------------------------------------------------------------------------
#endif
