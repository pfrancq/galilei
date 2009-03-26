/*

	GALILEI Research Project

	GDocXML.h

	DocXML representing the document content - Header.

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
	R::RURI URL;

	/**
	* File containing the document (must be local).
	*/
	R::RURI FileName;

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
	GDocXML(const R::RURI& url,const R::RURI& filename);

	/**
	* Initialize the DocXML structure with the default tags and entities.
	*/
	void InitDocXML(void);

	/**
	* Get the URL.
	* @returns Pointer to a C string.
	*/
	R::RURI GetURL(void) const;

	/**
	* Get the File Name.
	* @returns Pointer to a C string.
	*/
	R::RURI GetFile(void) const;

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
	* Add a tag metaData to the links structure.
	* @returns a pointer to the tag containing the metaData of the link.
	*/
	R::RXMLTag* AddLink(void);

protected:

	/**
	* Add a tag to a metadata.
	* @param tag            Name of the tag.
	* @param val            Title.
	* @param metaData       Pointer to the metadata (if the pointer is null, the
	*                       metadata of the document is used).
	*/
	R::RXMLTag* Add(R::RString tag,R::RString val,R::RXMLTag* metaData);

public:

	/**
	* Add a title tag to a metadata.
	* @param val            Title.
	* @param metaData       Pointer to the metadata (if the pointer is null, the
	*                       metadata of the document is used).
	*/
	R::RXMLTag* AddTitle(R::RString val,R::RXMLTag* metaData=0);

	/**
	* Add a creator tag to a metadata.
	* @param val            Creator.
	* @param metaData       Pointer to the metadata (if the pointer is null, the
	*                       metadata of the document is used).
	*/
	R::RXMLTag* AddCreator(R::RString val,R::RXMLTag* metaData=0);

	/**
	* Add a subject tag to a metadata.
	* @param val            Subject.
	* @param metaData       Pointer to the metadata (if the pointer is null, the
	*                       metadata of the document is used).
	*/
	R::RXMLTag* AddSubject(R::RString val,R::RXMLTag* metaData=0);

	/**
	* Add a description tag to a metadata.
	* @param val            Description.
	* @param metaData       Pointer to the metadata (if the pointer is null, the
	*                       metadata of the document is used).
	*/
	R::RXMLTag* AddDescription(R::RString val,R::RXMLTag* metaData=0);

	/**
	* Add a publisher tag to a metadata.
	* @param val            Publisher.
	* @param metaData       Pointer to the metadata (if the pointer is null, the
	*                       metadata of the document is used).
	*/
	R::RXMLTag* AddPublisher(R::RString val,R::RXMLTag* metaData=0);

	/**
	* Add a contributor tag to a metadata.
	* @param val            Contributor.
	* @param metaData       Pointer to the metadata (if the pointer is null, the
	*                       metadata of the document is used).
	*/
	R::RXMLTag* AddContributor(R::RString val,R::RXMLTag* metaData=0);

	/**
	* Add a date tag to a metadata.
	* @param val            Date.
	* @param metaData       Pointer to the metadata (if the pointer is null, the
	*                       metadata of the document is used).
	*/
	R::RXMLTag* AddDate(R::RString val,R::RXMLTag* metaData=0);

	/**
	* Add a type tag to a metadata.
	* @param val            Type.
	* @param metaData       Pointer to the metadata (if the pointer is null, the
	*                       metadata of the document is used).
	*/
	R::RXMLTag* AddType(R::RString val,R::RXMLTag* metaData=0);

	/**
	* Add a format tag to a metadata.
	* @param val            Format.
	* @param metaData       Pointer to the metadata (if the pointer is null, the
	*                       metadata of the document is used).
	*/
	R::RXMLTag* AddFormat(R::RString val,R::RXMLTag* metaData=0);

	/**
	* Add an identifier tag to a metadata.
	* @param val            Identifier.
	* @param metaData       Pointer to the metadata (if the pointer is null, the
	*                       metadata of the document is used).
	*/
	R::RXMLTag* AddIdentifier(R::RString val,R::RXMLTag* metaData=0);

	/**
	* Add a source tag to a metadata.
	* @param val            Source.
	* @param metaData       Pointer to the metadata (if the pointer is null, the
	*                       metadata of the document is used).
	*/
	R::RXMLTag* AddSource(R::RString val,R::RXMLTag* metaData=0);

	/**
	* Add a language tag to a metadata.
	* @param val            Language.
	* @param metaData       Pointer to the metadata (if the pointer is null, the
	*                       metadata of the document is used).
	*/
	R::RXMLTag* AddLanguage(R::RString val,R::RXMLTag* metaData=0);

	/**
	* Add a relation tag to a metadata.
	* @param val            Relation.
	* @param metaData       Pointer to the metadata (if the pointer is null, the
	*                       metadata of the document is used).
	*/
	R::RXMLTag* AddRelation(R::RString val,R::RXMLTag* metaData=0);

	/**
	* Add a coverage tag to a metadata.
	* @param val            Coverage.
	* @param metaData       Pointer to the metadata (if the pointer is null, the
	*                       metadata of the document is used).
	*/
	R::RXMLTag* AddCoverage(R::RString val,R::RXMLTag* metaData=0);

	/**
	* Add a rights tag to a metadata.
	* @param val            Rights.
	* @param metaData       Pointer to the metadata (if the pointer is null, the
	*                       metadata of the document is used).
	*/
	R::RXMLTag* AddRights(R::RString val,R::RXMLTag* metaData=0);

	/**
	* Destruct the DocXML document.
	*/
	virtual ~GDocXML(void);
};


}  //-------- End of namespace GALILEI -----------------------------------------


//------------------------------------------------------------------------------
#endif
