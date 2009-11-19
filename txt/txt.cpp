/*

	GALILEI Research Project

	TXT.cpp

	Filter for text Files - Implementation.

	Copyright 2001 by Valery Vandaele.
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
// include files for GALILEI
#include <txt.h>
#include <rxmlfile.h>


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <gdocxml.h>
using namespace GALILEI;
using namespace R;
using namespace std;


//-----------------------------------------------------------------------------
//
// class GFilterTXT
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GFilterTXT::GFilterTXT(GPlugInFactory* fac)
	: GFilter(fac)
{
	AddMIME("text/plain");
}


//-----------------------------------------------------------------------------
void GFilterTXT::Analyze(const RURI&,const RURI& file,const RURI& docxml)
{
	RXMLTag* part;
	RXMLTag* tag;
	bool Paragraph;
	bool Read;

	RString NextLine;
	RString Line;
	const RChar* ptr;

	// Init Part
	Doc=new GDocXML(docxml,file);
	RTextFile Src(Doc->GetFile());
	Src.Open(R::RIO::Read);

	// Create the metaData tag and the first information
	part=Doc->GetContent();
	Doc->AddIdentifier(Doc->GetURL()());

	Read=true;

	while(!Src.End())
	{
		Doc->AddTag(part,tag=new RXMLTag("docxml:p"));

		if(Read)
		{
			if(!Src.End())
				Line=NextLine;
		}

		// Paragraph are supposed to be terminated by at least one blank line
		Paragraph=true;
		while((!Src.End())&&Paragraph)
		{
			// Read a Line
			NextLine=Src.GetLine(false);

			// Look if it is a blank line
			ptr=NextLine();
			while((!ptr->IsNull())&&(ptr->IsSpace()))
				ptr++;

			// If blank line -> it is the end of a paragraph
			if(ptr->IsNull())
			{
				Paragraph=false;
			}
			else
			{
				if(!Line.IsEmpty())
					Line+=' ';
				Line+=NextLine;
			}
		}
		AnalyzeBlock(Line,tag);
		if(tag->IsEmpty())
			Doc->DeleteTag(tag);
	}

	// Save the structure and delete everything
	RXMLFile Out(docxml,Doc);
	Out.Open(RIO::Create);
	delete Doc;
}


//------------------------------------------------------------------------------
void GFilterTXT::CreateParams(RConfig*)
{
}


//-----------------------------------------------------------------------------
GFilterTXT::~GFilterTXT(void)
{
}


//------------------------------------------------------------------------------
CREATE_FILTER_FACTORY("Text Filter","Text Filter",GFilterTXT)
