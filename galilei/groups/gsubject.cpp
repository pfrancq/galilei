/*

	GALILEI Research Project

	GSubject.cpp

	GSubject - Implementation.

	Copyright 2002 by the Universit� Libre de Bruxelles.

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
// include files for ANSI C/C++
#include <stdlib.h>
#include <iostream>
#include <math.h>


//-----------------------------------------------------------------------------
//include files for R Project
#include <rstd/rcursor.h>
#include <rstd/rtextfile.h>
using namespace R;


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <groups/gsubject.h>
#include <groups/ggroups.h>
#include <profiles/gprofile.h>
#include <profiles/gsubprofile.h>
#include <profiles/guser.h>
#include <profiles/gprofdoc.h>
#include <langs/glang.h>
using namespace GALILEI;



//-----------------------------------------------------------------------------
//
// class GSubject
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GSubject::GSubject(unsigned int id,const char* name,GLang* l,bool u)
	 : RNode<GSubject,false>(10,2), GGroupVector(id,l), Name(name), Used(u),
	   Docs(1000,500)
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
unsigned int GALILEI::GSubject::GetNbDocs(void) const
{
	return(Docs.NbPtr);
}


//-----------------------------------------------------------------------------
GSubject::~GSubject(void)
{
}
