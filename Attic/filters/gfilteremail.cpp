/*

	GALILEI Research Project

	GFilterEMail.cpp

	Filter for Mails - Implementation.

	(C) 2001 by P. Francq.

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
#include <iostream.h> // for cout only.
#include <stdio.h>
#include <sys/stat.h>
#if unix
	#include <unistd.h>
#else
	#include <io.h>
#endif
#include <fcntl.h>


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <filters/gfilteremail.h>
using namespace GALILEI;



//-----------------------------------------------------------------------------
//
// class GFilterEMail
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GFilterEMail::GFilterEMail(GURLManager* mng)
	: GFilter(mng,"EMail Filter","text/email","$Revision$"), Buffer(0)
{
	AddMIME("text/email");
}


//-----------------------------------------------------------------------------
bool GFilterEMail::ExtractCmd(char* line,RXMLTag* metaData)
{
	char* ptr=line;

	// Look an the first word
	while((*ptr)&&((isalnum(*ptr))||((*ptr)=='-')))
		ptr++;

	// If the next character is not a ':', this is not a cmd.
	while((*ptr)&&(((*ptr)==' ')||((*ptr)=='\t')))
		ptr++;
	if((!(*ptr))||((*(ptr))!=':'))
		return(false);

	// After skipped the spaces, and separate the command pointed by cmd and
	// the information pointed by ptr.
	while((*ptr)&&(((*ptr)==' ')||((*ptr)=='\t')))
		ptr++;
	(*(ptr++))=0;

	// Analyse the different metaData possible.
	if(!strcasecmp(line,"subject"))
	{
		AnalyzeBlock(ptr,Doc->AddTitle());
	}
	else if(!strcasecmp(line,"summary"))
	{
		AnalyzeBlock(ptr,Doc->AddSubject());
	}
	else if(!strcasecmp(line,"Date"))
	{
		Doc->AddDate(ptr);
	}
	else if(!strcasecmp(line,"keywords"))
	{
		AnalyzeKeywords(ptr,',',Doc->AddSubject());
	}

	return(true);
}


//-----------------------------------------------------------------------------
bool GFilterEMail::Analyze(GDocXML* doc)
{
	RXMLTag* part;
	RXMLTag* tag;
	bool Header;
	bool Paragraph;
	char* findstr;
	int accessmode,handle;
	struct stat statbuf;

	// Init Part
	Doc=doc;
	accessmode=O_RDONLY;
	#if !unix
		accessmode=O_BINARY;
	#endif
	handle=open(Doc->GetFile(),accessmode);
	fstat(handle, &statbuf);
	Begin=Pos=Buffer=new char[statbuf.st_size+1];
	read(handle,Buffer,statbuf.st_size);
	Buffer[statbuf.st_size]=0;
	close(handle);
	SkipSpaces();
	Begin=Pos;

	// Create the metaData tag and the first information
	part=Doc->GetMetaData();
	Doc->AddIdentifier(Doc->GetURL());
	Doc->AddFormat("text/email");

	// Email have and the beginning information on each line.
	Header=true;        // There headers to read.
	while((*Pos)&&Header)
	{
		// Read a line and Analyse it for a command.
		while((*Pos)&&((*Pos)!='\n')&&((*Pos)!='\r'))
			Pos++;
		(*(Pos++))=0;      // Skip the end of line.
		Header=ExtractCmd(Begin,part);

		// If Multiple blank lines and last line is a command, skip them
		if(Header)
		{
			while((*Pos)&&(((*Pos)=='\n')||((*Pos)=='\r')))
				Pos++;
			Begin=Pos;
		}
	}

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
	while(*Pos)
	{
		part->AddTag(tag=new RXMLTag("P"));
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
GFilterEMail::~GFilterEMail(void)
{
}
