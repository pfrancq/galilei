/*
	GALILEI Research Project

	GLinkCalcItAlgo.h

	Link Calculation for Iterative Algorithm - Header.

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
#ifndef GLinkCalcItAlgoH
#define GLinkCalcItAlgoH


//-----------------------------------------------------------------------------
//include file for GALILEI
#include <galilei.h>
#include <docs/glinks.h>
#include <docs/glinkcalc.h>
#include <docs/glinkcalcparams.h>

//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/**
* The GLinkCalcItAlgoParam represents all the parameter used in the LinkCalcItAlgo module.
* @author Valery Vandaele.
* @short GLinkCalcItAlgo Parameters.
*/
class GLinkCalcItAlgoParams : public GLinkCalcParams
{
public:

	/**
	* number iteration for the algo.
	*/
	unsigned int NbIteration;

	/**
	* Number of results.
	*/
	unsigned int NbResults;

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
	GLinkCalcItAlgoParams& operator=(const GLinkCalcItAlgoParams& src);

	/**
	* Constructor.
	*/
	GLinkCalcItAlgoParams(void);
};


//-----------------------------------------------------------------------------
/**
* The GLinkCalcItAlgo class provides a method to compute the links of the
* documents with an iterative algorithm.
* @author  Vandaele Valery
* @short Iterative Algorithm of Link Description Computing Method..
*/
class GLinkCalcItAlgo : public GLinkCalc
{
private :

	/**
	* description computing parameters
	*/
	GLinkCalcItAlgoParams* Params;

	/**
	* Container of outgoing links.
	*/
	RStd::RContainer<GLinks,unsigned int,true,true>* Links_Out;

	/**
	* Container of incomming links.
	*/
	RStd::RContainer<GLinks,unsigned int,true,true>* Links_In;

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
	* Initialize the values needed by th algorithm
	*/
	void InitAlgo(void);

	/**
	* Compute the weight of Incomming Links.                      
	*/
	void Operation_I(void);

	/**
	* Compute the weight of Outgoing Links.
	*/
	void Operation_O(void);

	/**
	* Normalization of weight for Incomming Links.
	*/
	void Normalization_I(void);

	/**
	* Normalization of weight for Outgoing Links.
	*/
	void Normalization_O(void);

	/**
	* Find the best values for Outgoing links;
	* @param nb             The number of best values to return
	* @param lcur           The GLinksCursor on the container to be treated
	* @param Bv             The container of Links for the best Values.
	*/
	void FindBestValues(unsigned int nb,GLinksCursor lcur,RContainer<GLinks,unsigned int,true,false>* Bv);

	/*
	* Find min value in a struct BestValue
	* @param BestValue        The struct where the min must be found.
	* @paral nbVal            The number of values in struct BestValue.
	* @return BestValue       The structure containing the min values.
	*/
	struct BestValue findMin(struct BestValue* in, unsigned int nbVal);

public :

	/**
	* constructor of GLinkCalcItAlgo
	* @param ses               The Session.
	* @param p                 Parameters for the method.
	*/
	GLinkCalcItAlgo(GSession* ses,GLinkCalcItAlgoParams* p) throw(bad_alloc);

	/**
	* Compute links for all the documents.
	* @param rec               The receiver for the signals.
	*/
	virtual void Compute(GSlot* rec);

	/**
	* Get a cursor over the links contained in Links_Out.
	*/
	GLinksCursor& GetLinksOutCursor(void);

	/**
	* Get a cursor over the links contained in Links_In.
	*/
	GLinksCursor& GetLinksInCursor(void);

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
	* destructor of GLinkCalcItAlgo
	*/
	virtual ~GLinkCalcItAlgo(void);
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
