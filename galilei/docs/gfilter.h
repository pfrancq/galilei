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
using namespace RStd;


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
*/
class GFilter
{	
protected:
	/**
	* Manager.
	*/
	GURLManager* Manager;

	/**
	* URL of the document to filter.
	*/
	RString URL;

	/**
	* Structure corresponding to the document.
	*/
	GDocXML* Doc;

public:

	/**
	* Construct the filter for a specific document.
	* @param url            URL of the document.
	* @param mng            Manager.
	*/
	GFilter(const RString& url,GURLManager* mng);

	/**
	* Compare method used by RContainer.
	*/
	int Compare(const GFilter* f) const {return(-1);}

	/**
	* Compare method used by RContainer.
	*/
	int Compare(const GFilter& f) const {return(-1);}

	/**
	* Add a specific MIME-Type for this filter.
	* @param mime           Name of the MIME-Type.
	*/
	void AddMIME(const RString& mime);

	/**
	* Analyze the document and fill the XML structure with the information
	* about its content. This method must be reimplemented in the child
	* classes.
	* @param doc            XML Structure that will represent the document.
	*/
	virtual bool Analyze(GDocXML* doc)=0;

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

	/**
	* Destructor of the filter.
	*/
	virtual ~GFilter(void);
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
