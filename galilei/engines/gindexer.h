/*

	GALILEI Research Project

	GIndexer.h

	Indexer - Header.

	Copyright 2004-2009 by Pascal Francq (pascal@francq.info).
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
#include <rvectorint.h>
#include <rbinaryfile.h>


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
* - The vectors representing the objects (GWeightInfos) in ".desc" files.
* - The XML structure of the documents (GDocStruct) in ".struct" files.
* - The index of the objects ".index" files.
*
* Each session has its own directory (the name of the corresponding world) and
* each type of objects has its own directory in it ("Documents", "Profiles",
* etc).
* @author Pascal Francq
* @short Indexer.
*/
class GIndexer : virtual public GBasicSession
{
	/**
	 * Root dir for the documents.
	 */
	R::RString DirDocs;

	/**
	 * Root dir for the profiles.
	 */
	R::RString DirProfiles;

	/**
	 * Root dir for the communities.
	 */
	R::RString DirCommunities;

	/**
	 * Root dir for the topics.
	 */
	R::RString DirTopics;

	/**
	 * Inverted file for documents.
	 */
	R::RBinaryFile* DocsInvertedFile;

public:

	/**
	* Construct the indexer.
	*/
	GIndexer(void);

	/**
	 * Apply the configuration.
	 */
	void Apply(void);

	/**
	* Clear the information of a given object type related to the indexing.
	* @param objtype         Type of the object.
	*/
	void Clear(tObjType objtype);

	/**
	* Load the description of a given object.
	* @param infos           Container that will hold the description.
	* @param type            Type of the object (otDoc,otProfile,otCommunity,otTopic).
	* @param id              Identifier of the object.
	*/
	void LoadInfos(GWeightInfos& infos,tObjType type,size_t id);

	/**
	* Save the description of a given object.
	* @param infos           Container that will hold the description.
	* @param type            Type of the object (otDoc,otProfile,otCommunity,otTopic).
	* @param id              Identifier of the object.
	*/
	void SaveInfos(GWeightInfos& infos,tObjType type,size_t id);

	/**
	 * Method that load the structure of a document.
	 * @param docstruct      Structure of the document.
	 * @param doc            Document.
	 */
	void LoadStruct(GDocStruct& docstruct,GDoc* doc);

	/**
	 * Method that save the structure of a document.
	 * @param docstruct      Structure of the document.
	 * @param doc            Document.
	 */
	void SaveStruct(GDocStruct& docstruct,GDoc* doc);

private:

	/**
	 * Build the buffer for a range of concepts.
	 * @param buffer                   Buffer to fill.
	 * @param min                      "Minimum" concept.
	 * @param max                      "Maximum" concept.
	 */
	void BuildBuffer(size_t* buffer,size_t min,size_t max);

public:

	/**
	 * Build the reference for a given type of objects.
	 * @param type            Type of objects to search for.
	 * @param slot            Slot to use.
	 *
	 * The indexer will use a buffer to build part of the index. It parses the
	 * whole document collection as many times as necessary to treat all the
	 * concepts. The number of passes increases if the size of the buffer
	 * decreases.
	 */
	void BuildRefs(tObjType type,GSlot* slot=0);

	/**
	* Find all the references of a given concept.
	* @param concept         Concept to search for.
	* @param refs            Vector that will contain the references. The
	*                        vector is emptied and the documents identifiers
	*                        are ordered.
	* @param type            Type of objects to search for.
	*
	* Here is an example of a search for all the documents containing the
	* English word "connection":
	* @code
	* GConceptType* English(Session->GetConceptType("enTerms",false));  // Get a pointer to the English dictionary.
	* RString Word(English->GetLang()->GetStemming("connection"));      // Get the stem to search for.
	* GConcept* Concept(English->GetConcept(Word));                     // Get the corresponding concept.
	* RVectorInt<size_t,false> Docs(20);                                // Vector that will contain the documents identifiers.
	* Session->FindRefs(Concept,Docs,otDoc);                            // Make the search.
	* for(Docs.Start();!Docs.End().Docs.Next())
	*    cout<<Docs()<<"\t";                                            // Print the identifiers.
	* cout<<endl;
	* @endcode
	*/
	void FindRefs(GConcept* concept,R::RVectorInt<size_t,false>& refs,tObjType type);

	/**
	* Destruct the indexer.
	*/
	virtual ~GIndexer(void);
};


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
