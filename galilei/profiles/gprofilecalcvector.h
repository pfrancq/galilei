/*

	GALILEI Research Project

	GProfileCalcVector.h

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
#ifndef GProfileCalcVectorH
#define GProfileCalcVectorH


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
* The GProfileCalcVector class provides an implementation of the computing
* method "Statistical", which is based on the idea that the profile vector
* is the one obtain from the documents judged as relevant.
*
* The different parameters of the method:
* @param MaxNonZero         Maximal number of weights of the profile vector not
*                           to be zero. If this paramater is null, all the
*                           weights are left.
* @param IdfFactor          Specify if for the weights of documents the idf
*                           factor of the vector model theory must be computed.
* @author Pascal Francq
* @short Statistical Profile Computing Method.
*/
class GProfileCalcVector : public GProfileCalc
{
protected:
	class GNbDocsLangs;
	class InternVector;

	/**
	* Occurences of the index terms in the "OK" and "N" documents for the
	* different languages.
	*/
	RStd::RContainer<InternVector,unsigned int,true,true> Vector;

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
	* Maximal number of the non-zero weights in the vector.
	*/
	unsigned int MaxNonZero;

	/**
	* Must the idf factor be computed.
	*/
	bool IdfFactor;

public:

	/**
	* Constructor.
	* @param session        Session.
	*/
	GProfileCalcVector(GSession* session) throw(bad_alloc);

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
	* Computes the OK lists for a profile.
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
	virtual ~GProfileCalcVector(void);
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif

