/*

	GALILEI Research Project

	GSubject.cpp

	Subject - Implementation.

	Copyright 2002-2008 by the Université Libre de Bruxelles.

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
#include <glang.h>
#include <gdoc.h>
#include <gcommunity.h>
#include <gtopic.h>
using namespace R;
using namespace GALILEI;
using namespace std;



//------------------------------------------------------------------------------
//
// class GSubject::Intern
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
class GSubject::Intern
{
public:

	size_t Id;                                              // Identifier of the subject.
	RString Name;                                           // Name of the subject.
	bool Used;                                              // Determine if the subject is used.
	RContainer<GDoc,false,true> Docs;                       // Documents attached to this subject.
	RContainer<GProfile,false,true> Profiles;               // Profiles attached to this subject.
	GCommunity* Community;                                  // Ideal community.
	GTopic* Topic;                                          // Ideal topic.

	Intern(size_t id,const char* name,bool u) :
	 Id(id), Name(name), Used(u), Docs(1000,500), Profiles(10,5), Community(0), Topic(0)
	{}
};


//------------------------------------------------------------------------------
//
// class GSubject
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GSubject::GSubject(size_t id,const char* name,bool u)
	 : RNode<GSubject,true,false>(), Data(0)
{
	Data=new Intern(id,name,u);
}


//------------------------------------------------------------------------------
int GSubject::Compare(const GSubject& sub) const
{
	return(Data->Id-sub.Data->Id);
}


//------------------------------------------------------------------------------
int GSubject::Compare(const size_t id) const
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
GSubject* GSubject::GetIdealGroup(GProfile* prof) const
{
	if(Data->Profiles.IsIn(prof->GetId()))
		return(const_cast<GSubject*>(this));

	GSubject* subject(0);
	RCursor<GSubject> Cur(GetNodes());
	for(Cur.Start();(!Cur.End())&&(!subject);Cur.Next())
		subject=Cur()->GetIdealGroup(prof);
	return(subject);
}


//------------------------------------------------------------------------------
GSubject* GSubject::GetIdealGroup(GDoc* doc) const
{
	if(Data->Docs.IsIn(*doc))
		return(const_cast<GSubject*>(this));

	GSubject* subject(0);
	RCursor<GSubject> Cur(GetNodes());
	for(Cur.Start();(!Cur.End())&&(!subject);Cur.Next())
		subject=Cur()->GetIdealGroup(doc);
	return(subject);
}


//------------------------------------------------------------------------------
bool GSubject::IsIn(GProfile* prof) const
{
	return(Data->Profiles.IsIn(*prof));
}


//------------------------------------------------------------------------------
bool GSubject::IsIn(GDoc* doc) const
{
	return(Data->Docs.IsIn(*doc));
}

//------------------------------------------------------------------------------
void GSubject::ClearIdealGroup(tObjType type)
{
	switch(type)
	{
		case otCommunity:
			Data->Community=0;
			break;
		case otTopic:
			Data->Topic=0;
			break;
		default:
			break;
	}
}


//------------------------------------------------------------------------------
void GSubject::AssignIdealGroup(GCommunity* com)
{
	Data->Community=com;
}


//------------------------------------------------------------------------------
void GSubject::AssignIdealGroup(GTopic* top)
{
	Data->Topic=top;
}


//------------------------------------------------------------------------------
GCommunity* GSubject::GetIdealCommunity(void) const
{
	return(Data->Community);
}


//------------------------------------------------------------------------------
GTopic* GSubject::GetIdealTopic(void) const
{
	return(Data->Topic);
}


//------------------------------------------------------------------------------
size_t GSubject::GetNbIdealGroups(tObjType type) const
{
	size_t nb(0);

	switch(type)
	{
		case otProfile:
			if(Data->Profiles.GetNb())
				nb++;
			break;
		case otDoc:
			if(Data->Docs.GetNb())
				nb++;
			break;
		default:
			throw GException("GSubject::GetNbIdealGroups(tObjType) doesn't support type"+GetObjType(type));
	}
	RCursor<GSubject> Cur(GetNodes());
	for(Cur.Start();!Cur.End();Cur.Next())
		nb+=Cur()->GetNbIdealGroups(type);
	return(nb);
}


//------------------------------------------------------------------------------
size_t GSubject::GetNbTopicsDocs(void) const
{
	size_t nb(0);

	if(Data->Docs.GetNb())
		nb++;
	RCursor<GSubject> Cur(GetNodes());
	for(Cur.Start();!Cur.End();Cur.Next())
		nb+=Cur()->GetNbTopicsDocs();
	return(nb);
}


//------------------------------------------------------------------------------
size_t GSubject::GetNbObjs(const GCommunity* com) const
{
	size_t tot(0);
	RCursor<GProfile> Prof(Data->Profiles);
	for(Prof.Start();!Prof.End();Prof.Next())
		if(com->IsIn(Prof()))
			tot++;
	return(tot);
}


//------------------------------------------------------------------------------
size_t GSubject::GetNbObjs(const GTopic* top) const
{
	size_t tot(0);
	RCursor<GDoc> Doc(Data->Docs);
	for(Doc.Start();!Doc.End();Doc.Next())
		if(top->IsIn(Doc()))
			tot++;
	return(tot);
}


//------------------------------------------------------------------------------
size_t GSubject::GetNbDocs(const RContainer<GDoc,false,false>* docs) const
{
	size_t tot;
	RCursor<GDoc> sub(Data->Docs);

	for(sub.Start(),tot=0;!sub.End();sub.Next())
	{
		if(docs->IsIn(*sub()))
			tot++;
	}
	return(tot);
}


//------------------------------------------------------------------------------
void GSubject::Insert(GProfile* profile)
{
	Data->Profiles.InsertPtr(profile);
}


//------------------------------------------------------------------------------
R::RCursor<GProfile> GALILEI::GSubject::GetObjs(GProfile*) const
{
	return(R::RCursor<GProfile>(Data->Profiles));
}


//------------------------------------------------------------------------------
R::RCursor<GDoc> GALILEI::GSubject::GetObjs(GDoc*) const
{
	return(R::RCursor<GDoc>(Data->Docs));
}


//------------------------------------------------------------------------------
size_t GALILEI::GSubject::GetNbObjs(tObjType type) const
{
	switch(type)
	{
		case otDoc:
			return(Data->Docs.GetNb());
		case otProfile:
			return(Data->Profiles.GetNb());
		default:
			return(0);
	}
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
size_t GSubject::GetId(void) const
{
	return(Data->Id);
}


//------------------------------------------------------------------------------
bool GSubject::IsUsed(void) const
{
	return(Data->Used);
}


//------------------------------------------------------------------------------
void GSubject::SetUsed(GSession* session,size_t nbprofiles,size_t& nbsocial)
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
		prof=new GProfile(Cur(),cNoRef,Data->Name,cNoRef,RDate::GetToday(),RDate::GetToday(),RDate::GetToday(),nbsocial,5);
		session->AssignId(prof);
		session->InsertProfile(prof);
		if(nbsocial)
			nbsocial--;
		session->GetSubjects()->InsertProfileSubject(prof,Data->Id);
		nbprofiles--;
	}
}


//------------------------------------------------------------------------------
void GSubject::ReInit(void)
{
	Data->Profiles.Clear();
	Data->Community=0;
	Data->Topic=0;
	Data->Used=false;
}


//------------------------------------------------------------------------------
GSubject::~GSubject(void)
{
	delete Data;
}
