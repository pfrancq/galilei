/*

	GALILEI Research Project

	GFilter.h

	Generic Filter for documents - Header.

	(C) 2001 by P. Francq.

	Version $Revision$

	Last Modify: $Date$

*/



//-----------------------------------------------------------------------------
#ifndef GFilterH
#define GFilterH


//-----------------------------------------------------------------------------
// include files for R Project
#include <rstd/rstring.h>


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <gdocs/gdocxml.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// forward class declaration
class GURLManager;


//-----------------------------------------------------------------------------
/**
* The GFilter class provides a representation of a document to filter in a XML
* structure.
* @author Pascal Francq
* @short Generic Filter.
*/
class GFilter
{	
protected:

	/**
	* Manager.
	*/
	GURLManager* Manager;

	/**
	* Structure corresponding to the document currently analyzed.
	*/
	GDocXML* Doc;

	/**
	* Name of the filter.
	*/
	RStd::RString Name;

	/**
	* Mimetypes handle by the Filters separated by a semi-colon.
	*
	* Example: text/html;text/xml
	*/
	RStd::RString MIMES;

	/**
	* Version of the filter.
	*/
	RStd::RString Version;

public:

	/**
	* Construct the filter for a specific document.
	* @param mng            Manager.
	* @param name           Name of the filter.
	* @param mimes          MIME Types of the filter.
	* @param vers           Version of the filter.
	*/
	GFilter(GURLManager* mng,const char* name,const char* mimes,const char* ver);

	/**
	* Compare method used by RContainer.
	*/
	int Compare(const GFilter* f) const {return(-1);}

	/**
	* Compare method used by RContainer.
	*/
	int Compare(const GFilter& f) const {return(-1);}

	/**
	* Get the name of the filter.
	* @returns Pointer to a C String.
	*/
	const char* GetName(void) const {return(Name());}

	/**
	* Get the Mime Types of the filter.
	* @returns Pointer to a C String.
	*/
	const char* GetMIMES(void) const {return(MIMES());}

	/**
	* Get the version of the filter.
	* @returns Pointer to a C String.
	*/
	const char* GetVersion(void) const {return(Version());}

protected:

	/**
	* Add a specific MIME-Type for this filter.
	* @param mime           Name of the MIME-Type.
	*/
	void AddMIME(const char* mime);

	/**
	* @return true if the character represent a end of sentence.
	*/
	static inline bool IsEndSentence(char c)
	{
		return((c=='.')||(c=='!')||(c=='?')||(c==';')||(c==':'));
	}

	/**
	* Analyse a block of text and create a list of tags 'sentence'.
	* @param block          Block containing the text.
	* @param attach         XML tag where the sentences must be attach.
	*/
	void AnalyzeBlock(char* block,RXMLTag* attach);

	/**
	* Analyse a list of keywords sepating by a single character.
	* @param list           List of keywords.
	* @param sep            Separator to use.
	* @param attach         XML tag where the sentences must be attach.
	*/
	void AnalyzeKeywords(char* list,char sep,RXMLTag* attach);

public:

	/**
	* Analyze the document and fill the XML structure with the information
	* about its content. This method must be reimplemented in the child
	* classes.
	* @param doc            XML Structure that will represent the document.
	*/
	virtual bool Analyze(GDocXML* doc)=0;

	/**
	* Destructor of the filter.
	*/
	virtual ~GFilter(void);
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
