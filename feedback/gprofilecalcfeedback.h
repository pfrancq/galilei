/*

	GALILEI Research Project

	GProfileCalcFeedback.h

	Vector Computing Method - Header.

	(C) 2001-2002 by P. Francq.

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


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// forward class declaration
class GIWordsWeights;
class GSubProfileVector;


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
	* Gloabal vectors computed.
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

public:

	/**
	* Constructor.
	* @param session        Session.
	*/
	GProfileCalcFeedback(GSession* session) throw(bad_alloc);

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
	* Get the number of non-zero weights in the vector.
	*/
	unsigned int GetMaxNonZero(void) const {return(MaxNonZero);}

	/**
	* Set the number of non-zero weights in the vector.
	* @param n              Number of non-zero weights.
	*/
	void SetMaxNonZero(unsigned int n) {MaxNonZero=n;}

	/**
	* Get the "IdfFactor" parameter
	* @returns boolean.
	*/
	bool GetIdfFactor(void) const {return(IdfFactor);}

	/**
	* Set the "IdfFactor" pamater.
	* @param b              true if the idf factor must be computed.
	*/
	void SetIdfFactor(bool b) {IdfFactor=b;}

	/**
	* Get the "RelFactor" Parameter.
	* @returns double.
	*/
	double GetRelFactor(void) const {return(RelFactor);}

	/**
	* Set the "RelFactor" Parameter.
	* @param b              Value of the "RelFactor" parameter.
	*/
	void SetRelFactor(double b) {RelFactor=b;}

	/**
	* Get the "FuzzyFactor" Parameter.
	* @returns double.
	*/
	double GetFuzzyFactor(void) const {return(FuzzyFactor);}

	/**
	* Set the "FuzzyFactor" Parameter.
	* @param g              Value of the "FuzzyFactor" parameter.
	*/
	void SetFuzzyFactor(double g) {FuzzyFactor=g;}

	/**
	* Get the "NoRelFactor" Parameter.
	* @returns double.
	*/
	double GetNoRelFactor(void) const {return(NoRelFactor);}

	/**
	* Set the "NoRelFactor" Parameter.
	* @param b              Value of the "NoRelFactor" parameter.
	*/
	void SetNoRelFactor(double b) {NoRelFactor=b;}

	/**
	* Get the "AddFuzzy" parameter
	* @returns boolean.
	*/
	bool GetAddFuzzy(void) const {return(AddFuzzy);}

	/**
	* Set the "AddFuzzy" pamater.
	* @param b              true if the fuzzy part must be added.
	*/
	void SetAddFuzzy(bool b) {AddFuzzy=b;}

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

