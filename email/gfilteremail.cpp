/*

	GALILEI Research Project

	GFilterEMail.cpp

	Filter for Mails - Implementation.

	Copyright 2001 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).

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
// include files for GALILEI Plugins
#include <gfilteremail.h>


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <docs/gdocxml.h>
#include <docs/gfiltermanager.h>
using namespace GALILEI;
using namespace R;
using namespace std;



//-----------------------------------------------------------------------------
//
// class GFilterEMail
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GFilterEMail::GFilterEMail(GFactoryFilter* fac)
	: GFilter(fac), BlankLines(false)
{
	AddMIME("text/email");
}


//-----------------------------------------------------------------------------
void GFilterEMail::ApplyConfig(void)
{
	BlankLines=Factory->GetBool("BlankLines");
}


//-----------------------------------------------------------------------------
bool GFilterEMail::ExtractCmd(const RString& line)
{
	RString Cmd(line.GetLen());
	const RChar* ptr=line();
	unsigned int len=0;
	unsigned int pos;

	// Look an the first word
	while((!ptr->IsNull())&&((ptr->IsAlNum())||((*ptr)==RChar('-'))))
	{
		ptr++;
		len++;
	}
	
	// Skip Spaces.
	while((!ptr->IsNull())&&(((*ptr)==' ')||((*ptr)==RChar('\t'))))
		ptr++;

	// If the next character is not a ':', this is not a cmd.
	if((!(!ptr->IsNull()))||((*(ptr))!=RChar(':')))
		return(false);

	// Mark end of command and put it in lowercase and Skip ':'
	Cmd=line.Mid(0,len).ToLower();

	// After skipped the spaces, and separate the command pointed by line and
	// the information pointed by ptr.
	pos=len;
	len=0;
	ptr++;
	while((!ptr->IsNull())&&(((*ptr)==' ')||((*ptr)==RChar('\t'))))
	{
		ptr++;
		len++;
	}

	// Analyse the different metaData possible.
	RString metaData=line.Mid(pos,len);
	if((Cmd=="subject")&&(!metaData.IsEmpty()))
	{
		AnalyzeBlock(metaData,Doc->AddTitle());
	}
	else if((Cmd=="summary")&&(!metaData.IsEmpty()))
	{
		AnalyzeBlock(metaData,Doc->AddSubject());
	}
	else if((Cmd=="from")&&(!metaData.IsEmpty()))
	{
		AnalyzeBlock(metaData,Doc->AddCreator());
	}
	else if((Cmd=="organization")&&(!metaData.IsEmpty()))
	{
		AnalyzeBlock(metaData,Doc->AddPublisher());
	}
	else if((Cmd=="date")&&(!metaData.IsEmpty()))
	{
		Doc->AddDate(metaData);
	}
	else if((Cmd=="keywords")&&(!metaData.IsEmpty()))
	{
		AnalyzeKeywords(metaData,',',Doc->AddSubject());
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
	bool Stop;
	bool NextStop;
//	char tmp;
	bool Read;
//	char* findstr;
	RString NextLine;
	RString Line;
	const RChar* ptr;

	try
	{
		// Init Part
		Doc=doc;
		RTextFile Src(Doc->GetFile());
		Stop=Src.Eof();

		// Create the metaData tag and the first information
		part=Doc->GetMetaData();
		Doc->AddIdentifier(Doc->GetURL());
	
		// Email have at the beginning information on each line.
		Header=true;        // There headers to read.
		Read=true;
		while((!Src.Eof())&&Header)
		{
			// Read a line
			if(Read)
				Line=Src.GetLine(false);

			// Add the next lines beginning with a space
			if(!Src.Eof())
			{
				NextLine=Src.GetLine(false);
				while((NextLine()->IsSpace())&&(!Src.Eof()))
				{
					Line+=NextLine;
					NextLine=Src.GetLine(false);
				}
			}

			// Analyse the line for a command.
			Header=ExtractCmd(Line);
	
			// If Multiple blank lines are allowed and last line is a command, skip them
			if(Header&&BlankLines)
			{
				while((NextLine.IsEmpty())&&(!Src.Eof()))
					NextLine=Src.GetLine(false);
			}
			
			// Next line becomes the current one
			Line=NextLine;
			Read=false;
			Stop=NextStop;
		}
	
		// Look if the email is signed.
/*		findstr=strstr(Begin,("-----BEGIN PGP SIGNED MESSAGE-----"));
		if(findstr)
		{
			Begin=findstr+strlen("-----BEGIN PGP SIGNED MESSAGE-----")+1;   // Skip this;
			// Search end of mail
			findstr=strstr(Begin,"-----BEGIN PGP SIGNATURE-----");
			if(findstr)
				(*findstr)=0;
		}   
		else
			Begin[strlen(Begin)-1]='\n';*/
	
		// Look for the content
		part=Doc->GetContent();

		while((!Read)||(!Src.Eof()))
		{
			Doc->AddTag(part,tag=new RXMLTag("docxml:p"));

			// If necessary -> read a new line f
			if(!Read)
				Read=true;
			else
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
					Line+=NextLine;
			}
			AnalyzeBlock(Line,tag);
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
