/*

	GALILEI Research Project

	GFilterMSDoc.cpp

	A MSDoc filter - Implementation.

	Copyright 2001-2003 by the Universit� Libre de Bruxelles.

	Authors:
		Vandaele Valery(vavdaele@ulb.ac.be).

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


// #define OPI_SUPPORT 0
//
// #define MULTITHREADED _REENTRANT


//------------------------------------------------------------------------------
// include files for ANSI C/C++
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>

//------------------------------------------------------------------------------
// include files for wvWare Project
#include <wv2/ustring.h>
#include <wv2/word97_generated.h>

//------------------------------------------------------------------------------
// include files for GALILEI
#include <gfiltermsdoc.h>
#include <docs/gdocxml.h>
#include <filters/gurlmanager.h>
using namespace GALILEI;
using namespace R;



//------------------------------------------------------------------------------
//
// class GFilterMSDoc
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GFilterMSDoc::GFilterMSDoc(GFactoryFilter* fac)
	: GFilter(fac)
{

 	AddMIME(fac->GetMng(),"application/msword");
}


//------------------------------------------------------------------------------
RString& GFilterMSDoc::ConvertUtoRString(const wvWare::UString& ustr) throw(bad_alloc)
{
	RString *res=RString::GetString();
	(*res)=ustr.cstring().c_str();

	return (*res);
}


//------------------------------------------------------------------------------
RString& GFilterMSDoc::ConvertChar(const RString& str) throw(bad_alloc)
{
	RString *res=RString::GetString();
	const char* ptr1;

	ptr1=str.Latin1() ;
	cout<<ptr1<<endl;
	(*res)="";

	while(*ptr1)
	{
		if((*ptr1)==25)  // the code to be replaced
		{
			(*res)+='\'';
			ptr1++;
		}
		else
			(*res)+=(*(ptr1++));
	}
	return(*res);
}


//------------------------------------------------------------------------------
int GFilterMSDoc::fldToFieldType(const wvWare::FLD* fld)
{
	// assume unhandled
	int fieldType = -1;

	// sanity check
    if( !fld ) return -1;

	switch( fld->flt )
	{
	case 15:    fieldType =  1; break;  // title
	case 17:    fieldType =  2; break;  // author
	case 18:    fieldType =  3; break;  // keywords
	case 19:    fieldType = 11; break;  // comments (unhandled)
	case 21:    fieldType =  4; break;  // createdate (unhandled)
	case 22:    fieldType = -1; break;  // savedate (unhandled)
	case 23:    fieldType = -1; break;  // printdate (unhandled)
	case 25:    fieldType = -1; break;  // edittime (unhandled)
	case 29:    fieldType =  0; break;  // filename
	case 32:    fieldType = -1; break;  // time (unhandled)
	case 60:    fieldType =  2; break;  // username <-> author name
	case 61:    fieldType =  2; break;  // userinitials (unhandled)
	case 62:    fieldType = -1; break;  // useraddress (unhandled)

	case 88:    fieldType =  5; break;  //  hyperlinks
	default:    fieldType = -1; break;
	}

	return fieldType;
}


//------------------------------------------------------------------------------
void GFilterMSDoc::AddField()
{
	switch(FieldType)
	{
		case 0:    Doc->AddIdentifier(FieldValue); break; //Add filename
		case 1:    Doc->AddTitle(FieldValue); break;  // Add the title of the page.
		case 2:    Doc->AddCreator(FieldValue); break; //Add the author of the page.
		case 3:    AnalyzeKeywords(FieldValue.Latin1(),',',Doc->AddSubject()); break;  // Add keywords
		case 4:    Doc->AddDate(FieldValue); break;  // Add the Date
		case 5:    Doc->AddIdentifier(FieldValue,Doc->AddLink()) ; break;
	}
}


//------------------------------------------------------------------------------
void GFilterMSDoc::ReadMetaData()
{
	RString str="";
	wvWare::AssociatedStrings strings(Parser->associatedStrings());


	if (!strings.author().isNull())
	{
		str=ConvertChar(ConvertUtoRString(strings.author()));
		Doc->AddCreator(str.Latin1());
	}
	if(!strings.title().isNull())
	{
		str=ConvertChar(ConvertUtoRString(strings.title()));
		Doc->AddTitle(str.Latin1());
	}

}


//------------------------------------------------------------------------------
void GFilterMSDoc::subDocFound(const wvWare::FunctorBase* functor, int data)
{
	SubDocument subDoc(functor, data);
	SubDocQueue.push(subDoc);
}


//------------------------------------------------------------------------------
void GFilterMSDoc::WriteParagraph(RString par)
{
	RXMLTag* part;
	RXMLTag* tag;

	if ( !par.IsEmpty() )
	{
		cout << "write Paragraph"<<endl;
		part=Doc->GetContent();
		part->AddTag(tag=new RXMLTag("docxml:p"));
		AnalyzeBlock(par.Latin1(),tag);
	}

	Paragraph="";
}


//------------------------------------------------------------------------------
bool GFilterMSDoc::Analyze(GDocXML* doc) throw(GException)
{
	//RXMLTag* tag;
	RString *fileName;

	// Init Part*/
	Doc=doc;
	bodyFound=false;
	endNoteNumber=0;
	footNoteNumber=0;
	sectionNumber=0;
	bInParagraph=false;
	InsideField =false;
	FieldAfterSeparator=false;
	FieldType=0;

	// Create the metaData tag and the first information
	Doc->AddIdentifier(Doc->GetURL());
	Doc->AddFormat("text/msword");


	// get fileName
	fileName = new RString(Doc->GetFile());

	// Init Parser
	Parser = wvWare::ParserFactory::createParser(Doc->GetFile());
	TableHandler = new  wvWare::TableHandler();

	if(Parser)  // I in case of major ERROR -> Unsupported format
	{
		Parser->setSubDocumentHandler(this);
		Parser->setTextHandler(this);
		Parser->setTableHandler(TableHandler);

		ReadMetaData();
	}

	else return false;

	if (Parser == 0L)
	{
		throw(GException("The file in use has uncompatible format"));
		return false;
	}
	if (!Parser->parse())
	{
		throw(GException("An error occurs during filer parsing"));
		return false;
	}

	if (!bodyFound)
	{
		throw(GException("The document body was not found : wrong format"));
	}

	// process SubDocQueue
	while(! SubDocQueue.empty())
	{
		SubDocument subdoc(SubDocQueue.front());
		(*subdoc.functorPtr)(); //call it
		delete subdoc.functorPtr; //delete it
		SubDocQueue.pop();
	}

	// process tables ??

	return true;
}


//------------------------------------------------------------------------------

//methods for SubDocument Handler.

//------------------------------------------------------------------------------
void GFilterMSDoc::bodyStart(void)
{
	bodyFound=true;
}


//------------------------------------------------------------------------------
void GFilterMSDoc::footnoteStart(void)
{
	cout<< "coucou , passe dans foot Start"<<endl;

	//Grab data that was stored whith the functor, that triggered this parsing
	SubDocument subDoc( SubDocQueue.front());
	int type = subDoc.data;

	if (type == wvWare::FootnoteData::Endnote )
		++endNoteNumber;
	else
		++footNoteNumber;
}

//------------------------------------------------------------------------------
/*void GFilterMSDoc::footnoteEnd(void)
{
	cout<< "coucou , passe dans foot End"<<endl;
}*/


//------------------------------------------------------------------------------
//Methods for TextHandler
//------------------------------------------------------------------------------
/*void GFilterMSDoc::sectionStart(wvWare::SharedPtr<const wvWare::Word97::SEP> sep)
{
	sectionNumber++;
	if (sectionNumber == 1)
	{
		//FirsSectionFound(sep);
		cout <<" implementer first section found"<<endl;
	}
	else
	{	//Not the first section. Check for a page break
		// 1= New Column, 2=new Page, 3=even Page, 4=odd Page
		if (sep->bkc >=1)
		{
			pageBreak();
		}
	}
}
*/


//------------------------------------------------------------------------------
void GFilterMSDoc::headersFound( const wvWare::HeaderFunctor& parseHeaders)
{
	cout << "headers found "<<endl;
	if (sectionNumber ==1)
	{
		subDocFound( new wvWare::HeaderFunctor(parseHeaders),0);
	}
}


//------------------------------------------------------------------------------
void GFilterMSDoc::paragraphStart(wvWare::SharedPtr<const wvWare::ParagraphProperties> /*paragraphProp*/)
{
	if( bInParagraph)
	{
		paragraphEnd();
	}
	bInParagraph =true;
	//ParagraphProper = paragraphProp;

	//Paragraph="";
	cout << "--- paragraph start --- possibilite de gestion des propriete des paragraph ainsi que des style"<<endl;
}


//------------------------------------------------------------------------------
void GFilterMSDoc::paragraphEnd()
{
	cout << "--- paragraph end --- gerer les tables.."<<endl;

	WriteParagraph(Paragraph);

	bInParagraph = false;
}


//------------------------------------------------------------------------------
void GFilterMSDoc::fieldStart(const wvWare::FLD* fld, wvWare::SharedPtr<const wvWare::Word97::CHP> /*chp*/)
{
	cout << "passe dans field Start :"<<fld<<endl;
	FieldType = fldToFieldType(fld);
	InsideField=true;
	FieldAfterSeparator = false;
	FieldValue="";
}


//------------------------------------------------------------------------------
void GFilterMSDoc::fieldSeparator(const wvWare::FLD* /*fld*/,wvWare::SharedPtr<const wvWare::Word97::CHP> /*chp*/)
{
	FieldAfterSeparator = true;
}


//------------------------------------------------------------------------------
void GFilterMSDoc::fieldEnd(const wvWare::FLD* /*fld*/,wvWare::SharedPtr<const wvWare::Word97::CHP> /*chp*/)
{
	if (FieldType >=0)
	{
		AddField();
	}

	FieldValue ="";
	FieldType=-1;
	InsideField=false;
	FieldAfterSeparator = false;
}


//------------------------------------------------------------------------------
void GFilterMSDoc::runOfText(const wvWare::UString& text, wvWare::SharedPtr<const wvWare::Word97::CHP> /*chp*/)
{
	RString newTxt = ConvertChar( ConvertUtoRString(text)) ;
	cout << "TEXT :: "<< newTxt.Latin1()<< endl;

	//text after fieldtart and before fieldSeparator is useless
	if (InsideField && !FieldAfterSeparator) return;

	//if we can handle the field, consume the text
	if (InsideField && FieldAfterSeparator && (FieldType >=0))
	{
		FieldValue+= newTxt;
		return;
	}

	Paragraph += newTxt;
}


//------------------------------------------------------------------------------
void GFilterMSDoc::CreateParams(GParams*)
{
}


//------------------------------------------------------------------------------
GFilterMSDoc::~GFilterMSDoc()
{
}


//------------------------------------------------------------------------------
CREATE_FILTER_FACTORY("MSDoc Filter",GFilterMSDoc)
