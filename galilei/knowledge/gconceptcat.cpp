/*

	GALILEI Research Project

	GConceptCat.cpp

	Concept Category - Implementation.

	Copyright 20011-2011 by Pascal Francq (pascal@francq.info).

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
#include <gconceptcat.h>
#include <gsession.h>
#include <gstorage.h>
using namespace GALILEI;
using namespace R;



//-----------------------------------------------------------------------------
//
// GConceptCat
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GConceptCat::GConceptCat(GSession* session,size_t id,const RString& name)
	: GObject(session,id,name,otConceptCat), Types(30)
{
}


//-----------------------------------------------------------------------------
int GConceptCat::Compare(const GConceptCat& cat) const
{
	return(Name.Compare(cat.Name));
}


//-----------------------------------------------------------------------------
int GConceptCat::Compare(const GConceptCat* cat) const
{
	return(Name.Compare(cat->Name));
}


//------------------------------------------------------------------------------
int GConceptCat::Compare(const R::RString& name) const
{
	return(Name.Compare(name));
}


//------------------------------------------------------------------------------
GConceptType* GConceptCat::GetConceptType(const RString& name) const
{
	return(Types.GetPtr(name));
}


//------------------------------------------------------------------------------
void GConceptCat::InsertConceptType(GConceptType* type)
{
	Types.InsertPtr(type);
}


//------------------------------------------------------------------------------
void GConceptCat::DeleteConceptType(GConceptType* type)
{
	Types.DeletePtr(*type);
}
