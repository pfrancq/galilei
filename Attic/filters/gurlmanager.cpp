/*

	R Project Library

	GURLManager.cpp

	Generic Manager to handle URL file - Implementation.

	(C) 2001 by Pascal Francq

	Version $Revision$

	Last Modify: $Date$

*/



//-----------------------------------------------------------------------------
// include files for ANSI C/C++
#include <ctype.h>


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <urlmanagers/gurlmanager.h>
#include <filters/gmimefilter.h>
#include <filters/gfilter.h>
#include <filters/gfilteremail.h>
#include <filters/gfilterhtml.h>
#include <gsessions/gsession.h>
using namespace GALILEI;
using namespace RStd;



//-----------------------------------------------------------------------------
//
// class GURLManager
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GALILEI::GURLManager::GURLManager(void)
	: MIMES(50,25), Filters(50,25)
{
	new GFilterHTML(this);
	new GFilterEMail(this);
}


//-----------------------------------------------------------------------------
void GALILEI::GURLManager::Download(const char* URL,RString& tmpFile) throw(GException)
{
}


//-----------------------------------------------------------------------------
void GALILEI::GURLManager::Delete(RString& tmpFile) throw(GException)
{
}


//-----------------------------------------------------------------------------
GDocXML* GALILEI::GURLManager::CreateDocXML(GDoc* doc) throw(GException)
{
	RString tmpFile(50);
	const char* ptr;
	int i;
	GDocXML* xml=0;
	bool Dwn;

	// Verify it is a local file (file:/) or nothing.
	ptr=doc->GetURL();
	i=0;
	while((*ptr)&&(isalnum(*ptr)))
	{
		i++;
		ptr++;
	}
	if(i&&((*ptr)==':')&&(strncmp(ptr,"file",i)))
	{
		// Suppose the download failed
		doc->IncFailed();
		Download(doc->GetURL(),tmpFile);
		Dwn=true;
		// Download OK
		doc->DecFailed();
	}
	else
	{
		tmpFile=doc->GetURL();
		Dwn=false;
	}

	// Analyse it.
	xml=new GDocXML(doc->GetURL(),tmpFile());
	doc->GetMIMEType()->GetFilter()->Analyze(xml);

	// Delete it
	if(Dwn)
		Delete(tmpFile);

	// Return XML structure
	return(xml);
}


//-----------------------------------------------------------------------------
void GALILEI::GURLManager::AddMIME(const char* mime,GFilter* f)
{
	if(!Filters.IsIn(f))
		Filters.InsertPtr(f);
	MIMES.InsertPtr(new GMIMEFilter(mime,f));
}


//-----------------------------------------------------------------------------
GMIMEFilter* GALILEI::GURLManager::GetMIMEType(const char* mime) const
{
	if(!mime) return(0);
	return(MIMES.GetPtr<const char*>(mime));
}


//-----------------------------------------------------------------------------
GALILEI::GURLManager::~GURLManager(void)
{
}
