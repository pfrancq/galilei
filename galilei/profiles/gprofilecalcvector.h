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


//-----------------------------------------------------------------------------
/**
* The GProfileCalcVector class provides a representation for a method to compute
* a specific profile, i.e. its sub-profiles by using the vector method.
* @author Pascal Francq
* @short "Lists" Profile' Computing Method.
*/
class GProfileCalcVector : public GProfileCalc
{
	/**
	* List of words' frequences in the "OK" and "N" documents for the different
	* languages.
	*/
	RStd::RContainer<GIWordsWeights,unsigned int,true,true> OK;

	/**
	* List of words' frequences in the "KO" documents for the different
	* languages.
	*/
	RStd::RContainer<GIWordsWeights,unsigned int,true,true> KO;

	/**
	* Size of the OK and KO lists of the profiles.
	*/
	unsigned int Size;

public:

	/**
	* Constructor.
	* @param session        Session.
	* @param size           Size of the OK and KO lists of the profiles.
	*/
	GProfileCalcVector(GSession* session,const unsigned int size) throw(bad_alloc);

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
	* Get the size of the lists OK/KO.
	* @return Integer.
	*/
	unsigned int GetSize(void) const {return(Size);}

	/**
	* Set the size of the lists OK/KO.
	* @param size           Size to use for futher computation.
	*/
	void SetSize(unsigned int size) {Size=size;}

	/**
	* Destructor.
	*/
	virtual ~GProfileCalcVector(void);
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif

