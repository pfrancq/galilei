/*

	R Project Library

	GFilterManager.cpp

	Generic filter manager handling URL - Implementation.

	Copyright 2001-2003 by the Université Libre de Bruxelles.

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



//------------------------------------------------------------------------------
// include files for ANSI C/C++
#include <ctype.h>
#include <stdexcept>
#include <dirent.h>


//------------------------------------------------------------------------------
// include files for GALILEI
#include <docs/gfiltermanager.h>
#include <docs/gfilter.h>
#include <docs/gdoc.h>
#include <docs/gdocxml.h>
using namespace GALILEI;
using namespace R;
using namespace ltmm;



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
// class GFilterManager
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GFilterManager::GFilterManager(const char* path,bool dlg) throw(bad_alloc,GException)
	: R::RContainer<GFactoryFilter,unsigned int,true,true>(10,5), MIMES(50,25)
{
	DIR* dp;
	struct dirent* ep;
	RString Path(path);
	RString Msg;
	RString Name;
	char DlgLib[100];
	int len;

	loader<>& l=loader<>::instance();
	Path+="/filters";
	dp=opendir(Path);
	Path+="/";
	if(!dp) return;
	while((ep=readdir(dp)))
	{
		len=strlen(ep->d_name);
		if(len<3) continue;
		if(strcmp(&ep->d_name[len-3],".la")) continue;
		if((len>7)&&(!strcmp(&ep->d_name[len-7],"_dlg.la"))) continue;
		try
		{
			// Create the factory and insert it
			Name=Path+ep->d_name;
			handle<>& myhandle = l.load(Name);
			symbol* myinit   = myhandle.find_symbol("FactoryCreate");
			GFactoryFilter* myfactory = ((GFactoryFilterInit)(*(*myinit)))(this,ep->d_name);
			if(strcmp(API_FILTER_VERSION,myfactory->GetAPIVersion()))
			{
				Msg+=ep->d_name;
				Msg+=" - Plugin not compatible with API Version\n";
				continue;
			}
			InsertPtr(myfactory);

			// Look if dialog boxes are available
			if(!dlg) continue;
			try
			{
				strcpy(DlgLib,Name);
				DlgLib[Name.GetLen()-3]=0;
				strcat(DlgLib,"_dlg.la");
				handle<>& myhandle2 = l.load(DlgLib);
				myfactory->SetAbout(myhandle2.find_symbol("About"));
				myfactory->SetConfig(myhandle2.find_symbol("Configure"));
			}
			catch(...)
			{
			}
		}
		catch(std::exception& e)
		{
			Msg+=ep->d_name;
			Msg+=" - ";
			Msg+=e.what();
			Msg+="\n";
		}
	}
	closedir(dp);

	// If something in Msg -> error
	if(Msg.GetLen())
		throw(GException(Msg));
}


//------------------------------------------------------------------------------
void GFilterManager::Download(const char*,RString&) throw(GException)
{
}


//------------------------------------------------------------------------------
const char* GFilterManager::DetermineMIMEType(const char*) throw(GException)
{
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
	const char* mime;

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

	// Verify if the MIME type is defined -> if not try to guess
	mime=doc->GetMIMEType();
	if(!mime)
	{
		mime=DetermineMIMEType(tmpFile);
		if(mime)
			doc->SetMIMEType(mime);
	}

	// Create a DocXML.
	xml=new GDocXML(doc->GetURL(),tmpFile);

	// If MIME type defined -> analyze it.
	if(mime)
	{
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
void GFilterManager::AddMIME(const char* mime,GFilter* f) throw(bad_alloc)
{
	MIMES.InsertPtr(new GMIMEFilter(mime,f));
}


//------------------------------------------------------------------------------
void GFilterManager::DelMIMES(GFilter* f)
{
	RContainer<GMIMEFilter,unsigned int,false,false> Rem(5,5);

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
GFactoryFilterCursor& GFilterManager::GetFiltersCursor(void)
{
	GFactoryFilterCursor *cur=GFactoryFilterCursor::GetTmpCursor();
	cur->Set(this);
	return(*cur);
}


//------------------------------------------------------------------------------
GFilterManager::~GFilterManager(void)
{
}
