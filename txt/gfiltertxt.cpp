/*

	GALILEI Research Project

	GFilterTXT.cpp

	Filter for TXT Files - Implementation.

	Copyright 2001 by the Université Libre de Bruxelles.

	Authors:
		Vandaele Valery (vavdaele@ulb.ac.be).

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
// include files for ANSI C/C++
#include <iostream> // for cout only.
#include <stdio.h>
#include <sys/stat.h>
#ifdef _BSD_SOURCE
	#include <unistd.h>
#else
	#include <io.h>
#endif
#include <fcntl.h>


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <gfiltertxt.h>
using namespace GALILEI;
using namespace R;



//-----------------------------------------------------------------------------
//
// class GFilterTXT
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GFilterTXT::GFilterTXT(GFactoryFilter* fac)
	: GFilter(fac), Buffer(0)
{
	AddMIME(fac->GetMng(),"text/plain");
}


//-----------------------------------------------------------------------------
bool GFilterTXT::Analyze(GDocXML* doc)
{
	RXMLTag* part;
	RXMLTag* tag;
	bool Paragraph;
	int accessmode,handle;
	struct stat statbuf;

	// Init Part
	Doc=doc;
	accessmode=O_RDONLY;
	#ifndef _BSD_SOURCE
		accessmode=O_BINARY;
	#endif
	handle=open(Doc->GetFile(),accessmode);
	fstat(handle, &statbuf);
	Begin=Pos=Buffer=new char[statbuf.st_size+1];
	read(handle,Buffer,statbuf.st_size);
	Buffer[statbuf.st_size]=0;
	close(handle);
	SkipSpaces();

	// Look for the content
	part=Doc->GetContent();
	Pos=Begin; // Remember the first line which is not a command
	while(*Pos)
	{
		part->AddTag(tag=new RXMLTag("docxml:p"));
		SkipSpaces();
		Begin=Pos;
		// Paragraph are supposed to be terminated by at least one blank line
		Paragraph=true;
		while((*Pos)&&Paragraph)
		{
			// Read a Line
			while((*Pos)&&((*Pos)!='\n')&&((*Pos)!='\r'))
				Pos++;
			Pos++;      // Skip the '\n'.
			// Skip Spaces and Tab
			while((*Pos)&&(((*Pos)==' ')||((*Pos)=='\t')))
				Pos++;
			// Look if the next lines is a blank one.
			if((*Pos)=='\n')
			{
				(*(Pos++))=0;      // Mark the end of the paragraph.
				Paragraph=false;
			}
		}
		AnalyzeBlock(Begin,tag);
	}

	// Done Part
	if(Buffer)
	{
		delete[] Buffer;
		Buffer=0;
	}

	// Return OK
	return(true);
}


//------------------------------------------------------------------------------
void GFilterTXT::Configure(GFactoryFilter*)
{
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
CREATE_FILTER_FACTORY("Text",GFilterTXT,true,false)
