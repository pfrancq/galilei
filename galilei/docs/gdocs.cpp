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
#include <docs/gdocs.h>
#include <docs/gdoc.h>
#include <infos/glang.h>
using namespace GALILEI;
using namespace R;



//------------------------------------------------------------------------------
//
// class GDocs:: GDocsLang
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
class GDocs::GDocsLang : public R::RContainer<GDoc,false,true>
{
public:
	GLang* Lang;

	GDocsLang(GLang* lang) throw(std::bad_alloc)
		: RContainer<GDoc,false,true>(10000,5000), Lang(lang) {}
	int Compare(const GDocsLang& docLang) const;
	int Compare(const GLang* lang) const;
	virtual ~GDocsLang(void) {}
};


//-----------------------------------------------------------------------------
int GDocs::GDocsLang::Compare(const GDocsLang& docLang) const
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
int GDocs::GDocsLang::Compare(const GLang* lang) const
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
//
// class GDocs:: GDocRefURL
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
class GDocs::GDocRefURL
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
// class GDocs
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GDocs::GDocs(unsigned int nb) throw(std::bad_alloc)
	: RContainer<GDoc,true,true>(nb+(nb/2),nb/2), DocsLang(2,1),
	  DocsRefUrl(nb+(nb/2),nb/2)
{
}


//-----------------------------------------------------------------------------
R::RCursor<GDoc> GDocs::GetDocsCursor(void)
{
	return(R::RCursor<GDoc>(*this));
}


//-----------------------------------------------------------------------------
R::RCursor<GDoc> GDocs::GetDocsCursor(GLang* lang) throw(GException)
{
	GDocsLang* ptr;
	R::RCursor<GDoc> cur;

	ptr=DocsLang.GetPtr<GLang*>(lang);
	if(!ptr)
	{
		cur.Clear();
		return(cur);
	}
	cur.Set(*ptr);
	return(cur);
}


//-----------------------------------------------------------------------------
unsigned int GDocs::FillDocs(GDoc** docs) throw(GException,std::bad_alloc)
{
	return(GetTab(docs));
}


//-------------------------------------------------------------------------------
unsigned int GDocs::GetNbDocs(GLang* lang) const
{
	GDocsLang* docL = DocsLang.GetPtr<GLang*>(lang);
	if (!docL) return 1;
	return(docL->GetNb());
}


//------------------------------------------------------------------------------
unsigned int GDocs::GetNewId(void) const
{
	if(!GetNb()) return(1);
	return((*this)[GetNb()-1]->GetId()+1);
}


//-----------------------------------------------------------------------------
void GDocs::InsertDoc(GDoc* d) throw(std::bad_alloc)
{
	GDocsLang* docsLang;

	// Test if the document has an id
	if(d->GetId()==cNoRef)
		d->SetId(GetNewId());

	// Insert the document
	InsertPtrAt(d,d->GetId());
	docsLang = DocsLang.GetInsertPtr<GLang*>(d->GetLang());
	docsLang->InsertPtr(d);

	//insert the doc in the DocsRefUrl container.
	DocsRefUrl.InsertPtr(new GDocRefURL(d));
}


//-----------------------------------------------------------------------------
void GDocs::MoveDoc(GDoc* d) throw(std::bad_alloc)
{
	GDocsLang* docsLang;

	// Remove doc from container of docs with no language
	docsLang=DocsLang.GetPtr<GLang*>(0);
	docsLang->DeletePtr(d);

	// Move doc to container of appropriated language.
	docsLang=DocsLang.GetInsertPtr(d->GetLang());
	docsLang->InsertPtr(d);
}


//-------------------------------------------------------------------------------
GDoc* GDocs::GetDoc(unsigned int id) throw(std::bad_alloc, GException)
{
	GDoc* d;

	d=(*this)[id];
	if(!d)
		throw GException("Unknown document");
	return(d);
}


//-------------------------------------------------------------------------------
GDoc* GDocs::GetDoc(const char* url) throw(std::bad_alloc)
{
	GDocRefURL* ref;

	ref=DocsRefUrl.GetPtr(url);
	if(!ref) return(0);
	return(ref->Doc);
}


//-----------------------------------------------------------------------------
void GDocs::Clear(void)
{
	DocsLang.Clear();
	DocsRefUrl.Clear();
	R::RContainer<GDoc,true,true>::Clear();
}


//-----------------------------------------------------------------------------
GDocs::~GDocs(void)
{
}
