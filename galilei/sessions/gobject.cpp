/*

	GALILEI Research Project

	GObject.cpp

	Generic GALILEI Object - Implementation.

	Copyright 2009-2015 by Pascal Francq (pascal@francq.info).

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
#include <gstatement.h>
#include <gsession.h>
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
	: RObject(name), Session(session), Id(id), ObjType(objtype),
	  Subjects(0), Predicates(0), Values(0)
{
	if(!Session)
		mThrowGException("Cannot allocate an object without a session");
}


//------------------------------------------------------------------------------
GObject::GObject(const GObject& obj)
	: RObject(obj.GetName()), Session(obj.Session),Id(obj.Id), ObjType(obj.ObjType),
	  Subjects(0), Predicates(0), Values(0)
{
}


//-----------------------------------------------------------------------------
void GObject::VerifyStatements(tStatementElement what,size_t size)
{
	switch(what)
	{
		case seSubject:
			if(!Subjects)
				Subjects=new R::RContainer<GStatement,false,true>(size,50);
			else
				Subjects->VerifyTab(size);
			break;
		case sePredicate:
			if(!Predicates)
				Predicates=new R::RContainer<GStatement,false,false>(size,50);
			else
				Predicates->VerifyTab(size);
			break;
		case seValue:
			if(!Values)
				Values=new R::RContainer<GStatement,false,false>(size,50);
			else
				Values->VerifyTab(size);
			break;
	}
}


//-----------------------------------------------------------------------------
int GObject::Compare(const GObject& obj) const
{
	return(CompareIds(Id,obj.Id));
}


//-----------------------------------------------------------------------------
int GObject::Compare(size_t id) const
{
	return(CompareIds(Id,id));
}


//-----------------------------------------------------------------------------
int GObject::Search(const GObject& obj) const
{
	return(Name.Compare(obj.Name));
}


//-----------------------------------------------------------------------------
int GObject::Search(const RString& name) const
{
	return(Name.Compare(name));
}


//------------------------------------------------------------------------------
void GObject::SetId(size_t id)
{
	if(Id!=cNoRef)
		mThrowGException("Object '"+RString::Number(Id)+"' has already an identifier");
	Id=id;
}


//------------------------------------------------------------------------------
RCursor<GStatement> GObject::GetObjs(const GStatement*,tStatementElement what)
{
	switch(what)
	{
		case seSubject:
			if(!Subjects)
				return(RCursor<GStatement>());
			return(RCursor<GStatement>(*Subjects));
		case sePredicate:
			if(!Predicates)
				return(RCursor<GStatement>());
			return(RCursor<GStatement>(*Predicates));
		case seValue:
			if(!Values)
				return(RCursor<GStatement>());
			return(RCursor<GStatement>(*Values));
	}
	return(RCursor<GStatement>());
}


//------------------------------------------------------------------------------
void GObject::InsertObj(tStatementElement what,GStatement* statement)
{
	switch(what)
	{
		case seSubject:
			if(!Subjects)
				Subjects=new R::RContainer<GStatement,false,true>(10,10);
			Subjects->InsertPtr(statement);
			break;
		case sePredicate:
			if(!Predicates)
				Predicates=new R::RContainer<GStatement,false,false>(10,10);
			Predicates->InsertPtr(statement);
			break;
		case seValue:
			if(!Values)
				Values=new R::RContainer<GStatement,false,false>(10,10);
			Values->InsertPtr(statement);
			break;
	}
}


//------------------------------------------------------------------------------
void GObject::DeleteObj(tStatementElement what,GStatement* statement)
{
	switch(what)
	{
		case seSubject:
			if(Subjects)
				Subjects->DeletePtr(*statement);
			break;
		case sePredicate:
			if(Predicates)
				Predicates->DeletePtr(*statement);
			break;
		case seValue:
			if(Values)
				Values->DeletePtr(*statement);
			break;
	}
}


//------------------------------------------------------------------------------
void GObject::HandlerNotFound(const RNotification&)
{
}


//------------------------------------------------------------------------------
GObject::~GObject(void)
{
	if(Subjects)
	{
		delete Subjects;
		Subjects=0;
	}
	if(Predicates)
	{
		delete Predicates;
		Predicates=0;
	}
		if(Values)
	{
		delete Values;
		Values=0;
	}
}
