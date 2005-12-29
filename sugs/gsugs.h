/*

	GALILEI Research Project

	GSugs.h

	Create suggestions - Header.

	Copyright 2003-2005 by the Université Libre de Bruxelles.

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



//------------------------------------------------------------------------------
#ifndef GSugsH
#define GSugsH


//------------------------------------------------------------------------------
// include files for GALILEI
#include <gpostgroup.h>


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* The GSugs provides a method to compute for each profile a set of potential
* relevant documents ordred by similarity.
* @author Pascal Francq
* @short Suggestions.
*/
class GSugs : public GPostGroup
{
	/**
	* Array of documents.
	*/
	R::RContainer<GFdbk,false,false>* Docs;

	bool UseGroupsSugs;

	bool UseLevels;

	unsigned int NbSugs;

public:

	/**
	* Constructor.
	* @param fac             Factory.
	*/
	GSugs(GFactoryPostGroup* fac);

	/**
	* Create the parameters.
	* @param params          Parameters to configure.
	*/
	static void CreateParams(GParams* params);

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
	* Make the grouping for a specific Language.
	*/
	virtual void Run(void);

	/**
	* Destructor.
	*/
	virtual ~GSugs(void);
};


}  //-------- End of namespace GALILEI----------------------------------------


//-----------------------------------------------------------------------------
#endif

