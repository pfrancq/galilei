/*

	GALILEI Research Project

	GAnalyzer.h

	Token Analyzer - Header.

	Copyright 2001-2014 by Pascal Francq (pascal@francq.info).

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
#ifndef GAnalyzerH
#define GAnalyzerH


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <gplugin.h>
#include <gtoken.h>


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
 * The GAnalyzer provides a class to treat a set of tokens extracted from the
 * document currently analyzed. An analyzer can ask to treat each token or
 * each occurrence (depending on their type).
 * @short Token Analyzer
 */
class GAnalyzer : public GPlugIn
{
public:

	/**
	* Construct the analyzer.
	* @param session        Session.
	* @param fac            Factory.
	*/
	GAnalyzer(GSession* session,GPlugInFactory* fac);

	/**
	 * Treat the tokens extracted from the document. In fact, the analyzer can
	 * treat each token or each single occurrence.
	 * @param analyzer       Analyzer.
	 */
	virtual void TreatTokens(GDocAnalyze* analyzer)=0;

	/**
	 * Destructor.
	 */
	virtual ~GAnalyzer(void);

	friend class GDocAnalyze;
};


//-------------------------------------------------------------------------------
#define CREATE_ANALYZER_FACTORY(name,desc,plugin)\
	CREATE_FACTORY(GALILEI::GAnalyzer,plugin,"Analyzer",R::RString::Null,name,desc)


}  //-------- End of namespace GALILEI -----------------------------------------


//-----------------------------------------------------------------------------
#endif
