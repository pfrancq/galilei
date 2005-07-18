/*

	GALILEI Research Project

	GDocXML.h

	DocXML representing the document content - Header.

	Copyright 2001-2003 by the Université Libre de Bruxelles.

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
#ifndef GDocXMLH
#define GDocXMLH


//------------------------------------------------------------------------------
// include files for R Project
#include <rxmlstruct.h>


//------------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* The GDocXML class provides a XML representation for the content of a given
* document. For the metadata, the Dublin Core Metadata Element Set is used with
* the RDF format.
* @author Pascal Francq
* Short DocXML document.
*/
class GDocXML : public R::RXMLStruct
{
	/**
	* URL of the document.
	*/
	R::RString URL;

	/**
	* File containg the document (must be local).
	*/
	R::RString FileName;

	/**
	* Tag holding the metadata.
	*/
	R::RXMLTag* MetaData;

	/**
	* Tag holding the content.
	*/
	R::RXMLTag* Content;

	/**
	* Tag holding the links.
	*/
	R::RXMLTag* Links;

public:
	/**
	* Construct a DocXML document.
	* @param url            URL of the document.
	* @param filename       File of the document.
	*/
	GDocXML(R::RString url,R::RString filename) throw(std::bad_alloc);

	/**
	* Get the URL.
	* @returns Pointer to a C string.
	*/
	R::RString GetURL(void) const;

	/**
	* Get the File Name.
	* @returns Pointer to a C string.
	*/
	R::RString GetFile(void) const;

	/**
	* Get an access to the metadata of the document.
	* @returns A pointer to the tag containing the metadata of the
	*          corresponding document.
	*/
	R::RXMLTag* GetMetaData(void);

	/**
	* Get an access to the content of the document.
	* @returns A pointer to the tag containing the content of the corresponding
	*          document.
	*/
	R::RXMLTag* GetContent(void);

	/**
	* Get an access to the links of the documents.
	* @returns A pointer to the tag containing the links of the corresponding
	*          document.
	*/
	R::RXMLTag* GetLinks(void);

	/**
	* Add a tag metaData to the links stucture.
	* @returns a pointer to the tag containing the metaData of the link.
	*/
	R::RXMLTag* AddLink(void) throw(std::bad_alloc);

	/**
	* Add a title tag to a metadata.
	* @param val            Title.
	* @param metaData       Pointer to the metadata (if the pointer is null, the
	*                       metadata of the document is used).
	*/
	void AddTitle(R::RString val,R::RXMLTag* metaData=0) throw(std::bad_alloc);

	/**
	* Add a title tag to a metadata.
	* @param metaData       Pointer to the metadata (if the pointer is null, the
	*                       metadata of the document is used).
	* @returns Pointer to a R::RXMLTag.
	*/
	R::RXMLTag* AddTitle(R::RXMLTag* metaData=0) throw(std::bad_alloc);

	/**
	* Add a creator tag to a metadata.
	* @param val            Creator.
	* @param metaData       Pointer to the metadata (if the pointer is null, the
	*                       metadata of the document is used).
	*/
	void AddCreator(R::RString val,R::RXMLTag* metaData=0) throw(std::bad_alloc);

	/**
	* Add a creator tag to a metadata.
	* @param metaData       Pointer to the metadata (if the pointer is null, the
	*                       metadata of the document is used).
	* @returns Pointer to a R::RXMLTag.
	*/
	R::RXMLTag* AddCreator(R::RXMLTag* metaData=0) throw(std::bad_alloc);

	/**
	* Add a subject tag to a metadata.
	* @param val            Subject.
	* @param metaData       Pointer to the metadata (if the pointer is null, the
	*                       metadata of the document is used).
	*/
	void AddSubject(R::RString val,R::RXMLTag* metaData=0) throw(std::bad_alloc);

	/**
	* Add a subject tag to a metadata.
	* @param metaData       Pointer to the metadata (if the pointer is null, the
	*                       metadata of the document is used).
	* @returns Pointer to a R::RXMLTag.
	*/
	R::RXMLTag* AddSubject(R::RXMLTag* metaData=0) throw(std::bad_alloc);

	/**
	* Add a description tag to a metadata.
	* @param val            Description.
	* @param metaData       Pointer to the metadata (if the pointer is null, the
	*                       metadata of the document is used).
	*/
	void AddDescription(R::RString val,R::RXMLTag* metaData=0) throw(std::bad_alloc);

	/**
	* Add a description tag to a metadata.
	* @param metaData       Pointer to the metadata (if the pointer is null, the
	*                       metadata of the document is used).
	* @returns Pointer to a R::RXMLTag.
	*/
	R::RXMLTag* AddDescription(R::RXMLTag* metaData=0) throw(std::bad_alloc);

	/**
	* Add a publisher tag to a metadata.
	* @param val            Publisher.
	* @param metaData       Pointer to the metadata (if the pointer is null, the
	*                       metadata of the document is used).
	*/
	void AddPublisher(R::RString val,R::RXMLTag* metaData=0) throw(std::bad_alloc);

	/**
	* Add a publisher tag to a metadata.
	* @param metaData       Pointer to the metadata (if the pointer is null, the
	*                       metadata of the document is used).
	* @returns Pointer to a R::RXMLTag.
	*/
	R::RXMLTag* AddPublisher(R::RXMLTag* metaData=0) throw(std::bad_alloc);

	/**
	* Add a contributor tag to a metadata.
	* @param val            Contributor.
	* @param metaData       Pointer to the metadata (if the pointer is null, the
	*                       metadata of the document is used).
	*/
	void AddContributor(R::RString val,R::RXMLTag* metaData=0) throw(std::bad_alloc);

	/**
	* Add a contributor tag to a metadata.
	* @param metaData       Pointer to the metadata (if the pointer is null, the
	*                       metadata of the document is used).
	* @returns Pointer to a R::RXMLTag.
	*/
	R::RXMLTag* AddContributor(R::RXMLTag* metaData=0) throw(std::bad_alloc);

	/**
	* Add a date tag to a metadata.
	* @param val            Date.
	* @param metaData       Pointer to the metadata (if the pointer is null, the
	*                       metadata of the document is used).
	*/
	void AddDate(R::RString val,R::RXMLTag* metaData=0) throw(std::bad_alloc);

	/**
	* Add a date tag to a metadata.
	* @param metaData       Pointer to the metadata (if the pointer is null, the
	*                       metadata of the document is used).
	* @returns Pointer to a R::RXMLTag.
	*/
	R::RXMLTag* AddDate(R::RXMLTag* metaData=0) throw(std::bad_alloc);

	/**
	* Add a type tag to a metadata.
	* @param val            Type.
	* @param metaData       Pointer to the metadata (if the pointer is null, the
	*                       metadata of the document is used).
	*/
	void AddType(R::RString val,R::RXMLTag* metaData=0) throw(std::bad_alloc);

	/**
	* Add a type tag to a metadata.
	* @param metaData       Pointer to the metadata (if the pointer is null, the
	*                       metadata of the document is used).
	* @returns Pointer to a R::RXMLTag.
	*/
	R::RXMLTag* AddType(R::RXMLTag* metaData=0) throw(std::bad_alloc);

	/**
	* Add a format tag to a metadata.
	* @param val            Format.
	* @param metaData       Pointer to the metadata (if the pointer is null, the
	*                       metadata of the document is used).
	*/
	void AddFormat(R::RString val,R::RXMLTag* metaData=0) throw(std::bad_alloc);

	/**
	* Add a format tag to a metadata.
	* @param metaData       Pointer to the metadata (if the pointer is null, the
	*                       metadata of the document is used).
	* @returns Pointer to a R::RXMLTag.
	*/
	R::RXMLTag* AddFormat(R::RXMLTag* metaData=0) throw(std::bad_alloc);

	/**
	* Add an identifier tag to a metadata.
	* @param val            Identifier.
	* @param metaData       Pointer to the metadata (if the pointer is null, the
	*                       metadata of the document is used).
	*/
	void AddIdentifier(R::RString val,R::RXMLTag* metaData=0) throw(std::bad_alloc);

	/**
	* Add an identifier tag to a metadata.
	* @param metaData       Pointer to the metadata (if the pointer is null, the
	*                       metadata of the document is used).
	* @returns Pointer to a R::RXMLTag.
	*/
	R::RXMLTag* AddIdentifier(R::RXMLTag* metaData=0) throw(std::bad_alloc);

	/**
	* Add a source tag to a metadata.
	* @param val            Source.
	* @param metaData       Pointer to the metadata (if the pointer is null, the
	*                       metadata of the document is used).
	*/
	void AddSource(R::RString val,R::RXMLTag* metaData=0) throw(std::bad_alloc);

	/**
	* Add a source tag to a metadata.
	* @param metaData       Pointer to the metadata (if the pointer is null, the
	*                       metadata of the document is used).
	* @returns Pointer to a R::RXMLTag.
	*/
	R::RXMLTag* AddSource(R::RXMLTag* metaData=0) throw(std::bad_alloc);

	/**
	* Add a language tag to a metadata.
	* @param val            Language.
	* @param metaData       Pointer to the metadata (if the pointer is null, the
	*                       metadata of the document is used).
	*/
	void AddLanguage(R::RString val,R::RXMLTag* metaData=0) throw(std::bad_alloc);

	/**
	* Add a language tag to a metadata.
	* @param metaData       Pointer to the metadata (if the pointer is null, the
	*                       metadata of the document is used).
	* @returns Pointer to a R::RXMLTag.
	*/
	R::RXMLTag* AddLanguage(R::RXMLTag* metaData=0) throw(std::bad_alloc);

	/**
	* Add a relation tag to a metadata.
	* @param val            Relation.
	* @param metaData       Pointer to the metadata (if the pointer is null, the
	*                       metadata of the document is used).
	*/
	void AddRelation(R::RString val,R::RXMLTag* metaData=0) throw(std::bad_alloc);

	/**
	* Add a relation tag to a metadata.
	* @param metaData       Pointer to the metadata (if the pointer is null, the
	*                       metadata of the document is used).
	* @returns Pointer to a R::RXMLTag.
	*/
	R::RXMLTag* AddRelation(R::RXMLTag* metaData=0) throw(std::bad_alloc);

	/**
	* Add a coverage tag to a metadata.
	* @param val            Coverage.
	* @param metaData       Pointer to the metadata (if the pointer is null, the
	*                       metadata of the document is used).
	*/
	void AddCoverage(R::RString val,R::RXMLTag* metaData=0) throw(std::bad_alloc);

	/**
	* Add a coverage tag to a metadata.
	* @param metaData       Pointer to the metadata (if the pointer is null, the
	*                       metadata of the document is used).
	* @returns Pointer to a R::RXMLTag.
	*/
	R::RXMLTag* AddCoverage(R::RXMLTag* metaData=0) throw(std::bad_alloc);

	/**
	* Add a rights tag to a metadata.
	* @param val            Rights.
	* @param metaData       Pointer to the metadata (if the pointer is null, the
	*                       metadata of the document is used).
	*/
	void AddRights(R::RString val,R::RXMLTag* metaData=0) throw(std::bad_alloc);

	/**
	* Add a rights tag to a metadata.
	* @param metaData       Pointer to the metadata (if the pointer is null, the
	*                       metadata of the document is used).
	* @returns Pointer to a R::RXMLTag.
	*/
	R::RXMLTag* AddRights(R::RXMLTag* metaData=0) throw(std::bad_alloc);

	/**
	* Destruct the DocXML document.
	*/
	virtual ~GDocXML(void);
};


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
