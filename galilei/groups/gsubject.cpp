/*

	GALILEI Research Project

	GSubject.cpp

	Subject - Implementation.

	Copyright 2002-2003 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be)
		Julien Lamoral (jlamoral@ulb.ac.be).
		David Wartel (dwartel@ulb.ac.be).

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
#include <groups/gsubject.h>
#include <profiles/gsubprofile.h>
#include <docs/gdoc.h>
using namespace R;
using namespace GALILEI;



//------------------------------------------------------------------------------
//
// class GSubject
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GSubject::GSubject(unsigned int id,const char* name,GLang* l,bool u) throw(std::bad_alloc)
	 : RNode<GSubject,false>(10,2), GGroup(id,l,false), Name(name), Used(u),
	   Docs(1000,500)
{
}


//------------------------------------------------------------------------------
int GSubject::Compare(const GSubject* sub) const
{
	return(Id-sub->Id);
}


//------------------------------------------------------------------------------
int GSubject::Compare(const GSubject& sub) const
{
	return(Id-sub.Id);
}


//------------------------------------------------------------------------------
int GSubject::Compare(const unsigned int id) const
{
	return(Id-id);
}


//------------------------------------------------------------------------------
int GSubject::Compare(const char* name) const
{
	return(Name.Compare(name));
}


//------------------------------------------------------------------------------
void GSubject::InsertDoc(GDoc* d) throw(std::bad_alloc)
{
	Docs.InsertPtr(d);
}


//------------------------------------------------------------------------------
GDocCursor GALILEI::GSubject::GetDocsCursor(void)
{
	GDocCursor cur(Docs);
	return(cur);
}


//------------------------------------------------------------------------------
unsigned int GALILEI::GSubject::GetNbDocs(void) const
{
	return(Docs.NbPtr);
}


//------------------------------------------------------------------------------
RString GSubject::GetName(void) const
{
	return(Name);
}


//------------------------------------------------------------------------------
void GSubject::SetUsed(bool b)
{
	Used=b;
}


//------------------------------------------------------------------------------
GSubject::~GSubject(void)
{
}
