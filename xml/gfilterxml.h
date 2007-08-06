/*

	GALILEI Research Project

	GFilterXML.h

	A XML filter - Header.

	Copyright 2001-2007 by the Université Libre de Bruxelles.

	Authors:
		Vandaele Valery (vavdaele@ulb.ac.be)
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

/**
* How to use the tTags:
* Each tTag is associated to a king of docxml information.
* tNULL :               Represents a tag that is not associated to a docxml Tag
*			but Which content will be add to the content of the docxml structure
* tMAIN :               Represents the upper tag (root tag)
*tATTRIBUT 		Represents an attribute of a tag
*tSKIP                  Represents a tag that must be skipped (tag+content)
*tTITLE			The tag represents the title of the doc
*tDATE			The tag represents the date of the doc
*tAUTHOR		The tag represents the author of the doc
*tPUBLI			The tag represents the publisher of the doc
*tSOURCE		The tag represents the source of the doc
*tCONTRIB		The tag represents the contibutor of the doc
*tDESCRIPT		The tag represents the description of the doc (usually a list of keywords)
*tRIGHTS		The tag represents the copyrights of the doc
*tLANG			The tag represents the language of the doc
*tMETA			The tag represents the metadata of the doc
*tIDENTIFIER		The tag represents the identifier of the doc (reserved)
*tRELATION		The tag represents the relation of the doc
*tCOVERAGE		The tag represents the coverage of the doc
*tTEXT			The tag represents the main Text of the doc
*tPAR			The tag represents a paragraph of the doc
*tSUBJCET		The tag represents the subject of the doc
*tSUMMARY		The tag represents the summary of the doc (this will be add to the content of the docxml structure as a par)
tH1->tH6		The tag represents the heading style of a part of the doc
*
*/


//-----------------------------------------------------------------------------
#ifndef GFilterXMLH
#define GFilterXMLH


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>
#include <gfilter.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
* The GFilterXML class provides a representation of a XML document to filter in a
* XML structure.
* @author Vandaele Valery
* @short XML's Filter.
*/
class GFilterXML: public GFilter
{
	// Enumeration of all possible tags
	enum tTag{tNULL,tMAIN,tATTRIBUT,tSKIP,tTITLE,tDATE,tAUTHOR,tPUBLI,tSOURCE,tCONTRIB,tDESCRIPT,tRIGHTS,tLANG,tMETA,tIDENTIFIER,tRELATION,tCOVERAGE,tTEXT,tPAR,tSUBJECT,tSUMMARY,tH1,tH2,tH3,tH4,tH5,tH6};

	// forward declaration of class Attributs and Tag containing correspondence between XML tags and docXML tags
	class Attribut;
	class Tag;
	class Def;
	class MimeDef;

	/*
	* Structure to maintain Tags name and eventually the parent associated to the tag
	*/
	typedef struct TagStruct{
		R::RString Name;
		R::RString Parent;
		TagStruct(RString n,RString p): Name(n),Parent(p){}
		int Compare(const TagStruct& tag) const
		{
			//if identical test the parent
			if(!Name.Compare(tag.Name))
				return (Parent.Compare(tag.Parent));
			else
				return(Name.Compare(tag.Name));
		}
		int Compare(const TagStruct* tag) const
		{
			//if identical test the parent
			if(!Name.Compare(tag->Name))
				return (Parent.Compare(tag->Parent));
			else
				return(Name.Compare(tag->Name));
		}
	};

	/**
	* The directory where the xml definition files are stored
	*/
	RString xmlDefPath;

	/**
	* Definitions (all set of tags with their corresponding mime type).
	*/
	R::RContainer<Def,true,true>* Definitions;

	/**
	* Link between MimeTypes and associated Definitions
	*/
	R::RContainer<MimeDef,true,true>* MimeDefinitions;

	/**
	* The current used definityion for the tags
	*/
	Def* CurrentDefinition;

	/**
	* Maintain the state of the definitions
	* false if not inited or path has changed
	*/
	bool DefinitionIsInited;

public:

	/**
	* Construct the XML filter for a specific XML document.
	* @param fac            Factory.
	* @param name           Name of the filter.
	*/
	GFilterXML(GFactoryFilter* fac);

	/**
	* Analyze the document and fill the XML structure with the information
	* about its content.
	* @param doc            XML Structure that will represent the document.
	*/
	virtual bool Analyze(GDocXML* doc);

	/**
	* Configurations were applied from the factory.
	*/
	virtual void ApplyConfig(void);


protected:

	/**
	* Insert Tag in docXML structure and return inserted Tag (No content is added)
	* @param currentTag       the tag to add to the structure
	* @param parentTag        the parent tag in the docxml structure.
	* @return RXMLTag         pointer to the inserted tag in docxml structure
	*/
	RXMLTag* InsertTag(tTag t,RXMLTag* parentTag);

	/**
	* Analyse Tag
	* @param curTag            the tag to insert in docxml struct
	* @param curTag            the last tag added to the structure.
	* @param parentTag         the parent Tag in docxml(to which the curTag must be added)
	*/
	void AnalyseTag(RXMLTag* curTag,RXMLTag* currentTagParent,RXMLTag* parentTag);

	/**
	* Analyse attributes
	* @param currentTag            the tag to insert in docxml struct
	*/
	void AnalyzeAttributes(RXMLTag* currentTag,RXMLTag* currentTagParent);

	/**
	* Load the different tags definitions from files
	*/
	void LoadDefinitions(void);

	/**
	* Read the content of the filter definition
	*/
	void FillFilterDefinitions(R::RXMLTag* tag);

	/**
	* read the content of the tags definitions
	*/
	void FillTagsDefinition(R::RXMLTag* currentTag,Def* def);

	/**
	* read the content of the attributes tags definitions
	*/
	void FillAttributesDefinition(RXMLTag* currentTag,Tag* t);

	/**
	* read the content of the mimeTypes definitions
	*/
	void FillMimeDefinition(R::RXMLTag* currentTag, Def* def);

	/**
	* Convert a RString into a tTag Enum
	*/
	tTag ConvertRStringtoTagEnum(RString str);

	/**
	* Get a cursor over the container of definitions
	*/
	R::RCursor<Def> GetDefinitionsCursor(void);

	/**
	* Get a cursor over the container of mimedefinitions
	* (link between one mime type and its corresponding tag definition)
	*/
	R::RCursor<MimeDef> GetMimeDefinitionsCursor(void);

public:

	//-----------------------------------------------------------------------------
	//virtual void Connect(GSession* session);

	/**
	* Create the parameters.
	* @param params          Parameters to configure.
	*/
	static void CreateParams(RConfig* params);

	/**
	*The destructor
	*/
	virtual ~GFilterXML(void);
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
