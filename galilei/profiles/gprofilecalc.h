/*

	GALILEI Research Project

	GProfileCalc.h

	Generic Profile Computing Method - Header.

	Copyright 2001-2011 by Pascal Francq (pascal@francq.info).
	Copyright 2001-2008 by the Université Libre de Bruxelles (ULB).

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
#ifndef GProfileCalcH
#define GProfileCalcH


//------------------------------------------------------------------------------
// include files for GALILEI
#include <gplugin.h>
#include <gpluginmanager.h>
#include <gvector.h>


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* The GProfileCalc class provides a representation for a method to compute a
* specific profile.
* @author Pascal Francq
* @short Generic Profile Computing Method.
*/
class GProfileCalc : public GPlugIn
{
protected:

	/**
	 * Vector that should contain the structure after the analyze.
	 */
	R::RContainer<GVector,true,true> Vectors;

public:

	/**
	* Construct the computing method.
	* @param session         Session.
	* @param fac             Factory of the plug-in.
	*/
	GProfileCalc(GSession* session,GPlugInFactory* fac);

	/**
	* Compute a profile.
	* @param profile         Reference to the profile to compute.
	*/
	virtual void Compute(const GProfile* profile)=0;

	/**
	* Destructor of the computing method.
	*/
	virtual ~GProfileCalc(void);

	friend class GSession;
};


//-------------------------------------------------------------------------------
#define CREATE_PROFILECALC_FACTORY(name,desc,plugin) \
	CREATE_FACTORY(GALILEI::GProfileCalc,plugin,"ProfileCalc",R::RString::Null,name,desc)


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
