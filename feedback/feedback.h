/*

	GALILEI Research Project

	GProfileCalcFeedback.h

	Feedback-based Profile Computing Method - Header.

	Copyright 2001-2012 by Pascal Francq (pascal@francq.info).
	Copyright 2001-2008 Université Libre de Bruxelles (ULB).

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
#ifndef GProfileCalcFeedbackH
#define GProfileCalcFeedbackH


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>
#include <gdescriptionset.h>
#include <gdescriptionfilter.h>
#include <gprofilecalc.h>
#include <gprofile.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
* The GProfileCalcFeedback class provides an implementation of the computing
* method "Feedback" inspired from the user relevance methods of the search
* engines. The vector of the profile is computed with a linear combination of
* the vectors of documents judged by the profile.
*
* The different parameters of the method:
* @param LMax               Maximal number of most weighted concepts to be
*                           taken. If this parameter is null, all the weights
*                           are left.
* @param LMin               Maximal number of less weighted concepts to be
*                           taken.
* @param Relfactor          Factor corresponding to the relevant documents.
* @param Fuzzyfactor        Factor corresponding to the fuzzy documents.
* @param Irrelfactor        Factor corresponding to the irrelevant documents.
* @param Incremental        Incremental Mode?
* @param Alpha              Factor of the previous description.
* @author Pascal Francq
* @short Feedback Profile Computing Method.
*/
class GProfileCalcFeedback : public GProfileCalc, GDescriptionFilter
{
   /**
	* Maximal number of most weighted concepts to be taken.
	*/
	size_t LMax;

   /**
	* Maximal number of less weighted concepts to be taken.
	*/
   size_t LMin;

	/**
	* Incremental mode for the computing.
	*/
	bool IncrementalMode;

   /**
    * Factor used for the incremental mode.
    */
   double Alpha;

	/**
	* Factors of the descriptions.
	*/
	double Factors[4];

	/**
	* Set of descriptions.
	*/
	GDescriptionSet* Sets[4];

   /**
	* Descriptions computed.
	*/
	GDescription Desc[4];

   /**
	 * Internal description.
	 */
	GDescription Internal;

   /**
	* Temporary descriptions gathered.
	*/
	GDescription Tmp;

public:

	/**
	* Constructor.
	* @param session        Session.
	* @param fac            Factory.
	*/
	GProfileCalcFeedback(GSession* session,GPlugInFactory* fac);

	/**
	* Configurations were applied from the factory.
	*/
	virtual void ApplyConfig(void);
	/**
	* Compute a profile.
	* @param profile        Profile to compute.
	*/
	virtual void Compute(const GProfile* profile);

	/**
	* Create the parameters.
	*/
	virtual void CreateConfig(void);

	/**
	* Destructor.
	*/
	virtual ~GProfileCalcFeedback(void);
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
