/*
	GALILEI Research Project

	GLinkCalcCorrespondence.h

	Link Calculation for Correspondence Algorithm  - Header.

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
#ifndef GLinkCalcCorrespondenceH
#define GLinkCalcCorrespondenceH


//-----------------------------------------------------------------------------
//include file for GALILEI
#include <galilei.h>
#include <docs/glinks.h>
#include <docs/gbalancedlinks.h>
#include <docs/glinkcalc.h>
#include <docs/glinkcalcparams.h>

//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/**
* The GLinkCalcCorrespondenceParam represents all the parameter used in the LinkCalcCorrespondence module.
* @author Valery Vandaele.
* @short GLinkCalcCorrespondence Parameters.
*/
class GLinkCalcCorrespondenceParams : public GLinkCalcParams
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
	GLinkCalcCorrespondenceParams& operator=(const GLinkCalcCorrespondenceParams& src);

	/**
	* Constructor.
	*/
	GLinkCalcCorrespondenceParams(void);
};


//-----------------------------------------------------------------------------
/**
* The GLinkCalcCorrespondence class provides a method to compute the links of the
* documents with an iterative algorithm.
* @author  Vandaele Valery
* @short Iterative Algorithm of Link Description Computing Method..
*/
class GLinkCalcCorrespondence : public GLinkCalc
{
private :

	/**
	* description computing parameters
	*/
	GLinkCalcCorrespondenceParams* Params;

	/**
	* Container of outgoing links.
	*/
	R::RContainer<GLinks,unsigned int,true,true>* Links_Out;

	/**
	* The Norme for the weight of the links contained in Links_Out
	*/
	float Norme_Out;

  /**
  * The Norme for the weight of the links contained in Links_In
  */
	float Norme_In;


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
	* Compute the weight of Incomming Links (wi = AVG weigth of page pointing to i).
	* @params out_links          The container of outgoing links for the computed profile
	* @params in_links             The container of incomming links for the computed profile
	*/
	void Operation_I(R::RContainer<GBalancedLinks,unsigned int,true,true>* out_Links,R::RContainer<GBalancedLinks,unsigned int,true,true>* in_Links);

	/**
	* Compute the weight of Outgoing Links (wi = AVG weigth of page that i point).
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
	* constructor of GLinkCalcCorrespondence
	* @param ses               The Session.
	* @param p                 Parameters for the method.
	*/
	GLinkCalcCorrespondence(GSession* ses,GLinkCalcCorrespondenceParams* p) throw(bad_alloc);

	/**
	* Initialize the values needed by th algorithm
	*/
	virtual void InitAlgo(void);

	/**
	* Add a new doc (not present in the initAlgo phase) to the graph.
	*/
	virtual void AddDoc(GDoc* doc);

	/**
	* Compute links for all the documents.
	* @param prof               The profile to compute.
	*/
	virtual void Compute(GProfile* prof);

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
	* destructor of GLinkCalcCorrespondence
	*/
	virtual ~GLinkCalcCorrespondence(void);
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
