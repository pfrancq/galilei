/*

	GALILEI Research Project

	GProfilesBehaviours.h

	Agreement and DisAgreement Ratios for subprofiles - Header.

	Copyright 2003 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).
		David Wartel (dwartel@ulb.ac.be).

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



//------------------------------------------------------------------------------
#ifndef GProfilesBehavioursH
#define GProfilesBehavioursH


//------------------------------------------------------------------------------
// include files for GALILEI
#include <sessions/galilei.h>


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* The GProfilesBehaviours class manage the agreement and disagreeemnt ratio
*for subprofiles
* @author Pascal Francq and David Wartel
* @short SubProfiles agreement and disagreement ratio.
*/
class GProfilesBehaviours
{
	// Internal Class
	class GProfilesBehaviour;

	/**
	*/
	R::RContainer<GProfilesBehaviour,unsigned int,true,true> Ratios;

	/**
	* Session managing the (sub)profiles and the documents.
	*/
	GSession* Session;

	/*
	* Must the behaviours be stock in a container
	* or be recomputed each time
	*/
	bool Memory;

public:

	/**
	* Constructor of Users
	* @param lang           Language of the subprofile.
	* @param memory      use container to stock behaviours in memory?
	*/
	GProfilesBehaviours(GSession* session, bool memory) throw(std::bad_alloc);

	/**
	* Re-initialize the similarities. This method can be used for testing
	* purpose when it is necessary to start from different initial conditions.
	*/
	void ReInit(void) throw(std::bad_alloc);

	/**
	* Updates the different ratios.
	*/
	void Update(void) throw(std::bad_alloc);

	/**
	* returns the status of memory
	*/
	bool GetMemory(void) {return Memory;}

	/**
	* Return the disagreement ratio between two subprofiles .
	* @param sub1           The Pointer to the first subprofile
	* @param sub2           The Pointer to the second subprofile
	*/
	double GetDisagreementRatio(GSubProfile* sub1,GSubProfile* sub2) throw(GException);

	/**
	* Return the agreement ratio between two subprofiles .
	* @param sub1           The Pointer to the first subprofile
	* @param sub2           The Pointer to the second subprofile
	*/
	double GetAgreementRatio(GSubProfile* sub1,GSubProfile* sub2) throw(GException);

	/**
	* Add a subprofile to the listof the modified one.
	* @param sub             Pointer to the subprofile.
	*/
	void AddModifiedProfile(GSubProfile* sub) throw(std::bad_alloc,GException);

	/**
	* returns the current session
	*/
	GSession* GetSession(void) {return Session;}

	/**
	* Destructor.
	*/
	virtual ~GProfilesBehaviours(void);
};


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
