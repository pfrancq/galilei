/*

	GALILEI Research Project

	GFilterRTF.h

	A RTF filter - Header.

	Copyright 2003 by the Universit�Libre de Bruxelles.

	Authors:
		Vandaele Valery(vavdaele@ulb.ac.be)
		Kumps Nicolas (nkumps@ulb.ac.be)

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
#include <sessions/galilei.h>
#include <docs/gfilter.h>
#include <codetochar.h>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------


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
		int Compare(const RString s) const {return(Name.Compare(s));}
	};

	/**
	* Header Tags.
	*/
	R::RContainer<Tag,true,true>* Tags;

	/**
	* Pointer to parameters of the current tag.
	*/
	char* Params;

	/**
	* The container of XML code
	*/
	R::RContainer<CodeToChar,true,true> Chars;

public:

	/**
	* Construct the XML filter for a specific XML document.
	* @param fac            Factory.
	* @param name           Name of the filter.
	*/
	GFilterRTF(GFactoryFilter* fac);

	/**
	* Analyze the document and fill the XML structure with the information
	* about its content.
	* @param doc            XML Structure that will represent the document.
	*/
	virtual bool Analyze(GDocXML* doc) throw(std::bad_alloc,GException);

protected:

	/**
	*Function who init the container of code and correspondant char
	*/
	void InitCharContainer(void);

	/**
	* This function  parse every sub block of the string of the document(block = {...} )
	* @param str		the string to search
	* @param text          if true ->text else metadatas
	*/
	void FindBlock(R::RString str,bool text=false);

	/*
	* Find the first tag in the string
	* @return       The tag found
	*/
	Tag* FindTag(RString str);

	/**
	* Analyse the content of the string and save it in the right docxmlTag
	* @param str            the string to analyse
	* @param t              Tag associated with the text to insert
	*/
	void AnalyseMeta(R::RString str, Tag* t);

	/**
	* Analyse the content of the string and save it in the right docxmlTag
	* @param str            the string to analyse
	*/
	void AnalyseText(R::RString str);

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
	static void CreateParams(GParams* params);

	/**
	*The destructor
	*/
	virtual ~GFilterRTF(void);
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
