/*

	GALILEI Research Project

	GSubject.cpp

	Subject - Implementation.

	Copyright 2002-2003 by the Universit�Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be)
		Julien Lamoral (jlamoral@ulb.ac.be).
		David Wartel (dwartel@ulb.ac.be).

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
#include <groups/gsubject.h>
//#include <profiles/gsubprofile.h>
#include <profiles/gprofile.h>
#include <docs/gdoc.h>
using namespace R;
using namespace GALILEI;



//------------------------------------------------------------------------------
//
// class GSubject
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GSubject::GSubject(unsigned int id,const char* name,bool u) throw(std::bad_alloc)
	 : RNode<GSubject,false>(10,2), Id(id), Name(name), Used(u),
	   Docs(1000,500), Profiles(10,5)
{
}


//------------------------------------------------------------------------------
int GSubject::Compare(const GSubject* sub) const
{
	return(Id-sub->Id);
}


//------------------------------------------------------------------------------
int GSubject::Compare(const GSubject& sub) const
{
	return(Id-sub.Id);
}


//------------------------------------------------------------------------------
int GSubject::Compare(const unsigned int id) const
{
	return(Id-id);
}


//------------------------------------------------------------------------------
int GSubject::Compare(const char* name) const
{
	return(Name.Compare(name));
}


//------------------------------------------------------------------------------
void GSubject::Insert(GDoc* doc)
{
	Docs.InsertPtr(doc);
}


//------------------------------------------------------------------------------
R::RCursor<GDoc> GALILEI::GSubject::GetDocsCursor(void)
{
	R::RCursor<GDoc> cur(Docs);
	return(cur);
}


//------------------------------------------------------------------------------
unsigned int GALILEI::GSubject::GetNbDocs(void) const
{
	return(Docs.GetNb());
}


//------------------------------------------------------------------------------
void GSubject::Insert(GProfile* profile)
{
	Profiles.InsertPtr(profile);
}


//------------------------------------------------------------------------------
R::RCursor<GProfile> GALILEI::GSubject::GetProfilesCursor(void)
{
	R::RCursor<GProfile> cur(Profiles);
	return(cur);
}


//------------------------------------------------------------------------------
unsigned int GALILEI::GSubject::GetNbProfiles(void) const
{
	return(Profiles.GetNb());
}


//------------------------------------------------------------------------------
RString GSubject::GetName(void) const
{
	return(Name);
}


//------------------------------------------------------------------------------
RString GSubject::GetFullName(void) const
{
	RString ret=Name;
	GSubject* parent=GetParent();

	while(parent)
	{
		ret=parent->GetName()+"/"+ret;
		parent=parent->GetParent();
	}
	return(ret);
}


//------------------------------------------------------------------------------
void GSubject::SetUsed(bool b)
{
	Used=b;
}


//------------------------------------------------------------------------------
GGroup* GSubject::CreateGroup(GLang* lang) const
{
	GGroup* Group=new GGroup(Id,lang,false);
	RCursor<GProfile> Cur(Profiles);
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		GSubProfile* sub=Cur()->GetSubProfile(lang);
		if(sub)
			Group->InsertSubProfile(sub);
	}
	return(Group);
}


//------------------------------------------------------------------------------
GSubject::~GSubject(void)
{
}
