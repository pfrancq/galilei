/*

	GALILEI Research Project

	GMetaEngineSum.h

	Meta Engine for extraction of results from different search engines - Header.

	Copyright 2001-2003 by the Universit�Libre de Bruxelles.

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
#ifndef GMetaEngineSumH
#define GMetaEngineSumH


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>
#include <gmetaengine.h>
#include <gengine.h>
#include <genginedoc.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------

//------------------------------------------------------------------------------
/**
* The GMetaEngineSum class provides a representation for a results extractor
* for differents Search Engines
* @author Valery Vandaele
* @short results extractor for different search engines
*/
class GMetaEngineSum : public GMetaEngine
{
	/**
	* Container of results.
	* The url, title, description and ranking are store in an objcet GEngineDoc
	*/
	R::RContainer<GEngineDoc,true,false> Results;

	/**
	* The set of keywords
	*/
	R::RContainer<R::RString,true,false> KeyWords;

	/**
	* The set of words to create the query
	*/
	R::RContainer<R::RString,false,false> QueryWords;

	/**
	* specify if the meta engine must use weight associated to the engines.
	*/
	bool UseWeight;

	/**
	* Maximum number of document to return
	*/
	unsigned int MaxResNb;

	/**
	* Number of keywords to keep to create a valid query.
	*/
	unsigned int NValue;

	/**
	* specify if the meta engine must use a single query
	*/
	bool SingleQuery;

	/**
	* specify if the meta engine must use ranch of combinaison of queries
	* If Both SingleQuery and RanchQuery are false ->Use Combinaison of query but not ranched
	*/
	bool RanchQuery;

	/**
	* Specifies wether the values for parametr "P" of the Cnp combinaison must be take
	* as a percentage of the value of "n" or as an absolute value.
	*/
	bool PercentValue;

	/**
	* The Lower value for combinaison Cnp exprimed as a percentage of "n"
	*/
	unsigned int PLowerPC;

	/**
	* The Upper value for combinaison Cnp exprimed as a percentage of "n"
	*/
	unsigned int PUpperPC;

	/**
	* The Lower value for combinaison Cnp exprimed as a absolute value
	*/
	unsigned int PLowerAbs;

	/**
	* The Upper value for combinaison Cnp exprimed as a absolute value
	*/
	unsigned int PUpperAbs;

public:

	/**
	* Constructor.
	* @param fac             Factory.
	*/
	GMetaEngineSum(GFactoryMetaEngine* fac);

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
	* Send a query to the meta Search engine
	* all the available search engines are processed with the query.
	* @param keyWords        The set of keywords on witch the query will be based
	* @param useAllKwds      specifies whether all keywords must be used or only
				 one part defined by the NValue parameter.
	*/
	void Query(RContainer<RString,true,false> &keyWords, bool useAllKwds);

	/**
	* Retreive the results from the searches.
	* The results are first sorted (using the gobal ranking)
	* and then stored in the RContainer
	*/
	void Process();

	/**
	* Get the number of results to use (choosed in the option)
	*/
	size_t GetNbResUsed(void) {return(MaxResNb);};

	/**
	* Clear all results from the search engine
	*/
	void Clear(void);

	/**
	* Add a result from the engine
	* @param url             The url of the document
	* @param title           The title of the document
	* @param desc            The description of the document
	* @param rank            The ranking of the document
	* @param engine          The name of the engine from which the result come
	*/
	virtual void AddResult(R::RString url,R::RString title, R::RString desc, int rank,R::RString engine);

	/**
	* Create the parameters.
	* @param params          Parameters to configure.
	*/
	static void CreateParams(RConfig* params);
private:

	/**
	* Send a query to the meta Search engine
	* all the available search engines are processed with the query.
	* @param keyWords        The set of keywords on witch the query will be based
	* @param NbResuls        The number of results to retreive from each search engine
	*/
	void QueryEngines(R::RContainer<R::RString,false,false> &keyWords);

	/**
	* Function used by "qsort" to sort the results
	*/
	static int sortOrder(const void* a, const void* b);

	/**
	* Function to combine the words of the query
	* @param pos             Value to remember the position of the last
				 inserted element (used for recurtion).
	* @param nbTot           the P value for a combinaison Cnp
	*/
	void CombineQuery(unsigned int pos,unsigned int nbTot);

	/**
	* This function is used to compute the global ranking
	* for documents extracted from different search engines.
	* Results are then sort using this global ranking
	*/
	void ComputeGlobalRanking(void);

	/**
	* Get a cursor of the documents from the search engines
	* @return GEngineDocCursor
	*/
	virtual R::RCursor<GEngineDoc> GetEngineDocs(void);

	/**
	* Destructor.
	*/
	virtual ~GMetaEngineSum(void);
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
