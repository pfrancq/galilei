/*

	GALILEI Research Project

	GFilterEMail.h

	Filter's document for an email - Header.

	(C) 2001 by P. Francq.

	Version $Revision$

	Last Modify: $Date$

*/



//-----------------------------------------------------------------------------
#ifndef GDocXMLH
#define GDocXMLH


//-----------------------------------------------------------------------------
// include files for R Project
#include <rxml/rxmlstruct.h>


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
	RXMLTag* MetaData;

	/**
	* Tag holding the content.
	*/
	RXMLTag* Content;

	/**
	* Tag holding the links.
	*/
	RXMLTag* Links;

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
	RXMLTag* GetMetaData(void) {return(MetaData);}

	/**
	* @returns A pointer to the tag containing the content of the corresponding
	*          document.
	*/
	RXMLTag* GetContent(void) {return(Content);}

	/**
	* @returns A pointer to the tag containing the links of the corresponding
	*          document.
	*/
	RXMLTag* GetLinks(void) {return(Links);}

	/**
	* Add a title to the metadata.
	* @param val            Value to assign.
	*/
	void AddTitle(const char* val) throw(bad_alloc);

	/**
	* Add a title to the metadata.
	* @returns Pointer to a XML Tag.
	*/
	RXMLTag* AddTitle(void) throw(bad_alloc);

	/**
	* Add a creator to the metadata.
	* @param val            Value to assign.
	*/
	void AddCreator(const char* val) throw(bad_alloc);

	/**
	* Add a creator to the metadata.
	* @returns Pointer to a XML Tag.
	*/
	RXMLTag* AddCreator(void) throw(bad_alloc);

	/**
	* Add a subject to the metadata.
	* @param val            Value to assign.
	*/
	void AddSubject(const char* val) throw(bad_alloc);

	/**
	* Add a subject to the metadata.
	* @returns Pointer to a XML Tag.
	*/
	RXMLTag* AddSubject(void) throw(bad_alloc);

	/**
	* Add a description to the metadata.
	* @param val            Value to assign.
	*/
	void AddDescription(const char* val) throw(bad_alloc);

	/**
	* Add a description to the metadata.
	* @returns Pointer to a XML Tag.
	*/
	RXMLTag* AddDescription(void) throw(bad_alloc);

	/**
	* Add a publisher to the metadata.
	* @param val            Value to assign.
	*/
	void AddPublisher(const char* val) throw(bad_alloc);

	/**
	* Add a publisher to the metadata.
	* @returns Pointer to a XML Tag.
	*/
	RXMLTag* AddPublisher(void) throw(bad_alloc);

	/**
	* Add a contributor to the metadata.
	* @param val            Value to assign.
	*/
	void AddContributor(const char* val) throw(bad_alloc);

	/**
	* Add a contributor to the metadata.
	* @returns Pointer to a XML Tag.
	*/
	RXMLTag* AddContributor(void) throw(bad_alloc);

	/**
	* Add a date to the metadata.
	* @param val            Value to assign.
	*/
	void AddDate(const char* val) throw(bad_alloc);

	/**
	* Add a date to the metadata.
	* @returns Pointer to a XML Tag.
	*/
	RXMLTag* AddDate(void) throw(bad_alloc);

	/**
	* Add a type to the metadata.
	* @param val            Value to assign.
	*/
	void AddType(const char* val) throw(bad_alloc);

	/**
	* Add a type to the metadata.
	* @returns Pointer to a XML Tag.
	*/
	RXMLTag* AddType(void) throw(bad_alloc);

	/**
	* Add a format to the metadata.
	* @param val            Value to assign.
	*/
	void AddFormat(const char* val) throw(bad_alloc);

	/**
	* Add a format to the metadata.
	* @returns Pointer to a XML Tag.
	*/
	RXMLTag* AddFormat(void) throw(bad_alloc);

	/**
	* Add an identifier to the metadata.
	* @param val            Value to assign.
	*/
	void AddIdentifier(const char* val) throw(bad_alloc);

	/**
	* Add an identifier to the metadata.
	* @returns Pointer to a XML Tag.
	*/
	RXMLTag* AddIdentifier(void) throw(bad_alloc);

	/**
	* Add a source to the metadata.
	* @param val            Value to assign.
	*/
	void AddSource(const char* val) throw(bad_alloc);

	/**
	* Add a source to the metadata.
	* @returns Pointer to a XML Tag.
	*/
	RXMLTag* AddSource(void) throw(bad_alloc);

	/**
	* Add a language to the metadata.
	* @param val            Value to assign.
	*/
	void AddLanguage(const char* val) throw(bad_alloc);

	/**
	* Add a language to the metadata.
	* @returns Pointer to a XML Tag.
	*/
	RXMLTag* AddLanguage(void) throw(bad_alloc);

	/**
	* Add a relation to the metadata.
	* @param val            Value to assign.
	*/
	void AddRelation(const char* val) throw(bad_alloc);

	/**
	* Add a relation to the metadata.
	* @returns Pointer to a XML Tag.
	*/
	RXMLTag* AddRelation(void) throw(bad_alloc);

	/**
	* Add a coverage to the metadata.
	* @param val            Value to assign.
	*/
	void AddCoverage(const char* val) throw(bad_alloc);

	/**
	* Add a coverage to the metadata.
	* @returns Pointer to a XML Tag.
	*/
	RXMLTag* AddCoverage(void) throw(bad_alloc);

	/**
	* Add rights to the metadata.
	* @param val            Value to assign.
	*/
	void AddRights(const char* val) throw(bad_alloc);

	/**
	* Add rights to the metadata.
	* @returns Pointer to a XML Tag.
	*/
	RXMLTag* AddRights(void) throw(bad_alloc);

	/**
	* Destruct the document.
	*/
	virtual ~GDocXML(void);
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
