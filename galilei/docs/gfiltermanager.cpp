/*

	R Project Library

	GFilterManager.cpp

	Generic filter manager handling URL - Implementation.

	Copyright 2001-2003 by the Universit�Libre de Bruxelles.

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



//------------------------------------------------------------------------------
// include files for ANSI C/C++
#include <ctype.h>
#include <stdexcept>
#include <fnmatch.h>


//------------------------------------------------------------------------------
// include files for R Library
#include <rstd/rxmlfile.h>


//------------------------------------------------------------------------------
// include files for GALILEI
#include <docs/gfiltermanager.h>
#include <docs/gfilter.h>
#include <docs/gdoc.h>
#include <docs/gdocxml.h>
using namespace GALILEI;
using namespace R;




//------------------------------------------------------------------------------
//
// class GFilterManager::GMIMEFilter
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
class GFilterManager::GMIMEFilter
{
public:
	RString Name;
	GFilter* Filter;

	GMIMEFilter(const char* n,GFilter* f) : Name(n), Filter(f) {}
	int Compare(const GMIMEFilter* f) const {return(Name.Compare(f->Name));}
	int Compare(const GMIMEFilter& f) const {return(Name.Compare(f.Name));}
	int Compare(const R::RString& t) const {return(Name.Compare(t));}
	int Compare(const char* t) const {return(Name.Compare(t));}
};



//------------------------------------------------------------------------------
//
// class GFilterManager::GMIMEExt
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
class GFilterManager::GMIMEExt
{
public:
	RString Name;
	RString Ext;

	GMIMEExt(const RString& n,const RString& e) : Name(n), Ext(e) {}
	int Compare(const GMIMEExt* f) const {return(Name.Compare(f->Ext));}
	int Compare(const GMIMEExt& f) const {return(Name.Compare(f.Ext));}
	int Compare(const R::RString& e) const {return(Name.Compare(e));}
	int Compare(const char* e) const {return(Name.Compare(e));}
};



//------------------------------------------------------------------------------
//
// class GFilterManager
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GFilterManager::GFilterManager(const char* path,bool dlg) throw(std::bad_alloc,GException)
	: R::RContainer<GFactoryFilter,true,true>(10,5),GPluginManager("Filter",path), MIMES(50,25),
	  Exts(50,25)
{
	RString Path(path);
	RString MIME;
	Path+="/filters";
	LoadPlugins<GFactoryFilter,GFactoryFilterInit,GFilterManager>(this,Path.Latin1(),API_FILTER_VERSION, dlg);

	// Try to open list of MIME types
	try
	{
		RXMLStruct xml;
		RXMLFile File("/etc/galilei/galilei.mimes",&xml);
		R::RCursor<RXMLTag> Cur,Cur2;

		File.Process();

		// Go trough all MIME types
		Cur=xml.GetTag("mimeTypes")->GetXMLTagsCursor();
		for(Cur.Start();!Cur.End();Cur.Next())
		{
			MIME=Cur()->GetAttrValue("code");

			// Go through all file extension
			Cur2=Cur()->GetXMLTagsCursor();
			for(Cur2.Start();!Cur2.End();Cur2.Next())
				Exts.InsertPtr(new GMIMEExt(MIME,Cur2()->GetAttrValue("ext")));
		}
	}
	catch(...)
	{
		// No MIME types found
	}
}


//------------------------------------------------------------------------------
void GFilterManager::Download(const char*,RString&) throw(GException)
{
}


//------------------------------------------------------------------------------
const char* GFilterManager::DetermineMIMEType(const char* tmpfile) throw(GException)
{
	RCursor<GMIMEExt> Cur(Exts);

	// Go through each extension
	for(Cur.Start();!Cur.End();Cur.Next())
		if(fnmatch(Cur()->Ext.Latin1(),tmpfile,0)!=FNM_NOMATCH)
			return(Cur()->Name.Latin1());
	return(0);
}


//------------------------------------------------------------------------------
void GFilterManager::Delete(RString&) throw(GException)
{
}


//------------------------------------------------------------------------------
GDocXML* GFilterManager::CreateDocXML(GDoc* doc) throw(GException)
{
	RString tmpFile(250);
	char tmp[250];
	const char* ptr;
	int i;
	GDocXML* xml=0;
	bool Dwn;
	bool Url;
	GMIMEFilter* f=0;
	RString mime;

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

		// if the download can't be done an error is then send
		Download(doc->GetURL(),tmpFile);
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

	// Verify if the MIME type is defined -> if not try to guess
	mime=doc->GetMIMEType();
	if(mime.IsEmpty())
	{
		mime=DetermineMIMEType(tmpFile);
		if(!mime.IsEmpty())
			doc->SetMIMEType(mime);
	}

	// Create a DocXML.
	xml=new GDocXML(doc->GetURL(),tmpFile);

	// If MIME type defined -> analyze it.
	if(!mime.IsEmpty())
	{
		xml->AddFormat(mime);
		f=MIMES.GetPtr<const char*>(mime);
		if(f)
			f->Filter->Analyze(xml);
	}

	// Delete it
	if(Dwn)
		Delete(tmpFile);

	// Return XML structure
	return(xml);
}


//------------------------------------------------------------------------------
void GFilterManager::AddMIME(const char* mime,GFilter* f) throw(std::bad_alloc)
{
	MIMES.InsertPtr(new GMIMEFilter(mime,f));
}


//------------------------------------------------------------------------------
void GFilterManager::DelMIMES(GFilter* f)
{
	RContainer<GMIMEFilter,false,false> Rem(5,5);

	// Find All MIMES types to deleted
	for(MIMES.Start();!MIMES.End();MIMES.Next())
	{
		if(MIMES()->Filter==f)
			Rem.InsertPtr(MIMES());
	}

	// Delete all MIMES
	for(Rem.Start();!Rem.End();Rem.Next())
		MIMES.DeletePtr(Rem());
}


//------------------------------------------------------------------------------
const char* GFilterManager::GetMIMEType(const char* mime) const
{
	GMIMEFilter* fil;

	if(!mime) return(0);
	fil=MIMES.GetPtr<const char*>(mime);
	if(!fil) return(0);
	return(fil->Name);
}


//------------------------------------------------------------------------------
R::RCursor<GFactoryFilter> GFilterManager::GetFiltersCursor(void)
{
	R::RCursor<GFactoryFilter> cur(this);
	return(cur);
}


//------------------------------------------------------------------------------
void GFilterManager::ReadConfig(RXMLTag* t)
{
	R::RCursor<GFactoryFilter> Cur;
	
	if(!t) return;
	Cur=GetFiltersCursor();
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		Cur()->ReadConfig(t);
	}
}


//------------------------------------------------------------------------------
void GFilterManager::SaveConfig(RXMLStruct* xml,RXMLTag* t)
{
	R::RCursor<GFactoryFilter> Cur;
	Cur=GetFiltersCursor();
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		Cur()->SaveConfig(xml,t);
	}
}

//------------------------------------------------------------------------------
GFilterManager::~GFilterManager(void)
{
}
