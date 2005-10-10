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
#include <gsubprofile.h>
#include <guser.h>
#include <glang.h>
#include <gdoc.h>
using namespace R;
using namespace GALILEI;
using namespace std;



//------------------------------------------------------------------------------
//
//  GSubProfiles
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Set of subprofiles of a given language.
class GSubProfiles : public R::RContainer<GSubProfile,false,true>
{
public:

	GLang* Lang;                 // Language

	// Constructor and Compare methods.
	GSubProfiles(GLang* lang,unsigned int s)
		: RContainer<GSubProfile,false,true>(s,s/2), Lang(lang) {}
	int Compare(const GLang* lang) const {return(Lang->Compare(lang));}
	int Compare(const GSubProfiles& s) const {return(Lang->Compare(s.Lang));}
	int Compare(const GSubProfiles* s) const {return(Lang->Compare(s->Lang));}
};



//------------------------------------------------------------------------------
//
// class GSubject::Intern
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
class GSubject::Intern
{
public:

	unsigned int Id;                                        // Identificator of the subject.
	R::RString Name;                                        // Name of the subject.
	bool Used;                                              // Determine if the subject is used.
	R::RContainer<GDoc,false,true> Docs;                    // Profiles attached to this subject.
	R::RContainer<GProfile,false,true> Profiles;            // Profiles attached to this subject.
	R::RContainer<GSubProfiles,true,true> SubProfiles;      // SubProfiles attached to this subject.

	Intern(unsigned int id,const char* name,bool u) :
	 Id(id), Name(name), Used(u), Docs(1000,500), Profiles(10,5), SubProfiles(10,5)
	{}
};


//------------------------------------------------------------------------------
//
// class GSubject
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GSubject::GSubject(GSubjects* subjects,unsigned int id,const char* name,bool u)
	 : RNode<GSubject,true,false>(subjects), Data(0)
{
	Data=new Intern(id,name,u);
}


//------------------------------------------------------------------------------
int GSubject::Compare(const GSubject& sub) const
{
	return(Data->Id-sub.Data->Id);
}


//------------------------------------------------------------------------------
int GSubject::Compare(const unsigned int id) const
{
	return(Data->Id-id);
}


//------------------------------------------------------------------------------
int GSubject::Compare(const RString& name) const
{
	return(Data->Name.Compare(name));
}


//------------------------------------------------------------------------------
void GSubject::Insert(GDoc* doc)
{
	Data->Docs.InsertPtr(doc);
}


//------------------------------------------------------------------------------
GSubject* GSubject::GetIdealGroup(GSubProfile* sub) const
{
	GSubject* subject;

	RCursor<GProfile> Prof(Data->Profiles);
	for(Prof.Start();!Prof.End();Prof.Next())
	{
		if(Prof()->IsIn(sub))
			return(const_cast<GSubject*>(this));
	}
	RCursor<GSubject> Cur(GetNodes());
	for(Cur.Start(),subject=0;(!Cur.End())&&(!subject);Cur.Next())
		subject=Cur()->GetIdealGroup(sub);
	return(subject);
}


//------------------------------------------------------------------------------
bool GSubject::IsIn(GSubProfile* sub) const
{
	RCursor<GProfile> Prof(Data->Profiles);
	for(Prof.Start();!Prof.End();Prof.Next())
	{
		if(Prof()->IsIn(sub))
			return(true);
	}
	return(false);
}


//------------------------------------------------------------------------------
bool GSubject::IsIn(GProfile* prof) const
{
	return(Data->Profiles.IsIn(*prof));
}


//------------------------------------------------------------------------------
unsigned int GSubject::GetNbIdealGroups(const GLang* lang) const
{
	GSubject* subject;
	unsigned int nb;

	RCursor<GProfile> Prof(Data->Profiles);
	for(Prof.Start(),nb=0;!Prof.End();Prof.Next())
	{
		if(Prof()->IsIn(lang,false))
		{
			nb++;
			break;
		}
	}
	RCursor<GSubject> Cur(GetNodes());
	for(Cur.Start(),subject=0;(!Cur.End())&&(!subject);Cur.Next())
		nb+=Cur()->GetNbIdealGroups(lang);
	return(nb);
}


//------------------------------------------------------------------------------
unsigned int GSubject::GetNbSubProfiles(const GLang* lang) const
{
	unsigned int nb;

	RCursor<GProfile> Prof(Data->Profiles);
	for(Prof.Start(),nb=0;!Prof.End();Prof.Next())
	{
		if(Prof()->IsIn(lang,false))
			nb++;
	}
	return(nb);
}


//------------------------------------------------------------------------------
void GSubject::InsertSubProfile(GSubProfile* s)
{
	GLang* l;
	GSubProfiles* list;

	l=s->GetLang();
	list=Data->SubProfiles.GetPtr<const GLang*>(l);
	if(!list)
		Data->SubProfiles.InsertPtr(list=new GSubProfiles(l,Data->Profiles.GetMaxNb()));
//	list->InsertPtrAt(s,s->GetId());
	if(!list->IsIn(s))
		list->InsertPtr(s);
}


//------------------------------------------------------------------------------
RCursor<GSubProfile> GSubject::GetSubProfiles(const GLang* lang) const
{
	GSubProfiles* ptr=Data->SubProfiles.GetPtr(lang);
	if(ptr)
		return(RCursor<GSubProfile>(*ptr));
	return(RCursor<GSubProfile>());
}


//------------------------------------------------------------------------------
unsigned int GSubject::GetNbSubProfiles(const GGroup* grp) const
{
	unsigned int tot;
	RCursor<GProfile> sub(Data->Profiles);
	GLang* lang=grp->GetLang();

	for(sub.Start(),tot=0;!sub.End();sub.Next())
	{
		GSubProfile* subprofile=sub()->GetSubProfile(lang);
		if((subprofile)&&(grp->IsIn(subprofile)))
			tot++;
	}
	return(tot);
}


//------------------------------------------------------------------------------
R::RCursor<GDoc> GALILEI::GSubject::GetDocs(void)
{
	R::RCursor<GDoc> cur(Data->Docs);
	return(cur);
}


//------------------------------------------------------------------------------
unsigned int GALILEI::GSubject::GetNbDocs(void) const
{
	return(Data->Docs.GetNb());
}


//------------------------------------------------------------------------------
void GSubject::Insert(GProfile* profile)
{
	Data->Profiles.InsertPtr(profile);

	// Go trought the subprofiles
	RCursor<GSubProfile> Sub(profile->GetSubProfiles());
	for(Sub.Start();!Sub.End();Sub.Next())
		InsertSubProfile(Sub());
}


//------------------------------------------------------------------------------
R::RCursor<GProfile> GALILEI::GSubject::GetProfiles(void)
{
	R::RCursor<GProfile> cur(Data->Profiles);
	return(cur);
}


//------------------------------------------------------------------------------
unsigned int GALILEI::GSubject::GetNbProfiles(void) const
{
	return(Data->Profiles.GetNb());
}


//------------------------------------------------------------------------------
RString GSubject::GetName(void) const
{
	return(Data->Name);
}


//------------------------------------------------------------------------------
RString GSubject::GetFullName(void) const
{
	RString ret=Data->Name;
	GSubject* parent=GetParent();

	while(parent)
	{
		ret=parent->GetName()+"/"+ret;
		parent=parent->GetParent();
	}
	return(ret);
}


//------------------------------------------------------------------------------
unsigned int GSubject::GetId(void) const
{
	return(Data->Id);
}


//------------------------------------------------------------------------------
bool GSubject::IsUsed(void) const
{
	return(Data->Used);
}


//------------------------------------------------------------------------------
void GSubject::SetUsed(GSession* session,size_t nbprofiles,unsigned int& nbsocial)
{
	GProfile* prof;
	static const RString UserNames[]={"Pascal Francq","Alain Delchambre","Hugues Bersini","Francois Heinderyckx",
		"Faiza Gaultier","Marco Saerens","Marjorie Paternostre","David Wartel","Valery Vandaele","Nicolas Kumps","Julien Lamoral",
		"Sarah Rolfo","Jean-Baptiste Valsamis","Jean-Pierre Devroey","Herve Gilson"};

	Data->Used=true;
	// Verify that they are enough users -> If not, create new ones
	if(session->GetNbUsers()<nbprofiles+Data->Profiles.GetNb())
	{
		size_t maxusers=sizeof(UserNames)/sizeof(char*);
		size_t newusers=nbprofiles+Data->Profiles.GetNb()-session->GetNbUsers()+1;
		for(size_t i=session->GetNbUsers();(--newusers)&&(i<maxusers);i++)
		{
			GUser* user=new GUser(cNoRef,UserNames[i],UserNames[i],session->GetSubjects()->GetNbNodes());
			session->AssignId(user);
			session->InsertUser(user);
		}
		for(newusers++;--newusers;)
		{
			RString Usr("User"+RString::Number(session->GetNbUsers()+1));
			GUser* user=new GUser(cNoRef,Usr,Usr,session->GetSubjects()->GetNbNodes());
			session->AssignId(user);
			session->InsertUser(user);
		}
	}

	// For each user without a profile in this topic, create one.
	RCursor<GUser> Cur(session->GetUsers());
	for(/*nbprofiles,*/Cur.Start();(nbprofiles)&&(!Cur.End());Cur.Next())
	{
		if(Cur()->GetPtr(Data->Name,false))
			continue;
		prof=new GProfile(Cur(),cNoRef,Data->Name,nbsocial,5);
		session->AssignId(prof);
		session->InsertProfile(prof);
		if(nbsocial)
			nbsocial--;
		session->GetSubjects()->InsertProfileSubject(prof,Data->Id);
		nbprofiles--;
	}
}


//------------------------------------------------------------------------------
/*GGroup* GSubject::CreateGroup(GLang* lang) const
{
	GGroup* Group=new GGroup(Data->Id,lang,false,RDate(""),RDate(""));
	RCursor<GProfile> Cur(Data->Profiles);
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		GSubProfile* sub=Cur()->GetSubProfile(lang);
		if(sub)
			Group->InsertSubProfile(sub);
	}
	return(Group);
}*/


//------------------------------------------------------------------------------
void GSubject::ReInit(void)
{
/*	RCursor<GSubProfiles> Cur(Data->SubProfiles);
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		RCursor<GSubProfile> Cur2(*Cur());
		for(Cur2.Start();!Cur2.End();Cur2.Next())
			cout<<Cur2()->GetId()<<endl;
	}*/
	Data->Profiles.Clear();
	Data->SubProfiles.Clear();
	Data->Used=false;
}


//------------------------------------------------------------------------------
GSubject::~GSubject(void)
{
	delete Data;
}
