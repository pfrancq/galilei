/*

	GALILEI Research Project

	GStemming.h

	Stemming Algorithms - Header.

	Copyright 2011-2014 by Pascal Francq (pascal@francq.info).

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
#ifndef GStemmingH
#define GStemmingH


//------------------------------------------------------------------------------
// include files for R/GALILEI projects
#include <ganalyzer.h>
using namespace GALILEI;
using namespace R;
using namespace std;


//-----------------------------------------------------------------------------
/**
 * The GStemming class provides a way to stem the words.
 * @short Stemming Algorithms
 */
class GStemming : public GAnalyzer
{
public:

	/**
	* Construct the analyzer.
	* @param session        Session.
	* @param fac            Factory.
	*/
	GStemming(GSession* session,GPlugInFactory* fac);

	/**
	 * Extract concepts from current tokens. It is the responsibility of the
	 * analyzer to eventually delete some tokens (for example stopwords) or to
	 * replace certain tokens by others (such as replacing a word by its stem).
	 * @param analyzer       Analyzer.
	 */
	virtual void TreatTokens(GDocAnalyze* analyzer);
};




//------------------------------------------------------------------------------
#endif
