/*

	GALILEI Research Project

	GProfileCalcFeedback.h

	Vector Computing Method - Header.

	Copyright 2001-2007 by the Université libre de Bruxelles.

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



//-----------------------------------------------------------------------------
#ifndef GProfileCalcFeedbackH
#define GProfileCalcFeedbackH


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>
#include <gprofilecalc.h>
#include <gweightinfos.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
* The GProfileCalcFeedback class provides an implementation of the computing
* method "Feedback" inspired from the user relevance methods of the search
* engines. The vector of the profile is computed with a linear combinaison of
* the vectors of documents judged by the profile.
*
* The different parameters of the method:
* @param MaxNonZero         Maximal number of weights of the profile vector not
*                           to be zero. If this paramater is null, all the
*                           weights are left.
* @param IdfFactor          Specify if for the weights of documents the idf
*                           factor of the vector model theory must be computed.
* @param Relfactor          Factor corresponding to the relevant documents
*                           (OK+N).
* @param Fuzzyfactor        Factor corresponding to the fuzzy documents (KO).
* @param NoRelfactor        Factor corresponding to the no relevant documents
*                           (H).
* @param AddFuzzy           Specify if the fuzzy part must be added or
*                           substract.
* @author Pascal Francq
* @short "Feedbacks" Profile Computing Method.
*/
class GProfileCalcFeedback : public GProfileCalc
{
protected:
	/**
	* Information computed.
	*/
	R::RContainer<GWeightInfo,false,true> Infos;

	/**
	* Maximal number of the non-zero weights in the vector.
	*/
	unsigned int MaxNonZero;

	/**
	* Number of the non-zero weights for irrelevant entities in the vector.
	*/
	unsigned int NegNonZero;

	/**
	* Factor of the vectors corresponding to the relevant part.
	*/
	double RelFactor;

	/**
	* Factor of the vectors corresponding to the fuzzy relevant part.
	*/
	double FuzzyFactor;

	/**
	* Factor of the vectors corresponding to the irrelevant part.
	*/
	double IrrelFactor;

	/**
	* Positive feedback.
	*/
	bool Positive;

	/**
	* Global vector computed.
	*/
	GWeightInfos Vectors;

	/**
	* Number of documents assessed by a profile used for the computing where
	* each index term appears.
	*/
	GWeightInfos NbDocsWords;

	/**
	* Number of documents assessed by a profile used for the computing.
	*/
	unsigned int NbDocs;

	/**
	* Ordered vector for current computed profile.
	*/
	GWeightInfo** Order;

	/**
	* Maximal size allocate for a profile.
	*/
	unsigned int MaxOrderSize;

	/**
	* Incremental mode for the computing.
	*/
	bool IncrementalMode;

public:

	/**
	* Constructor.
	* @param session        Session.
	*/
	GProfileCalcFeedback(GFactoryProfileCalc* fac);

	/**
	* Configurations were applied from the factory.
	*/
	virtual void ApplyConfig(void);

	/**
	* Connect to a Session.
	* @param session         The session.
	*/
	virtual void Connect(GSession* session);

	/**
	* Disconnect from a Session.
	* @param session         The session.
	*/
	virtual void Disconnect(GSession* session);

private:

	/**
	* Compute the global vectors.
	* @param profile        Profile to compute.
	*/
	void ComputeGlobal(void);

	/**
	* Computes the subprofile of a profile.
	* @param s              SubProfile to compute.
	*/
	void ComputeSubProfile(void);

public:

	/**
	* Compute a profile.
	* @param profile        Profile to compute.
	*/
	virtual void Compute(GSubProfile* subprofile);

	/**
	* Create the parameters.
	* @param params          Parameters to configure.
	*/
	static void CreateParams(RConfig* params);

	/**
	* Destructor.
	*/
	virtual ~GProfileCalcFeedback(void);
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
