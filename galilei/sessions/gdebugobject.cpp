/*

	GALILEI Research Project

	GDebugObject.h

	Object that can provide debugging information - Implementation.

	Copyright 2007-2009 by Pascal Francq (pascal@francq.info).
	Copyright 2007-2008 by the Université Libre de Bruxelles (ULB).

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
#include <gdebugobject.h>
#include <gsession.h>
#include <gslot.h>
using namespace R;
using namespace GALILEI;
using namespace std;



//------------------------------------------------------------------------------
//
// class GDebugObject
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GDebugObject::GDebugObject(const RString& name)
	: Name(name), Debug(0)
{
	GSession::AddDebugObject(this);
}


//------------------------------------------------------------------------------
int GDebugObject::Compare(const GDebugObject& obj) const
{
	return(Name.Compare(obj.Name));
}


//------------------------------------------------------------------------------
int GDebugObject::Compare(const R::RString& name) const
{
	return(Name.Compare(name));
}


//------------------------------------------------------------------------------
void GDebugObject::DebugInfo(const RString&)
{
}


//------------------------------------------------------------------------------
GDebugObject::~GDebugObject(void)
{
	GSession::RemoveDebugObject(this);
}
