/*

	GALILEI Research Project

	GFilterMSDoc.h

	A MSDoc filter - Header.

	Copyright 2001-2003 by the Université Libre de Bruxelles.

	Authors:
		Vandaele Valery(vavdaele@ulb.ac.be).

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
#ifndef GFilterMSDocH
#define GFilterMSDocH


//-----------------------------------------------------------------------------
// include files for ANSI C/C++
#include <deque>
#include <queue>


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>
#include <gfilter.h>

//-----------------------------------------------------------------------------
// include files for wvWare Project
#include <wv2/parser.h>
#include <wv2/parserfactory.h>
#include <wv2/handlers.h>
#include <wv2/functor.h>
#include <wv2/associatedstrings.h>
#include <wv2/paragraphproperties.h>
#include <wv2/fields.h>

//-----------------------------------------------------------------------------
// forward declaration

namespace wvWare {
	class Parser;
	namespace Word97 {
		class BRC;
	}
}


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
* The GFilterMSDoc class provides a representation of a document to filter in a
* XML structure.
* @author Vandaele Valery
* @short MSDoc's Filter.
*/
class GFilterMSDoc : public GFilter , public wvWare::SubDocumentHandler, public wvWare::TextHandler
{
	/**
	* Pointer to the current position in the buffer.
	*/
	R::RChar* Pos;

	/**
	* Pointer to begin of a block.
	*/
	R::RChar* Begin;


	// variables for SubDocumentHandler
	/*
	* Boolean value : true if the body has been found
	*/
	bool bodyFound;

	/*
	* The Number of FootNote encoutered
	*/
	int footNoteNumber;

	/*
	* The Number of EndNote (closing of FootNote) encoutered
	*/
	int endNoteNumber;


	// TextHandler vars
	/*
	* The number of section encoutered
	*/
	int sectionNumber;

	/*
	* remember if we are inside the paragraph of not.
	*/
	bool bInParagraph;

	/*
	* Properties of the paragraph - Not In USE !!!
	*/
	wvWare::SharedPtr<const wvWare::ParagraphProperties> ParagraphProper;

	/*
	* The Text contained in the paragraph
	*/
	R::RString Paragraph;

	/*
	* remember if we are inside a Field.
	*/
	bool InsideField;

	/*
	* remember if the field separator has already been met.
	*/
	bool FieldAfterSeparator;

	/*
	* remember the type of the currrent field.
	*/
	int FieldType;

	/*
	* remember the information contained in the current field.
	*/
	R::RString FieldValue;

	/*
	* The Parser for the MSWord document
	*/
	wvWare::SharedPtr<wvWare::Parser> Parser;

	/*
	* Table Handler not in use !!!!
	*/
	wvWare::TableHandler* TableHandler;


	typedef const wvWare::FunctorBase* FunctorPtr;
	struct SubDocument
	{
		SubDocument(FunctorPtr ptr, int d) : functorPtr(ptr), data(d){}
		~SubDocument() {}
		FunctorPtr functorPtr;
		int data;
	};

	/*
	* queue of SubDocuments
	*/
	std::queue<SubDocument> SubDocQueue;

public:

	/**
	* Construct the MSDoc filter for a specific MSDoc document.
	* @param fac            Factory.
	*/
	GFilterMSDoc(GFactoryFilter* fac);

	/**
	* Analyze the document and fill the XML structure with the information
	* about its content.
	* @param doc            XML Structure that will represent the document.
	*/
	virtual bool Analyze(GDocXML* doc);

protected:

	//-----------------Conversion-----------------------------
	/*
	* Convert a string from class "UString" to class "RString"
	* But a character \' has not the correct code --> always apply after the ConvertChar Method !!!!!
	* @param ustr                   The UString to convert
	* @return RString               The string converted to RString
	*/
	R::RString ConvertUtoRString(const wvWare::UString& ustr);

	/*
	* Replace the code of the character \' ( because wrong code is produced by the wvWare::Parser.
	* @param str                    The string where the char \' must be replaced.
	* @return RString               The string with the correct char code.
	*/
	R::RString ConvertChar(const R::RString& str);

	/*
	* convert wv2.FLD to FIELD.Subtype
	* @param fld               Describe the type of the field.
	* @return int              return nt to describe the kind of field for use in galilei.
	*/
	int fldToFieldType(const wvWare::FLD* fld);

	//-----------------End Conversion-----------------------------

	/*
	* Read the MetaDatas contained in the document.
	*/
	void ReadMetaData();

	/*
	* Add the subDoc to the pile when a new one is found.
	*/
	void subDocFound(const wvWare::FunctorBase* functor, int data);

	/*
	* Write the current paragraph
	* @params par               the text of the paragraph to be written.
	*/
	void WriteParagraph(R::RString par);

	/*
	* Add field to the DocXML struct -->title, author, links,...
	*/
	void AddField();

public:

	// All methods for SubDocumentHandler .
	//-------------------------------------

	/*
	* from wvWare::SubDocumentHandler
	* Go there when begining parsing the body
	*/
	virtual void bodyStart(void);

	/*
	* from wvWare::SubDocumentHandler
	* Go there when begining parsing the body
	*/
	//virtual void bodyEnd(void);

	/*
	* from wvWare::SubDocumentHandler
	* Go there when parsing the headers
	*/
	//virtual void headersStart();

	/*
	* from wvWare::SubDocumentHandler
	* Go there when finishing the headers.
	*/
	//virtual void headersEnd();

	/*
	* from wvWare::SubDocumentHandler
	* Go there when parsing a header
	*/
	//virtual void headerStart(wvWare::HeaderData::Type type );

	/*
	* from wvWare::SubDocumentHandler
	* Go there when finishing the header.
	*/
	//virtual void headerEnd(void);

	/*
	* from wvWare::SubDocumentHandler
	* Go there when begining a new footnote
	*/
	virtual void footnoteStart(void);

	/*
	* from wvWare::SubDocumentHandler
	* Go there when finishing the footnote.
	*/
	//virtual void footnoteEnd(void);


	// All methods for TextHandler .
	//-------------------------------------

	/*
	* from wvWare::TextHandler
	* Go there when begining a new section
	*/
	//virtual void sectionStart(wvWare::SharedPtr<const wvWare::Word97::SEP> sep);

	/*
	* from wvWare::TextHandler
	* Go there when finishing the current section
	*/
	//virtual void sectionEnd();

	/*
	* from wvWare::TextHandler
	* Go there when a page break within a section is encontered
	*/
	//virtual void pageBreak();

	/*
	* from wvWare::TextHandler
	* Emitted when a header or footer is found.
	*/
	virtual void headersFound( const wvWare::HeaderFunctor& parseHeaders);

	/*
	* from wvWare::TextHandler
	* Denote the start of a paragraph.
	@param paragraphProp           The properties of the current paragraph.
	*/
	virtual void paragraphStart(wvWare::SharedPtr<const wvWare::ParagraphProperties> paragraphProp);

	/*
	* from wvWare::TextHandler
	* Denote the end of a paragraph
	*/
	virtual void paragraphEnd();

	/*
	* from wvWare::TextHandler
	* Indicates the start of a field. Field consist of two parts separated by a separator.
	* @param fld           Describe the type of the field for the live field
	* @param chp           The character properties of the field start.
	*/
	virtual void fieldStart(const wvWare::FLD* fld, wvWare::SharedPtr<const wvWare::Word97::CHP> /*chp*/);

	/*
	* from wvWare::TextHandler
 	* This callback separates the two part of a field.
	* @param fld           Describe the type of the field for the live field
	* @param chp           The character properties of the field start.
	*/
	virtual void fieldSeparator(const wvWare::FLD* /*fld*/,wvWare::SharedPtr<const wvWare::Word97::CHP> /*chp*/);

	/*
	* from wvWare::TextHandler
	* Indicates the end of the fields
	* @param fld           Describe the type of the field for the live field
	* @param chp           The character properties of the field start.
	*/
	virtual void fieldEnd(const wvWare::FLD* /*fld*/,wvWare::SharedPtr<const wvWare::Word97::CHP> chp);

	/*
	* from wvWare::TextHandler
	* runOfText is the main worker of the API. Each runOfText contains a chunk of text of the original document.
	* @param text          The text of the run.
	* @param chp           The character properties of the field start.
	*/
	virtual void runOfText(const wvWare::UString& text, wvWare::SharedPtr<const wvWare::Word97::CHP> chp);

public:

	/**
	* Create the parameters.
	* @param params          Parameters to configure.
	*/
	static void CreateParams(GParams* params);

	/**
	* Destructor.
	*/
	virtual ~GFilterMSDoc(void);
};


}  //-------- End of namespace GALILEI ----------------------------------------


//-----------------------------------------------------------------------------
#endif
