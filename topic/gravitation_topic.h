/*

	GALILEI Research Project

	Gravitation_Topic.h

	Topic Description Computing Method - Header.

	Copyright 2008-2011 by Pascal Francq (pascal@francq.info).

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
	* Ordered vector for current computed topic.
	*/
	const GConceptRef** Order;

	/**
	* Maximal size allocated.
	*/
	size_t MaxOrderSize;

   /**
	 * Internal structure.
	 */
	R::RContainer<GVector,true,true> Internal;

public:

	/**
	* Constructor.
	* @param session         Session.
	* @param fac             Factory.
	*/
	GTopicCalcGravitation(GSession* session,GPlugInFactory* fac);

	/**
	* Configurations were applied from the factory.
	*/
	virtual void ApplyConfig(void);

	/**
	* Compute a topic.
	* @param grp            Topic to compute.
	*/
	virtual void Compute(const GTopic* grp);

	/**
	* Create the parameters.
	*/
	virtual void CreateConfig(void);

	/**
	* Destructor.
	*/
	virtual ~GTopicCalcGravitation(void);
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif

