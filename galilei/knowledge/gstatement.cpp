/*

	GALILEI Research Project

	GStatement.cpp

	Statement - Implementation.

	Copyright 2006-2014 by Pascal Francq (pascal@francq.info).
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
#include <gstatement.h>
#include <gpredicate.h>
#include <gconcept.h>
using namespace GALILEI;
using namespace R;



//-----------------------------------------------------------------------------
//
// GStatement
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GStatement::GStatement(size_t id,GPredicate* predicate,GObject* xi,GObject* xj,double weight)
	: Id(id), Predicate(predicate), Xi(xi), Xj(xj), Weight(weight)
{
}


//-----------------------------------------------------------------------------
int GStatement::Compare(const GStatement& statement) const
{
	int i(CompareIds(Xi->GetId(),statement.Xi->GetId()));
	if(i)
		return(i);
	i=CompareIds(Xj->GetId(),statement.Xj->GetId());
	if(i)
		return(i);
	return(CompareIds(Predicate->GetId(),statement.Predicate->GetId()));
}


//-----------------------------------------------------------------------------
void GStatement::SetId(size_t id)
{
	Id=id;
}


//-----------------------------------------------------------------------------
GStatement::~GStatement(void)
{
}
