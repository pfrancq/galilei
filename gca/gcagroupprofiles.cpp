/*

	GALILEI Research Project

	GCAGroupProfiles.h

	GCA Plug-in for Profiles - Implementation

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



//-----------------------------------------------------------------------------
// include files for GALILEI
#include <ggroupprofiles.h>


//-----------------------------------------------------------------------------
// include files for GCA
#include <gcaplugin.h>
using namespace GALILEI;



//-----------------------------------------------------------------------------
/**
* The GCAGroupProfiles provides a representation for a method to group some
* profiles using the GCA.
* @author Pascal Francq
* @short GGA for Profiles.
*/
class GCAGroupProfiles : public GCAPlugIn<GProfile,GCommunity>, public GGroupProfiles
{
public:

	/**
	* Constructor.
	* @param f              Factory.
	*/
	GCAGroupProfiles(GPlugInFactory* fac)
		: GCAPlugIn<GProfile,GCommunity>("Profiles Grouping",otProfile,otCommunity), GGroupProfiles(fac) {}

	/**
	 * Class name.
	 */
	virtual R::RCString GetClassName(void) const {return("GCAGroupProfiles");}

	/**
	* Configurations were applied from the factory.
	*/
	virtual void ApplyConfig(void)
	{GCAPlugIn<GProfile,GCommunity>::ApplyConfig(Factory);}

protected:

	/**
	 * Get a pointer to the objects to cluster.
	 */
	virtual R::RCursor<GProfile> GetObjs(void) const {return(Session->GetProfiles());}

	/**
	* Make the grouping for a specific Language.
	*/
	virtual void Run(void) {RunGrouping(Session,"Profiles",Session->GetCommunities());}

public:

	/**
	* Create the parameters.
	* @param params          Parameters to configure.
	*/
	static void CreateParams(R::RConfig* params)
	{GCAPlugIn<GProfile,GCommunity>::CreateParams(params);}
};


//------------------------------------------------------------------------------
CREATE_GROUPPROFILES_FACTORY("GCA for Profiles","GCA for Profiles",GCAGroupProfiles)
