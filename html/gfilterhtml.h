/*

	GALILEI Research Project

	GFilterHTML.h

	A HTML filter - Header.

	Copyright 2001 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).
		Vandaele Valery (vavdaele@ulb.ac.be)

	Version $Revision$

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
#ifndef GFilterHTMLH
#define GFilterHTMLH


//-----------------------------------------------------------------------------
// include files for R Project
//#include <rstd/rxmlfile.h>
//#include <rstd/rxmlstruct.h>
//#include <rstd/rxmltag.h>


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <codetochar.h>
#include <filters/gfilter.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
* The GFilterHTML class provides a representation of a document to filter in a
* XML structure.
* @author Pascal Francq
* @short HTML's Filter.
*/
class GFilterHTML: public GFilter
{
	class Tag;

	/**
	* Header Tags.
	*/
	R::RContainer<Tag,unsigned int,true,true>* Tags;

	/**
	* Buffer containing all the document.
	*/
	char* Buffer;

	/**
	* Pointer to the current position in the buffer.
	*/
	char* Pos;

	/**
	* Pointer to the beginning of the block actually treated.
	*/
	char* Block;

	/**
	* Total length of the block actually treated.
	*/
	unsigned int BlockLen;

	/**
	* Must the block be inserted (is there something else than spaces?)
	*/
	bool bBlockIns;

	/**
	* Total length of the current tag.
	*/
	unsigned int TagLen;

	/**
	* Pointer to the part to skip of the tag is not a valid one.
	*/
	char* SkipTag;

	/**
	* Pointer to the beginning of the current Tag.
	*/
	char* BeginTag;

	/**
	* Pointer to parameters of the current tag.
	*/
	char* Params;

	/**
	* Pointer to parameters of the next tag.
	*/
	char* NextParams;

	/**
	* Current tag.
	*/
	Tag* CurTag;

	/**
	* Next tag.
	*/
	Tag* NextTag;

	/**
	* The container of HTML code
	*/
	R::RContainer<CodeToChar,unsigned int,true,true> Chars;

	/**
	* Determine if the current tag is a closing tag or an open one.
	*/
	bool bEndTag;

	/**
	* Determine if the next tag is a closing tag or an open one.
	*/
	bool bEndNextTag;

	/**
	* Holds the most opened level in the structure.
	*/
	unsigned int MinOpenLevel;

	/**
	* Base of the document for URL of the links.
	*/
	char* Base;

public:

	/**
	* Construct the HTML filter for a specific HTML document.
	* @param fac            Factory.
	*/
	GFilterHTML(GFactoryFilter* fac);

	/**
	* Analyze the document and fill the XML structure with the information
	* about its content.
	* @param doc            XML Structure that will represent the document.
	*/
	virtual bool Analyze(GDocXML* doc);

protected:

	/**
	*Function who init the container of code and correspondant char
	*/
	void InitCharContainer(void);

	/**
	* This function skip spaces.
	*/
	inline void SkipSpaces(void)
	{while((*Pos)&&isspace(*Pos)) Pos++;}

	/**
	* Functiun who analyse the header of the html document
	*/
	void AnalyseHeader(void);

	/**
	* Functiun who analyse the body of the html document
	*/
	void AnalyseBody(void);

	/**
	* Analyze the tag "LINKS" of the html document.
	* @param params         Parameters contanined in the LINK tag.
	* @param block          Block contained after the tag.
	* @param ins            Must the block be inserted.
	*/
	void AnalyseLink(char* params,char* block,bool ins);

	/**
	* Function who analyze the tag "LINKS" of the html document
	* @param params        the parameters of the BASE tag
	*/
	void AnalyseBase(char* params);
	
	/**
	* Function who reconstruct the partial url of a link
	* @param u             The url to modify
	* @param char*         A pointer to the new URL
	*/
	char* ConstructURL(char* u);

	
	/**
	* Analyse the differents parameters contained in the link tag
	* @param params         the parameters to be analysed
	* @returns RXMLTag      pointer to the parent Tag
	*/
	R::RXMLTag* AnalyseLinkParams(char* params);

	
	/**
	* Treat a META tag.
	* @param params         Parameters of the META tag.
	* @param metaData       XML Tag representing the meta data of the document.
	*/
	void ReadMetaTag(char* params,R::RXMLTag* metaData);

	/**
	* This function replace codes by the corresponding characters.
	* Ex.: &egrave; by 'è'.
	*/
	void ReplaceCode(void);

	/**
	* Read the next HTML tag. In particular, the pointers BeginTag and Params
	* are set correctly. Set bEndTag to true if it is a ending tag.
	*/
	void ReadNextTag(void);

	/**
	* Call NexTag to read the next tag. If the tag is not valid, it is replace by
	* spaces. Everythin in comments or between two <SCRIPT> tags are also
	* replace by spaces.
	*/
	Tag* GetValidTag(void);

	/**
	* Call NexTag to read the next tag. If the tag is not valid, it is replace by
	* spaces. Everythin in comments or between two <SCRIPT> tags are also
	* replace by spaces. At the end:
	* - CurTag points to the current tag to handle.
	* - Block contains the string between CurTag and the next valid tag of the file.
	* - bBlockIns is true if the block contains non-space characters.
	* - Params contains the parameter of the tag.
	* - bEndTag is true if the current is a closing one.
	*/
	void NextValidTag(void);

public:

	/**
	* Create the parameters.
	* @param params          Parameters to configure.
	*/
	static void CreateParams(GParams* params);

	/**
	*The destructor
	*/
	virtual ~GFilterHTML(void);
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
