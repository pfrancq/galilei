/*

	GALILEI Research Project

	GDocs.cpp

	Documents managed by a session - Implementation.

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
// include files for GALILEI
#include <gdocs.h>
#include <gdoc.h>
#include <glang.h>
#include <gdict.h>
using namespace GALILEI;
using namespace R;



//------------------------------------------------------------------------------
//
// class GDocsLang
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
class GDocsLang : public R::RContainer<GDoc,false,true>
{
public:
	GLang* Lang;

	GDocsLang(GLang* lang) throw(std::bad_alloc)
		: RContainer<GDoc,false,true>(10000,5000), Lang(lang) {}
	int Compare(const GDocsLang& docLang) const;
	int Compare(const GLang* lang) const;
	void Clear(void);
	virtual ~GDocsLang(void) {}
};


//-----------------------------------------------------------------------------
int GDocsLang::Compare(const GDocsLang& docLang) const
{
	if(!Lang)
	{
		if(!docLang.Lang) return(0);
		return(-1);
	}
	if(!docLang.Lang) return(1);
	return(Lang->Compare(*docLang.Lang));
}


//-----------------------------------------------------------------------------
int GDocsLang::Compare(const GLang* lang) const
{
	if(!Lang)
	{
		if(!lang) return(0);
		return(-1);
	}
	if(!lang) return(1);
	return(Lang->Compare(lang));
}


//------------------------------------------------------------------------------
void GDocsLang::Clear(void)
{
	if(Lang&&Lang->GetDict())
		Lang->GetDict()->Clear(otDoc);
	R::RContainer<GDoc,false,true>::Clear();
}



//------------------------------------------------------------------------------
//
// class GDocRefURL
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
class GDocRefURL
{
public:
	GDoc* Doc;

	GDocRefURL(GDoc* d) : Doc(d) {}
	int Compare(const GDocRefURL& doc) const {return(Doc->GetURL().Compare(doc.Doc->GetURL()));}
	int Compare(const char* url) const  {return(Doc->GetURL().Compare(url));}
	~GDocRefURL(void) {}
};



//-----------------------------------------------------------------------------
//
// class GDocs::Intern
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
class GDocs::Intern : public R::RContainer<GDoc,true,true>
{
public:
	R::RContainer<GDocsLang,true,true> DocsLang; // Documents ordered by language and identificator.
	R::RContainer<GDocRefURL,true,true> DocsRefUrl; // Documents ordered by URL.

	Intern(unsigned int nb)
		: RContainer<GDoc,true,true>(nb+(nb/2),nb/2), DocsLang(2,1),
	  	DocsRefUrl(nb+(nb/2),nb/2)
	{}
};



//-----------------------------------------------------------------------------
//
// class GDocs
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GDocs::GDocs(unsigned int nb)
	: Data(0)
{
	Data=new Intern(nb);
}


//-----------------------------------------------------------------------------
R::RCursor<GDoc> GDocs::GetDocs(void) const
{
	return(R::RCursor<GDoc>(*Data));
}


//-----------------------------------------------------------------------------
R::RCursor<GDoc> GDocs::GetDocs(GLang* lang) const
{
	GDocsLang* ptr;
	R::RCursor<GDoc> cur;

	ptr=Data->DocsLang.GetPtr<GLang*>(lang);
	if(!ptr)
	{
		cur.Clear();
		return(cur);
	}
	cur.Set(*ptr);
	return(cur);
}


//-----------------------------------------------------------------------------
unsigned int GDocs::GetNbDocs(void) const
{
	return(Data->GetNb());
}


//-----------------------------------------------------------------------------
unsigned int GDocs::GetMaxPosDoc(void) const
{
	return(Data->GetMaxPos());
}


//-----------------------------------------------------------------------------
unsigned int GDocs::FillDocs(GDoc** docs)
{
	return(Data->GetTab(docs));
}


//-------------------------------------------------------------------------------
unsigned int GDocs::GetNbDocs(GLang* lang) const
{
	GDocsLang* docL = Data->DocsLang.GetPtr<GLang*>(lang);
	if (!docL) return 1;
	return(docL->GetNb());
}


//------------------------------------------------------------------------------
unsigned int GDocs::GetNewId(void) const
{
	if(!Data->GetNb()) return(1);
	return((*Data)[Data->GetNb()-1]->GetId()+1);
}


//-----------------------------------------------------------------------------
void GDocs::InsertDoc(GDoc* d)
{
	GDocsLang* docsLang;

	// Test if the document has an id
	if(d->GetId()==cNoRef)
		d->SetId(GetNewId());

	// Insert the document
	Data->InsertPtrAt(d,d->GetId());
	docsLang = Data->DocsLang.GetInsertPtr<GLang*>(d->GetLang());
	docsLang->InsertPtr(d);

	//insert the doc in the DocsRefUrl container.
	Data->DocsRefUrl.InsertPtr(new GDocRefURL(d));
}


//-----------------------------------------------------------------------------
void GDocs::MoveDoc(GDoc* d)
{
	GDocsLang* docsLang;

	// Remove doc from container of docs with no language
	docsLang=Data->DocsLang.GetPtr<GLang*>(0);
	docsLang->DeletePtr(d);

	// Move doc to container of appropriated language.
	docsLang=Data->DocsLang.GetInsertPtr(d->GetLang());
	docsLang->InsertPtr(d);
}


//-------------------------------------------------------------------------------
GDoc* GDocs::GetDoc(unsigned int id)
{
	GDoc* d;

	if(id>Data->GetMaxPos())
		return(0);
	d=(*Data)[id];
	if(!d)
		throw GException("Unknown document");
	return(d);
}


//-------------------------------------------------------------------------------
GDoc* GDocs::GetDoc(const char* url)
{
	GDocRefURL* ref;

	ref=Data->DocsRefUrl.GetPtr(url);
	if(!ref) return(0);
	return(ref->Doc);
}


//-----------------------------------------------------------------------------
void GDocs::Clear(void)
{
	RCursor<GDocsLang> Cur(Data->DocsLang);
	for(Cur.Start();!Cur.End();Cur.Next())
		Cur()->Clear();
	Data->DocsRefUrl.Clear();
	Data->R::RContainer<GDoc,true,true>::Clear();
}


//-----------------------------------------------------------------------------
GDocs::~GDocs(void)
{
}
