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
* The GProfileCalcFeedback class provides a representation for a method to compute
* a specific profile, i.e. its sub-profiles by using the vector method.
* @author Pascal Francq
* @short Vector Profile Computing Method.
*/
class GProfileCalcFeedback : public GProfileCalc
{
protected:
	class GNbDocsLangs;

	/**
	* Gloabal vectors computed.
	*/
	RStd::RContainer<GIWordsWeights,unsigned int,true,true> Vectors;

	/**
	* Number of documents where each index term of the "OK" and "N" documents
	* for the different languages appears.
	*/
	RStd::RContainer<GIWordsWeights,unsigned int,true,true> NbDocsWords;

	/**
	* Number of documents per languages.
	*/
	RStd::RContainer<GNbDocsLangs,unsigned int,true,true> NbDocsLangs;

	/**
	* Maximal number of the non-zero weights in the vector.
	*/
	unsigned int MaxNonZero;

	/**
	* "Beta" Parameter of the standard user feedback theory.
	*/
	double Beta;

	/**
	* "Gamma" Parameter of the standard user feedback theory.
	*/
	double Gamma;

	/**
	* Add the informations from the KO doc or substract them.
	*/
	bool AddKO;

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
	* Get the "Beta" Parameter.
	* @returns double value of the "beta" parameter.
	*/
	double GetBeta(void) const {return(Beta);}

	/**
	* Set the "Beta" Parameter.
	* @param b              Value of the "gamma" parameter.
	*/
	void SetBeta(double b) {Beta=b;}

	/**
	* Get the "Gamma" Parameter.
	* @returns double value of the "gamma" parameter.
	*/
	double GetGamma(void) const {return(Gamma);}

	/**
	* Set the "Gamma" Parameter.
	* @param g              Value of the "gamma" parameter.
	*/
	void SetGamma(double g) {Gamma=g;}

	/**
	* See if the KO documents are added or substract while compiling the
	* profile.
	* @returns true if there are added.
	*/
	bool GetAddKO(void) const {return(AddKO);}

	/**
	* Set if the KO documents are added or substract while compiling the
	* profile.
	* @param b              true if there are added.
	*/
	void SetAddKO(bool b) {AddKO=b;}

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

