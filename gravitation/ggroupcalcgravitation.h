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
#include <sessions/galilei.h>
#include <groups/ggroupcalc.h>


//-----------------------------------------------------------------------------
namespace GALILEI{


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
	* Maximal number of the non-zero weights in the vector.
	*/
	unsigned int MaxNonZero;

	/**
	* Ordered vector for current computed profile.
	*/
	GWeightInfo** Order;

	/*
	* List of words used for computation.
	*/
	GWeightInfos* Vector;

	/**
	* Maximal size allocate for a profile.
	*/
	unsigned int MaxOrderSize;

public:

	/**
	* Constructor.
	* @param fac             Factory.
	*/
	GGroupCalcGravitation(GFactoryGroupCalc* fac) throw(bad_alloc);

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

	/**
	* Compute a group.
	* @param grp            Group to compute.
	*/
	virtual void Compute(GGroup* grp);

	/**
	* Get the name of the model used for the description.
	* @return C String.
	*/
	virtual const char* GetModelName(void) const {return("Vector");}

	/**
	* Create the parameters.
	* @param params          Parameters to configure.
	*/
	static void CreateParams(GParams* params);

	/**
	* Destructor.
	*/
	virtual ~GGroupCalcGravitation(void);
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif

