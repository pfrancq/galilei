/*

	GALILEI Research Project

	GPostProfile.h

	Generic Post-Profiling Computing Method - Header.

	Copyright 2003 by David Wartel.
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
#ifndef GPostProfileH
#define GPostProfileH


//------------------------------------------------------------------------------
// include files for GALILEI
#include <gplugin.h>
#include <gpluginmanager.h>


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* The GPostProfile class provides a representation for a generic method to compute
* the post-profiling methods.
* @author Wartel David
* @short Generic Post-Profiling Computing Method.
*/
class GPostProfile : public GPlugIn
{
public:

	/**
	* Constructor.
	* @param session         Session.
	* @param fac             Factory of the plug-in.
	*/
	GPostProfile(GSession* session,GPlugInFactory* fac);

	/**
	* Run the post-group method.
	*/
	virtual void Run(void)=0;

	/**
	* Destructor.
	*/
	virtual ~GPostProfile(void);
};


//------------------------------------------------------------------------------
#define CREATE_POSTPROFILE_FACTORY(name,desc,plugin)\
	CREATE_FACTORY(GALILEI::GPostProfile,plugin,"PostProfile",R::RString::Null,name,desc)


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
