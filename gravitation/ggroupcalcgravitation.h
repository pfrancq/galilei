/*

	GALILEI Research Project

	GGroupCalcGravitation.h

	Group Description is Gravitational Point Computing Method - Header.

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
#ifndef GGroupCalcGravitationH
#define GGroupCalcGravitationH


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
* The GCalcGravitationParam represents all the parameter used in the KMeans module.
* @short GCalcGravitation Parameters.
*/
class GCalcGravitationParams : public GGroupCalcParams
{
public:

	/**
	* Maximal number of the non-zero weights in the vector.
	*/
	unsigned int MaxNonZero;

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
	GCalcGravitationParams& operator=(const GCalcGravitationParams& src);

	/**
	* Constructor.
	*/
	GCalcGravitationParams(void);
};


//-----------------------------------------------------------------------------
/**
* The GGroupCalcGravitation class provides a method to compute the description
* of a specific group as the gravitational point of the corresponding
* subprofiles.
* @author Pascal Francq
* @short Gravitational Point Group Description Computing Method.
*/
class GGroupCalcGravitation : public GGroupCalc
{

	 /**
	* description computing parameters
	*/
	GCalcGravitationParams* Params;

	/**
	* Ordered vector for current computed profile.
	*/
	GIWordWeight** Order;

	/*
	* List of words used for computation.
	*/
	GIWordsWeights* Vector;

	/**
	* Maximal size allocate for a profile.
	*/
	unsigned int MaxOrderSize;

public:

	/**
	* Constructor.
	* @param session        Session.
	*/
	GGroupCalcGravitation(GSession* session, GCalcGravitationParams* p) throw(bad_alloc);

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
	virtual ~GGroupCalcGravitation(void);
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif

