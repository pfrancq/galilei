/*

	GALILEI Research Project

	GProfileCalcReWeighting.h

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
#ifndef GProfileCalcReWeightingH
#define GProfileCalcReWeightingH


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>
#include <profiles/gprofilecalc.h>
#include <profiles/gsubprofilevector.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
* The GProfileCalcReWeighting class provides a representation for a method to compute
* a specific profile, i.e. its sub-profiles by using the vector method.
* @author Pascal Francq
* @short Vector Profile Computing Method.
*/
class GProfileCalcReWeighting : public GProfileCalc
{
	class InternVector;
protected:

	/**
	* List of words frequences in the "OK" and "N" documents for the different
	* languages.
	*/
	RStd::RContainer<InternVector,unsigned int,true,true> OK;

	/**
	* List of words frequences in the "KO" documents for the different
	* languages.
	*/
	RStd::RContainer<InternVector,unsigned int,true,true> KO;

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

public:

	/**
	* Constructor.
	* @param session        Session.
	*/
	GProfileCalcReWeighting(GSession* session) throw(bad_alloc);

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
	* Computes the OK and KO lists for a profile.
	* @param profile        Profile to compute.
	*/
	void ComputeOKKO(GProfile* profile) throw(bad_alloc);

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
	virtual ~GProfileCalcReWeighting(void);
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
