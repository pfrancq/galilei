/*

	GALILEI Research Project

	GStatsSims.h

	Groups Evaluation - Header.

	Copyright 2002-2003 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).

	Version $Revision$

	Last Modify: $Date$

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
#ifndef GStatsSimsH
#define GStatsSimsH


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>
#include <sessions/gstatscalc.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
* The GStatsSims class provides a method to compute different measures on the
* groups.
* @author Pascal Francq
* @short Groups Evaluation.
*/
class GStatsSims : public GStatsCalc
{
	/**
	* Statistics on Documents.
	*/
	bool Docs;

	/**
	* Statistics between profiles and documents.
	*/
	bool ProfDoc;

	/**
	* Statistics between groups and documents.
	*/
	bool GroupDoc;

	/**
	* Statistics on profiles.
	*/
	bool Profiles;

	/**
	* Statistics on the number of documents assessed by more than one profile.
	*/
	bool SameDocProf;

	/**
	* Statistics between groups and profiles.
	*/
	bool GroupProf;

	/**
	* Should the details of the computation be stored in a file.
	*/
	bool File;

	/**
	* Name of the file.
	*/
	R::RString Name;

	/**
	* Determine if the statistics should be computed with the idf/isf factors.
	*/
	bool WithFactors;

	/**
	* Determine if the statistics should be computed withoutom the idf/isf factors.
	*/
	bool WithoutFactors;

public:

	/**
	* Constructor.
	* @param fac             Factory.
	*/
	GStatsSims(GFactoryStatsCalc* fac) throw(bad_alloc);

	/**
	* Configurations were applied from the factory.
	*/
	virtual void ApplyConfig(void);

	/**
	* Connect to a Session.
	* @param session         The session.
	*/
	virtual void Connect(GSession* session);

	/**
	* Disconnect from a Session.
	* @param session         The session.
	*/
	virtual void Disconnect(GSession* session);

	/**
	* Compute the statistics.
	* @param res             XML tag that will be hold the results.
	*/
	virtual void Compute(R::RXMLTag& res);

	/**
	* Create the parameters.
	* @param params          Parameters to configure.
	*/
	static void CreateParams(GParams* params);

public:

	/**
	* Destructor.
	*/
	virtual ~GStatsSims(void);
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
