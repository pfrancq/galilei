/*

	GALILEI Research Project

	Gravitation_Topic.h

	Topic Description Computing Method - Header.

	Copyright 2008-2009 by Pascal Francq (pascal@francq.info).

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
#ifndef GTopicCalcGravitationH
#define GTopicCalcGravitationH


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>
#include <gtopiccalc.h>
#include <gweightinfos.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
* The GTopicCalcGravitation class provides a method to compute the description
* of a specific group as the gravitational point of the corresponding
* subprofiles.
* @author Pascal Francq
* @short Gravitational Point Topic Description Computing Method.
*/
class GTopicCalcGravitation : public GTopicCalc
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
	* @param fac             Factory.
	*/
	GTopicCalcGravitation(GPluginFactory* fac);

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
	* @param grp            Topic to compute.
	*/
	virtual void Compute(const GTopic* grp);

	/**
	* Create the parameters.
	* @param params          Parameters to configure.
	*/
	static void CreateParams(R::RConfig* params);

	/**
	* Destructor.
	*/
	virtual ~GTopicCalcGravitation(void);
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif

