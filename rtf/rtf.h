/*

	GALILEI Research Project

	RTF.h

	A RTF filter - Header.

	Copyright 2001-2004 by Valery Vandaele.
	Copyright 2001-2004 by Nicolas Kumps.
	Copyright 2001-2008 by the Université Libre de Bruxelles (ULB).

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
#ifndef GFilterRTFH
#define GFilterRTFH


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>
#include <gfilter.h>
#include <codetochar.h>
using namespace GALILEI;
using namespace R;
using namespace std;


//-----------------------------------------------------------------------------
/**
* The GFilterRTF class provides a representation of a RTF document to filter in a
* RTF structure.
* @author Nicolas Kumps,Vandaele Valery
* @short RTF's Filter.
*/
class GFilterRTF: public GFilter
{
	class Tag
	{
	public:
		enum tTag{tNULL,tMAIN,tMETA,tTEXT,tSKIP,tTITLE,tAUTHOR,tPUBLI,tDESCRIPT,tDATE,tSUBJECT,tPAR};
		R::RString Name;
		tTag Type;

		Tag(const char* n,tTag t)
			: Name(n), Type(t) {}
		int Compare(const Tag* t) const {return(Name.Compare(t->Name));}
		int Compare(const Tag& t) const {return(Name.Compare(t.Name));}
		int Compare(const R::RString s) const {return(Name.Compare(s));}
	};

	/**
	* Header Tags.
	*/
	RContainer<Tag,true,true>* Tags;

	/**
	* Pointer to parameters of the current tag.
	*/
	char* Params;

	/**
	* The container of XML code
	*/
	RContainer<CodeToChar,true,true> Chars;

public:

	/**
	* Construct the RTF filter.
	* @param session         Session.
	* @param fac            Factory.
	*/
	GFilterRTF(GSession* session,GPlugInFactory* fac);

	/**
	* Analyze a document with a given URI.
	* @param analyzer        Analyzer.
	* @param doc             Document to analyze.
	* @param file            File to analyze (eventually a local copy of a
	*                        remote document).
	*/
	virtual void Analyze(GDocAnalyze* analyzer,const GDoc* doc,const R::RURI& file);

protected:

	/**
	*Function who initialize the container of codes and the corresponding char.
	*/
	void InitCharContainer(void);

	/**
	* This function  parse every sub block of the string of the document(block = {...} )
   * @param analyzer        Analyzer.
	* @param str		the string to search
	* @param text          if true ->text else metadatas
	*/
	void FindBlock(GDocAnalyze* analyzer,RString str,bool text=false);

	/*
	* Find the first tag in the string
	* @return       The tag found
	*/
	Tag* FindTag(RString str);

	/**
	* Analyze the content of a metadata.
   * @param analyzer        Analyzer.
	* @param str            the string to analyze
	* @param t              Tag associated with the text to insert
	*/
	void AnalyzeMeta(GDocAnalyze* analyzer,RString str, Tag* t);

	/**
	* Analyze the content of the string.
	* @param str            the string to analyze
   * @param analyzer        Analyzer.
	*/
	void AnalyzeText(GDocAnalyze* analyzer,RString str);

	/*
	* Replace codes from the string.
	* @param str            the ingoing string
	* @return RString       the string with code replaced.
	*/
	RString ReplaceCodes(RString str);

public:

	/**
	* The destructor.
	*/
	virtual ~GFilterRTF(void);
};


//-----------------------------------------------------------------------------
#endif
