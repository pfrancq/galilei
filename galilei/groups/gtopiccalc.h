/*

	GALILEI Research Project

	GTopicCalc.h

	Generic Topic Computing Method - Header.

	Copyright 2008-2010 by Pascal Francq (pascal@francq.info).

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



//------------------------------------------------------------------------------
#ifndef GTopicCalcH
#define GTopicCalcH


//------------------------------------------------------------------------------
// include files for GALILEI
#include <gplugin.h>
#include <gpluginmanager.h>
#include <gweightinfos.h>


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* The GTopicCalc class provides a representation for a generic method to compute
* the description of a specific topic.
* @author Pascal Francq
* @short Generic Topic Computing Method.
*/
class GTopicCalc : public GPlugIn
{
protected:

	/**
	 * Container that should contain the structure after the analyze.
	 */
	GWeightInfos Infos;

public:

	/**
	* Constructor.
	* @param fac             Factory of the plug-in.
	*/
	GTopicCalc(GPlugInFactory* fac);

	/**
	* Compute a topic.
	* @param topic           Topic to compute.
	*/
	virtual void Compute(const GTopic* topic)=0;

	/**
	* Destruct.
	*/
	virtual ~GTopicCalc(void);

	friend class GSession;
	friend class GSimulator;
};


//------------------------------------------------------------------------------
#define CREATE_TOPICCALC_FACTORY(name,desc,plugin)\
	CREATE_FACTORY(GALILEI::GTopicCalc,plugin,"TopicCalc",R::RString::Null,name,desc)


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
