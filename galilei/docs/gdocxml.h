/*

	GALILEI Research Project

	GFilterEMail.h

	Filter's document for an email - Header.

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
#include <rxml/rxmlstruct.h>
#include <rxml/rxmltag.h>


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>
using namespace GALILEI;


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
* The GDocXML class provides a XML representation for the content of a given
* document. For the metadata, the Dublin Core Metadata Element Set is used with
* the RDF format.
* @author Pascal Francq
*/
class GDocXML : public RXML::RXMLStruct
{
	/**
	* url of the document.
	*/
	RStd::RString URL;

	/**
	* File containg the document (must be local).
	*/
	RStd::RString FileName;

	/**
	* Tag holding the metadata.
	*/
	RXML::RXMLTag* MetaData;

	/**
	* Tag holding the content.
	*/
	RXML::RXMLTag* Content;

	/**
	* Tag holding the links.
	*/
	RXML::RXMLTag* Links;

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
	const char* GetURL(void) const {return(URL());}

	/**
	* Get the File Name.
	* @returns Pointer to a C string.
	*/
	const char* GetFile(void) const {return(FileName());}

	/**
	* @returns A pointer to the tag containing the metadata of the
	*          corresponding document.
	*/
	RXML::RXMLTag* GetMetaData(void) {return(MetaData);}

	/**
	* @returns A pointer to the tag containing the content of the corresponding
	*          document.
	*/
	RXML::RXMLTag* GetContent(void) {return(Content);}

	/**
	* @returns A pointer to the tag containing the links of the corresponding
	*          document.
	*/
	RXML::RXMLTag* GetLinks(void) {return(Links);}

	/**
	* Add a tag metaData to the links stucture
	* @returns a pointer to the tag containing the metaData of the link
	*/
	RXML::RXMLTag* AddLink(void) throw(bad_alloc);
	
	/**
	* Add a title to the metadata.
	* @param val            Value to assign.
	*/
	void AddTitle(const char* val,RXML::RXMLTag* metaData=0) throw(bad_alloc);

	/**
	* Add a title to the metadata.
	* @returns Pointer to a XML Tag.
	*/
	RXML::RXMLTag* AddTitle(RXML::RXMLTag* metaData=0) throw(bad_alloc);

	/**
	* Add a creator to the metadata.
	* @param val            Value to assign.
	*/
	void AddCreator(const char* val,RXML::RXMLTag* metaData=0) throw(bad_alloc);

	/**
	* Add a creator to the metadata.
	* @returns Pointer to a XML Tag.
	*/
	RXML::RXMLTag* AddCreator(RXML::RXMLTag* metaData=0) throw(bad_alloc);

	/**
	* Add a subject to the metadata.
	* @param val            Value to assign.
	*/
	void AddSubject(const char* val,RXML::RXMLTag* metaData=0) throw(bad_alloc);

	/**
	* Add a subject to the metadata.
	* @returns Pointer to a XML Tag.
	*/
	RXML::RXMLTag* AddSubject(RXML::RXMLTag* metaData=0) throw(bad_alloc);

	/**
	* Add a description to the metadata.
	* @param val            Value to assign.
	*/
	void AddDescription(const char* val,RXML::RXMLTag* metaData=0) throw(bad_alloc);

	/**
	* Add a description to the metadata.
	* @returns Pointer to a XML Tag.
	*/
	RXML::RXMLTag* AddDescription(RXML::RXMLTag* metaData=0) throw(bad_alloc);

	/**
	* Add a publisher to the metadata.
	* @param val            Value to assign.
	*/
	void AddPublisher(const char* val,RXML::RXMLTag* metaData=0) throw(bad_alloc);

	/**
	* Add a publisher to the metadata.
	* @returns Pointer to a XML Tag.
	*/
	RXML::RXMLTag* AddPublisher(RXML::RXMLTag* metaData=0) throw(bad_alloc);

	/**
	* Add a contributor to the metadata.
	* @param val            Value to assign.
	*/
	void AddContributor(const char* val,RXML::RXMLTag* metaData=0) throw(bad_alloc);

	/**
	* Add a contributor to the metadata.
	* @returns Pointer to a XML Tag.
	*/
	RXML::RXMLTag* AddContributor(RXML::RXMLTag* metaData=0) throw(bad_alloc);

	/**
	* Add a date to the metadata.
	* @param val            Value to assign.
	*/
	void AddDate(const char* val,RXML::RXMLTag* metaData=0) throw(bad_alloc);

	/**
	* Add a date to the metadata.
	* @returns Pointer to a XML Tag.
	*/
	RXML::RXMLTag* AddDate(RXML::RXMLTag* metaData=0) throw(bad_alloc);

	/**
	* Add a type to the metadata.
	* @param val            Value to assign.
	*/
	void AddType(const char* val,RXML::RXMLTag* metaData=0) throw(bad_alloc);

	/**
	* Add a type to the metadata.
	* @returns Pointer to a XML Tag.
	*/
	RXML::RXMLTag* AddType(RXML::RXMLTag* metaData=0) throw(bad_alloc);

	/**
	* Add a format to the metadata.
	* @param val            Value to assign.
	*/
	void AddFormat(const char* val,RXML::RXMLTag* metaData=0) throw(bad_alloc);

	/**
	* Add a format to the metadata.
	* @returns Pointer to a XML Tag.
	*/
	RXML::RXMLTag* AddFormat(RXML::RXMLTag* metaData=0) throw(bad_alloc);

	/**
	* Add an identifier to the metadata.
	* @param val            Value to assign.
	*/
	void AddIdentifier(const char* val,RXML::RXMLTag* metaData=0) throw(bad_alloc);

	/**
	* Add an identifier to the metadata.
	* @returns Pointer to a XML Tag.
	*/
	RXML::RXMLTag* AddIdentifier(RXML::RXMLTag* metaData=0) throw(bad_alloc);

	/**
	* Add a source to the metadata.
	* @param val            Value to assign.
	*/
	void AddSource(const char* val,RXML::RXMLTag* metaData=0) throw(bad_alloc);

	/**
	* Add a source to the metadata.
	* @returns Pointer to a XML Tag.
	*/
	RXML::RXMLTag* AddSource(RXML::RXMLTag* metaData=0) throw(bad_alloc);

	/**
	* Add a language to the metadata.
	* @param val            Value to assign.
	*/
	void AddLanguage(const char* val,RXML::RXMLTag* metaData=0) throw(bad_alloc);

	/**
	* Add a language to the metadata.
	* @returns Pointer to a XML Tag.
	*/
	RXML::RXMLTag* AddLanguage(RXML::RXMLTag* metaData=0) throw(bad_alloc);

	/**
	* Add a relation to the metadata.
	* @param val            Value to assign.
	*/
	void AddRelation(const char* val,RXML::RXMLTag* metaData=0) throw(bad_alloc);

	/**
	* Add a relation to the metadata.
	* @returns Pointer to a XML Tag.
	*/
	RXML::RXMLTag* AddRelation(RXML::RXMLTag* metaData=0) throw(bad_alloc);

	/**
	* Add a coverage to the metadata.
	* @param val            Value to assign.
	*/
	void AddCoverage(const char* val,RXML::RXMLTag* metaData=0) throw(bad_alloc);

	/**
	* Add a coverage to the metadata.
	* @returns Pointer to a XML Tag.
	*/
	RXML::RXMLTag* AddCoverage(RXML::RXMLTag* metaData=0) throw(bad_alloc);

	/**
	* Add rights to the metadata.
	* @param val            Value to assign.
	*/
	void AddRights(const char* val,RXML::RXMLTag* metaData=0) throw(bad_alloc);

	/**
	* Add rights to the metadata.
	* @returns Pointer to a XML Tag.
	*/
	RXML::RXMLTag* AddRights(RXML::RXMLTag* metaData=0) throw(bad_alloc);

	/**
	* Destruct the document.
	*/
	virtual ~GDocXML(void);
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
