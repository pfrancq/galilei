/*

	GALILEI Research Project

	GDoc.h

	Document - Header.

	Copyright 2001 by the Universit� Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).

	Version $Revision$

	Last Modify: $Date$

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



//-----------------------------------------------------------------------------
#ifndef GDocH
#define GDocH


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
* The GDoc class provides a representation of a document.
* @author Pascal Francq
* @short Document.
*/
class GDoc
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
	* Total number of words.
	*/
	unsigned int N;

	/**
	* Total number of valid words.
	*/
	unsigned int V;

	/**
	* Number of different words.
	*/
	unsigned int Ndiff;

	/**
	* Number of different valid words.
	*/
	unsigned int Vdiff;

	/**
	* Pointer to the language.
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
	* Date of last update of document's content.
	*/
	R::RDate Updated;

	/**
	* Date of last document's analysis.
	*/
	R::RDate Computed;

	/**
	* Profiles which have judge the current document.
	*/
	R::RContainer<GProfDoc,unsigned,false,true> Fdbks;

	/**
	* Count the number of downloads failed.
	*/
	unsigned int Failed;

	/**
	* Container of Link.
	*/
	R::RContainer<GLink,unsigned int,false,true>* LinkSet;

#if GALILEITEST
	/**
	* Subjects of the documents.
	*/
	R::RContainer<GSubject,unsigned int,false,true> Subjects;
#endif

public:

	/**
	* Construct the document.
	* @param url            URL of the document.
	* @param title          Name of the document.
	* @param id             Identifier of the document.
	* @param lang           Language of the document.
	* @param mime           MIME type of the document.
	* @param u              String representing the date of the last update.
	* @param a              String representing the date of the last analysis.
	* @param f              Number of fails.
	* @param n              Total number of words.
	* @param ndiff          Number of different words.
	* @param v              Total number of valid words.
	* @param vdiff          Number of different valid words.
	* @param nbf            Number of Feedbacks.
	*/
	GDoc(const char* url,const char* name,unsigned int id,GLang* lang,const char* mime,const char* u,const char* a,unsigned int f,unsigned int n,unsigned int ndiff,unsigned int v,unsigned int vdiff,unsigned int nbf=100) throw(bad_alloc);

	/**
	* Get the name of the model used for the description.
	* @return C String.                                                 
	*/
	virtual const char* GetModelName(void) const=0;

	/**
	* Look if the document has a representation?
	* @return bool.
	*/
	virtual bool HasRepresentation(void) const;

	/**
	* Compare function needed by R::RContainer.
	* @param doc            Document used for the comparaison.
	*/
	int Compare(const GDoc& doc) const;

	/**
	* Compare function needed by R::RContainer.
	* @param doc            Pointer to the document used for the comparaison.
	*/
	int Compare(const GDoc* doc) const;

	/**
	* Compare function needed by R::RContainer.
	* @param id            Identificator used for the comparaison.
	*/
	int Compare(const unsigned id) const;

	/**
	* Compare function needed by R::RContainer.
	* @param lang           Language used for the comparaison.
	*/
	int Compare(const GLang*) const;

	/**
	* This function clears the information related to the document.
	* @param l              Must the language be removed.
	*/
	virtual void ClearInfos(bool l);

	/**
	* Clear The Fdbks Container
	*/
	void ClearFdbks(void);

	/**
	* Get the URL.
	* @returns Pointer to a C string.
	*/
	const char* GetURL(void) const;

	/**
	* Get the Name.
	* @returns Pointer to a C string.
	*/
	const char* GetName(void) const;

	/**
	* Get the date of the last update of the document's content.
	* @returns Pointer to date.
	*/
	const R::RDate* GetUpdated(void) const;

	/**
	* Get the date of the last analysis of the document.
	* @returns Pointer to date.
	*/
	const R::RDate* GetComputed(void) const;

	/**
	* Get the MIME type.
	* @returns Pointer to a MIME type.
	*/
	const char* GetMIMEType(void) const;

	/**
	* Set the MIME type.
	* @param mime           Pointer to a MIME type.
	*/
	void SetMIMEType(const char* mime);

	/**
	* Return the state of the document.
	* @returns GALILEI::tObjState value.
	*/
	tObjState GetState(void) const;

	/**
	* Set the state of the document.
	* @param state          New state.
	*/
	void SetState(tObjState state);

	/**
	* Set the information for the document.
	* @param l              Language of the document.
	* @param n              Total number of words.
	* @param nd             Total number of different words.
	* @param v              Total number of valid words.
	* @param vd             Total number of different valid words.
	*/
	virtual void SetInfos(GLang *l,unsigned int n,unsigned int nd,unsigned int v,unsigned int vd);

	/**
	* Add a word with a certain occurences in the document.
	* @param id             Identificator of the word.
	* @param nb             Occurences of the word.
	*/
	virtual void AddWord(const unsigned int id,const double nb)=0;

	/**
	* Add a wordlist with a certain occurences in the document.
	* @param id             Identificator of the word.
	* @param nb             Occurences of the word.
	*/
	virtual void AddWordList(const unsigned int id,const double nb)=0;

	/**
	* @return Total number of words in the documents with stoplist.
	*/
	unsigned int GetN(void) {return(N);}
                                                                           
	/**
	* @return Total number of words in the documents.
	*/
	unsigned int GetV(void) {return(V);}

	/**
	* @return Number of different words in the documents.
	*/
	unsigned int GetNdiff(void) {return(Ndiff);}

	/**
	* @return Number of different words in the documents.
	*/
	unsigned int GetVdiff(void) {return(Vdiff);}

	/**
	* @return Pointer to the Language.
	*/
	GLang* GetLang(void) const {return(Lang);}

	/**
	* @return Identificator of the document.
	*/
	unsigned int GetId(void) const {return(Id);}

	/**
	* Set the identifier.
	* @param id             Identifier.
	*/
	void SetId(unsigned int id) {if(Id==cNoRef) Id=id;}

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
	* Get a Cursor on the feedback on the document.
	* @return GProfDocCursor.
	*/
	GProfDocCursor& GetProfDocCursor(void);

	/**
	* Get the number of judgments on the document.
	* @return unsigned int
	*/
	unsigned int GetNbFdbks(void) const;

	/**
	* Compute the similarity between documents.
	*/
	virtual double Similarity(const GDoc* doc) const;

	/**
	* Compute the global similarity between documents.
	*/
	virtual double GlobalSimilarity(const GDoc* doc) const;

	/**
	* Compute the similarity between a document and a subprofile.
	*/
	virtual double Similarity(const GSubProfile* sub) const;

	/**
	* Compute the global similarity between a document and a subprofile.
	*/
	virtual double GlobalSimilarity(const GSubProfile* sub) const;

	/**
	* Compute the similarity between a document and a group.
	*/
	virtual double Similarity(const GGroup* grp) const;

	/**
	* Compute the global similarity between a document and a group.
	*/
	virtual double GlobalSimilarity(const GGroup* grp) const;

	/**
	* Add a judgement for this document.
	* @param j              Judgement.
	*/
	void AddJudgement(GProfDoc* j) throw(bad_alloc);

	/**
	* Get the Number  of Outgoing links
	*/
	unsigned int GetNbLinks(void);

	/**
	* Add a new link to the document
	* @params doc           The Document representing the link to be inserted.
	*/
	void InsertLink(const GDoc* doc) throw(bad_alloc);

	/**
	* Add a new link to the document and set the number of occurences
	* @params doc            The Document representing the link to be inserted.
	* @params nbOccurs  The number of occurence of the link inside the document.
	*/
	void InsertLink(const GDoc* doc, unsigned int nbOccurs) throw(bad_alloc);
	
	/**
	* Add a new link to the document
	* @params doc           The Document representing the link to be inserted.
	* @param format         The mime type of the Link.
	* @param type           The type of the link.
	*/
	void InsertLink(const GDoc* doc,char* format,char* type) throw(bad_alloc);

	/**
	* Get a cursor on the Links of the document.
	* @return GLinkCursor.
	*/
	GLinkCursor& GetLinkCursor(void);


#if GALILEITEST

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
	GSubjectCursor& GetSubjectCursor(void);

#endif

	/**
	* Destruct the document.
	*/
	virtual ~GDoc(void);
};


//-----------------------------------------------------------------------------
/**
* The GDocCursor class provides a way to go trough a set of documents.
* @short Documents Cursor
*/
CLASSCURSOR(GDocCursor,GDoc,unsigned int)


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
