/*

	GALILEI Research Project

	RObjIR.cpp

	IR Object to place in a specific group - Implementation.

	(c) 2002 by P. Francq.

	Version $Revision$

	Last Modify: $Date$

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
#include <groups/gobjir.h>
using namespace GALILEI;



//-----------------------------------------------------------------------------
//
// class GObjIR
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GALILEI::GObjIR::GObjIR(const unsigned int id,GSubProfile* s)
	: Id(id), SubProfile(s)
{
}


//-----------------------------------------------------------------------------
GALILEI::GObjIR::GObjIR(const GObjIR* obj)
	: Id(obj->Id), SubProfile(obj->SubProfile)
{
}


//-----------------------------------------------------------------------------
int GALILEI::GObjIR::Compare(const unsigned int& id) const
{
	return(Id-id);
}


//-----------------------------------------------------------------------------
int GALILEI::GObjIR::Compare(const GObjIR& obj) const
{
  return(Id-obj.Id);
}


//-----------------------------------------------------------------------------
int GALILEI::GObjIR::Compare(const GObjIR* obj) const
{
  return(Id-obj->Id);
}


//-----------------------------------------------------------------------------
GALILEI::GObjIR::~GObjIR(void)
{
}
