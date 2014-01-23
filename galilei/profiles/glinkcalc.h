/*

	GALILEI Research Project

	GLinkCalc.h

	Generic Link Method - Header.

	Copyright 2003-2004 by Valery Vandaele.
	Copyright 2003-2008 Université Libre de Bruxelles (ULB).

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
#ifndef GLinkCalcH
#define GLinkCalcH


//------------------------------------------------------------------------------
//include file for GALILEI
#include <gplugin.h>
#include <gpluginmanager.h>


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* The GLinkCalc provides a representation for a generic method to compute links.
*
* See the documentation related to GPlugIn for more general information.
* @author Vandaele Valery
* @short Generic Link Method.
*/
class GLinkCalc : public GPlugIn
{
public:

	/**
	* Construct the computing method.
	* @param session        Session.
	* @param fac            Factory of the plug-in.
	*/
	GLinkCalc(GSession* session,GPlugInFactory* fac);

	/**
	* Compute the links for a given profile.
	* @param prof            Reference to the profile to compute.
	*/
	virtual void Compute(GProfile* prof)=0;

	/**
	* destructor of GLinkCalc
	*/
	virtual ~GLinkCalc(void);
};


//-------------------------------------------------------------------------------
#define CREATE_LINKCALC_FACTORY(name,plugin)\
	CREATE_FACTORY(GPlugInManager,GPlugInFactory,GLinkCalc,plugin,"LinkCalc",API_LINKCALC_VERSION,name)


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
