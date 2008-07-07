/*

	GALILEI Research Project

	GFilterPS.h

	A PS filter - Header.

	Copyright (C) 1995-1998, Digital Equipment Corporation.
	All rights reserved.
	See the file pstotext.txt for a full description.

	Modified by:
		Last modified on Fri Oct 16 16:27:54 PDT 1998 by mcjones
		Modified on Thu Nov 16 13:33:13 PST 1995 by deutsch
		Modified on 27-MAY-1998 13:08 by Hunter Goatley
 			Ported to VMS.  Various minor changes to allow it to work on
 			both VAX and Alpha (VAX C and DEC C).  VMS pipes don't work
			right, so the GS output is dumped to a temporary file that's
 			read, instead of reading from pipes (which is, of course, how
 			VMS implements pipes anyway).  Also added -output option.
		Pascal Francq (pfrancq@ulb.ac.be)
			Create Filter for GALILEI

*/



//-----------------------------------------------------------------------------
#ifndef GFilterPSH
#define GFilterPSH


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>
#include <gfilter.h>


//-----------------------------------------------------------------------------
// forward declaration
class Dict;
class UnicodeMap;


//-----------------------------------------------------------------------------
/**
* The GFilterPS class provides a representation of a document to filter in a
* XML structure.
* @author Pascal Francq
* @short HTML's Filter.
*/
class GFilterPS: public GALILEI::GFilter
{
	/**
	* Pointer to the current position in the buffer.
	*/
	R::RChar* Pos;

	/**
	* Pointer to begin of a block.
	*/
	R::RChar* Begin;

	/**
	* Buffer containing the text of the postscript file.
	*/
	R::RChar* CharBuffer;

	/**
	* Number of character actually filled.
	*/
	unsigned int CharLen;

	/**
	* Actual size allocated in the buffer.
	*/
	unsigned int CharMax;

public:

	/**
	* Construct the HTML filter for a specific HTML document.
	* @param fac            Factory.
	*/
	GFilterPS(GALILEI::GFactoryFilter* fac);

	/**
	* Analyze a document with a given URI that was downloaded in a local
	* temporary file and for which a DocXML must be created.
	* @param uri             URI of the file to analyze.
	* @param file            Local file to analyze.
	* @param docxml          Local file that will containing the DocXML.
	*/
	virtual void Analyze(const R::RURI& uri,const R::RString& file,const R::RString& docxml);


protected:

	/**
	* This function skip spaces.
	*/
	inline void SkipSpaces(void)
	{while((!Pos->IsNull())&&(Pos->IsSpace())) Pos++;}

	/**
	* Add a "C" string to the buffer.
	*/
	void StrToBuffer(const char* str);

public:

	/**
	* Create the parameters.
	* @param params          Parameters to configure.
	*/
	static void CreateParams(R::RConfig* params);

	/**
	* Destructor.
	*/
	virtual ~GFilterPS(void);
};


//-----------------------------------------------------------------------------
#endif
