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
#include <ctype.h>


//-----------------------------------------------------------------------------
// include files for R Project
#include <rio/rtextfile.h>
using namespace RIO;


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
GFilterEMail::GFilterEMail(const RString& url)
	: GFilter(url)
{
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
		Doc->AddNode(metaData,tag=new RXMLTag("title"));
		AnalyzeBlock(ptr,tag);
	}
	else if(!strcasecmp(line,"summary"))
	{
		Doc->AddNode(metaData,tag=new RXMLTag("abstract"));
		AnalyzeBlock(ptr,tag);
	}
	else if(!strcasecmp(line,"date"))
	{
		Doc->AddNode(metaData,tag=new RXMLTag("date"));
		tag->InsertAttr("value",ptr);
	}
	else if(!strcasecmp(line,"keywords"))
	{
		Doc->AddNode(metaData,tag=new RXMLTag("keywords"));
		AnalyzeKeywords(ptr,',',tag);
	}

	return(true);
}


//-----------------------------------------------------------------------------
bool GFilterEMail::Analyze(GDocXML* doc)
{
	RTextFile f(URL);
	char line1[250];
	bool print=false;
	RXMLTag* part;
	RXMLTag* tag;

	// Create the metaData tag and look for it
	Doc=doc;
	Doc->AddNode(Doc->GetTop(),part=new RXMLTag("metaData"));
	Doc->AddNode(part,tag=new RXMLTag("url"));
	tag->InsertAttr("url",URL);
	Doc->AddNode(part,tag=new RXMLTag("typeDoc"));
	tag->InsertAttr("typeDoc","email/text");
	while(!f.Eof())
	{
		strcpy(line1,f.GetLine());
		if(!ExtractCmd(line1,part)) break;
		print=true;
	}

	cout<<endl<<"Rest of text"<<endl<<endl;

	// Look for the content
	Doc->AddNode(Doc->GetTop(),part=new RXMLTag("content"));
	while(!f.Eof())
	{
		if(!strcasecmp("-----BEGIN PGP SIGNATURE-----",line1))
		{
			print=false;
			break;
		}
		if(!strcasecmp("-----BEGIN PGP SIGNED MESSAGE-----",line1))
			print=false;
		if(print)
			cout<<line1<<endl;
		strcpy(line1,f.GetLine());
		print=true;
	}
	if(print)
		cout<<line1<<endl;

	// Look for the links
	Doc->AddNode(Doc->GetTop(),part=new RXMLTag("links"));

	// Return OK
	return(true);
}


//-----------------------------------------------------------------------------
GFilterEMail::~GFilterEMail(void)
{
}
