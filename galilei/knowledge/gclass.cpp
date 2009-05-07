/*

	GALILEI Research Project

	GClass.cpp

	Class regrouping concepts - Implementation.

	Copyright 2009 by Pascal Francq (pascal@francq.info).

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
#include <gclass.h>
using namespace GALILEI;
using namespace R;
using namespace std;



//------------------------------------------------------------------------------
//
//  GClass
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GClass::GClass(size_t id,const R::RString& name,size_t size)
	: R::RNode<GClasses,GClass,true>(), GWeightInfosObj(id,otClass,name,size,osNew)
{
}


//------------------------------------------------------------------------------
int GClass::Compare(const GClass& grp) const
{
	return(R::CompareIds(Id,grp.Id));
}


//------------------------------------------------------------------------------
int GClass::Compare(const GClass* grp) const
{
	if(!grp)
		return(1);
	return(R::CompareIds(Id,grp->Id));
}


//------------------------------------------------------------------------------
int GClass::Compare(const size_t id) const
{
	return(R::CompareIds(Id,id));
}


//------------------------------------------------------------------------------
void GClass::Update(RContainer<GWeightInfo,false,true>& infos)
{
	// Assign information
	GWeightInfosObj::Clear();
	State=osUpdated;
	CopyInfos(infos);

	// Clear infos
	infos.Clear();

	// Emit an event that it was modified
	Emit(GEvent::eObjModified);
}


//------------------------------------------------------------------------------
GClass::~GClass(void)
{
}



//------------------------------------------------------------------------------
//
//  GClasses
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GClasses::GClasses(size_t max,size_t inc)
	: RTree<GClasses,GClass,true>(max,inc)
{
}

