/*

	GALILEI Research Project

	GIndexer.h

	Indexer - Header.

	Copyright 2004-2010 by Pascal Francq (pascal@francq.info).
	Copyright 2004-2008 Université Libre de Bruxelles (ULB).

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
#ifndef GIndexer_H
#define GIndexer_H


//------------------------------------------------------------------------------
// include files for R Project
#include <rnumcontainer.h>
#include <rconfig.h>
#include <rindexfile.h>


//------------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>
#include <gbasicsession.h>


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* The GIndexer class provides a class to manage the indexing of documents,
* profiles, topics and communities. In practice, it manages binary files:
* - The vectors representing the objects (GWeightInfos) are stored ".desc"
*   files.
* - The XML structure of the documents (GDocStruct) is stored in ".struct"
*   files.
* - The index of the objects are stored in ".index" files.
*
* Each session has its own directory (the name of the corresponding world).
* @author Pascal Francq
* @short Indexer.
*/
class GIndexer : virtual public GBasicSession
{
	class IndexType;

	/**
	 * File containing the structure of the documents.
	 */
	R::RIndexFile* StructDoc;

	/**
	 * Temporary vector of references.
	 */
	R::RNumContainer<size_t,true> tmpRefs;

	/**
	 * Types managed by the indexer.
	 */
	R::RContainer<IndexType,true,true> Types;

	/**
	 * Names of the types managed by the indexer.
	 */
	R::RContainer<R::RString,true,true> TypesNames;

public:

	/**
	* Construct the indexer.
	*/
	GIndexer(void);

	/**
	 * @return the names of the objects types supported.
	 */
	R::RCursor<R::RString> GetTypesNames(void) const {return(R::RCursor<R::RString>(TypesNames));}

	/**
	 * Create the configuration.
	 * @param config         Config file.
	 */
	void CreateConfig(R::RConfig* config);

	/**
	 * Apply the configuration.
	 * @param config         Config file.
	 */
	void Apply(R::RConfig* config);

	/**
	* Clear the structure file of documents.
	*/
	void ClearStruct(void);

	/**
	* Clear the descriptions of a given object type.
	* @param objtype         Type of the object.
	*/
	void ClearDesc(tObjType objtype);

	/**
	* Clear the index of a given object type.
	* @param objtype         Type of the object.
	*/
	void ClearIndex(tObjType objtype);

	/**
	 * Flush the caches of a given object type: All the blocks in memory that
	 * are dirtied are save on disk. This can be useful to do after a long
	 * computation (such as the document analysis).
	 * @param objtype         Type of the object.
	 */
	void FlushDesc(tObjType objtype);

	/**
	* Load the description of a given object.
	* @param infos           Container that will hold the description. If null,it is created.
	* @param type            Type of the object (otDoc,otProfile,otCommunity,otTopic).
	* @param blockid         Identifier of the block of the object.
	* @param id              Identifier of the object.
	*/
	void LoadInfos(GWeightInfos* &infos,tObjType type,size_t blockid,size_t id);

	/**
	* Save the description of a given object.
	* @param infos           Container that will hold the description.
	* @param type            Type of the object (otDoc,otProfile,otCommunity,otTopic).
	* @param blockid         Identifier of the block of the object (0 means the block will be found).
	* @param id              Identifier of the object.
	*/
	void SaveInfos(const GWeightInfos& infos,tObjType type,size_t& blockid,size_t id);

	/**
	 * Method that load the structure of a document.
	 * @param docstruct      Structure of the document. If null,it is created.
	 * @param blockid        Identifier of the block of the object.
	 * @param id             Identifier of the object.
	 */
	void LoadStruct(GDocStruct* &docstruct,size_t blockid,size_t id);

	/**
	 * Method that save the structure of a document.
	 * @param docstruct      Structure of the document.
  	 * @param blockid        Identifier of the block of the object (0 means the
  	 *                       block will be found).
 	 * @param id             Identifier of the object.
	 */
	void SaveStruct(GDocStruct* docstruct,size_t& blockid,size_t id);

	/**
	 * Update the index of a given type. In practice, the vector is parsed and,
	 * for each concept, the inverted file is update.
	 * @param infos          Vector.
	 * @param type           Type of the object.
	 * @param id             Identifier of the object.
	 * @param add            Object must be added or removed from the index.
	 */
	void UpdateRefs(const GWeightInfos& infos,tObjType type,size_t id,bool add);

	/**
	* Find all the references of a given concept for a given objects type.
	* @param concept         Concept to search for.
	* @param refs            Vector that will contain the references. The
	*                        vector is emptied and the objects identifiers
	*                        are ordered.
	* @param type            Type of objects to search for.
	*
	* Here is an example of a search for all the documents containing the
	* English word "connection":
	* @code
	* GConceptType* English(Session->GetConceptType("enTerms",false));         // Get a pointer to the English dictionary.
	* GConcept Word(English,English->GetLang()->GetStemming("connection"));    // Create a concept.
	* GConcept* Concept(Session->InsertConcept(Word));                         // Get the corresponding "real" concept.
	* RNumContainer<size_t,true> Docs(20);                                     // Vector that will contain the documents identifiers.
	* Session->LoadRefs(Concept,Docs,otDoc);                                   // Make the search.
	* for(Docs.Start();!Docs.End();Docs.Next())
	*    cout<<Docs()<<"\t";                                                   // Print the identifiers.
	* cout<<endl;
	* @endcode
	*/
	void LoadRefs(GConcept* concept,R::RNumContainer<size_t,true>& refs,tObjType type);

	/**
	 * Build the references from scratch for a given objects type.
	 * @param type           Type of the objects.
	 * @param slot           Slot receiving the information.
	 */
	void BuildRefs(tObjType type,GSlot* slot=0);

	/**
	* Destruct the indexer.
	*/
	virtual ~GIndexer(void);
};


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
