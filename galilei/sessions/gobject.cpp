/*

	GALILEI Research Project

	GObject.cpp

	Generic GALILEI Object - Implementation.

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
#include <gobject.h>
using namespace GALILEI;
using namespace R;
using namespace std;



//------------------------------------------------------------------------------
//
//  GObject
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GObject::GObject(GSession* session,size_t id,const RString& name,tObjType objtype)
	: RObject(name), Session(session), Id(id), ObjType(objtype)
{
	if(!Session)
		mThrowGException("Cannot allocate an object without a session");
}


//------------------------------------------------------------------------------
GObject::GObject(const GObject* obj)
	: RObject(obj->GetName()), Session(obj->Session),Id(obj->Id), ObjType(obj->ObjType)
{
}


//------------------------------------------------------------------------------
void GObject::SetId(size_t id)
{
	if(Id!=cNoRef)
		mThrowGException("Object '"+RString::Number(Id)+"' has already an identifier");
	Id=id;
}


//------------------------------------------------------------------------------
R::RString GObject::GetSearchStr(void) const
{
	return(GetName());
}


//------------------------------------------------------------------------------
void GObject::HandlerNotFound(const RNotification&)
{

}


//------------------------------------------------------------------------------
GObject::~GObject(void)
{
}
