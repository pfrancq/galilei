/*

	GALILEI Research Project

	GGroupCalcRelevant.h

	Group Description is Relevant SubProfile Point Computing Method - Header.

	Copyright 2002 by the Universit� Libre de Bruxelles.

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
#include <groups/ggroupcalcparams.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
* The GCalcRelevantParam represents all the parameter used in the KMeans module.
* @short GCalcRelevant Parameters.
*/
class GCalcRelevantParams : public GGroupCalcParams
{
public:

	/**
	* Global similarities used.
	*/
	bool GlobalSim;

	/**
	* Maximal number of the non-zero weights in the vector.
	*/
	unsigned int MaxNonZero;

	/**
	* Maximal size allocate for a profile.
	*/
	unsigned int MaxOrderSize;

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
	GCalcRelevantParams& operator=(const GCalcRelevantParams& src);

	/**
	* Constructor.
	*/
	GCalcRelevantParams(void);
};


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
	* description computing parameters
	*/
	GCalcRelevantParams* Params;

	/**
	* Ordered vector for current computed profile.
	*/
	GIWordWeight** Order;


public:

	/**
	* Constructor.
	* @param session        Session.
	*/
	GGroupCalcRelevant(GSession* session, GCalcRelevantParams* p) throw(bad_alloc);

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

