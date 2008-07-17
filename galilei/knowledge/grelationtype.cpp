/*

	GALILEI Research Project

	GRelationType.cpp

	Relation Type - Implementation.

	Copyright 2006 by the Universit� Libre de Bruxelles.

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



//-----------------------------------------------------------------------------
// include files for GALILEI
#include <grelationtype.h>
#include <grelation.h>
using namespace GALILEI;
using namespace R;



//-----------------------------------------------------------------------------
//
// GRelationType
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GRelationType::GRelationType(size_t id,const R::RString& name,const R::RString& desc)
	: Id(id), Name(name), Description(desc), Relations(20)
{
}


//-----------------------------------------------------------------------------
int GRelationType::Compare(const GRelationType& type) const
{
	return(Id-type.Id);
}


//-----------------------------------------------------------------------------
int GRelationType::Compare(size_t id) const
{
	return(Id-id);
}


//-----------------------------------------------------------------------------
int GRelationType::Compare(const R::RString& name) const
{
	return(Name.Compare(name));
}


//-----------------------------------------------------------------------------
GRelation* GRelationType::GetRelation(size_t id)
{
	return(Relations.GetPtr(id));
}

//-----------------------------------------------------------------------------
R::RCursor<GRelation> GRelationType::GetRelations(void) const
{
	return(R::RCursor<GRelation>(Relations));
}


//-----------------------------------------------------------------------------
void GRelationType::InsertRelation(GRelation* relation)
{
	Relations.InsertPtr(relation);
}


//-----------------------------------------------------------------------------
GRelationType::~GRelationType(void)
{
}
