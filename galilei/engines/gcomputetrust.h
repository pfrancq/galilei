/*

	GALILEI Research Project

	GComputeTrust.h

	Generic Trust Computing Method - Header.

	Copyright 2003-2015 by Pascal Francq (pascal@francq.info).
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
#ifndef GComputeTrustH
#define GComputeTrustH


//------------------------------------------------------------------------------
// include files for GALILEI
#include <gplugin.h>
#include <gpluginmanager.h>


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* The GComputeTrust class provides a representation for a generic method for
* trust computing.
*
* See the documentation related to GPlugIn for more general information.
* @short Generic Trust Computing Method.
*/
class GComputeTrust : public GPlugIn
{
public:

	/**
	* Constructor.
	* @param session         Session.
	* @param fac             Factory of the plug-in.
	*/
	GComputeTrust(GSession* session,GPlugInFactory* fac);

	/**
	* Run the trust computing method.
	*/
	virtual void Run(void)=0;

	/**
	* Destructor.
	*/
	virtual ~GComputeTrust(void);
};


//------------------------------------------------------------------------------
#define CREATE_COMPUTETRUST_FACTORY(name,desc,plugin)\
	CREATE_FACTORY(GALILEI::GComputeTrust,plugin,"ComputeTrust",R::RString::Null,name,desc)


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
