/*

	GALILEI Research Project

	GDocs.cpp

	List of documents - Implementation.

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
// include files for GALILEI
#include <langs/glang.h>
#include <docs/gdocs.h>
#include <docs/gdoc.h>
#include <docs/gdocslang.h>
#include <docs/gdocrefurl.h>
#include <sessions/gsession.h>
using namespace GALILEI;
using namespace R;



//-----------------------------------------------------------------------------
//
// class GDocs
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GALILEI::GDocs::GDocs(unsigned int nb) throw(bad_alloc)
	: RContainer<GDoc,unsigned,true,true>(nb+(nb/2),nb/2), bDocs(false), DocsLang(2,1),DocsRefUrl(2,1)
{
}


//-----------------------------------------------------------------------------
GDocCursor& GALILEI::GDocs::GetDocsCursor()
{
	GDocCursor *cur=GDocCursor::GetTmpCursor();
	cur->Set(this);
	return(*cur);
}


//-----------------------------------------------------------------------------
GDocCursor& GALILEI::GDocs::GetDocsCursor(GLang* lang) throw(GException)
{
	GDocsLang* ptr;

	ptr=DocsLang.GetPtr<GLang*>(lang);
	if(!ptr)
	{
		GDocCursor* cur= GDocCursor::GetTmpCursor();
		cur->Clear();
		return(*cur);
	}
	return(ptr->GetDocsLangCursor());
}


//-----------------------------------------------------------------------------
unsigned int GALILEI::GDocs::FillDocs(GDoc** docs,GLang* lang) throw(GException,bad_alloc)
{
	GDocsLang* ptr;

	ptr=DocsLang.GetPtr<GLang*>(lang);
	if(!ptr) return(0);
	memcpy(docs,ptr->Tab,sizeof(GDoc*)*ptr->NbPtr);
	return(ptr->NbPtr);
}


//-----------------------------------------------------------------------------
void GALILEI::GDocs::InitDocs(void) throw(bad_alloc,GException)
{
	// If documents already loaded, do nothing.
	if(bDocs) return;

	// Load the documents
	LoadDocs();
	bDocs=true;
}


//-----------------------------------------------------------------------------
void GALILEI::GDocs::InsertDoc(GDoc* d) throw(bad_alloc)
{
	GDocsLang* docsLang;

	InsertPtr(d);
	docsLang = DocsLang.GetInsertPtr<GLang*>(d->GetLang());
	if(docsLang)
		docsLang->InsertDoc(d);

	//insert the doc in the DocsRefUrl container.
	DocsRefUrl.InsertPtr( new GDocRefUrl(d));
}


//-----------------------------------------------------------------------------
void GALILEI::GDocs::MoveDoc(GDoc* d) throw(bad_alloc)
{
	GLang* l =0;
	GDocsLang* docsLang;

	// remove doc from container of docs with no language
	docsLang= DocsLang.GetPtr(l);
	docsLang->DeletePtr(d);

	// move doc to container of appropriated language.
	docsLang=DocsLang.GetInsertPtr(d->GetLang());
	docsLang->InsertPtr(d);
}


//-------------------------------------------------------------------------------
GDoc* GALILEI::GDocs::GetDoc(unsigned int id) throw(bad_alloc)
{
	return(GetPtr<unsigned int>(id));
}


//-------------------------------------------------------------------------------
GDoc* GALILEI::GDocs::GetDoc(unsigned int id, GLang* lang) throw(bad_alloc)
{
	return(DocsLang.GetPtr(lang)->GetPtr<unsigned int>(id));
}


//-------------------------------------------------------------------------------
GDoc* GALILEI::GDocs::GetDoc(const char* url) throw(bad_alloc)
{
	GDocRefUrl* tmp = DocsRefUrl.GetPtr(url);
	if (! tmp)
		return NULL;
	else
	return (tmp->GetDoc());
	//return (DocsRefUrl.GetPtr(url)->GetDoc());
}


//-------------------------------------------------------------------------------
GDocsLang* GALILEI::GDocs::GetDocsLang(GLang* lang)throw(bad_alloc)
{
	return(DocsLang.GetPtr<GLang*>(lang));
}


//-------------------------------------------------------------------------------
unsigned int GALILEI::GDocs::GetNbDocs(GLang* lang) const
{
	GDocsLang* docL = DocsLang.GetPtr<GLang*>(lang);
	if (!docL) return 1;
	return(docL->NbPtr);
}


//-----------------------------------------------------------------------------
GALILEI::GDocs::~GDocs(void)
{
}
