/*
	GALILEI Research Project

	GLinkCalcTresh.h

	Link Calculation for Treshold Kleinberg Algorithm ( based on Kleinberg works and SALSA ) - Header.

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
#ifndef GLinkCalcTreshH
#define GLinkCalcTreshH


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
* The GLinkCalcTreshParam represents all the parameter used in the LinkCalcHITS module.
* @author Valery Vandaele.
* @short GLinkCalcTresh Parameters.
*/
class GLinkCalcTreshParams : public GLinkCalcParams
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
	* percentage of authority doc to be counted in the operaton O
	*/
	unsigned int Kvalue;


	/**
	* Use method for hub treshold
	*/
	bool HTresh;

	/**
	* Use method for authority treshold
	*/
	bool ATresh;

	/**
	* Use method for Full treshold
	*/
	bool FTresh;

	/**
	* Use method for Authority treshold based on value K
	*/
	bool AKTresh;

	/**
	* Use method for Full treshold based on value K
	*/
	bool FKTresh;

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
	GLinkCalcTreshParams& operator=(const GLinkCalcTreshParams& src);

	/**
	* Constructor.
	*/
	GLinkCalcTreshParams(void);
};


//-----------------------------------------------------------------------------
/**
* The GLinkCalcTresh class provides a method to compute the links of the
* documents with differents iterative algorithm.
* All the algorithms are based on Kleinberg HITS Algo bu are hibrid.
* There are tree different for of this algoritm based on tree differents kind of treshold
* applied during the calculation.
* -1 The HTresh (Hub-Treshold) Algo:
*	This algo applies an threshold during operation I by adding only the the authorities for
*	which the weight is greater than the mean of the pointed authorities
* -2 The ATresh (Authority-Treshold) Algo:
*	This algo applies an threshold during operation O by adding only the the hubs
* 	or which the weight is greater than the mean of the pointing hubs.
*-3 The FTresh (Full-Treshold) Algo:µ
*	This algo applies treshold on both operation I and O just like mentioned above
*
* @author  Vandaele Valery
* @short Iterative Algorithm of Link Description Computing Method..
*/
class GLinkCalcTresh : public GLinkCalc
{
private :

	/**
	* description computing parameters
	*/
	GLinkCalcTreshParams* Params;

	/**
	* The Norme for the weight of the links contained in Links_Out
	*/
	float Norme_Out;

	/**
	* The Norme for the weight of the links contained in Links_In
	*/
	float Norme_In;

	// Variable de test:
	/*
	* MeanAddAuto
	*/
	double MeanAddAuto;

	/*
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
	* Compute the weight of Incomming Links (wi = SUM weigth of page pointing to i).
	* @params out_links          The container of outgoing links for the computed profile
	* @params in_links             The container of incomming links for the computed profile
	*/
	void Operation_I(R::RContainer<GBalancedLinks,unsigned int,true,true>* out_Links,R::RContainer<GBalancedLinks,unsigned int,true,true>* in_Links);

	/**
	* Compute the weight of Outgoing Links (wi = SUM weigth of page that i point).
	* @params out_links          The container of outgoing links for the computed profile
	* @params in_links             The container of incomming links for the computed profile
	*/
	void Operation_O(R::RContainer<GBalancedLinks,unsigned int,true,true>* out_Links,R::RContainer<GBalancedLinks,unsigned int,true,true>* in_Links);

	/**
	* Normalization of weight for Links.
	* @params Links           The container from where the values must be normalised.
	* @params Norme          The norme used to normalise
	*/
	void Normalization(R::RContainer<GBalancedLinks,unsigned int,true,true>* Links, float Norme);

	/**
	* Find the best values for Outgoing links;
	* @param nb             The number of best values to return
	* @param lcur           The GLinksCursor on the container to be treated
	* @param Bv             The container of Links for the best Values.
	*/
	void FindBestValues(unsigned int nb,GBalancedLinksCursor lcur,R::RContainer<GBalancedLinks,unsigned int,true,false>* Bv);

	/*
	* Find min value in a struct BestValue
	* @param BestValue        The struct where the min must be found.
	* @paral nbVal            The number of values in struct BestValue.
	* @return BestValue       The structure containing the min values.
	*/
	struct BestValue findMin(struct BestValue* in, unsigned int nbVal);

public :

	/**
	* constructor of GLinkCalcTresh
	* @param ses               The Session.
	* @param p                 Parameters for the method.
	*/
	GLinkCalcTresh(GSession* ses,GLinkCalcTreshParams* p) throw(bad_alloc);

	/**
	* Compute links for all the documents.
	* @param subprof               The subprofile to be computed.
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
	* destructor of GLinkCalcTresh
	*/
	virtual ~GLinkCalcTresh(void);
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
