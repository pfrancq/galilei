/*

	GALILEI Research Project

	GSubProfiles.cpp

	List of SubProfiles for a given Language - Implementation.

	Copyright 2001 by the Universit� Libre de Bruxelles.

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
#include <docs/gdoc.h>
#include <docs/gdocslang.h>
#include <langs/glang.h>

using namespace GALILEI;
using namespace RStd;



//-----------------------------------------------------------------------------
//
//  GDocsLang
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GALILEI::GDocsLang::GDocsLang(GLang* lang) throw(bad_alloc)
	: RContainer<GDoc,unsigned int,false,true>(10000,5000), Lang(lang)
{
}


//-----------------------------------------------------------------------------
void GALILEI::GDocsLang::InsertDoc(GDoc* p) throw(bad_alloc)
{
	InsertPtr(p);
}

                      
//-----------------------------------------------------------------------------
GDocCursor& GALILEI::GDocsLang::GetDocsLangCursor()
{
	GDocCursor *cur=GDocCursor::GetTmpCursor();
	cur->Set(this);
	return(*cur);
}


//-----------------------------------------------------------------------------
int GALILEI::GDocsLang::Compare(const GDocsLang& docLang) const
{
	if(!Lang)
	{
		if(!docLang.Lang) return(0);
		return(-1);
	}
	if(!docLang.Lang) return(1);
	return(Lang->Compare(docLang.Lang));
}


//-----------------------------------------------------------------------------
int GALILEI::GDocsLang::Compare(const GDocsLang* docLang) const
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
int GALILEI::GDocsLang::Compare(const GLang* lang) const
{
	if(!Lang)
	{
		if(!lang) return(0);
		return(-1);
	}
	if(!lang) return(1);
	return(Lang->Compare(lang));
}


//-----------------------------------------------------------------------------
GALILEI::GDocsLang::~GDocsLang(void)
{
}

 
