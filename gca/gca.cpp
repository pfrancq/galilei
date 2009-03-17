/*

	Genetic Community Algorithm

	GCA.cpp

	Main - Implementation.

	Copyright 2002-2007 by the Université Libre de Bruxelles.

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
#include <gdoc.h>
#include <gprofile.h>
#include <guser.h>
#include <gsession.h>


//-----------------------------------------------------------------------------
// includes files for GCA
#include <gca.h>
using namespace R;



//-----------------------------------------------------------------------------
//
// class GCAObj
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GCAObj::GCAObj(size_t id,GDoc* d)
	: RObjSC(id,d->GetName()), Element(d), ElementId(d->GetId())
{
}


//-----------------------------------------------------------------------------
GCAObj::GCAObj(size_t id,GProfile* p)
	: RObjSC(id,p->GetName(),p->IsSocial(),p->GetUser()->GetId()), Element(p), ElementId(p->GetId())
{
}


//-----------------------------------------------------------------------------
GCAObj::~GCAObj(void)
{
}



//-----------------------------------------------------------------------------
//
// class CGroup
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
CGroup::CGroup(CGroups* owner,size_t id)
	: R::RGroup<CGroup,GCAObj,CGroups>(owner,id)
{
}



//-----------------------------------------------------------------------------
//
// class CGroups
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
CGroups::CGroups(R::RCursor<GCAObj> objs,size_t max)
	: R::RGroups<CGroup,GCAObj,CGroups>(objs,max)
{
}
