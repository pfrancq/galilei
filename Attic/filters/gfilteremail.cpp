/*

	GALILEI Research Project

	GDocXML.h

	A XML representation of a document - Header.

	(C) 2001 by P. Francq.

	Version $Revision$

	Last Modify: $Date$

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
GFilterEMail::GFilterEMail(const RString& url,GURLManager* mng)
	: GFilter(url,mng), Buffer(0)
{
	int accessmode,handle;
	struct stat statbuf;

	AddExt("text/email");
	accessmode=O_RDONLY;
	#if !unix
		accessmode=O_BINARY;
	#endif
	handle=open(url,accessmode);
	fstat(handle, &statbuf);
	Begin=Pos=Buffer=new char[statbuf.st_size+1];
	read(handle,Buffer,statbuf.st_size);
	Buffer[statbuf.st_size]=0;
	SkipSpaces();
	Begin=Pos;
}


//-----------------------------------------------------------------------------
bool GFilterEMail::ExtractCmd(char* line,RXMLTag* metaData)
{
	char* ptr=line;
	RXMLTag* tag;

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
		Doc->AddNode(metaData,tag=new RXMLTag("Title"));
		AnalyzeBlock(ptr,tag);
	}
	else if(!strcasecmp(line,"summary"))
	{
		Doc->AddNode(metaData,tag=new RXMLTag("Abstract"));
		AnalyzeBlock(ptr,tag);
	}
	else if(!strcasecmp(line,"Date"))
	{
		Doc->AddNode(metaData,tag=new RXMLTag("Date"));
		tag->InsertAttr("Value",ptr);
	}
	else if(!strcasecmp(line,"keywords"))
	{
		Doc->AddNode(metaData,tag=new RXMLTag("Keywords"));
		AnalyzeKeywords(ptr,',',tag);
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

	// Create the metaData tag and the first information
	Doc=doc;
	Doc->AddNode(Doc->GetTop(),part=new RXMLTag("MetaData"));
	Doc->AddNode(part,tag=new RXMLTag("URL"));
	tag->InsertAttr("value",URL);
	Doc->AddNode(part,tag=new RXMLTag("TypeDoc"));
	tag->InsertAttr("TypeDoc","email/text");

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
	Doc->AddNode(Doc->GetTop(),part=new RXMLTag("Content"));
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

	// Look for the links
	Doc->AddNode(Doc->GetTop(),part=new RXMLTag("links"));

	// Return OK
	return(true);
}


//-----------------------------------------------------------------------------
GFilterEMail::~GFilterEMail(void)
{
	if(Buffer) delete[] Buffer;
}
