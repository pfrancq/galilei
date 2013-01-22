/*

	GALILEI Research Project

	PDF.cpp

	A PDF filter - Implementation.

	Copyright 2001-2012 by Pascal Francq (pascal@francq.info).
	Copyright 2001-2008 by the Université Libre de Bruxelles (ULB).
	Copyright 1996-2005 by Glyph & Cog, LLC.

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
#include <aconf.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include "GString.h"
#include "gmem.h"
#include "GlobalParams.h"
#include "Object.h"
#include "Stream.h"
#include "Array.h"
#include "Dict.h"
#include "XRef.h"
#include "Catalog.h"
#include "Page.h"
#include "PDFDoc.h"
#include "TextBufOutputDev.h"
#include "CharTypes.h"
#include "UnicodeMap.h"
#include "Error.h"
#include "config.h"



//------------------------------------------------------------------------------
// include files for R/GALILEI
#include <pdf.h>
#include <gdoc.h>
#include <rxmlfile.h>
#include <rdownload.h>
#include <gdocanalyze.h>



//------------------------------------------------------------------------------
//
// class GFilterPDF
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GFilterPDF::GFilterPDF(GSession* session,GPlugInFactory* fac)
	: GFilter(session,fac)
{
	AddMIME("text/pdf");
	AddMIME("text/x-pdf");
	AddMIME("application/pdf");
}


//------------------------------------------------------------------------------
void GFilterPDF::Analyze(GDocAnalyze* analyzer,const GDoc*,const R::RURI& file)
{
	PDFDoc *pdf;
	GString *fileName(new GString(file()));
	TextBufOutputDev *textOut;
	Object info;
	bool Paragraph;

	// read config file
	globalParams = new GlobalParams("");
	globalParams->setTextPageBreaks(gFalse);

	// open PDF file
	pdf = new PDFDoc(fileName,0,0);
	if((!pdf->isOk())||(!pdf->okToCopy()))
	{
		delete globalParams;
		delete pdf;
		ThrowGException("Not valid PDF file");
	}
	TextBufOutputDev::Encoding=RTextEncoding::GetTextEncoding(globalParams->getTextEncodingName()->getCString());

	// Write Meta Data
	pdf->getDocInfo(&info);
	if(info.isDict())
	{
		RString str;
		str=CreateString(info.getDict(),"Title");
		if(!str.IsEmpty())
			analyzer->ExtractDCMI("title",str,0);
		str=CreateString(info.getDict(),"Subject");
		if(!str.IsEmpty())
			analyzer->ExtractDCMI("subject",str,0);
		str=CreateString(info.getDict(),"Keywords");
		if(!str.IsEmpty())
			analyzer->ExtractDCMI("subject",str,0);
		str=CreateString(info.getDict(),"Author");
		if(!str.IsEmpty())
			analyzer->ExtractDCMI("creator",str,0);
		str=CreateString(info.getDict(),"Creator");
		if(!str.IsEmpty())
			analyzer->ExtractDCMI("creator",str,0);
		str=CreateString(info.getDict(),"Producer");
		if(!str.IsEmpty())
			analyzer->ExtractDCMI("publisher",str,0);
/*		str=CreateDate(info.getDict(),"CreationDate");
		if(!str.IsEmpty())
			Doc->AddDate(str);
		str=CreateDate(info.getDict(),"LastModifiedDate");
		if(!str.IsEmpty())
			Doc->AddDate(str);*/
	}
	info.free();

	// write text file
	textOut = new TextBufOutputDev(false, false, false);
	if(textOut->isOk())
	{
    	pdf->displayPages(textOut, 1, pdf->getNumPages(), 72, 72, 0, gFalse, gTrue, gFalse);
  	}

	// Look for the content
	Begin=Pos=TextBufOutputDev::RCharBuffer; // Remember the first line which is not a command
	SkipSpaces();
	Pos=Begin;
	while(Pos&&(!Pos->IsNull()))
	{
		SkipSpaces();
		Begin=Pos;
		// Paragraph are supposed to be terminated by at least one blank line
		Paragraph=true;
		while((!Pos->IsNull())&&Paragraph)
		{
			// Read a Line
			while((!Pos->IsNull())&&(Pos->Latin1()!='\n')&&(Pos->Latin1()!='\r'))
				Pos++;
			Pos++;      // Skip the '\n'.

			// Skip Spaces and Tab
			while((!Pos->IsNull())&&((Pos->Latin1()==' ')||(Pos->Latin1()=='\t')))
				Pos++;

			// Look if the next lines is a blank one.
			if(Pos->Latin1()=='\n')
			{
				(*(Pos++))=0;      // Mark the end of the paragraph.
				Paragraph=false;
			}
		}
		analyzer->ExtractDefaultText(Begin,0);
	}

	// Clean up
	delete textOut;
	delete pdf;
	delete globalParams;
}


//------------------------------------------------------------------------------
R::RString GFilterPDF::GetTextFragment(GDocFragment*)
{
	return("Cannot extract text fragment");
}


//------------------------------------------------------------------------------
RString GFilterPDF::CreateString(Dict* infoDict,const char* key/*,UnicodeMap *uMap*/)
{
	RString res;
	Object obj;
	GString *s1;
	GBool isUnicode;
	Unicode u;
	int i;

	if(infoDict->lookup(const_cast<char*>(key),&obj)->isString())
	{
		s1 = obj.getString();
		if((s1->getChar(0) & 0xff) == 0xfe && (s1->getChar(1) & 0xff) == 0xff)
		{
			isUnicode = gTrue;
			i = 2;
		}
		else
		{
			isUnicode = gFalse;
			i = 0;
		}
		while(i<obj.getString()->getLength())
		{
			if(isUnicode)
			{
				u =((s1->getChar(i) & 0xff) << 8)|(s1->getChar(i+1) & 0xff);
				i += 2;
			}
			else
			{
				u = s1->getChar(i) & 0xff;
				++i;
			}
			res+=u;
	}
	}
	obj.free();
	return(res);
}


//------------------------------------------------------------------------------
RString GFilterPDF::CreateDate(Dict* infoDict,const char* key)
{
	RString res;
	Object obj;
	char *s;

	if(infoDict->lookup(const_cast<char*>(key),&obj)->isString())
	{
		s = obj.getString()->getCString();
		if (s[0] == 'D' && s[1] == ':')
		{
			s += 2;
		}
		res=s;
	}
	obj.free();
	return(res);
}


//------------------------------------------------------------------------------
GFilterPDF::~GFilterPDF()
{
}


//------------------------------------------------------------------------------
CREATE_FILTER_FACTORY("PDF Filter","PDF Filter",GFilterPDF)
