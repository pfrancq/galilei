/*

	GALILEI Research Project

	GDoc.h

	Document - Header.

	Copyright 2001-2014 by Pascal Francq (pascal@francq.info).
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
#ifndef GDocH
#define GDocH


//------------------------------------------------------------------------------
// include files for R
#include <rnumcontainer.h>
#include <ruri.h>


//------------------------------------------------------------------------------
// include files for GALILEI
#include <gdescriptionobject.h>


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* The GDoc class provides a representation of a analyzed document. In fact, a
* document is represented as a set of vectors and, eventually, its concept tree
* (the positions and the depths of each concept occurrence). In the tensor space
* model, the (hyper)links between documents are represented as a vector
* corresponding to concept category "link"
*
* @warning Since the concept tree is memory consuming, it is not associated with
* the document and must be managed by the developer.
* @short Document.
*/
class GDoc : public GDescriptionObject<GDoc>
{
protected:

	/**
	* URI of the document.
	*/
	R::RURI URI;

	/**
	* Language of the description.
	*/
	GLang* Lang;

	/**
	* MIME Type of the document.
	*/
	R::RString MIMEType;

	/**
	* Date of last update of document content.
	*/
	R::RDate Updated;

	/**
	* Date of last document computation.
	*/
	R::RDate Computed;

	/**
	* Identifiers of the profiles which have assessed the document.
	*/
	R::RNumContainer<size_t,true>* Fdbks;

	/**
	* Identifiers of the corresponding topic.
	*/
	size_t GroupId;

	/**
	* Date of the attachment of the document into the topic.
	*/
	R::RDate Attached;

	/**
	 * Block identifier of the struct.
	 */
	size_t StructId;

	/**
	 * Tree representing the document.
    */
	GConceptTree* Tree;

public:

	/**
	* Construct a new document.
	* @param session         Session.
	* @param uri             URI of the document.
	* @param name            Name of the document.
	* @param lang            Language of the document.
	* @param mime            MIME type of the document.
	*/
	GDoc(GSession* session,const R::RURI& uri,const R::RString& name,GLang* lang,const R::RString& mime);

	/**
	* Construct a document.
	* @param session         Session.
	* @param uri             URI of the document.
	* @param name            Name of the document.
	* @param id              Identifier of the document.
	* @param blockid         Identifier of the block for the description.
	* @param structid        Identifier of the block for the structure.
	* @param lang            Language of the document.
	* @param mime            MIME type of the document.
	* @param grpid           Topic identifier.
	* @param c               Date of the last computation.
	* @param u               Date of the last updated.
	* @param a               Date of the last attached.
	*/
	GDoc(GSession* session,const R::RURI& uri,const R::RString& name,size_t id,size_t blockid,size_t structid,GLang* lang,const R::RString& mime,size_t grpid,const R::RDate& c,const R::RDate& u,const R::RDate& a);

	/**
    * @return the class name.
    */
	virtual R::RCString GetClassName(void) const {return("GDoc");}

	/**
	 * Defines that the documents have concept trees.
	 * @return true.
	 */
	static inline bool HasTree(void){return(true);}

	/**
	 * Get a string that represents the object when a search has to be
	 * performed in the list. For the document, it is the URL.
	 * @return the string used for searching an object.
	 */
	virtual R::RString GetSearchStr(void) const;

	/**
	* Compare two documents by comparing their identifier.
	* @see R::RContainer
	* @param doc             Document.
	* @return int
	*/
	int Compare(const GDoc& doc) const;

	/**
	* Compare two documents by comparing their identifier.
	* @see R::RContainer
	* @param doc             Pointer to the document.
	* @return int
	*/
	int Compare(const GDoc* doc) const;

	/**
	* Compare the identifier of a documents with a given identifier.
	* @see R::RContainer
	* @param id              Identifier.
	* @return int
	*/
	int Compare(const size_t id) const;

	/**
	* Compare the name of a documents with a given name.
	* @see R::RContainer
	* @param name             Name.
	* @return int
	*/
	int Compare(const R::RString& name) const;

	/**
	* Compare the language of a document with a given language.
	* @see R::RContainer and GDocs.
	* @param lang            Pointer to the language.
	* @return int
	*/
	int Compare(const GLang* lang) const;

	/**
	 * Clear the information associated with the document.
	 * @param disk            Clear the reference to the corresponding block on
	 *                        disk.
	 */
	void ClearInfos(bool disk);

	/**
	 * Clear the concept tree associated with the document.
	 * @param disk            Clear the reference to the corresponding block on
	 *                        disk.
	 */
	void ClearTree(bool disk);

	/**
	 * Release the tree.
    */
	void ReleaseTree(void);

	/**
	 * Load the concept tree of the document.
	 * @param tree           Reference to a pointer to the tree. If null, the
	 *                       tree is created.
	 */
	void LoadTree(GConceptTree* &tree) const;

	/**
    * @return the concept tree (and load it if necessary).
    */
	GConceptTree* GetTree(void) const;

private:

	/**
	 * Associate a concept tree with a document.
	 *
	 * The method is only called by GDocAnalyze to associate a tree to a document
	 * loaded into memory but not in the storage.
    * @param tree           Tree to associate.
    */
	void SetTree(GConceptTree& tree);

public:

	/**
	* Get the URI.
	* @return the URI.
	*/
	R::RURI GetURI(void) const {return(URI);}

	/**
	* Set the URI.
	* @param uri             URI to assign.       .
	*/
	void SetURI(R::RURI uri);

	/**
	* @return Pointer to the Language.
	*/
	inline GLang* GetLang(void) const {return(Lang);}

	/**
	* Get the date of the last update of the document content.
	* @return the date.
	*/
	R::RDate GetUpdated(void) const {return(Updated);}

	/**
	* Get the date of the last analysis of the document.
	* @return the date.
	*/
	R::RDate GetComputed(void) const {return(Computed);}

	/**
	* Get the MIME type of the document.
	* @returns String.
	*/
	R::RString GetMIMEType(void) const {return(MIMEType);}

	/**
	* Set the MIME type of the document.
	* @param mime            C String representing the MIME type.
	*/
	void SetMIMEType(const R::RString& mime);

	/**
	* Verify if the document must be (re-)computed.
	*/
	bool MustCompute(void) const;

	/**
	* Get the date of the last attachment.
	* @return the date.
	*/
	R::RDate GetAttached(void) const {return(Attached);}

	/**
	* Get the topic holding the document.
	*/
	size_t GetGroupId(void) const {return(GroupId);}

	/**
	* Set the topic holding the document.
	* @param groupid         Identifier of the topic.
	*/
	void SetGroup(size_t groupid);

	/**
	 * Get the identifier of the block containing the structure. If null, it
	 * means that the object is not defined.
	 * @return Identifier of the block.
	 */
	inline size_t GetStructId(void) const {return(StructId);}

	/**
	* @return a cursor on the identifier of the profiles which have assesses the
	* document.
	*/
	R::RNumCursor<size_t> GetFdbks(void) const;

	/**
	* Add a profile to the list of those which have assess the document.
	* @param profileid       Identifier of the profile.
	* @return true if it is a new feedback.
	*/
	bool InsertFdbk(size_t profileid);

	/**
	* Delete a profile from the list of those which have assess the document.
	* @param profileid       Identifier of the profile.
	*/
	void DeleteFdbk(size_t profileid);

	/**
	* Clear all the assessments on the document.
	*/
	void ClearFdbks(void);

	/**
	* Compute the agreement ratio between two documents, i.e. the ratio between
	* the number of profiles assessing them as relevant, and the total number
	* of profiles assessing them.
	* @param doc             Second document.
	* @param nbmin           Minimum number of common profiles for a non null
	*                        ratio.
	*/
	double GetAgreementRatio(const GDoc* doc,size_t nbmin) const;

	/**
	* Compute the disagreement ratio between two documents, i.e. the ratio
	* between the number of profiles assessing them one as relevant and the
	* other one differently by the other one, and the total number of profiles
	* assessing them.
	* @param doc             Second document.
	* @param nbmin           Minimum number of common profiles for a non null
	*                        ratio.
	*/
	double GetDisagreementRatio(const GDoc* doc,size_t nbmin) const;

	/**
	* Get the number of times the documents was assessed.
	*/
	size_t GetNbFdbks(void) const;

	/**
	* Set the date of the last update of the document content.
	* @param doc             Document updated.
	* @param date            Date of the update.
	*/
	void SetUpdated(R::RDate& date);

	/**
	* Update the representation of the document once a computation was done. The
	* computed date and the status are updated.
	*
	* If the document is an internal one, the following steps are done:
	* -# An 'hDocs[oeAboutToUpdated]' notification is send.
	* -# The references are updated.
	* -# Existing in memory description and tree are replaced.
	* -# If necessary, the document, its description and its tree are saved.
	* -# An 'hDocs[oeUpdated]' notification is send.
	*
	* If the document is an external one, its description and tree are
	* replaced.
	* @param lang            Language to assign.
	* @param desc            Description to assign.
	* @param tree            Tree to assign.
	* \warning The description and the tree are cleared by this method.
	*/
	void Update(GLang* lang,GDescription& desc,GConceptTree& tree);


	/**
	* Destruct the document.
	*/
	virtual ~GDoc(void);

	friend class GSession;
	friend class GIndexer;
	friend class GDocAnalyze;
   friend class GObjects<GDoc,hDocs>;
};


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
