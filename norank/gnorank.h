/*

	GALILEI Research Project

	GNoRank.h

	No Ranking Method - Header.

   Copyright 2015-2015 by Pascal Francq.

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
#ifndef GNoRankH
#define GNoRankH


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <gcomputerank.h>
using namespace std;
using namespace GALILEI;
using namespace R;


//------------------------------------------------------------------------------
/**
* The GNoRank class provides a representation for a ranking method that does
* nothing.
* @short No Ranking Method.
*/
class GNoRank : public GComputeRank
{
public:

	/**
	* Constructor.
	* @param session         Session.
	* @param fac             Factory.
	*/
	GNoRank(GSession* session,GPlugInFactory* fac);

	/**
	 * Rank a set of document fragments.
	 * @param engine         Engine which results must be ranked.
	 * @param query          Query.
	 */
	virtual void Rank(GEngine* engine,GSearchQuery* query);

	/**
	* Create the parameters.
	*/
	virtual void CreateConfig(void);

	/**
	* Configurations were applied from the factory.
	*/
	virtual void ApplyConfig(void);
};


//-----------------------------------------------------------------------------
#endif
