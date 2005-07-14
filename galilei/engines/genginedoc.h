/*

	GALILEI Research Project

	GEngineDoc.h

	Class to store the results of the extraction from a search engine - Header.

	Copyright 2004 by the Universit�Libre de Bruxelles.

	Authors:
		Valery Vandaele (vavdaele@ulb.ac.be)

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
#ifndef GEngineDocH
#define GEngineDocH


//------------------------------------------------------------------------------
// include files for ANSI C/C++
#include <stdexcept>


//------------------------------------------------------------------------------
// include files for R library
#include <rstd/rstd.h>
#include <rstd/rstring.h>
#include <rstd/rcontainer.h>


//------------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>
#include <granking.h>


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* The GEngineDoc class provides a representation for the storage of
* the results extracted from a search engine
* @author Valery Vandaele
* @short storage of results from a search engine
*/
class GEngineDoc
{
private:

	/**
	* The url of the resulting page.
	*/
	R::RString Url;

	/**
	* The title of the resulting page.
	*/
	R::RString Title;

	/**
	* The description of the resulting page.
	*/
	R::RString Description;

	/**
	* The global ranking for the curent document
	*/
	unsigned int GlobalRank;

	/**
	* The global ranking for the curent document
	*/
	unsigned int GlobalRank2;

	/**
	* Container of all rankings associated to this URL
	*/
	R::RContainer<GRanking,true,false> Rankings;

public:

	/**
	* Construct the document.
	* @param path            Path to find the plugins.
	* @param dlg             Should the dialog box be loaded.
	*/
	GEngineDoc(R::RString url,R::RString title,R::RString description,int rank, R::RString engine);

	/**
	* Get the url of the document
	* @return RString        the url of the document
	*/
	R::RString GetUrl(void) const {return Url;};

	/**
	* Get the Title of the document
	* @return RString        the title of the document
	*/
	R::RString GetTitle(void) const {return Title;};

	/**
	* Get the description of the document
	* @return RString        the description of the document
	*/
	R::RString GetDescription(void) const {return Description;};

	/**
	* Get the global ranking associated to this document
	* #return uint           the global ranking
	*/
	unsigned int GetGlobalRanking(void) const {return GlobalRank;};

	/**
	* Set the value of the global ranking
	* @param r               the new value of the global ranking
	*/
	void SetGlobalRanking(unsigned int r) {GlobalRank=r;};

	/**
	* Get the global ranking associated to this document
	* #return uint           the global ranking
	*/
	unsigned int GetGlobalRanking2(void) const {return GlobalRank2;};

	/**
	* Set the value of the global ranking
	* @param r               the new value of the global ranking
	*/
	void SetGlobalRanking2(unsigned int r) {GlobalRank2=r;};

	/**
	* Add new ranking for the current document
	* @param r               the ranking
	*/
	void AddRanking(GRanking* r);

	/**
	* Add new ranking for the current document
	* @param rank               the rank
	* @param engine             the engine
	*/
	void AddRanking(int rank,R::RString engine);

	/**
	* Method to compare two GEngineDoc
	* @param d           The element to compare
	* @results int       The result of the comparaison
	*/
	int Compare(const GEngineDoc& d) const;

	/**
	* Method to compare two GEngineDoc
	* @param d           The element to compare
	* @results int       The result of the comparaison
	*/
	int Compare(const GEngineDoc* d) const;

	/**
	* Method to compare GEngineDoc
	* @param url         The url to compare
	* @results int       The result of the comparaison
	*/
	int Compare(const R::RString url) const;

	/**
	* Get a cursor on the ranking of the engineDoc.
	* @return GRankingCursor.
	*/
	R::RCursor<GRanking> GetRankingsCursor(void) throw(GException);

	/**
	* Destructor of the URL manager.
	*/
	virtual ~GEngineDoc(void);
};


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
