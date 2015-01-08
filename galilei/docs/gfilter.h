/*

	GALILEI Research Project

	GFilter.h

	Generic Filter for documents - Header.

	Copyright 2001-2015 by Pascal Francq (pascal@francq.info).
	Copyright 2001-2008 Université Libre de Bruxelles (ULB).

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
#ifndef GFilterH
#define GFilterH


//------------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>
#include <gplugin.h>
#include <gpluginmanager.h>


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
 * The GFilter class provides a generic filter that extracts text from a
 * document of a given type. In practice, the filter calls the methods Extract*
 * from the GDocAnalyze class.
 *
 * Each filter is associated with a given set of MIME types. This association
 * is used to determine which filter should extracts the text from a document.
 *
 * See the documentation related to GPlugIn for more general information.
 * @author Pascal Francq
 * @short Generic Filter.
 */
class GFilter : public GPlugIn
{
public:

	/**
	* Construct the filter.
	* @param session        Session.
	* @param fac            Factory.
	*/
	GFilter(GSession* session,GPlugInFactory* fac);

	/**
	* Add a specific MIME-Type for the filter.
	* @param mime           Name of the MIME-Type.
	*/
	void AddMIME(R::RString mime);

	/**
	* Analyze a document with a given URI. This method must be re-implemented by
   * all filters.
	* @param analyzer        Analyzer.
	* @param doc             Document to analyze.
	* @param file            File to analyze (eventually a local copy of a
	*                        remote document).
	*/
	virtual void Analyze(GDocAnalyze* analyzer,const GDoc* doc,const R::RURI& file)=0;

	/**
	 * Extract a text fragment from a file with a given URI. This method should
	 * be re-implemented by filters.
	 *
	 * By default, it opens a simple text file in utf-8 and extract the text
	 * window defined (stopping at the first space or punctuation character).
	 * @param fragment       Fragment to extract.
    * @return a string containing the text fragment.
    */
	virtual R::RString GetTextFragment(GDocFragment* fragment);

	/**
	* Destruct the filter.
	*/
	virtual ~GFilter(void);

	friend class GDocAnalyze;
};


//-------------------------------------------------------------------------------
#define CREATE_FILTER_FACTORY(name,desc,plugin)\
	CREATE_FACTORY(GALILEI::GFilter,plugin,"Filter",R::RString::Null,name,desc)


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
