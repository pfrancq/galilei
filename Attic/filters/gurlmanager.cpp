/*

	R Project Library

	GURLManager.cpp

	Generic Manager to handle URL file - Implementation.

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
#include <ctype.h>


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <urlmanagers/gurlmanager.h>
#include <filters/gmimefilter.h>
#include <filters/gfilter.h>
#include <filters/gfilteremail.h>
#include <filters/gfilterhtml.h>
#include <filters/gfilterreu.h>
#include <docs/gdocxml.h>
#include <docs/gdoc.h>
#include <sessions/gsession.h>
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
	new GFilterReu(this);
}


//-----------------------------------------------------------------------------
void GALILEI::GURLManager::Download(const char* /*URL*/,RString& /*tmpFile*/) throw(GException)
{
}


//-----------------------------------------------------------------------------
void GALILEI::GURLManager::Delete(RString& /*tmpFile*/) throw(GException)
{
}


//-----------------------------------------------------------------------------
GDocXML* GALILEI::GURLManager::CreateDocXML(GDoc* doc) throw(GException)
{
	RString tmpFile(250);
	char tmp[250];
	const char* ptr;
	int i;
	GDocXML* xml=0;
	bool Dwn;
	bool Url;
	GMIMEFilter* f;

	// Look for the protocol
	ptr=doc->GetURL();
	i=0;
	while((*ptr)&&(isalnum(*ptr)))
	{
		i++;
		ptr++;
	}

	// If ':' find -> it is an URL
	if(i)
		Url=((*ptr)==':');
	else
		Url=false;

	// if URL and protocol different than 'file' -> Download it
	if(Url&&strncasecmp(doc->GetURL(),"file",i))
	{
		try
		{
			Download(doc->GetURL(),tmpFile);
		}
		catch(GException& e)
		{
			return(0);
		}
		Dwn=true;
	}
	else
	{
		// If URL skip 'file:'
		if(Url)
		{
			strcpy(tmp,doc->GetURL());
			// Move the all string (include the ending 0) from 5 characters.
			memcpy(tmp,&tmp[5],strlen(tmp)-4);
			tmpFile=tmp;
		}
		else
			tmpFile=doc->GetURL();
		Dwn=false;
	}

	// Analyse it.
	xml=new GDocXML(doc->GetURL(),tmpFile());
	f=doc->GetMIMEType();
	if(!f)
		return(0);
	f->GetFilter()->Analyze(xml);

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
GFilterCursor& GALILEI::GURLManager::GetFiltersCursor(void)
{
	GFilterCursor *cur=GFilterCursor::GetTmpCursor();
	cur->Set(Filters);
	return(*cur);
}


//-----------------------------------------------------------------------------
GALILEI::GURLManager::~GURLManager(void)
{
}
