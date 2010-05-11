/*

	GALILEI Research Project

	GStatsCalc.h

	Generic Statistics Method - Header.

	Copyright 2003-2010 by Pascal Francq (pascal@francq.info).
	Copyright 2003-2008 by the Université Libre de Bruxelles (ULB).

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
#ifndef GStatsCalcH
#define GStatsCalcH


//------------------------------------------------------------------------------
// include files for GALILEI
#include <gplugin.h>
#include <gpluginmanager.h>


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* The GStatsCalc class provides a representation for a set of statistics to
* compute.
* @author Pascal Francq
* @short Generic Statistics Method.
*/
class GStatsCalc : public GPlugIn
{
public:

	/**
	* Construct the statistics method.
	* @param session         Session.
	* @param fac             Factory of the plug-in.
	*/
	GStatsCalc(GSession* session,GPlugInFactory* fac);

	/**
	* Compute the statistics.
	* @param xml             XML Structure.
	* @param res             XML tag that will be hold the results.
	*/
	virtual void Compute(R::RXMLStruct* xml,R::RXMLTag& res)=0;

	/**
	* Add a tag representing an element with a value.
	* @param xml             XML Structure.
	* @param parent          XML tag that will be hold the result.
	* @param element         Name of the element.
	* @param val             Value.
	* @param fmt             Format to used to represent the number.
	* @return a pointer to the tag created.
	*/
	R::RXMLTag* AddTag(R::RXMLStruct* xml,R::RXMLTag* parent,R::RString element,double val,const char* fmt="%E");

	/**
	* Add a tag representing an element with a value.
	* @param xml             XML Structure.
	* @param parent          XML tag that will be hold the result.
	* @param element         Name of the element.
	* @param val             Value.
	* @return a pointer to the tag created.
	*/
	R::RXMLTag* AddTag(R::RXMLStruct* xml,R::RXMLTag* parent,R::RString element,R::RString val);

	/**
	* Destructor of the statistics method.
	*/
	virtual ~GStatsCalc(void);
};


//-------------------------------------------------------------------------------
#define CREATE_STATSCALC_FACTORY(name,desc,plugin)\
	CREATE_FACTORY(GALILEI::GStatsCalc,plugin,"StatsCalc",R::RString::Null,name,desc)


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
