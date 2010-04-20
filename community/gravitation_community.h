/*

	GALILEI Research Project

	Gravitation_Community.h

	Community Description Computing Method - Header.

	Copyright 2002-2010 by Pascal Francq (pascal@francq.info).
	Copyright 2002-2008 by the Université Libre de Bruxelles (ULB).

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
#ifndef GCommunityCalcGravitationH
#define GCommunityCalcGravitationH


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>
#include <gcommunitycalc.h>
#include <gweightinfos.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
* The GCommunityCalcGravitation class provides a method to compute the description
* of a specific community as the gravitational point of the corresponding
* profiles.
* @author Pascal Francq
* @short Gravitational Point Group Description Computing Method.
*/
class GCommunityCalcGravitation : public GCommunityCalc
{
	/**
	* Maximal number of the non-zero weights in the vector.
	*/
	size_t MaxNonZero;

	/**
	* Ordered vector for current computed profile.
	*/
	const GWeightInfo** Order;

	/**
	* Internal vector.
	*/
	GWeightInfos Vector;

	/**
	* Maximal size allocate for a profile.
	*/
	size_t MaxOrderSize;

public:

	/**
	* Constructor.
	* @param session         Session.
	* @param fac             Factory.
	*/
	GCommunityCalcGravitation(GSession* session,GPlugInFactory* fac);

	/**
	* Configurations were applied from the factory.
	*/
	virtual void ApplyConfig(void);

	/**
	* Compute a group.
	* @param grp            Group to compute.
	*/
	virtual void Compute(const GCommunity* grp);

	/**
	* Create the parameters.
	* @param fac             Factory.
	*/
	static void CreateParams(GPlugInFactory* fac);

	/**
	* Destruct.
	*/
	virtual ~GCommunityCalcGravitation(void);
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif

