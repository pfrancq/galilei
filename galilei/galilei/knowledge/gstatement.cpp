/*

	GALILEI Research Project

	GStatement.cpp

	Statement - Implementation.

	Copyright 2006-2015 by Pascal Francq (pascal@francq.info).
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
#include <gconcept.h>
using namespace GALILEI;
using namespace R;



//-----------------------------------------------------------------------------
//
// GStatement
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GStatement::GStatement(size_t id,GObject* subject,GObject* predicate,GObject* value,double weight)
	: Id(id), Subject(subject), Predicate(predicate), Value(value),
	  Weight(weight)
{
	if(!Subject)
		mThrowGException("Statement must have a valid subject");
	if(!Predicate)
		mThrowGException("Statement must have a valid predicate");
	if(!Value)
		mThrowGException("Statement must have a valid value");
}


//-----------------------------------------------------------------------------
int GStatement::Compare(const GStatement& statement) const
{
	int i;

	// Compare first the two subjects
	i=CompareIds(Subject->GetObjType(),statement.Subject->GetObjType());
	if(i)
		return(i);
	i=CompareIds(Subject->GetId(),statement.Subject->GetId());
	if(i)
		return(i);

	// Compare then the two predicayes
	i=CompareIds(Predicate->GetObjType(),statement.Predicate->GetObjType());
	if(i)
		return(i);
	i=CompareIds(Predicate->GetId(),statement.Predicate->GetId());
	if(i)
		return(i);

	// Compare then the two values
	i=CompareIds(Value->GetObjType(),statement.Value->GetObjType());
	if(i)
		return(i);
	i=CompareIds(Value->GetId(),statement.Value->GetId());
	if(i)
		return(i);

	// The two statements are identical
	return(0);
}


//-----------------------------------------------------------------------------
void GStatement::SetId(size_t id)
{
	Id=id;
}


//-----------------------------------------------------------------------------
void GStatement::SetWeight(double weight)
{
	Weight=weight;
}


//-----------------------------------------------------------------------------
GStatement::~GStatement(void)
{
}
