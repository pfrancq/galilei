/*

	GALILEI Research Project

	GUsers.cpp

	Users - Implementation.

	Copyright 2001-2003 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).

	Version $Revision$

	Last Modify: $Date$

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
#include <profiles/gusers.h>
#include <profiles/guser.h>
#include <profiles/gprofile.h>
#include <profiles/gsubprofile.h>
#include <infos/glang.h>
#include <profiles/gprofdoc.h>
using namespace R;
using namespace GALILEI;



//------------------------------------------------------------------------------
//
//  GUsers::GSubProfiles
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Set of subprofiles of a given language.
class GUsers::GSubProfiles : public R::RContainer<GSubProfile,unsigned int,true,true>
{
public:

	GLang* Lang;                 // Language

	// Constructor and Compare methods.
	GSubProfiles(GLang* lang,unsigned int s) throw(bad_alloc)
		: RContainer<GSubProfile,unsigned int,true,true>(s,s/2), Lang(lang) {}
	int Compare(const GLang* lang) const {return(Lang->Compare(lang));}
	int Compare(const GSubProfiles& s) const {return(Lang->Compare(s.Lang));}
	int Compare(const GSubProfiles* s) const {return(Lang->Compare(s->Lang));}

	// Get a cursor over the subprofiles of the system.
	GSubProfileCursor& GetSubProfilesCursor(void);

	// Destructor.
	virtual ~GSubProfiles(void) {}
};


//------------------------------------------------------------------------------
GSubProfileCursor& GUsers::GSubProfiles::GetSubProfilesCursor(void)
{
	GSubProfileCursor *cur=GSubProfileCursor::GetTmpCursor();
	cur->Set(this);
	return(*cur);
}



//------------------------------------------------------------------------------
//
//  GUsers
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GUsers::GUsers(unsigned int u,unsigned int p) throw(bad_alloc)
	: RContainer<GUser,unsigned,true,true>(u,u/2)
{
	Profiles=new RContainer<GProfile,unsigned int,true,true>(p,p/2);
	SubProfiles=new RContainer<GSubProfiles,unsigned int,true,true>(p,p/2);
}


//------------------------------------------------------------------------------
GUserCursor& GUsers::GetUsersCursor(void)
{
	GUserCursor *cur=GUserCursor::GetTmpCursor();
	cur->Set(this);
	return(*cur);
}


//------------------------------------------------------------------------------
void GUsers::InsertUser(GUser* usr) throw(bad_alloc)
{
	InsertPtr(usr);
}


//------------------------------------------------------------------------------
GUser* GUsers::GetUser(unsigned int id) const
{
	return(GetPtr<unsigned int>(id));
}


//------------------------------------------------------------------------------
unsigned int GUsers::GetNewId(tObjType obj) throw(GException)
{
	unsigned int id,i;
	RCursor<GSubProfiles,unsigned int> Cur;

	switch(obj)
	{
		case otProfile:
			if(Profiles->NbPtr)
				id=Profiles->Tab[NbPtr-1]->GetId()+1;
			else
				id=1;
			break;

		case otSubProfile:
			Cur.Set(SubProfiles);
			for(Cur.Start(),id=0;!Cur.End();Cur.Next())
			{
				if(!Cur()->NbPtr) continue;
				i=Cur()->Tab[Cur()->NbPtr-1]->GetId();
				if(id<i)
					id=i;
			}
			id++;
			break;

		case otUser:
			if(NbPtr)
				id=Tab[NbPtr-1]->GetId()+1;
			else
				id=1;
			break;

		default:
			throw GException("No a valid type");
			break;
	}
	return(id);
}


//------------------------------------------------------------------------------
void GUsers::InsertProfile(GProfile* p) throw(bad_alloc)
{
	Profiles->InsertPtr(p);
}


//------------------------------------------------------------------------------
GProfile* GUsers::GetProfile(const unsigned int id) const
{
	return(Profiles->GetPtr<unsigned int>(id));
}


//------------------------------------------------------------------------------
GProfileCursor& GUsers::GetProfilesCursor(void)
{
	GProfileCursor *cur=GProfileCursor::GetTmpCursor();
	cur->Set(Profiles);
	return(*cur);
}


//------------------------------------------------------------------------------
unsigned int GUsers::GetProfilesNb(void) const
{
	return(Profiles->NbPtr);
}


//------------------------------------------------------------------------------
void GUsers::InsertSubProfile(GSubProfile* s) throw(bad_alloc,GException)
{
	GLang* l;
	GSubProfiles* list;

	l=s->GetLang();
	list=SubProfiles->GetPtr<const GLang*>(l);
	if(!list)
		SubProfiles->InsertPtr(list=new GSubProfiles(l,Profiles->MaxPtr));
	list->InsertPtr(s);
}


//------------------------------------------------------------------------------
GSubProfile* GUsers::GetSubProfile(const unsigned int id) const throw(GException)
{
	RCursor<GSubProfiles,unsigned int> Cur;
	GSubProfile* ptr;

	Cur.Set(SubProfiles);
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		ptr=Cur()->GetPtr<unsigned int>(id);
		if(ptr) return(ptr);
	}
	return(0);
}


//------------------------------------------------------------------------------
GSubProfile* GUsers::GetSubProfile(const unsigned int id,GLang* lang) const throw(GException)
{
	return(SubProfiles->GetPtr<const GLang*>(lang)->GetPtr<unsigned int>(id));
}


//------------------------------------------------------------------------------
GSubProfileCursor& GUsers::GetSubProfilesCursor(GLang* lang) throw(GException)
{
	GSubProfiles* ptr=SubProfiles->GetPtr<const GLang*>(lang);
	if(ptr)
		return(ptr->GetSubProfilesCursor());
	GSubProfileCursor* cur=GSubProfileCursor::GetTmpCursor();
	cur->Set(static_cast<GSubProfiles*>(0));
	return(*cur);
}


//------------------------------------------------------------------------------
void GUsers::ClearSubProfilesGroups(void) throw(GException)
{
	RCursor<GSubProfiles,unsigned int> Cur;
	GSubProfileCursor Cur2;

	Cur.Set(SubProfiles);
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		Cur2.Set(Cur());
		for(Cur2.Start();!Cur2.End();Cur2.Next())
		{
			Cur2()->SetGroup(0);
		}
	}
}


//------------------------------------------------------------------------------
void GUsers::Clear(void) throw(GException)
 {
	if(SubProfiles)
	{
		delete SubProfiles;
		SubProfiles=0;
	}
	if(Profiles)
	{
		delete Profiles;
		Profiles=0;
	}
	RContainer<GUser,unsigned,true,true>::Clear();
 }


//------------------------------------------------------------------------------
GUsers::~GUsers(void)
{
	Clear();
}
