/*

	GALILEI Research Project

	GGroupCalcRelevant.h

	Group Description is Relevant SubProfile Point Computing Method - Header.

	Copyright 2002 by the Université Libre de Bruxelles.

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
#ifndef GGroupCalcRelevantH
#define GGroupCalcRelevantH


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>
#include <groups/ggroupcalc.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/**
* The GGroupCalcRelevant class provides a method to compute the description
* of a specific group as the most relevant subprofile.
* @author Pascal Francq
* @short Relevant SubProfile Group Description Computing Method.
*/
class GGroupCalcRelevant : public GGroupCalc
{
	/**
	* Global similarities used.
	*/
	bool GlobalSim;

	/**
	* Maximal number of the non-zero weights in the vector.
	*/
	unsigned int MaxNonZero;

	/**
	* Ordered vector for current computed profile.
	*/
	GIWordWeight** Order;

	/**
	* Maximal size allocate for a profile.
	*/
	unsigned int MaxOrderSize;

public:

	/**
	* Constructor.
	* @param session        Session.
	*/
	GGroupCalcRelevant(GSession* session) throw(bad_alloc);

	/**
	* Compute the sum of the similarities of a given profile to all the others.
	* @param grp            Group to compute.
	* @param sub            Subprofile used as reference.
	* @returns result.
	*/
	double ComputeSumSim(GGroup* grp,GSubProfile* sub);

	/**
	* Compute a group.
	* @param grp            Group to compute.
	*/
	virtual void Compute(GGroup* grp);

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
	* Get the status of the global similarity.
	* @return bool value.
	*/
	bool GetGlobalSim(void) const {return(GlobalSim);}

	/**
	* Set the status of the global similarity.
	* @param s              Global similarity?
	*/
	void SetGlobalSim(bool s) {GlobalSim=s;}

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
	* Get the name of the model used for the description.
	* @return C String.
	*/
	virtual const char* GetModelName(void) const {return("Vector");}

	/**
	* Destructor.
	*/
	virtual ~GGroupCalcRelevant(void);
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif

