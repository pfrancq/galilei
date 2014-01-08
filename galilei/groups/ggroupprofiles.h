/*

	GALILEI Research Project

	GGroupProfiles.h

	Generic Profiles Grouping Method - Header.

	Copyright 2001-2014 by Pascal Francq (pascal@francq.info).
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
#ifndef GGroupProfilesH
#define GGroupProfilesH


//------------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>
#include <gplugin.h>
#include <gpluginmanager.h>


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* The GGroupProfiles provides a representation for a generic method to group some
* profiles.
* @author Pascal Francq
* @short Generic Profiles Grouping Method.
*/
class GGroupProfiles : public GPlugIn
{
protected:

	/**
	* Profiles that must be grouped.
	*/
	R::RContainer<GProfile,false,false> Profiles;

public:

	/**
	* Constructor of the profiles grouping method.
	* @param session         Session.
	* @param fac             Factory of the plug-in.
	*/
	GGroupProfiles(GSession* session,GPlugInFactory* fac);

protected:

	/**
	* Make the grouping for the defined profiles.
	*/
	virtual void Run(void)=0;

public:

	/**
	* Group the Profiles.
	* @param rec            Receiver of the signals.
	*/
	void Grouping(GSlot* rec);

	/**
	* Destructor of the profiles grouping method.
	*/
	virtual ~GGroupProfiles(void);
};


//-------------------------------------------------------------------------------
#define CREATE_GROUPPROFILES_FACTORY(name,desc,plugin)\
	CREATE_FACTORY(GALILEI::GGroupProfiles,plugin,"GroupProfiles",R::RString::Null,name,desc)


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
