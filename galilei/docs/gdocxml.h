/*

	GALILEI Research Project

	GDocXML.h

	A XML representation of a document - Header.

	Copyright 2001 by the Université Libre de Bruxelles.

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
#ifndef GDocXMLH
#define GDocXMLH


//-----------------------------------------------------------------------------
// include files for R Project
#include <rstd/rxmlstruct.h>
#include <rstd/rxmltag.h>


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
* The GDocXML class provides a XML representation for the content of a given
* document. For the metadata, the Dublin Core Metadata Element Set is used with
* the RDF format.
* @author Pascal Francq
* Short Xml representation for documents
*/
class GDocXML : public R::RXMLStruct
{
	/**
	* url of the document.
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
	* Construct a XML document.
	* @param url            URL of the document.
	* @param filename       File of the document.
	*/
	GDocXML(const char* url,const char* filename) throw(bad_alloc);

	/**
	* Get the URL.
	* @returns Pointer to a C string.
	*/
	const char* GetURL(void) const;

	/**
	* Get the File Name.
	* @returns Pointer to a C string.
	*/
	const char* GetFile(void) const;

	/**
	* @returns A pointer to the tag containing the metadata of the
	*          corresponding document.
	*/
	R::RXMLTag* GetMetaData(void);

	/**
	* @returns A pointer to the tag containing the content of the corresponding
	*          document.
	*/
	R::RXMLTag* GetContent(void);

	/**
	* @returns A pointer to the tag containing the links of the corresponding
	*          document.
	*/
	R::RXMLTag* GetLinks(void);

	/**
	* Add a tag metaData to the links stucture
	* @returns a pointer to the tag containing the metaData of the link
	*/
	R::RXMLTag* AddLink(void) throw(bad_alloc);
	
	/**
	* Add a title to the metadata.
	* @param val            Value to assign.
	*/
	void AddTitle(const char* val,R::RXMLTag* metaData=0) throw(bad_alloc);

	/**
	* Add a title to the metadata.
	* @returns Pointer to a XML Tag.
	*/
	R::RXMLTag* AddTitle(R::RXMLTag* metaData=0) throw(bad_alloc);

	/**
	* Add a creator to the metadata.
	* @param val            Value to assign.
	*/
	void AddCreator(const char* val,R::RXMLTag* metaData=0) throw(bad_alloc);

	/**
	* Add a creator to the metadata.
	* @returns Pointer to a XML Tag.
	*/
	R::RXMLTag* AddCreator(R::RXMLTag* metaData=0) throw(bad_alloc);

	/**
	* Add a subject to the metadata.
	* @param val            Value to assign.
	*/
	void AddSubject(const char* val,R::RXMLTag* metaData=0) throw(bad_alloc);

	/**
	* Add a subject to the metadata.
	* @returns Pointer to a XML Tag.
	*/
	R::RXMLTag* AddSubject(R::RXMLTag* metaData=0) throw(bad_alloc);

	/**
	* Add a description to the metadata.
	* @param val            Value to assign.
	*/
	void AddDescription(const char* val,R::RXMLTag* metaData=0) throw(bad_alloc);

	/**
	* Add a description to the metadata.
	* @returns Pointer to a XML Tag.
	*/
	R::RXMLTag* AddDescription(R::RXMLTag* metaData=0) throw(bad_alloc);

	/**
	* Add a publisher to the metadata.
	* @param val            Value to assign.
	*/
	void AddPublisher(const char* val,R::RXMLTag* metaData=0) throw(bad_alloc);

	/**
	* Add a publisher to the metadata.
	* @returns Pointer to a XML Tag.
	*/
	R::RXMLTag* AddPublisher(R::RXMLTag* metaData=0) throw(bad_alloc);

	/**
	* Add a contributor to the metadata.
	* @param val            Value to assign.
	*/
	void AddContributor(const char* val,R::RXMLTag* metaData=0) throw(bad_alloc);

	/**
	* Add a contributor to the metadata.
	* @returns Pointer to a XML Tag.
	*/
	R::RXMLTag* AddContributor(R::RXMLTag* metaData=0) throw(bad_alloc);

	/**
	* Add a date to the metadata.
	* @param val            Value to assign.
	*/
	void AddDate(const char* val,R::RXMLTag* metaData=0) throw(bad_alloc);

	/**
	* Add a date to the metadata.
	* @returns Pointer to a XML Tag.
	*/
	R::RXMLTag* AddDate(R::RXMLTag* metaData=0) throw(bad_alloc);

	/**
	* Add a type to the metadata.
	* @param val            Value to assign.
	*/
	void AddType(const char* val,R::RXMLTag* metaData=0) throw(bad_alloc);

	/**
	* Add a type to the metadata.
	* @returns Pointer to a XML Tag.
	*/
	R::RXMLTag* AddType(R::RXMLTag* metaData=0) throw(bad_alloc);

	/**
	* Add a format to the metadata.
	* @param val            Value to assign.
	*/
	void AddFormat(const char* val,R::RXMLTag* metaData=0) throw(bad_alloc);

	/**
	* Add a format to the metadata.
	* @returns Pointer to a XML Tag.
	*/
	R::RXMLTag* AddFormat(R::RXMLTag* metaData=0) throw(bad_alloc);

	/**
	* Add an identifier to the metadata.
	* @param val            Value to assign.
	*/
	void AddIdentifier(const char* val,R::RXMLTag* metaData=0) throw(bad_alloc);

	/**
	* Add an identifier to the metadata.
	* @returns Pointer to a XML Tag.
	*/
	R::RXMLTag* AddIdentifier(R::RXMLTag* metaData=0) throw(bad_alloc);

	/**
	* Add a source to the metadata.
	* @param val            Value to assign.
	*/
	void AddSource(const char* val,R::RXMLTag* metaData=0) throw(bad_alloc);

	/**
	* Add a source to the metadata.
	* @returns Pointer to a XML Tag.
	*/
	R::RXMLTag* AddSource(R::RXMLTag* metaData=0) throw(bad_alloc);

	/**
	* Add a language to the metadata.
	* @param val            Value to assign.
	*/
	void AddLanguage(const char* val,R::RXMLTag* metaData=0) throw(bad_alloc);

	/**
	* Add a language to the metadata.
	* @returns Pointer to a XML Tag.
	*/
	R::RXMLTag* AddLanguage(R::RXMLTag* metaData=0) throw(bad_alloc);

	/**
	* Add a relation to the metadata.
	* @param val            Value to assign.
	*/
	void AddRelation(const char* val,R::RXMLTag* metaData=0) throw(bad_alloc);

	/**
	* Add a relation to the metadata.
	* @returns Pointer to a XML Tag.
	*/
	R::RXMLTag* AddRelation(R::RXMLTag* metaData=0) throw(bad_alloc);

	/**
	* Add a coverage to the metadata.
	* @param val            Value to assign.
	*/
	void AddCoverage(const char* val,R::RXMLTag* metaData=0) throw(bad_alloc);

	/**
	* Add a coverage to the metadata.
	* @returns Pointer to a XML Tag.
	*/
	R::RXMLTag* AddCoverage(R::RXMLTag* metaData=0) throw(bad_alloc);

	/**
	* Add rights to the metadata.
	* @param val            Value to assign.
	*/
	void AddRights(const char* val,R::RXMLTag* metaData=0) throw(bad_alloc);

	/**
	* Add rights to the metadata.
	* @returns Pointer to a XML Tag.
	*/
	R::RXMLTag* AddRights(R::RXMLTag* metaData=0) throw(bad_alloc);

	/**
	* Destruct the document.
	*/
	virtual ~GDocXML(void);
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
