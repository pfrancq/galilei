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
#include <docs/gdocs.h>
#include <docs/gdoc.h>
#include <sessions/gsession.h>
using namespace GALILEI;
using namespace RStd;



//-----------------------------------------------------------------------------
//
// class GDocs
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GALILEI::GDocs::GDocs(unsigned int nb) throw(bad_alloc)
	: RContainer<GDoc,unsigned,true,true>(nb+(nb/2),nb/2), bDocs(false)
{
}


//-----------------------------------------------------------------------------
GDocCursor& GALILEI::GDocs::GetDocsCursor(void)
{
	GDocCursor *cur=GDocCursor::GetTmpCursor();
	cur->Set(this);
	return(*cur);
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
	InsertPtr(d);
}


//-----------------------------------------------------------------------------
GDoc* GALILEI::GDocs::GetDoc(unsigned int id) throw(bad_alloc)
{
	return(GetPtr<unsigned int>(id));
}


//-----------------------------------------------------------------------------
GALILEI::GDocs::~GDocs(void)
{
}
