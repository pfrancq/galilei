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

	Version $Revision$

*/



//-----------------------------------------------------------------------------
#ifndef GFilterPSH
#define GFilterPSH


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <sessions/galilei.h>
#include <docs/gfilter.h>


//-----------------------------------------------------------------------------
// forward declaration
class Dict;
class UnicodeMap;


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
* The GFilterPS class provides a representation of a document to filter in a
* XML structure.
* @author Pascal Francq
* @short HTML's Filter.
*/
class GFilterPS: public GFilter
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
	* Buffer containing the texte of the postscript file.
	*/
	R::RChar* CharBuffer;

	/**
	* Number of character actually filled.
	*/
	unsigned int CharLen;

	/**
	* Actuel size allocated in the buffer.
	*/
	unsigned int CharMax;

public:

	/**
	* Construct the HTML filter for a specific HTML document.
	* @param fac            Factory.
	*/
	GFilterPS(GFactoryFilter* fac);

	/**
	* Analyze the document and fill the XML structure with the information
	* about its content.
	* @param doc            XML Structure that will represent the document.
	*/
	virtual bool Analyze(GDocXML* doc) throw(std::bad_alloc,GException);

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
	static void CreateParams(GParams* params);

	/**
	* Destructor.
	*/
	virtual ~GFilterPS(void);
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
