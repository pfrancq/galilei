/*

	GALILEI Research Project

	GProfileCalcFeedback.h

	Vector Computing Method - Header.

	Copyright 2001-2002 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).

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
#ifndef GProfileCalcFeedbackH
#define GProfileCalcFeedbackH


//-----------------------------------------------------------------------------
// include files for R Project
#include <rstd/rcontainer.h>
using namespace RStd;


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <profiles/gprofilecalc.h>
#include <profiles/gcalcparams.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// forward class declaration
class GIWordsWeights;
class GSubProfileVector;


//-----------------------------------------------------------------------------
/**
* The GFeedbackParam represents all the parameter used in the CalcFeedback module.
* @short GFeedback Parameters.
*/
class GFeedbackParams : public GCalcParams
{
public:

	/**
	* Maximal size allocate for a profile.
	*/
	unsigned int MaxOrderSize;

	/**
	* Maximal number of the non-zero weights in the vector.
	*/
	unsigned int MaxNonZero;

	/**
	* Factor associated with the vectors corresponding to the relevant part
	* (OK and N).
	*/
	double RelFactor;

	/**
	* Factor associated with the vectors corresponding to the fuzzy relevant
	* part (KO).
	*/
	double FuzzyFactor;

	/**
	* Factor associated with the vectors corresponding to the non relevant part
	* (H).
	*/
	double NoRelFactor;

	/**
	* Add the vectors from the fuzzy relevant documents.
	*/
	bool AddFuzzy;

	/**
	* Must the idf factor be computed.
	*/
	bool IdfFactor;

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
	GFeedbackParams& operator=(const GFeedbackParams& src);

	/**
	* Constructor.
	*/
	GFeedbackParams(void);
};


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
	class GNbDocsLangs;
	class InternVector;

	/**
	* feedback params
	*/
	GFeedbackParams* Params;

	/**
	* Global vectors computed.
	*/
	RStd::RContainer<InternVector,unsigned int,true,true> Vectors;

	/**
	* Number of documents where each index term of the "OK" and "N" documents
	* for the different languages appears.
	*/
	RStd::RContainer<InternVector,unsigned int,true,true> NbDocsWords;

	/**
	* Number of documents per languages.
	*/
	RStd::RContainer<GNbDocsLangs,unsigned int,true,true> NbDocsLangs;

	/**
	* Ordered vector for current computed profile.
	*/
	GIWordWeight** Order;

public:

	/**
	* Constructor.
	* @param session        Session.
	*/
	GProfileCalcFeedback(GSession* session, GFeedbackParams* p) throw(bad_alloc);

	/**
	* Set a parameter of the grouping method.
	* @param param          Name of the parameter.
	* @param value          Value of the parameter.
	*/
	virtual void SetParam(const char* param,const char* value);

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
	* Compute the global vectors.
	* @param profile        Profile to compute.
	*/
	void ComputeGlobal(GProfile* profile) throw(bad_alloc);

	/**
	* Computes the subprofile of a profile.
	* @param s              SubProfile to compute.
	*/
	void ComputeSubProfile(GSubProfileVector* s) throw(bad_alloc);

	/**
	* Compute a profile.
	* @param profile        Profile to compute.
	*/
	virtual void Compute(GProfile* profile);

	/**
	* Get the type of the method implemented. This method is used for the
	* vector model.
	* @returns tSubProfileDesc enum type.
	*/
	virtual tSubProfileDesc GetType(void) const {return(sdVector);}

	/**
	* Destructor.
	*/
	virtual ~GProfileCalcFeedback(void);
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif

