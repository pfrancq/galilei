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
	void AddMIME(const R::RString& mime);

	/**
	* Add a specific scheme for the filter.
	* @param scheme          Name of the scheme.
	*/
	void AddScheme(const R::RString& scheme);

	/**
	 * Get a valid URI (something that a normal RFile can handle) and specify
	 * if the corresponding document must be deleted (because it is temporary
	 * file) or not once the caller does not need it anymore.
    * @param uri            Original URI.
    * @param mustdelete     Must the document be deleted by the caller (modified
	 *                       by the function.
    * @return an URI.
    */
	virtual R::RURI GetValidURI(const R::RURI& uri,bool& mustdelete);

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
	 * Extract a text fragment. This method can be re-implemented by filters.
	 *
	 * @param doc            Document to extract from.
	 * @param begin          Beginning position.
	 * @param end            Ending position.
    * @return a string containing the text fragment.
    */
	virtual R::RString GetTextFragment(GDoc* doc,size_t begin,size_t end);

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
