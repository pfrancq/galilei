/*

	GALILEI Research Project

	GFilterTXT.cpp

	Filter for TXT Files - Implementation.

	Copyright 2001 by the Universit�Libre de Bruxelles.

	Authors:
		Vandaele Valery (vavdaele@ulb.ac.be).

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
#include <gfiltertxt.h>

//-----------------------------------------------------------------------------
// include files for GALILEI
#include <docs/gdocxml.h>
#include <docs/gfiltermanager.h>
using namespace GALILEI;
using namespace R;
using namespace std;


//-----------------------------------------------------------------------------
//
// class GFilterTXT
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GFilterTXT::GFilterTXT(GFactoryFilter* fac)
	: GFilter(fac)
{
	AddMIME("text/plain");
}


//-----------------------------------------------------------------------------
bool GFilterTXT::Analyze(GDocXML* doc) throw(bad_alloc,GException)
{
	RXMLTag* part;
	RXMLTag* tag;
	bool Paragraph;
	bool Read;

	RString NextLine;
	RString Line;
	const RChar* ptr;

	try
	{
		// Init Part
		Doc=doc;
		RTextFile Src(Doc->GetFile());
		Src.Open(R::Read);

		// Create the metaData tag and the first information
		part=Doc->GetContent();
		Doc->AddIdentifier(Doc->GetURL());

		Read=true;

		while(!Src.Eof())
		{
			Doc->AddTag(part,tag=new RXMLTag("docxml:p"));

			if(Read)
			{
				if(!Src.Eof())
					Line=NextLine;
			}

			// Paragraph are supposed to be terminated by at least one blank line
			Paragraph=true;
			while((!Src.Eof())&&Paragraph)
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
	}
	catch(bad_alloc)
	{
		throw;
	}
	catch(GException)
	{
		throw;
	}
	catch(RException& e)
	{
		throw GException(e.GetMsg());
	}
	catch(...)
	{
		throw GException("Unexcepted exception");
	}

	// Return OK
	return(true);
}


//------------------------------------------------------------------------------
void GFilterTXT::CreateParams(GParams*)
{
}


//-----------------------------------------------------------------------------
GFilterTXT::~GFilterTXT(void)
{
}


//------------------------------------------------------------------------------
CREATE_FILTER_FACTORY("Text Filter",GFilterTXT)
