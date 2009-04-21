/*

	GALILEI Research Project

	GDoc.h

	Document - Header.

	Copyright 2001-2009 by Pascal Francq (pascal@francq.info).
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
#include <rvectorint.h>
#include <ruri.h>


//------------------------------------------------------------------------------
// include files for GALILEI
#include <gweightinfos.h>


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* The GDoc class provides a representation of a analyzed document. In fact, a
* document is represented as a multi-space vector and its XML structure is also
* indexed. Since the structure is memory consuming, it is not loaded by default
* and it is recommended it to release it when not used anymore.
* @author Pascal Francq
* @short Document.
*/
class GDoc : public GWeightInfos
{
protected:

	/**
	* URL of the document.
	*/
	R::RURI URL;

	/**
	* Name of the document.
	*/
	R::RString Name;

	/**
	* Identifier of the document.
	*/
	size_t Id;

	/**
	 * Structure of the document.
	 */
	GDocStruct* Struct;

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
	R::RVectorInt<size_t,true>* Fdbks;

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
	 * Number of "VTD" records of the XML structure (0 implies that the document
	 * has no structure).
	 */
	size_t NbRecs;

	/**
	 * Number of location caches.
	 */
	size_t NbLCs;

public:

	/**
	* Construct a document.
	* @param url             URL of the document.
	* @param name            Name of the document.
	* @param id              Identifier of the document.
	* @param lang            Language of the document.
	* @param mime            MIME type of the document.
	* @param grpid           Topic identifier.
	* @param c               Date of the last computation.
	* @param u               Date of the last updated.
	* @param a               Date of the last attached.
	* @param nbrecs          Number of "VTD" records.
	* @param nblcs           Number of location caches.
	*/
	GDoc(const R::RURI& url,const R::RString& name,size_t id,GLang* lang,const R::RString& mime,size_t grpid,const R::RDate& c,const R::RDate& u,const R::RDate& a,size_t nbrecs,size_t nblcs);

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
	*/
	void ClearInfos(void);

	/**
	 * Clear the structure associated with the document.
	 */
	void ClearStruct(void);

	/**
	 * Get the structure of the document.
	 */
	GDocStruct* GetStruct(void) const;

	/**
	 * Release the structure of a document from the memory.
	 */
	void ReleaseStruct(void);

	/**
	* Load information from the current storage.
	*/
	virtual void LoadInfos(void) {GWeightInfos::LoadInfos(otDoc,Id);}

	/**
	* Get the URL.
	* @returns RURI.
	*/
	R::RURI GetURL(void) const {return(URL);}

	/**
	* Get the name of the document.
	* @returns RString.
	*/
	R::RString GetName(void) const {return(Name);}

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
	* Get the identifier of the document.
	*/
	size_t GetId(void) const {return(Id);}

	/**
	* Set the identifier of the document.
	* @param id              Identifier.
	*/
	void SetId(size_t id);

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
	 * Get the number of "VTD" record in the structure of the document (0
	 * implies that the structure is empty).
	 */
	size_t GetNbRecs(void) const {return(NbRecs);}

	/**
	 * Get the number of Location caches for the structure of the document.
	 * @return
	 */
	size_t GetNbLCs(void) const {return(NbLCs);}

	/**
	* Get a cursor on the identifier of the profiles which have assesses the
	* documents.
	* @return R::RVectorInt*.
	*/
	R::RVectorInt<size_t,true>* GetFdbks(void) const;

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
	* Get the number of common profiles which have assess both documents as
	* relevant.
	* @param doc            Pointer to a document.
	*/
	size_t GetCommonOKProfiles(const GDoc* doc) const;

	/**
	* Get the number of common profiles which have assess both documents.
	* @param doc            Pointer to a document.
	*/
	size_t GetCommonProfiles(const GDoc* doc) const;

	/**
	* Get the number of common profiles which have assess both documents
	* differently.
	* @param doc            Pointer to a document.
	*/
	size_t GetCommonDiffProfiles(const GDoc* doc) const;

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

	/**
	* Assign a new description to the document.
	* @param lang            Pointer to the language.
	* @param infos           Pointer to the information.
	* @param ram             Must the information be maintained is RAM.
	* @param delref          Delete the references (must be set to true if the
	*                        document has already a description).
	* \warning The container infos is cleared by this method.
	*/
	void Update(GLang* lang,GWeightInfos& infos,GDocStruct& docstruct,bool ram,bool delref);

	/**
	 * Assign a structure to the current document.
	 */
	void AssignStruct() const;

public:

	/**
	* Destruct the document.
	*/
	virtual ~GDoc(void);
};


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
