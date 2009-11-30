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

	RXMLParser* Parser;

public:

	/**
	* Construct the XML filter for a specific XML document.
	* @param fac            Factory.
	* @param name           Name of the filter.
	*/
	GFilterRTF(GPlugInFactory* fac);

	/**
	* Analyze a document with a given URI for which a DocXML must be created.
	* This method must be re-implemented by all filters.
	* @param doc             Document to analyze.
	* @param uri             URI of the file to analyze.
	* @param parser          Current parser of the XML stream.
	* @param rec             Receiver for the signals.
	*/
	virtual void Analyze(GDoc* doc,const RURI& uri,RXMLParser* parser,GSlot* rec);

protected:

	/**
	*Function who initialize the container of codes and the corresponding char.
	*/
	void InitCharContainer(void);

	/**
	* This function  parse every sub block of the string of the document(block = {...} )
	* @param str		the string to search
	* @param text          if true ->text else metadatas
	*/
	void FindBlock(RString str,bool text=false);

	/*
	* Find the first tag in the string
	* @return       The tag found
	*/
	Tag* FindTag(RString str);

	/**
	* Analyze the content of the string and save it in the right docxmlTag
	* @param str            the string to analyze
	* @param t              Tag associated with the text to insert
	*/
	void AnalyzeMeta(RString str, Tag* t);

	/**
	* Analyze the content of the string and save it in the right docxmlTag
	* @param str            the string to analyze
	*/
	void AnalyzeText(RString str);

	/*
	* Replace codes from the srting
	* @param str            the ingoing string
	* @return RString       the string with code replaced.
	*/
	RString ReplaceCodes(RString str);

public:

	/**
	* Create the parameters.
	* @param params          Parameters to configure.
	*/
	static void CreateParams(RConfig* params);

	/**
	*The destructor
	*/
	virtual ~GFilterRTF(void);
};


//-----------------------------------------------------------------------------
#endif
