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
GGCAObj::GGCAObj(const unsigned int id,GProfile* p)
	: Id(id), Profile(p), SumPjk(0.0)
{
}


//-----------------------------------------------------------------------------
GGCAObj::GGCAObj(const GGCAObj* obj)
	: Id(obj->Id), Profile(obj->Profile)
{
}


//-----------------------------------------------------------------------------
int GGCAObj::Compare(const unsigned int& id) const
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
