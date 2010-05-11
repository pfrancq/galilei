/*

	GALILEI Research Project

	Sugs.h

	Create suggestions - Header.

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



//------------------------------------------------------------------------------
#ifndef SugsH
#define SugsH


//------------------------------------------------------------------------------
// include files for GALILEI
#include <gcomputesugs.h>
#include <gsugs.h>
using namespace R;
using namespace GALILEI;


//------------------------------------------------------------------------------
/**
* The GSugs provides a method to compute for each profile a set of potential
* relevant documents ordered by similarity.
* @author Pascal Francq
* @short Suggestions.
*/
class Sugs : public GComputeSugs
{
	/**
	* Suggestion.
	*/
	GSugs TheSugs;

	/**
	 * Use the confidence levels.
	 */
	bool UseLevels;

	/**
	 * Number of suggestions.
	 */
	size_t NbSugs;

public:

	/**
	* Constructor.
	* @param session         Session.
	* @param fac             Factory.
	*/
	Sugs(GSession* session,GPlugInFactory* fac);

	/**
	* Create the parameters.
	*/
	virtual void CreateConfig(void);

	/**
	* Configurations were applied from the factory.
	*/
	virtual void ApplyConfig(void);

    /**
	* Make the grouping for a specific Language.
	*/
	virtual void Run(void);
};


//-----------------------------------------------------------------------------
#endif

