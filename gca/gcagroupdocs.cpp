/*

	Genetic Documents Algorithm

	GCAGroupDocs.cpp

	GDA Plug-in for Documents - Implementation

	Copyright 2006-2008 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).

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
#include <ggroupdocs.h>


//-----------------------------------------------------------------------------
// include files for GCA
#include <gcaplugin.h>
using namespace GALILEI;



//-----------------------------------------------------------------------------
/**
* The GCAGroupDocs provides a representation for a method to group some
* documents using the GCA.
* @author Pascal Francq
* @short GGA for Documents.
*/
class GCAGroupDocs : public GCA<GDoc,GTopic,GFactoryGroupDocs>, public GGroupDocs
{
public:

	/**
	* Constructor.
	* @param f              Factory.
	*/
	GCAGroupDocs(GFactoryGroupDocs* fac)
		: GCA<GDoc,GTopic,GFactoryGroupDocs>("Documents Grouping",otDoc), GGroupDocs(fac) {}

	/**
	 * Class name.
	 */
	virtual R::RCString GetClassName(void) const {return("GCAGroupDocs");}

	/**
	* Configurations were applied from the factory.
	*/
	virtual void ApplyConfig(void);

protected:

	/**
	 * Get a pointer to the objects to cluster.
	 */
	virtual R::RCursor<GDoc> GetObjs(void) const {return(Session->GetDocs());}

	/**
	* Make the grouping for a specific Language.
	*/
	virtual void Run(void)
	{GCA<GDoc,GTopic,GFactoryGroupDocs>::Run(Session);}

public:

	/**
	* Create the parameters.
	* @param params          Parameters to configure.
	*/
	static void CreateParams(R::RConfig* params)
	{GCA<GDoc,GTopic,GFactoryGroupDocs>::CreateParams(params);}
};


//-----------------------------------------------------------------------------
void GCAGroupDocs::ApplyConfig(void)
{
	GCA<GDoc,GTopic,GFactoryGroupDocs>::ApplyConfig(Factory);
	Params.ParamsSim=Factory->FindParam<RParamStruct>("Sim Criterion");
	Params.ParamsAgreement=Factory->FindParam<RParamStruct>("Agreement Criterion");
	Params.ParamsDisagreement=Factory->FindParam<RParamStruct>("Disagreement Criterion");
}


//------------------------------------------------------------------------------
CREATE_GROUPDOCS_FACTORY("GCA for Documents",GCAGroupDocs)
