/*

	GALILEI Research Project

	GRelation.cpp

	Relation - Implementation.

	Copyright 2006 by the Université Libre de Bruxelles.

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
#include <grelation.h>
#include <gconcept.h>
using namespace GALILEI;
using namespace R;



//-----------------------------------------------------------------------------
//
// GRelation
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GRelation::GRelation(unsigned int id,const R::RString& name,GConcept* subject,unsigned int type,GConcept* object,double weight)
	: Id(id), Name(name), Subject(subject), Type(type), Object(object), Weight(weight)
{
}


//-----------------------------------------------------------------------------
GRelation::GRelation(const R::RString& name,GConcept* subject,unsigned int type,GConcept* object,double weight)
	: Id(cNoRef), Name(name), Subject(subject), Type(type), Object(object), Weight(weight)
{
}


//-----------------------------------------------------------------------------
int GRelation::Compare(const GRelation& relation) const
{
	return(Id-relation.GetId());
}


//-----------------------------------------------------------------------------
int GRelation::Compare(unsigned int id) const
{
	return(Id-id);
}


//-----------------------------------------------------------------------------
void GRelation::SetId(unsigned int id)
{
	Id=id;
}
