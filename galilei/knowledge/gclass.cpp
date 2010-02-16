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
#include <gsession.h>
using namespace GALILEI;
using namespace R;
using namespace std;



//------------------------------------------------------------------------------
//
//  GClass
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GClass::GClass(size_t id,size_t blockid,const R::RString& name)
	: R::RNode<GClasses,GClass,true>(), GWeightInfosObj(id,blockid,otClass,name,osNew)
{
}


//------------------------------------------------------------------------------
void GClass::Clear(void)
{
	RNode<GClasses,GClass,true>::Clear();
	GWeightInfosObj::Clear();
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
void GClass::Update(GSession* session,GWeightInfos& infos)
{
	// Remove its references
	DelRefs(otClass);
	session->UpdateRefs(infos,otClass,Id,false);

	// Assign information
	GWeightInfosObj::Clear();
	State=osUpdated;
	Transfer(infos);

	// Clear infos
	infos.Clear();

	// Update its references
	AddRefs(otClass);
	session->UpdateRefs(infos,otClass,Id,true);

	// Emit an event that it was modified
	Emit(GEvent::eObjModified);
}


//------------------------------------------------------------------------------
double GClass::GetUpOperationCost(void) const
{
	double Cost;

	if(Parent)
	{
		// The cost is the difference between the number of information entities of
		// the current class with its parents (it is of course supposed that all the
		// information entities of the parent are in the current class).
		Cost=GetVector().GetNb()-Parent->GetVector().GetNb();
	}
	else
	{
		// No parent -> all the information entities are to be 'added'.
		Cost=GetVector().GetNb();
	}

	return(Cost);
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

