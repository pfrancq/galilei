/*

	GALILEI Research Project

	GFilterSGML.h

	A SGML filter - Header.

	Copyright 2001 by the Université Libre de Bruxelles.

	Authors:
		Vandaele Valery (vavdaele@ulb.ac.be).

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
#ifndef GFilterSGMLH
#define GFilterSGMLH


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>
#include <filters/gfilter.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
* The GFilterSGML class provides a representation of a document to filter in a
* XML structure.
* @author Vandaele Valery
* @short SGML's Filter.
*/
class GFilterSGML: public GFilter
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
	* The container of SGML code
	*/
	R::RContainer<CodeToChar,unsigned int,true,true> Chars;

	/**
	* Determine if the current tag is a closing tag or an open one.
	*/
	bool bEndTag;

	/**
	* Holds the most opened level in the structure.
	*/
	unsigned int MinOpenLevel;


	/**
	*  TAG containing the content of the document.
	*/
	R::RXMLTag* contentT;
	
public:

	/**
	* Construct the SGML filter for a specific SGML document.
	* @param fac            Factory.
	* @param name           Name of the filter.
	*/
	GFilterSGML(GFactoryFilter* fac);

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
	* Function who analyse the content of the sgml document
	*/
	void AnalyseContent(void);
	
  /**
  * Function who analyse the document
  */
	void AnalyseDoc(void);

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
	* Create the parameters.
	* @param params          Parameters to configure.
	*/
	static void CreateParams(GParams* params);

	/**
	*The destructor
	*/
	virtual ~GFilterSGML(void);
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
