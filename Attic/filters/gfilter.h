/*

	GALILEI Research Project

	GFilter.h

	Generic Filter for documents - Header.

	Copyright 2001 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).

	Version $Revision$

	Last Modify: $Date$

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
#ifndef GFilterH
#define GFilterH


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>
#include <docs/gdocxml.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------


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
	* Structure corresponding to the document currently analyzed.
	*/
	GDocXML* Doc;

	/**
	* Name of the filter.
	*/
	R::RString Name;

	/**
	* Mimetypes handle by the Filters separated by a semi-colon.
	*
	* Example: text/html;text/xml
	*/
	R::RString MIMES;

	/**
	* Version of the filter.
	*/
	R::RString Version;

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
	int Compare(const GFilter* ) const {return(-1);}

	/**
	* Compare method used by RContainer.
	*/
	int Compare(const GFilter& ) const {return(-1);}

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
	* @param mng            Manager.
	* @param mime           Name of the MIME-Type.
	*/
	void AddMIME(GURLManager* mng,const char* mime);

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
	void AnalyzeBlock(char* block,R::RXMLTag* attach);

	/**
	* Analyse a list of keywords sepating by a single character.
	* @param list           List of keywords.
	* @param sep            Separator to use.
	* @param attach         XML tag where the sentences must be attach.
	*/
	void AnalyzeKeywords(char* list,char sep,R::RXMLTag* attach);

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


//-----------------------------------------------------------------------------
/**
* The GFilterCursor class provides a way to go trough a set of filters.
* @short SubProfiles Cursor
*/
CLASSCURSOR(GFilterCursor,GFilter,unsigned int)


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
