/*

	GALILEI Research Project

	GFilterHTML.h

	A HTML filter - Header.

	Copyright 2001 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).

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
#include <rstd/rstring.h>
#include <rstd/rcontainer.h>
using namespace RStd;
#include <rxml/rxmlfile.h>
#include <rxml/rxmlstruct.h>
using namespace RXML;


//-----------------------------------------------------------------------------
// include files for ANSI C/C++
//#include <rio/rtextfile.h>
using namespace RIO;


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <filters/codetochar.h>
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
	RStd::RContainer<Tag,unsigned int,true,true>* Tags;

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
	* Current tag.
	*/
	Tag* CurTag;

	/**
	* The container of HTML code
	*/
	RContainer<CodeToChar,unsigned int,true,true> Chars;

	/**
	* Determine if the current tag is a closing tag or an open one.
	*/
	bool bEndTag;

	/**
	* Holds the most opened level in the structure.
	*/
	unsigned int MinOpenLevel;

public:

	/**
	* Construct the HTML filter for a specific HTML document.
	* @param mng            Manager.
	*/
	GFilterHTML(GURLManager* mng);

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
	* Treat a META tag.
	* @param params         Parameters of the META tag.
	* @param metaData       XML Tag representing the meta data of the document.
	*/
	void ReadMetaTag(char* params,RXMLTag* metaData);

	/**
	* This function replace codes by the corresponding characters.
	* Ex.: &egrave; by 'è'.
	*/
	void ReplaceCode(void);

	/**
	* Read the next tag. In particular, the pointers BeginTag and Params are
	* set correctly. Set bEndTag to true if it is a ending tag.
	*/
	void NextTag(void);

	/**
	* Call NexTag to read the next tag. If the tag is not valid, it is replace by
	* spaces. Everythin in comments or between two <SCRIPT> tags are also
	* replace by spaces.
	*/
	void NextValidTag(void);

public:

	/**
	*The destructor
	*/
	virtual ~GFilterHTML(void);
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
