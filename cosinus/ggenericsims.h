/*

	GALILEI Research Project

	GenericSims.h

	GenericSims - Implementation.

	Copyright 2005-2008 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).

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
#include <gmeasure2elements.h>
#include <gconcepttype.h>
//#include <gsignalhandler.h>
/*#include <ggalileiapp.h>
#include <glang.h>

#include <gdoc.h>
#include <ggroup.h>
#include <gmeasure2elements.h>
#include <gconcepttype.h>*/
using namespace GALILEI;



//------------------------------------------------------------------------------
class GGenericSims;
	
//------------------------------------------------------------------------------
class GSimType
{
protected:
	
	GGenericSims* Owner;
	GConceptType* Type;
	
public:
	GSimType(GGenericSims* owner,GConceptType* type);
	virtual double Compute(RCursor<GWeightInfo>& Obj1,RCursor<GWeightInfo>& Obj2);
	int Compare(const GSimType& t) const {return(Type->Compare(t.Type));}
	int Compare(const GSimType* t) const {return(Type->Compare(t->Type));}
	int Compare(const GConceptType* t) const {return(Type->Compare(t));}
	virtual ~GSimType(void) {}
	
	friend class GGenericSims;
};


//------------------------------------------------------------------------------
class GSimTypeXMLIndex : public GSimType
{
public:
	GSimTypeXMLIndex(GGenericSims* owner,GConceptType* type) :
		GSimType(owner,type) {}
	virtual double Compute(RCursor<GWeightInfo>& Obj1,RCursor<GWeightInfo>& Obj2);
};


//------------------------------------------------------------------------------
class GGenericSims : public GMeasure2Elements
{
protected:
	
	/**
	 * Type of similarity:
	 * #- 1 : Multi-vector.
	 * #- 2 : Language only.
	 */
	unsigned int SimType;
	
	/**
	 * Factor used for the computation of the global similarity.
	 */
	double Factor;
	
	/**
	 * Vector corresponding to the first object.
	 */
	GWeightInfos* vec1;
	
	/**
	 * Vector corresponding to the second object.
	 */
	GWeightInfos* vec2;
	
	/**
	 * Similarity to compute for the different spaces.
	 */
	RContainer<GSimType,true,true> Types;
	
	/**
	 * Space of the objects.
	 */
	tObjType Space;
	
public:
	
	/**
	* Constructor of the similarities between two elements of the same type. The 
	* measures may be symetric or not.
	* @param fac             Factory of the plug-in.
	* @param sym             Symetric measure?
	* @param type           Type of the elements in the lines.
	*/
	GGenericSims(GFactoryMeasure* fac,bool sym,tObjType type);
	
	/**
	* Configurations were applied from the factory.
	*/
	virtual void ApplyConfig(void);

	/**
	 * Connect to the session.
	 * * @param session        Session.
	 */
	virtual void Connect(GSession* session);
	
	/**
	 * Disconnect from the session
	 * @param session        Session.
	 */
	virtual void Disconnect(GSession* session);
	
	/**
	* Compute a similarity between two lists of weighted information entities.
	* The method uses the cosinus of the corresponding vectors. A vector of a
	* list is build using this list and a Inverse Frequence Factor (IFF) of the
	* object type (idf, isf or ivf) for a given information entity space
	* (language). If one of the list is empty, the similarity is null.
	*/
	double SimilarityIFFMV(void);

	/**
	* Compute a similarity between two lists of weighted information entities.
	* The method uses the cosinus of the corresponding vectors. A vector of a
	* list is build using this list and a Inverse Frequence Factor (IFF) of the
	* object type (idf, isf or ivf) for a given information entity space
	* (language). If one of the list is empty, the similarity is null.
	*/
	double SimilarityIFFL(void);
	
	/**
	 * Compute the similarity between two objects that must inherits from the 
	 * class GWeightInfos.
	 */
	double Compute(void* obj1,void* obj2);
	
	/**
	 * Create the parameters.
	 */
	static void CreateParams(RConfig* params);
	
	friend class GSimType;
	friend class GSimTypeXMLIndex;
};


//------------------------------------------------------------------------------
#endif
