/*

	GALILEI Research Project

	GClasses.cpp

	Class regrouping concepts - Implementation.

	Copyright 2009-2012 by Pascal Francq (pascal@francq.info).

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
#include <gclasses.h>
#include <gclass.h>
#include <gsession.h>
using namespace GALILEI;
using namespace R;
using namespace std;



//------------------------------------------------------------------------------
//
//  GClasses
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GClasses::GClasses(size_t max)
	: RTree<GClasses,GClass,false>(), GObjects<GClass>(max,"Classes",otClass)
{
}


//------------------------------------------------------------------------------
GClasses* GClasses::GetClassTree(void)
{
	return(this);
}


//------------------------------------------------------------------------------
void GClasses::InsertObj(GClass* parent,GClass* obj)
{
	GObjects<GClass>::InsertObj(obj);
	RTree<GClasses,GClass,false>::InsertNode(parent,obj);
}


//-----------------------------------------------------------------------------
void GClasses::DeleteObj(GClass* obj)
{
	RTree<GClasses,GClass,false>::DeleteNode(obj,true);
	GObjects<GClass>::DeleteObj(obj);
}

//-----------------------------------------------------------------------------
void GClasses::Clear(void)
{
	RTree<GClasses,GClass,false>::Clear();
	GObjects<GClass>::Clear(pClass);
}


//-----------------------------------------------------------------------------
void GClasses::Clear(const GClass* obj)
{
	RTree<GClasses,GClass,false>::Clear();
	GObjects<GClass>::Clear(obj);
}