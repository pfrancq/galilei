/*

	GALILEI Research Project

	GSubject.cpp

	GSubject - Implementation.

	Copyright 2002 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be)
		Julien Lamoral (jlamoral@ulb.ac.be).
		David Wartel (dwartel@ulb.ac.be).

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
//include files for R Project
#include <rstd/rcontainercursor.h>
#include <rio/rtextfile.h>
using namespace RIO;
using namespace RStd;


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <groups/gsubject.h>
#include <groups/ggroup.h>
#include <groups/ggroups.h>
#include <profiles/gprofile.h>
#include <profiles/gsubprofile.h>
#include <profiles/guser.h>
#include <profiles/gprofdoc.h>
#include <langs/glang.h>
using namespace GALILEI;


//-----------------------------------------------------------------------------
//include specific files
#include <stdlib.h>
#include <iostream>
#include <math.h>




//-----------------------------------------------------------------------------
//
// class GSubject
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GSubject::GSubject(unsigned int id,const char* name,GLang* l,bool u)
	 : RNode<GSubject,false>(10,2), Id(id), Name(name), Used(u), Lang(l),
	   Docs(1000,500), SubProfiles(20,10)
{
}


//-----------------------------------------------------------------------------
void GSubject::InsertDoc(GDoc* d) throw(bad_alloc)
{
	Docs.InsertPtr(d);
}


//-----------------------------------------------------------------------------
GDocCursor& GALILEI::GSubject::GetDocsCursor()
{
	GDocCursor *cur=GDocCursor::GetTmpCursor();
	cur->Set(Docs);
	return(*cur);
}


//-----------------------------------------------------------------------------
unsigned int GSubject::GetNbDocs(void) const
{
	return(Docs.NbPtr);
}


//-----------------------------------------------------------------------------
void GSubject::InsertSubProfile(GSubProfile* s) throw(bad_alloc)
{
	SubProfiles.InsertPtr(s);
}


//-----------------------------------------------------------------------------
GSubProfileCursor& GALILEI::GSubject::GetSubProfilesCursor(void)
{
	GSubProfileCursor *cur=GSubProfileCursor::GetTmpCursor();
	cur->Set(SubProfiles);
	return(*cur);
}


//-----------------------------------------------------------------------------
unsigned int GSubject::GetNbSubProfiles(void) const
{
	return(SubProfiles.NbPtr);
}


//-----------------------------------------------------------------------------
GLang* GALILEI::GSubject::GetLang(void) const
{
	return(Lang);
}


//-----------------------------------------------------------------------------
void GSubject::SetLang(GLang* lang)
{
	Lang=lang;
}


//-----------------------------------------------------------------------------
GSubject::~GSubject(void)
{
}
