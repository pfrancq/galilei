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
#include <gsubject.h>
#include <gsubjects.h>
#include <gsession.h>
#include <gprofile.h>
#include <guser.h>
#include <gdoc.h>
using namespace R;
using namespace GALILEI;
using namespace std;



//------------------------------------------------------------------------------
//
// class GSubject
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GSubject::GSubject(unsigned int id,const char* name,bool u)
	 : RNode<GSubject,false>(10,2), Id(id), Name(name), Used(u),
	   Docs(1000,500), Profiles(10,5)
{
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
int GSubject::Compare(const RString& name) const
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
void GSubject::SetUsed(GSession* session,bool used,size_t nbprofiles,unsigned int& nbsocial)
{
	GProfile* prof;
	static const RString UserNames[]={"Pascal Francq","Alain Delchambre","Hugues Bersini","Francois Heinderyckx",
		"Faiza Gaultier","Marco Saerens","Marjorie Paternostre","David Wartel","Valery Vandaele","Nicolas Kumps","Julien Lamoral",
		"Sarah Rolfo","Jean-Baptiste Valsamis","Jean-Pierre Devroey","Herve Gilson"};

	Used=used;
	if(Used)
	{
		// Verify that they are enough users -> If not, create new ones
		if(session->GetNbUsers()<nbprofiles+Profiles.GetNb())
		{
			size_t maxusers=sizeof(UserNames)/sizeof(char*);
			size_t newusers=nbprofiles+Profiles.GetNb()-session->GetNbUsers()+1;
			for(size_t i=session->GetNbUsers();(--newusers)&&(i<maxusers);i++)
			{
				session->InsertUser(new GUser(session->GUsers::GetNewId(otUser),UserNames[i],UserNames[i],session->GetSubjects()->GetNbNodes()));
			}
			for(newusers++;--newusers;)
			{
				RString Usr("User"+itou(session->GetNbUsers()+1));
				session->InsertUser(new GUser(session->GUsers::GetNewId(otUser),Usr,Usr,session->GetSubjects()->GetNbNodes()));
			}
		}

		// For each user without a profile in this topic, create one.
		RCursor<GUser> Cur(session->GetUsersCursor());
		for(/*nbprofiles,*/Cur.Start();(nbprofiles)&&(!Cur.End());Cur.Next())
		{
			if(Cur()->GetPtr(Name,false))
				continue;
			session->InsertProfile(prof=new GProfile(Cur(),session->GUsers::GetNewId(otProfile),Name,nbsocial,5));
			if(nbsocial)
				nbsocial--;
			session->GetSubjects()->InsertProfileSubject(prof,Id);
			nbprofiles--;
		}
	}
	else
	{
		Profiles.Clear();
	}
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
