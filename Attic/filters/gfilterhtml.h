/*

	GALILEI Research Project

	GFilterHTML.h

	A HTML filter - Header.

	(C) 2001 by J. LAMORAL

	Version $Revision$

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
#include <rio/rtextfile.h>
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
*/


class GFilterHTML: public GFilter
{

public:

	/**
	* Buffer containing all the email.
	*/
	char* Buffer;

private:

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
	*Some XMl tag
	*/
	RXMLTag *top,*objs,*act,*meta,*content,*links;

	/**
	*Some XMl tag used in get current value
	*/
	RXMLTag* TagVector[8] ;


public:

	/**
	* Construct the HTML filter for a specific HTML document.
	* @param mng            Manager.
	*/
	GFilterHTML(GURLManager* mng);

	/**
	* Functiun who analyse the header of the html document
	*/
	void AnalyseHeader(GDocXML* doc);
	
private:
	/**
	* Functiun who analyse the body of the html document
	*/
	void AnalyseBody(GDocXML* doc);
	
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
	void GetValue (GDocXML* doc);
	
	/**
	*Get the value into 2 tag used in body
	*@param  current      The name of the current tag
	*/
	void GetValueCurentTag (char* current,GDocXML* doc);

	/**
	*Get the value into 2 tag used in metadata for the keywords and the resume
	*/
	void GetMetaValue (GDocXML* doc);

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
	virtual bool Analyze(GDocXML* doc);

	/**
	*Return true if the caractere is a space a tab or a carriage return
	*/
	bool IsSpace(void);

	/**
	*Skip spaces into the buffer
	*/
	void SkipSpaces(void);
	
	
	/**
	* Add a tag in the xml struct
	* @param level          	The level of the tag
	* @param tag				The name of the tag
	* @param doc            XML Structure that will represent the document.
	*/
	void AddTag(int level,char* tag,GDocXML* doc);

	/**
	*Initialisation of the html string
	*/
	void InitWords(void);

	/**
	*Goto to the next word
	*/
	bool NextWord(void);
	
public:
	/**
	*The destructor
	*/
	virtual ~GFilterHTML(void);
	
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
