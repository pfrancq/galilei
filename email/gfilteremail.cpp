/*

	GALILEI Research Project

	GFilterEMail.cpp

	Filter for Mails - Implementation.

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
// include files for GALILEI Plugins
#include <gfilteremail.h>

//-----------------------------------------------------------------------------
// include files for GALILEI
#include <docs/gdocxml.h>
#include <docs/gfiltermanager.h>
using namespace GALILEI;
using namespace R;



//-----------------------------------------------------------------------------
//
// class GFilterEMail
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GFilterEMail::GFilterEMail(GFactoryFilter* fac)
	: GFilter(fac), Buffer(0),
	  BlankLines(false)
{
	AddMIME("text/email");
}


//-----------------------------------------------------------------------------
void GFilterEMail::ApplyConfig(void)
{
	BlankLines=Factory->GetBool("BlankLines");
}


//-----------------------------------------------------------------------------
bool GFilterEMail::ExtractCmd(char* line,RXMLTag* /*metaData*/)
{
	char* ptr=line;

	// Look an the first word
	while((*ptr)&&((isalnum(*ptr))||((*ptr)=='-')))
		ptr++;

	// Skip Spaces.
	while((*ptr)&&(((*ptr)==' ')||((*ptr)=='\t')))
		ptr++;

	// If the next character is not a ':', this is not a cmd.
	if((!(*ptr))||((*(ptr))!=':'))
		return(false);

	// Mark end of command Skip :
	(*(ptr++))=0;

	// After skipped the spaces, and separate the command pointed by line and
	// the information pointed by ptr.
	while((*ptr)&&(((*ptr)==' ')||((*ptr)=='\t')))
		ptr++;

	// Analyse the different metaData possible.
	if((!strcasecmp(line,"subject"))&&(*ptr))
	{
		AnalyzeBlock(ptr,Doc->AddTitle());
	}
	else if((!strcasecmp(line,"summary"))&&(*ptr))
	{
		AnalyzeBlock(ptr,Doc->AddSubject());
	}
	else if((!strcasecmp(line,"from"))&&(*ptr))
	{
		AnalyzeBlock(ptr,Doc->AddCreator());
	}
	else if((!strcasecmp(line,"organization"))&&(*ptr))
	{
		AnalyzeBlock(ptr,Doc->AddPublisher());
	}
	else if((!strcasecmp(line,"date"))&&(*ptr))
	{
		Doc->AddDate(ptr);
	}
	else if((!strcasecmp(line,"keywords"))&&(*ptr))
	{
		AnalyzeKeywords(ptr,',',Doc->AddSubject());
	}

	return(true);
}


//-----------------------------------------------------------------------------
bool GFilterEMail::Analyze(GDocXML* doc) throw(bad_alloc,GException)
{
	RXMLTag* part;
	RXMLTag* tag;
	bool Header;
	bool Paragraph;
	char tmp;
	char* findstr;
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

	// Create the metaData tag and the first information
	part=Doc->GetMetaData();
	Doc->AddIdentifier(Doc->GetURL());
	Doc->AddFormat("text/email");

	// Email have and the beginning information on each line.
	Header=true;        // There headers to read.
	while((*Pos)&&Header)
	{
		// Read a line and Analyse it for a command.
		Begin=Pos;
		while((*Pos)&&((*Pos)!='\n')&&((*Pos)!='\r'))
			Pos++;
		tmp=(*Pos);
		(*(Pos++))=0;      // Skip the end of line.
		Header=ExtractCmd(Begin,part);

		// If Multiple blank lines are allowed and last line is a command, skip them
		if(Header&&BlankLines)
		{
			while((*Pos)&&(((*Pos)=='\n')||((*Pos)=='\r')))
				Pos++;
		}
	}
	(*(Pos-1))=tmp;

	// Look if the email is signed.
	findstr=strstr(Begin,("-----BEGIN PGP SIGNED MESSAGE-----"));
	if(findstr)
	{
		Begin=findstr+strlen("-----BEGIN PGP SIGNED MESSAGE-----")+1;   // Skip this;
		// Search end of mail
		findstr=strstr(Begin,"-----BEGIN PGP SIGNATURE-----");
		if(findstr)
			(*findstr)=0;
	}   
	else
		Begin[strlen(Begin)-1]='\n';

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


//-----------------------------------------------------------------------------
bool GFilterEMail::IsBlankLines(void)
{
	return(BlankLines);
}


//-----------------------------------------------------------------------------
void GFilterEMail::SetBlankLines(bool b)
{
	BlankLines=b;
}


//------------------------------------------------------------------------------
void GFilterEMail::CreateParams(GParams* params)
{
	params->InsertPtr(new GParamBool("BlankLines",false));
}


//-----------------------------------------------------------------------------
GFilterEMail::~GFilterEMail(void)
{
}


//------------------------------------------------------------------------------
CREATE_FILTER_FACTORY("EMail Filter",GFilterEMail)
