/*
	GALILEI Research Project

	GLinkCalcSALSA.h

	Link Calculation for SALSA Algorithm - Header.

	Copyright 2003 by the Université Libre de Bruxelles.

	Authors
		 Vandaele Valery(vavdaele@ulb.ac.be)

	Version $Revision$

	Last Modify: $Date$

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



//-----------------------------------------------------------------------------
#ifndef GLinkCalcSALSAH
#define GLinkCalcSALSAH


//-----------------------------------------------------------------------------
//include file for GALILEI
#include <galilei.h>
#include <docs/glinks.h>
#include <docs/gbalancedlinks.h>
#include <docs/glinkcalc.h>
#include <docs/glinkcalcparams.h>

#include <rmath/rgenericsparsematrix.h>
#include <rmath/rgenericsparsevector.h>
#include <rmath/rgenericcell.h>
#include <rmath/rgenericcellcursor.h>
#include <rmath/rgenericvectorcursor.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/**
* The GLinkCalcSALSAParam represents all the parameter used in the LinkCalcSALSA module.
* @author Valery Vandaele.
* @short GLinkCalcSALSA Parameters.
*/
class GLinkCalcSALSAParams : public GLinkCalcParams
{
public:

	/**
	* number iteration for the algo.
	*/
	unsigned int NbIteration;

	/**
	* Number of results for Hubs.
	*/
	unsigned int NbResultsHub;

		/**
	* Number of results for Autorities.
	*/
	unsigned int NbResultsAuto;

	/**
	* Determine if the number of link to the same URL must be limited.
	*/
	bool LimitLink;

	/**
	* Determine the number of link to the same page.
	*/
	unsigned int NbLinks;

	/**
	* Determine if multiple link can be use in the same doc.
	*/
	bool UseMultipleLink;

	/**
	* Get the settings of the method coded in a string.
	* return Pointer to a C string.
	*/
	virtual const char* GetSettings(void);

	/**
	* Set the settings for the method using a string.
	* @param char*          C string coding the settings.
	*/
	virtual void SetSettings(const char*);

	/**
	* Assignment operator.
	* @param p              Parameters used as source.
	*/
	GLinkCalcSALSAParams& operator=(const GLinkCalcSALSAParams& src);

	/**
	* Constructor.
	*/
	GLinkCalcSALSAParams(void);
};


//-----------------------------------------------------------------------------
/**
* The GLinkCalcSALSA class provides a method to compute the links of the
* documents with an iterative algorithm named SALSA.
* @author  Vandaele Valery
* @short Iterative Algorithm of Link Description Computing Method..
*/
class GLinkCalcSALSA : public GLinkCalc
{
private :

	/**
	* description computing parameters
	*/
	GLinkCalcSALSAParams* Params;

	/**
	* The Norme for the weight of the links contained in out_Links
	*/
	float Norme_H;

  /**
  * The Norme for the weight of the links contained in in_Links
  */
	float Norme_A;

	//********
	// Variable de test:
	/*
	* MeanAddAuto
	*/
	double MeanAddAuto;

	/*    test
	* MeanAddHubs
	*/
	double MeanAddHub;

	unsigned int nbEtapes;


protected :
	/**
	* Special structure for best Values.
	* id ->  id of the doc
	* Value -> Weight of the doc.
	*/
	struct BestValue
	{
		unsigned int Id;
		float Value;
	};

private :

	/**
	* Compute the weight of Incomming Links .
	* @params Qh                    The transient matrix for the computed profile
	* @params Xh                     The sub-dominant eigen vector for Hubs of the system
	*/
	void Operation_H(R::RGenericSparseMatrix<R::RGenericCell<double>,true>* Qh,R::RGenericSparseVector<R::RGenericCell<double>,true>* Xhubs);

	/**
	* Compute the weight of Outgoing Links .
	* @params Qa                    The transient Matrix for the computed profile
	* @params Xa                     The sub-dominant eigen vector for Autorities of the system
	*/
	void Operation_A(R::RGenericSparseMatrix<R::RGenericCell<double>,true>* Qa, R::RGenericSparseVector<R::RGenericCell<double>,true>* Xauto);

	/**
	* Normalization of weight for Links.
	* @params X                  The vector which must be normalised.
	* @params Norme          The norme used to normalise
	*/
	void Normalization(R::RGenericSparseVector<R::RGenericCell<double>,true>* X, double Norme);

	/**
	* Find the best values for Outgoing links;
	* @param nb             The number of best values to return
	* @param X               The vector to be treated
	* @param Bv             The vector of id of Links for the best Values.
	*/
	void FindBestValues(unsigned int nb,R::RGenericSparseVector<R::RGenericCell<double>,true>* X,R::RGenericSparseVector<R::RGenericCell<int>,true>* Best_Links);

	/*
	* Find min value in a struct BestValue
	* @param BestValue        The struct where the min must be found.
	* @paral nbVal            The number of values in struct BestValue.
	* @return BestValue       The structure containing the min values.
	*/
	struct BestValue findMin(struct BestValue* in, unsigned int nbVal);

public :

	/**
	* constructor of GLinkCalcSALSA
	* @param ses               The Session.
	* @param p                 Parameters for the method.
	*/
	GLinkCalcSALSA(GSession* ses,GLinkCalcSALSAParams* p) throw(bad_alloc);

	/**
	* Initialize the values needed by th algorithm
	*/
//	/*virtual*/ void InitAlgo(void);

	/**
	* Compute links for all the documents.
	* @param prof               The profile to compute.
	*/
	virtual void Compute(GSubProfile* subprof);

	/**
	* Get the settings of the method coded in a string.
	* return Pointer to a C string.
	*/
	virtual const char* GetSettings(void);

	/**
	* Set the settings for the method using a string.
	* @param s              C string coding the settings.
	*/
	virtual void SetSettings(const char* s);

	/**
	* destructor of GLinkCalcHITS
	*/
	virtual ~GLinkCalcSALSA(void);
};


//-----------------------------------------------------------------------------
/**
* The RGenericCellCursorDouble class provides a way to go trough a set of Generic cell of type <double>.
* @short Generic cell cursor for double
*/
CLASSGENERICCELLCURSOR(RGenericCellCursorDouble,double)


//-----------------------------------------------------------------------------
/**
* The RGenericVectorCursorDouble class provides a way to go trough a set of Generic Vector
* containing generic cells of type <double>.
* @short Generic vector cursor for double
*/
CLASSGENERICVECTORCURSOR(RGenericVectorCursorDouble,double)


//-----------------------------------------------------------------------------
/**
* The RGenericCellCursorDouble class provides a way to go trough a set of Generic cell of type <int>.
* @short Generic cell cursor for int
*/
CLASSGENERICCELLCURSOR(RGenericCellCursorInt,int)


//-----------------------------------------------------------------------------
/**
* The RGenericVectorCursorDouble class provides a way to go trough a set of Generic Vector
* containing generic cells of type <int>.
* @short Generic vector cursor for int
*/
CLASSGENERICVECTORCURSOR(RGenericVectorCursorInt,int)


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
