/*

	GALILEI Research Project

	GEngineXML.h

	XML Search Engine - Header.

	Copyright 2010-2015 by Pascal Francq (pascal@francq.info).
   Copyright 2004-2005 by Jean-Baptiste Valsamis.
	Copyright 2005-2009 by Faïza Abbaci.

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
#ifndef GEngineXMLH
#define GEngineXMLH


//-----------------------------------------------------------------------------
// include files for R/GALILEI Projects
#include <robject.h>
#include <gengine.h>
using namespace std;
using namespace R;
using namespace GALILEI;


//------------------------------------------------------------------------------
// forward declaration
class GQuery;
class GQueryRes;


//------------------------------------------------------------------------------
/**
* The GEngineXML class provides a search engine over XML documents
* @short XML Search Engine
*/
class GEngineXML : public RObject, public GEngine
{
	class cTreeRef;

	/**
	* Number of results
	*/
	size_t NbResults;

	/**
	 * Retrieve only documents.
	 */
	bool OnlyDocs;

	/**
	 * Container of the most used document trees.
	 */
	RContainer<cTreeRef,true,true> Trees;

	/**
	 * Beginning synaptic position of a window (<=0).
	 */
	int BeginWindowPos;

	/**
	 * Ending synaptic position of a window (>=0).
	 */
	int EndWindowPos;

	/**
	 * The last query executed.
	 */
	GQuery* Query;

public:

	/**
	* Construct XML search engine.
	* @param session         Session.
	* @param fac             Factory.
	*/
	GEngineXML(GSession* session,GPlugInFactory* fac);

	/**
	* Create the parameters.
	*/
	virtual void CreateConfig(void);

	/**
	* Configurations were applied from the factory.
	*/
	virtual void ApplyConfig(void);

	/**
	* Request a query.
	* @param query           Query.
	*/
	virtual void PerformRequest(const RString& query);

	/**
	 * Get the beginning synaptic position of a window (<=0).
	 */
	size_t GetBeginWindowPos(void) const {return(BeginWindowPos);}

	/**
	 * Get the ending synaptic position of a window (>=0).
	 */
	size_t GetEndWindowPos(void) const {return(EndWindowPos);}

	/**
	 * Get the last query exexuted.
    * @return
    */
	GQuery* GetQuery(void) const {return(Query);}

private:

	/**
	 * Method used to ordered the blocks by descending order of accesses.
	 * @param a              Pointer to a block.
	 * @param b              Pointer to the second block.
	 */
	static int sortOrderAccess(const void* a,const void* b);

public:

	/**
	 * Get a concept tree for a given document. If it is not loaded into memory,
	 * the engine loads it.
	 * @param docid          Identifier of the document.
	 * @return a pointer to the tree.
	 */
	const GConceptTree* GetTree(size_t docid);

	/**
	 * Destructor.
    */
	~GEngineXML(void);


	friend class GProm;
};


//-----------------------------------------------------------------------------
#endif
