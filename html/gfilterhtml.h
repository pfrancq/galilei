/*

	GALILEI Research Project

	GDocXML.h

	A XML representation of a document - Header.

	(C) 2001 by J. LAMORAL

	Version $Revision$

*/


//-----------------------------------------------------------------------------
#ifndef GFilterHTML_H
#define GFilterHTML_H


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
#include <rio/rtextfile.h>
using namespace RIO;


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <filters/codetochar.h>
#include <filters/gfilter.h>
#include <glib.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------




//-----------------------------------------------------------------------------
/**
* The GFilterHTML class provides a representation of a document to filter in a
* XML structure.
* @author Pascal Francq
*/

class GFilterHTML: public GFilter
{

public:

    /**
	* Buffer containing all the email.
	*/
	char* Buffer;

	/**
	* Pointer to the current position in the buffer.
	*/
	char* Pos;

	/**
	* Pointer to the beginning of the block actually treated.
	*/
	char* Begin;
	
	/**
	* The container of html code
	*/
	RContainer<CodeToChar,unsigned int,true,true> Chars;
	
	/**
	* Pointer to the current position in the buffer.
	*/
	char* hold;
	
	/**
	*Pointer to the value of the tag.
	*/
	char* ptrvalue;
	
	
	/**
	* The current Tag and the Old tag used for close non closed tag
	*/
	RString TAG,OldTag;

		
	/**
	* Some counter used for the counting of opened and closed tag
	*/
	int h [7];                      // some counter

		
	/**
	* counter used for the counting of opened and closed tag on p
	*/
	int p;

			
	/**
	*Some counter used for the counting  closed tag
	*/
	int ClassementVector [8];
	
	
	/**
	* Used for cout the open tag
	*/
	bool NextWirteTag;
	
	/**
	* true if the buffer is at the end of the document
	*/
	bool end;
	
	/**
	* the xml structure corresponding to the html document
	*/
	RXMLStruct s;
	
	
	/**
	*Some XMl tag
	*/
	RXMLTag *top,*objs,*act,*meta,*content,*links;
	
	/**
	*Some XMl tag used in get current value
	*/
	RXMLTag* TagVector [];


public:

	/**
	* Construct the HTML filter for a specific HTML document.
	* @param url            URL of the document.
	* @param mng            Manager.
	*/
	GFilterHTML(const RString& url,GURLManager* mng);
	
	
	/**
	*Function who init the container of code and correspondant char
	*/
	void InitCharContainer (void);
	
	/**
	*Function who compare 2 tag
	*@param current       The name of the tag to be compare
	*/
	bool TagCompare (const char* current);
	
	/**
	*Get the value into 2 tag used in metadata for the title
	*/
	void GetValue ();
	
	/**
	*Get the value into 2 tag used in body
	*@param  current      The name of the current tag
	*/
	void GetValueCurentTag (char* current);
	

	/**
	*Get the value into 2 tag used in metadata for the keywords and the resume
	*/
	void GetMetaValue ();
	
	/**
	*Write a tag and his attibute to stdout
	*@param name                the name of the tag
	*@param Attribut            the attrbute of the tag
	*/
	void WriteTag (char* name,char* Attribut);
	
	/**
	*Write a end tag to stdout
	*@param name     the name of the tag
	*/
	void WriteEndTag (char* name);
	
	/**
	*Functiun who close all tag whit a level > level of the current closed tag
	*@param level               the level  of the current  tag (1 for h1 ..6 for h6, 7for p)
	*/
	void CloseallTag(int level);
	
	/**
	*Go to the next tag
	*/
	void NextTag (void);
	
	/**
	*Go to the next end tag
	*@param name               the name of the tag
	*/
	void NextEndTag (char* name);
	
    /**
	* Analyze the document and fill the XML structure with the information
	* about its content.
	* @param doc            XML Structure that will represent the document.
	*/
	void Analyse();

	/**
	*Return true if the caractere is a space a tab or a carriage return
	*/
	bool IsSpace(void);
	
	/**
	*Skip spaces into the buffer
	*/
	void SkipSpaces(void);
	
	/**
	* Analyze an HTML document and fill the XML structure with the information content.
	*
	* Actually, only local files can be treated.
	* @param doc            XML Structure that will represent the email.
	*/
	virtual bool Analyze(GDocXML* doc);
	
	/**
	*Initialisation of the html string
	*/
	void InitWords(void);
	
	/**
	*Goto to the next word
	*/
	bool NextWord(void);
	
	
	/**
	*The destructor
	*/
	~GFilterHTML();
	
};//-------- End of namespace GALILEI ----------------------------------------
}


//-----------------------------------------------------------------------------
#endif
