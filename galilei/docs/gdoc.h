/*

	GALILEI Research Project

	GDoc.h

	Document - Header.

	Copyright 2001-2004 by the Université libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).

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
// include files for GALILEI
#include <sessions/galilei.h>
#include <infos/gweightinfos.h>
#include <groups/gsubject.h>


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* The GDoc class provides a representation of a analyzed document.
* @author Pascal Francq
* @short Document.
*/
class GDoc : public GWeightInfos
{
protected:

	/**
	* URL of the document.
	*/
	R::RString URL;

	/**
	* Name of the document.
	*/
	R::RString Name;

	/**
	* Identifier of the document.
	*/
	unsigned int Id;

	/**
	* Pointer to the language of the document.
	*/
	GLang* Lang;

	/**
	* State of the document.
	*/
	tObjState State;

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
	* Profiles which have assessed the document.
	*/
	R::RContainer<GProfileProxy,true,true> Fdbks;

	/**
	* Count the number of downloads failed.
	*/
	unsigned int Failed;

	/**
	* Links "out" of the document.
	*/
	R::RContainer<GLink,true,true> LinkSet;

	/**
	* Subjects of the document.
	*/
	R::RContainer<GSubject,false,true> Subjects;

public:

	/**
	* Construct a document.
	* @param url            URL of the document.
	* @param name           Name of the document.
	* @param id             Identifier of the document.
	* @param lang           Language of the document.
	* @param mime           MIME type of the document.
	* @param u              String representing the date of the last update.
	* @param a              String representing the date of the last analysis.
	* @param f              Number of fails.
	* @param nbf            Number of assessments.
	*/
	GDoc(const R::RString& url,const R::RString& name,unsigned int id,GLang* lang,const R::RString&  mime,const R::RString& u,const R::RString& a,unsigned int f,unsigned int nbf=100) throw(std::bad_alloc);

	/**
	* Construct a document.
	* @param url            URL of the document.
	* @param name           Name of the document.
	* @param mime           MIME type of the document.
	*/
	GDoc(const R::RString& url,const R::RString& name,const R::RString& mime) throw(std::bad_alloc);

	/**
	* Compare two documents by comparing their identificator.
	* @see R::RContainer
	* @param doc             Document.
	* @return int
	*/
	int Compare(const GDoc& doc) const;

	/**
	* Compare two documents by comparing their identificator.
	* @see R::RContainer
	* @param doc             Pointer to the document.
	* @return int
	*/
	int Compare(const GDoc* doc) const;

	/**
	* Compare the identificator of a documents with a given identificator.
	* @see R::RContainer
	* @param id              Identificator.
	* @return int
	*/
	int Compare(const unsigned id) const;

	/**
	* Compare the language of a document with a given language.
	* @see R::RContainer and GDocs.
	* @param lang            Pointer to the language.
	* @return int
	*/
	int Compare(const GLang* lang) const;

proxy:

	/**
	* Verify if the document is defined. By default, a document is suppose to be
	* undefined.
	* @return false.
	*/
	bool IsDefined(void) const;

	/**
	* Get the URL.
	* @returns RString.
	*/
	R::RString GetURL(void) const;

	/**
	* Get the name of the document.
	* @returns RString.
	*/
	R::RString GetName(void) const;

	/**
	* Set the name of the document.
	* @param name            Name.
	*/
	void SetName(const R::RString& name) throw(std::bad_alloc);

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
	* Return the state of the document.
	* @returns tObjState value.
	*/
	tObjState GetState(void) const;

	/**
	* Set the state of the document.
	* @param state          New state.
	*/
	void SetState(tObjState state);

	/**
	* @return Pointer to the Language.
	*/
	GLang* GetLang(void) const {return(Lang);}

	/**
	* Get the identificator of the document.
	* @return unsigned int.
	*/
	unsigned int GetId(void) const {return(Id);}

	/**
	* Set the identifier of the document.
	* @param id             Identifier.
	*/
	void SetId(unsigned int id) throw(GException);

	/**
	* Get the number of failed for the document.
	* @returns unsigned int.
	*/
	unsigned int GetFailed(void) const {return(Failed);}

	/**
	* Initialise the number of attemps to 0.
	*/
	void InitFailed(void) {Failed=0;}

	/**
	* Increase the number of failed attemp.
	*/
	void IncFailed(void) {Failed++;}

	/**
	* Decrease the number of failed attemp.
	*/
	void DecFailed(void) {Failed--;}

	/**
	* Get a cursor on the profiles which have assesses the documents.
	* @return R::RCursor<GProfileProxy>.
	*/
	R::RCursor<GProfileProxy> GetFdbks(void);

	/**
	* Get a Cursor on the weighted information entities.
	*/
	R::RCursor<GWeightInfo> GetWeightInfoCursor(void);

	/**
	* Get the list of information entities representing the document.
	* @return Constant pointer to a GWeightInfos.
	*/
	const GWeightInfos* GetInfos(void) const {return(this);}

	/**
	* Compute the maximal weight of the information entities in the document.
	* @return double.
	*/
	double GetMaxWeight(void) const throw(GException) {return(GWeightInfos::GetMaxWeight());}

	/**
	* Compute a similarity between two documents.
	* @param doc             Pointer to a document.
	*/
	double Similarity(const GDoc* doc) const;

	/**
	* Compute a similarity between two documents using a Inverse Frequence
	* Factor (IFF).
	* @param doc             Pointer to a document.
	*/
	double SimilarityIFF(const GDoc* doc) const throw(GException);

	/**
	* Compute a similarity between a document and a subprofile.
	* @param sub             Pointer to a subprofile.
	*/
	double Similarity(const GSubProfile* sub) const;

	/**
	* Compute a similarity between a document and a subprofile using a Inverse
	* Frequence Factor (IFF).
	* @param sub             Pointer to a subprofile.
	*/
	double SimilarityIFF(const GSubProfile* sub) const throw(GException);

	/**
	* Compute a similarity between a document and a group.
	* @param grp             Pointer to a group.
	*/
	double Similarity(const GGroup* grp) const;

	/**
	* Compute a similarity between a document and a group using a Inverse
	* Frequence Factor (IFF).
	* @param grp             Pointer to a group.
	*/
	double SimilarityIFF(const GGroup* grp) const throw(GException);

	/**
	* Add a profile to the list of those which have assess the document.
	* @param id               Identificator of the profile.
	*/
	void InsertFdbk(unsigned int id) throw(std::bad_alloc);

	/**
	* Delete a profile from the list of those which have assess the document.
	* @param id               Identificator of the profile.
	*/
	void DeleteFdbk(unsigned int id) throw(std::bad_alloc);

	/**
	* Clear all the assessments on the document.
	*/
	void ClearFdbks(void);

	/**
	* Get the number of times the documents was assessed.
	*/
	unsigned int GetNbFdbks(void) const;

	/**
	* Get the number of outgoing links
	* @return unsigned int.
	*/
	unsigned int GetNbLinks(void);

	/**
	* Add a new link to the document
	* @param doc            The document representing the link to be inserted.
	*/
	void InsertLink(const GDoc* doc) throw(std::bad_alloc);

	/**
	* Add a new link to the document and set the number of occurences of this
	* link.
	* @param doc             The Document representing the link to be inserted.
	* @param nbOccurs        The number of occurences of the link inside the
	*                        document.
	*/
	void InsertLink(const GDoc* doc, unsigned int nbOccurs) throw(std::bad_alloc);

	/**
	* Get a cursor on the Links of the document.
	* @return GLinkCursor.
	*/
	R::RCursor<GLink> GetLinkCursor(void);

	/**
	* Insert a new subject for this document.
	* @param s              Subject.
	*/
	void InsertSubject(GSubject* s);

	/**
	* Look if a document is from a given subject.
	* @param s              Subject.
	* @return bool
	*/
	bool IsFromSubject(const GSubject* s);

	/**
	* Look if a document is in the parent subject.
	* @param s              Subject.
	* @return bool
	*/
	bool IsFromParentSubject(const GSubject* s);

	/**
	* Get a Cursor on the subjects.
	* @return GSubjectCursor.
	*/
	R::RCursor<GSubject> GetSubjectCursor(void);

	/**
	* Get the number of subjects associated with the document.
	*/
	unsigned int GetNbSubjects(void);

	/**
	* Update the document by assigning it a set of information and a language.
	* @param lang             Pointer to the language.
	* @param infos            Pointer to the information.
	* @param computed         The update is called after a computation (and not
	*                         after a loading from a database).
	* \warning The container infos is cleared by this method.
	*/
	void Update(GLang* lang,R::RContainer<GWeightInfo,false,true>* infos,bool computed);

public:

	/**
	* Destruct the document.
	*/
	virtual ~GDoc(void);
};


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
