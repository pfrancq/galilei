/*

	GALILEI Research Project

	GEngineDoc.h

	Class to store the results of the extraction from a search engine - Header.

	Copyright 2004 by the Université Libre de Bruxelles.

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
#include <rstd.h>
#include <rstring.h>
#include <rcontainer.h>


//------------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>
#include <granking.h>


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* The GEngineDoc class provides a representation for a document proposed by a
* search engine (with its ranking).
* @author Valery Vandaele
* @short Search Engine Result
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
	* The global ranking for the current document
	*/
	size_t GlobalRank;

	/**
	* The global ranking for the current document
	*/
	size_t GlobalRank2;

	/**
	* Container of all rankings associated to this URL
	*/
	R::RContainer<GRanking,true,false> Rankings;

public:

	/**
	* Constructor of the engine.
	* @param url             URL of the document.
	* @param title           Title of the document.
	* @param description     Description of the document.
	* @param rank            Ranking of the document.
	* @param engine          Name of the engine that retrieves it.
	*/
	GEngineDoc(R::RString url,R::RString title,R::RString description,int rank, R::RString engine);

	/**
	* Get the url of the document
	*/
	R::RString GetUrl(void) const {return(Url);}

	/**
	* Get the Title of the document
	*/
	R::RString GetTitle(void) const {return(Title);}

	/**
	* Get the description of the document
	*/
	R::RString GetDescription(void) const {return(Description);}

	/**
	* Get the global ranking associated to this document
	*/
	size_t GetGlobalRanking(void) const {return(GlobalRank);}

	/**
	* Set the value of the global ranking
	* @param r               New value of the global ranking
	*/
	void SetGlobalRanking(size_t r) {GlobalRank=r;}

	/**
	* Get the global ranking associated to this document
	*/
	size_t GetGlobalRanking2(void) const {return(GlobalRank2);}

	/**
	* Set the value of the global ranking
	* @param r               New value of the global ranking
	*/
	void SetGlobalRanking2(size_t r) {GlobalRank2=r;}

	/**
	* Add new ranking for the current document
	* @param r               Ranking
	*/
	void AddRanking(GRanking* r);

	/**
	* Add new ranking for the current document
	* @param rank            Rank
	* @param engine          Engine
	*/
	void AddRanking(int rank,R::RString engine);

	/**
	* Method to compare two GEngineDoc
	* @param d               Element to compare
	*/
	int Compare(const GEngineDoc& d) const;

	/**
	* Method to compare two GEngineDoc
	* @param d               Element to compare
	*/
	int Compare(const GEngineDoc* d) const;

	/**
	* Method to compare GEngineDoc
	* @param url             Url to compare
	*/
	int Compare(const R::RString url) const;

	/**
	* Get a cursor on the ranking of the engineDoc.
	* @return GRankingCursor.
	*/
	R::RCursor<GRanking> GetRankings(void);

	/**
	* Destruct.
	*/
	virtual ~GEngineDoc(void);
};


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
