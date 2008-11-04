/*

	Genetic Community Algorithm

	GCAObj.h

	Objects to group - Implementation.

	Copyright 2002-2007 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, write to the Free Software
	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

*/



//-----------------------------------------------------------------------------
// include files for GALILEI
#include <gdoc.h>
#include <gprofile.h>
#include <guser.h>
#include <gsession.h>


//-----------------------------------------------------------------------------
// include files for GCA
#include <gcaobj.h>



//-----------------------------------------------------------------------------
//
// class GCAObj
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GCAObj::GCAObj(size_t id,GDoc* d)
	: Id(id), Element(d), ElementId(d->GetId()), Social(true), ParentId(0)
{
}


//-----------------------------------------------------------------------------
GCAObj::GCAObj(size_t id,GProfile* p)
	: Id(id), Element(p), ElementId(p->GetId()), Social(p->IsSocial()), ParentId(p->GetUser()->GetId())
{
}


//-----------------------------------------------------------------------------
GCAObj::GCAObj(const GCAObj* obj)
	: Id(obj->Id), Element(obj->Element), ElementId(obj->ElementId), Social(obj->Social), ParentId(obj->ParentId)
{
}


//-----------------------------------------------------------------------------
int GCAObj::Compare(const size_t id) const
{
	return(CompareIds(Id,id));
}


//-----------------------------------------------------------------------------
int GCAObj::Compare(const GCAObj& obj) const
{
  return(CompareIds(Id,obj.Id));
}


//-----------------------------------------------------------------------------
int GCAObj::Compare(const GCAObj* obj) const
{
  return(CompareIds(Id,obj->Id));
}


//-----------------------------------------------------------------------------
GCAObj::~GCAObj(void)
{
}
