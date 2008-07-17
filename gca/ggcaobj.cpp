/*

	Genetic Community Algorithm

	GGCAObj.h

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
#include <ggcaobj.h>



//-----------------------------------------------------------------------------
//
// class GGCAObj
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GGCAObj::GGCAObj(size_t id,GDoc* d)
	: Id(id), Element(d), ElementId(d->GetId()), Social(false), ParentId(0), SumPjk(0.0)
{
}


//-----------------------------------------------------------------------------
GGCAObj::GGCAObj(size_t id,GProfile* p)
	: Id(id), Element(p), ElementId(p->GetId()), Social(p->IsSocial()), ParentId(p->GetUser()->GetId()),SumPjk(0.0)
{
}


//-----------------------------------------------------------------------------
GGCAObj::GGCAObj(const GGCAObj* obj)
	: Id(obj->Id), Element(obj->Element), ElementId(obj->ElementId), Social(obj->Social), ParentId(obj->ParentId), SumPjk(obj->SumPjk)
{
}


//-----------------------------------------------------------------------------
int GGCAObj::Compare(const size_t id) const
{
	return(Id-id);
}


//-----------------------------------------------------------------------------
int GGCAObj::Compare(const GGCAObj& obj) const
{
  return(Id-obj.Id);
}


//-----------------------------------------------------------------------------
int GGCAObj::Compare(const GGCAObj* obj) const
{
  return(Id-obj->Id);
}


//-----------------------------------------------------------------------------
GGCAObj::~GGCAObj(void)
{
}
