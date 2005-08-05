/*

	GALILEI Research Project

	GUsers.cpp

	Users - Implementation.

	Copyright 2001-2003 by the Universit�Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).

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
// include files for R Project
#include <rvectorint.h>


//------------------------------------------------------------------------------
// include files for GALILEI
#include <gusers.h>
#include <guser.h>
#include <gsession.h>
#include <gstorage.h>
#include <gprofile.h>
#include <gsubprofile.h>
#include <glang.h>
#include <gdict.h>
#include <gdoc.h>
#include <gweightinfo.h>
using namespace R;
using namespace GALILEI;



//------------------------------------------------------------------------------
//
//  GUsers::GSubProfiles
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Set of subprofiles of a given language.
class GUsers::GSubProfiles : public R::RContainer<GSubProfile,true,true>
{
public:

	GLang* Lang;                 // Language

	// Constructor and Compare methods.
	GSubProfiles(GLang* lang,unsigned int s)
		: RContainer<GSubProfile,true,true>(s,s/2), Lang(lang) {}
	int Compare(const GLang* lang) const {return(Lang->Compare(lang));}
	int Compare(const GSubProfiles& s) const {return(Lang->Compare(s.Lang));}
	int Compare(const GSubProfiles* s) const {return(Lang->Compare(s->Lang));}

	// Get a cursor over the subprofiles of the system.
	RCursor<GSubProfile> GetSubProfilesCursor(void) const {return(RCursor<GSubProfile>(*this));}

	void Clear(void);

	// Destructor.
	virtual ~GSubProfiles(void) {}
};


//------------------------------------------------------------------------------
void GUsers::GSubProfiles::Clear(void)
{
	if(Lang&&Lang->GetDict())
		Lang->GetDict()->Clear(otSubProfile);
	R::RContainer<GSubProfile,true,true>::Clear();
}



//------------------------------------------------------------------------------
//
//  GUsers
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GUsers::GUsers(unsigned int u,unsigned int p)
	: RContainer<GUser,true,true>(u,u/2), Profiles(p,p/2), SubProfiles(p,p/2)
{
}


//------------------------------------------------------------------------------
R::RCursor<GUser> GUsers::GetUsersCursor(void) const
{
	return(R::RCursor<GUser>(*this));
}


//------------------------------------------------------------------------------
void GUsers::InsertUser(GUser* usr)
{
	InsertPtrAt(usr,usr->GetId(),true);
}


//------------------------------------------------------------------------------
GUser* GUsers::GetUser(unsigned int id) const
{
	if(id>GetMaxPos())
		return(0);
	return(const_cast<GUser*>((*this)[id]));
}


//------------------------------------------------------------------------------
unsigned int GUsers::GetNewId(tObjType obj)
{
	unsigned int id,i;
	RCursor<GSubProfiles> Cur;

	switch(obj)
	{
		case otProfile:
			if(Profiles.GetNb())
				id=Profiles[Profiles.GetMaxPos()]->GetId()+1;  // Not [GetNb()-1] because first profile has an identificator of 1
			else
				id=1;
			break;

		case otSubProfile:
			Cur.Set(SubProfiles);
			for(Cur.Start(),id=0;!Cur.End();Cur.Next())
			{
				if(!Cur()->GetNb()) continue;
				i=(*Cur())[Cur()->GetMaxPos()]->GetId()+1;
				if(id<i)
					id=i;
			}
			break;

		case otUser:
			if(GetNb())
				id=(*this)[GetMaxPos()]->GetId()+1; // Not [GetNb()-1] because first user has an identificator of 1
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
void GUsers::InsertProfile(GProfile* p)
{
	Profiles.InsertPtrAt(p,p->GetId());
}


//------------------------------------------------------------------------------
GProfile* GUsers::GetProfile(const unsigned int id) const
{
	if(id>Profiles.GetMaxPos())
		return(0);
	return(const_cast<GProfile*>(Profiles[id]));
}


//------------------------------------------------------------------------------
R::RCursor<GProfile> GUsers::GetProfilesCursor(void) const
{
	return(R::RCursor<GProfile>(Profiles));
}


//------------------------------------------------------------------------------
unsigned int GUsers::GetProfilesNb(void) const
{
	return(Profiles.GetNb());
}


//------------------------------------------------------------------------------
void GUsers::InsertSubProfile(GSubProfile* s)
{
	GLang* l;
	GSubProfiles* list;

	l=s->GetLang();
	list=SubProfiles.GetPtr<const GLang*>(l);
	if(!list)
		SubProfiles.InsertPtr(list=new GSubProfiles(l,Profiles.GetMaxNb()));
//	list->InsertPtrAt(s,s->GetId());
	list->InsertPtr(s);
}


//------------------------------------------------------------------------------
GSubProfile* GUsers::GetSubProfile(const unsigned int id) const
{
	GSubProfile* ptr;

	RCursor<GSubProfiles> Cur(SubProfiles);
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		//ptr=Cur()->GetPtrAt(id);
		ptr=Cur()->GetPtr(id);
		if(ptr)
			return(ptr);
	}
	return(0);
}


//------------------------------------------------------------------------------
GSubProfile* GUsers::GetSubProfile(const unsigned int id,const GLang* lang) const
{
	GSubProfiles* subs=SubProfiles.GetPtr(lang);
	if(subs)
		return(subs->GetPtr(id));
	return(0);
}


//------------------------------------------------------------------------------
RCursor<GSubProfile> GUsers::GetSubProfilesCursor(const GLang* lang) const
{
	GSubProfiles* ptr=SubProfiles.GetPtr(lang);
	if(ptr)
		return(ptr->GetSubProfilesCursor());
	return(RCursor<GSubProfile>());
}


//------------------------------------------------------------------------------
void GUsers::UpdateProfiles(unsigned int docid)
{
	GSession* session=GSession::Get();
	if(!session)
		return;

	// If there are some profile -> propagate in memory
	GDoc* doc=session->GetDoc(docid);
	if(doc&&Profiles.GetNb())
	{
		RVectorInt<true>* fdbks=doc->GetFdbks();
		if(fdbks)
		{
			for(fdbks->Start();!fdbks->End();fdbks->Next())
			{
				GProfile* prof=GetProfile((*fdbks)());
				if(!prof)
					continue;
				prof->HasUpdate(docid);
			}
		}
	}

	// Use database
	if(session->GetStorage()&&((!session->GetStorage()->MustLoadAll()))||(session->MustSave(otSubProfile)))
		session->GetStorage()->UpdateProfiles(docid);
}


//------------------------------------------------------------------------------
void GUsers::ClearUsers(void)
{
	RCursor<GSubProfiles> Cur(SubProfiles);
	for(Cur.Start();!Cur.End();Cur.Next())
		Cur()->Clear();
	Profiles.Clear();
	R::RContainer<GUser,true,true>::Clear();
}


//------------------------------------------------------------------------------
GUsers::~GUsers(void)
{
}
