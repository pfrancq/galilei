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
#include <galilei.h>
#include <gweightinfos.h>
#include <gsubject.h>


//------------------------------------------------------------------------------
// forward class declaration
namespace R{
	template<bool bOrder> class RVectorInt;
}


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
	* Identificator of the profiles which have assessed the document.
	*/
	R::RVectorInt<true>* Fdbks;

	/**
	* Count the number of downloads failed.
	*/
	unsigned int Failed;

	/**
	* Links "out" of the document.
	*/
	R::RContainer<GLink,true,true> LinkSet;

	/**
	* Id of the owner
	*/
	unsigned int OwnerId;

public:

	/**
	* Construct a document.
	* @param url             URL of the document.
	* @param name            Name of the document.
	* @param id              Identifier of the document.
	* @param lang            Language of the document.
	* @param mime            MIME type of the document.
	* @param u               Date of the last update.
	* @param a               Date of the last analysis.
	* @param f               Number of fails.
	* @param ownerid         Owner Identifier of the document.
	*/
	GDoc(const R::RString& url,const R::RString& name,unsigned int id, GLang* lang,const R::RString&  mime,const R::RDate& u,const R::RDate& a,unsigned int f,unsigned int ownerid);

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

	/**
	* Load information from the current storage.
	* @param infos           Container hold
	*/
	virtual void LoadInfos(void) const;

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
	* @param id              Identifier.
	*/
	void SetId(unsigned int id);

	/**
	* Get the owner identificator of the document.
	* @return unsigned int.
	*/
	unsigned int GetOwnerId(void) const {return(OwnerId);}

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
	* Get a cursor on the identificator of the profiles which have assesses the
	* documents.
	* @return R::RVectorInt*.
	*/
	R::RVectorInt<true>* GetFdbks(void) const;

	/**
	* Add a profile to the list of those which have assess the document.
	* @param id              Identificator of the profile.
	*/
	void InsertFdbk(unsigned int id);

	/**
	* Delete a profile from the list of those which have assess the document.
	* @param id              Identificator of the profile.
	*/
	void DeleteFdbk(unsigned int id);

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
	unsigned int GetNbLinks(void) const;

	/**
	* Add a new link to the document and set the number of occurences of this
	* link.
	* @param doc             The Document representing the link to be inserted.
	* @param nboccurs        The number of occurences of the link inside the
	*                        document. If the number of occurences is set to 0,
	*                        an occurence number of 1 is used.
	*/
	void InsertLink(const GDoc* doc, unsigned int nboccurs=0);

	/**
	* Get a cursor on the Links of the document.
	* @return GLinkCursor.
	*/
	R::RCursor<GLink> GetLinkCursor(void) const;

	/**
	* Update the document by assigning it a set of information and a language.
	* @param lang            Pointer to the language.
	* @param infos           Pointer to the information.
	* @param computed        The update is called after a computation (and not
	*                        after a loading from a database).
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
