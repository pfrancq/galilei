/*

	GALILEI Research Project

	GObjectIndex.h

	Object Index - Implementation.

	Copyright 2001-2012 by Pascal Francq (pascal@francq.info).

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
#include <gobjectindex.h>
#include <gconcept.h>
using namespace std;
using namespace R;
using namespace GALILEI;

GObjectSyntacticPos::GObjectSyntacticPos(void)
	: Concept(0), Pos(30)
{

}
//------------------------------------------------------------------------------
int GObjectSyntacticPos::Compare(const GObjectSyntacticPos& obj) const
{
	return(CompareIds(Concept->GetId(),obj.Concept->GetId()));
}

//------------------------------------------------------------------------------
//
// class GObjectIndex
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GObjectIndex::GObjectIndex(size_t nb)
	: Objs(nb), SyntacticPos(1,10)
{

}


//------------------------------------------------------------------------------
GObjectIndex::~GObjectIndex(void)
{
}