/*

	GALILEI Research Project

	GDocs.cpp

	Documents managed by a session - Implementation.

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
class GDocs::GDocsLang : public R::RContainer<GDoc,unsigned int,false,true>
{
public:
	GLang* Lang;

	GDocsLang(GLang* lang) throw(bad_alloc)
		: RContainer<GDoc,unsigned int,false,true>(10000,5000), Lang(lang) {}
	int Compare(const GDocsLang* docLang) const;
	int Compare(const GLang* lang) const;
	virtual ~GDocsLang(void) {}
};


//-----------------------------------------------------------------------------
int GDocs::GDocsLang::Compare(const GDocsLang* docLang) const
{
	if(!Lang)
	{
		if(!docLang->Lang) return(0);
		return(-1);
	}
	if(!docLang->Lang) return(1);
	return(Lang->Compare(docLang->Lang));
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
	int Compare(const GDocRefURL* doc) const {return(Doc->GetURL().Compare(doc->Doc->GetURL()));}
	int Compare(const char* url) const  {return(Doc->GetURL().Compare(url));}
	~GDocRefURL(void) {}
};



//-----------------------------------------------------------------------------
//
// class GDocs
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GDocs::GDocs(unsigned int nb) throw(bad_alloc)
	: RContainer<GDoc,unsigned int,true,true>(nb+(nb/2),nb/2), DocsLang(2,1),
	  DocsRefUrl(nb+(nb/2),nb/2)
{
}


//-----------------------------------------------------------------------------
GDocCursor& GDocs::GetDocsCursor(void)
{
	GDocCursor *cur=GDocCursor::GetTmpCursor();
	cur->Set(this);
	return(*cur);
}


//-----------------------------------------------------------------------------
GDocCursor& GDocs::GetDocsCursor(GLang* lang) throw(GException)
{
	GDocsLang* ptr;
	GDocCursor *cur=GDocCursor::GetTmpCursor();

	ptr=DocsLang.GetPtr<GLang*>(lang);
	if(!ptr)
	{
		cur->Clear();
		return(*cur);
	}
	cur->Set(ptr);
	return(*cur);
}


//-----------------------------------------------------------------------------
unsigned int GDocs::FillDocs(GDoc** docs,GLang* lang) throw(GException,bad_alloc)
{
	GDocsLang* ptr;

	ptr=DocsLang.GetPtr<GLang*>(lang);
	if(!ptr) return(0);
	memcpy(docs,ptr->Tab,sizeof(GDoc*)*ptr->NbPtr);
	return(ptr->NbPtr);
}


//-------------------------------------------------------------------------------
unsigned int GDocs::GetNbDocs(GLang* lang) const
{
	GDocsLang* docL = DocsLang.GetPtr<GLang*>(lang);
	if (!docL) return 1;
	return(docL->NbPtr);
}


//------------------------------------------------------------------------------
unsigned int GDocs::GetNewId(void) const
{
	if(!NbPtr) return(1);
	return(Tab[NbPtr-1]->GetId()+1);
}


//-----------------------------------------------------------------------------
void GDocs::InsertDoc(GDoc* d) throw(bad_alloc)
{
	GDocsLang* docsLang;

	InsertPtr(d);
	docsLang = DocsLang.GetInsertPtr<GLang*>(d->GetLang());
	docsLang->InsertPtr(d);

	//insert the doc in the DocsRefUrl container.
	DocsRefUrl.InsertPtr(new GDocRefURL(d));
}


//-----------------------------------------------------------------------------
void GDocs::MoveDoc(GDoc* d) throw(bad_alloc)
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
GDoc* GDocs::GetDoc(unsigned int id) throw(bad_alloc, GException)
{
	GDoc* d;

	d=GetPtr<unsigned int>(id);
	if(!d)
		throw GException("Unknown document");
	return(d);
}


//-------------------------------------------------------------------------------
GDoc* GDocs::GetDoc(const char* url) throw(bad_alloc)
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
	R::RContainer<GDoc,unsigned int,true,true>::Clear();
}


//-----------------------------------------------------------------------------
GDocs::~GDocs(void)
{
}
