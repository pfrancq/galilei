/*

	GALILEI Research Project

	GPredicate.cpp

	Predicate - Implementation.

	Copyright 2006-2012 by Pascal Francq (pascal@francq.info).
	Copyright 2006-2008 by the Université Libre de Bruxelles (ULB).

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
#include <gpredicate.h>
#include <gstatement.h>
using namespace GALILEI;
using namespace R;



//-----------------------------------------------------------------------------
//
// GPredicate
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GPredicate::GPredicate(size_t id,const RString& name,const RString& desc)
	: RObject(name), Id(id), Description(desc), Statements(20)
{
}


//-----------------------------------------------------------------------------
int GPredicate::Compare(const GPredicate& predicate) const
{
	return(GetName().Compare(predicate.GetName()));
}


//-----------------------------------------------------------------------------
int GPredicate::Compare(const RString& name) const
{
	return(GetName().Compare(name));
}


//-----------------------------------------------------------------------------
void GPredicate::SetId(size_t id)
{
	Id=id;
}


//-----------------------------------------------------------------------------
RCursor<GStatement> GPredicate::GetStatements(void) const
{
	return(RCursor<GStatement>(Statements));
}


//-----------------------------------------------------------------------------
void GPredicate::InsertStatement(GStatement* statement)
{
	Statements.InsertPtr(statement);
}


//-----------------------------------------------------------------------------
GPredicate::~GPredicate(void)
{
}
