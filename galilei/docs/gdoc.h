/*

	GALILEI Research Project

	GDoc.h

	Document - Header.

	Copyright 2001-2012 by Pascal Francq (pascal@francq.info).
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
* document is represented as a set of vectors and, eventually, its structure
* (the positions and the depths of each concepts contained).
*
* @warning Since the structure is memory consuming, it is not loaded by default
* and it is recommended it to release it when not used anymore.
* @author Pascal Francq
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
	 * Structure of the document.
	 */
	GConceptTree* Struct;

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
	* Date of last document's analysis.
	*/
	R::RDate Computed;

	/**
	* Identifiers of the profiles which have assessed the document.
	*/
	R::RNumContainer<size_t,true>* Fdbks;

	/**
	* Links "out" of the document.
	*/
	R::RContainer<GLink,true,true> LinkSet;

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
	 * Method used to correctly instantiate some template methods.
	 */
	void PrivateInit(void);

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
	 * Defines of the documents have a structure.
	 * @return true.
	 */
	static inline bool HasStruct(void){return(true);}

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
	 * Clear the structure associated with the document.
	 * @param disk            Clear the reference to the corresponding block on
	 *                        disk.
	 */
	void ClearStruct(bool disk);

	/**
	 * Get the structure of the document.
	 */
	GConceptTree* GetStruct(void) const;

	/**
	 * Release the structure of a document from the memory.
	 */
	void ReleaseStruct(void);

	/**
	* Get the URL.
	* @returns RURI.
	*/
	R::RURI GetURI(void) const {return(URI);}

	/**
	* @return Pointer to the Language.
	*/
	inline GLang* GetLang(void) const {return(Lang);}

	/**
	* Set the name of the document.
	* @param name            Name.
	*/
	void SetName(const R::RString& name);

	/**
	* Get the date of the last update of the document content.
	* @returns Pointer to date.
	*/
	R::RDate GetUpdated(void) const;

	/**
	* Get the date of the last analysis of the document.
	* @returns Pointer to date.
	*/
	R::RDate GetComputed(void) const;

	/**
	* Get the MIME type of the document.
	* @returns String.
	*/
	R::RString GetMIMEType(void) const;

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
	* @returns R::RDate.
	*/
	R::RDate GetAttached(void) const;

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
	* Get a cursor on the identifier of the profiles which have assesses the
	* documents.
	* @return R::RVector*.
	*/
	R::RNumContainer<size_t,true>* GetFdbks(void) const;

	/**
	* Add a profile to the list of those which have assess the document.
	* @param id              Identifier of the profile.
	*/
	void InsertFdbk(size_t id);

	/**
	* Delete a profile from the list of those which have assess the document.
	* @param id              Identifier of the profile.
	*/
	void DeleteFdbk(size_t id);

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
	* Get the number of outgoing links
	*/
	size_t GetNbLinks(void) const;

	/**
	* Add a new link to the document and set the number of occurrences of this
	* link.
	* @param doc             The Document representing the link to be inserted.
	* @param nboccurs        The number of occurrences of the link inside the
	*                        document. If the number of occurrences is set to 0,
	*                        an occurrence number of 1 is used.
	*/
	void InsertLink(const GDoc* doc,size_t nboccurs=0);

	/**
	* Get a cursor on the Links of the document.
	*/
	R::RCursor<GLink> GetLinks(void) const;

private:

	/**
	* Assign a new description to the document.
	* @param lang            Pointer to the language.
	* @param desc            Description.
	* @param ram             Must the information be maintained is RAM.
	* @param delref          Delete the references (must be set to true if the
	*                        document has already a description).
	* \warning The description is cleared by this method.
	*/
	void Update(GLang* lang,GDescription& desc,GConceptTree& docstruct,bool ram,bool delref);

public:

	/**
	* Destruct the document.
	*/
	virtual ~GDoc(void);

	friend class GSession;
	friend class GIndexer;
	friend class GDocAnalyze;
   friend class GObjects<GDoc>;
};


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
