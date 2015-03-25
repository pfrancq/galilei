/*

	GALILEI Research Project

	GComputeRank.h

	Compute Document Ranking - Header.

	Copyright 2015-2015 by Pascal Francq (pascal@francq.info).

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
#ifndef GComputeRankH
#define GComputeRankH


//------------------------------------------------------------------------------
//include file for GALILEI
#include <gplugin.h>
#include <gpluginmanager.h>
#include <gdocfragment.h>


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* The GComputeRank class provides a representation for a generic computing
* method to rank a set of document fragments.
*
* See the documentation related to GPlugIn for more general information.
* @short Generic Ranking Method
*/
class GComputeRank : public GPlugIn
{
public:

	/**
	* Construct a ranking method.
	* @param session         Session.
	* @param fac             Factory.
	*/
	GComputeRank(GSession* session,GPlugInFactory* fac);

	/**
	 * Rank a set of document fragments.
	 * @param engine         Engine which results must be ranked.
	 * @param query          Query.
	 */
	virtual void Rank(GEngine* engine,GSearchQuery* query)=0;

	/**
	* Destructor of the ranking method.
	*/
	virtual ~GComputeRank(void);
};


//-------------------------------------------------------------------------------
#define CREATE_COMPUTERANK_FACTORY(name,desc,plugin)\
	CREATE_FACTORY(GALILEI::GComputeRank,plugin,"ComputeRank",R::RString::Null,name,desc)


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
