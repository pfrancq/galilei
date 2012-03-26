/*

	GALILEI Research Project

	PS.h

	A PostScript filter - Header.

	Copyright 2003-2012 by Pascal Francq (pascal@francq.info).
	Copyright 2003-2008 by the Université Libre de Bruxelles (ULB).
	Copyright 1995-1998 by Digital Equipment Corporation.
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
*/



//-----------------------------------------------------------------------------
#ifndef GFilterPSH
#define GFilterPSH


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>
#include <gfilter.h>
using namespace GALILEI;
using namespace R;
using namespace std;


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
class GFilterPS: public GFilter
{
	/**
	* Pointer to the current position in the buffer.
	*/
	RChar* Pos;

	/**
	* Pointer to begin of a block.
	*/
	RChar* Begin;

	/**
	* Buffer containing the text of the postscript file.
	*/
	RChar* CharBuffer;

	/**
	* Number of character actually filled.
	*/
	size_t CharLen;

	/**
	* Actual size allocated in the buffer.
	*/
	size_t CharMax;

public:

	/**
	* Construct the PostScript filter.
	* @param session        Session.
	* @param fac            Factory.
	*/
	GFilterPS(GSession* session,GPlugInFactory* fac);

	/**
	* Analyze a document with a given URI.
	* @param analyzer        Analyzer.
	* @param doc             Document to analyze.
	* @param file            File to analyze (eventually a local copy of a
	*                        remote document).
	*/
	virtual void Analyze(GDocAnalyze* analyzer,const GDoc* doc,const R::RURI& file);

	/**
	 * Extract a text fragment from a file with a given URI.
	 * @param fragment       Fragment to extract.
    */
	virtual R::RString GetTextFragment(GDocFragment* fragment);

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
	* Destructor.
	*/
	virtual ~GFilterPS(void);
};


//-----------------------------------------------------------------------------
#endif
