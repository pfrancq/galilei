/*

	GALILEI Research Project

	GSubject.cpp

	Subject - Implementation.

	Copyright 2002-2009 by Pascal Francq (pascal@francq.info).
	Copyright 2002-2004 by Julien Lamoral.
	Copyright 2002-2004 by David Wartel.
	Copyright 2002-2008 by the Université Libre de Bruxelles (ULB).

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
#include <glink.h>
using namespace R;
using namespace GALILEI;
using namespace std;



//------------------------------------------------------------------------------
//
// class GSubject
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GSubject::GSubject(size_t id,const char* name,bool u)
	 : RNode<GSubject,true,false>(),  Id(id), Name(name), Used(u), AllDocs(1000,500), Docs(1000,500), Profiles(10,5), Community(0), Topic(0)
{
}


//------------------------------------------------------------------------------
int GSubject::Compare(const GSubject& sub) const
{
	return(CompareIds(Id,sub.Id));
}


//------------------------------------------------------------------------------
int GSubject::Compare(const size_t id) const
{
	return(CompareIds(Id,id));
}


//------------------------------------------------------------------------------
int GSubject::Compare(const RString& name) const
{
	return(Name.Compare(name));
}


//------------------------------------------------------------------------------
void GSubject::Insert(GDoc* doc)
{
	AllDocs.InsertPtr(doc);
}


//------------------------------------------------------------------------------
GSubject* GSubject::GetIdealGroup(GProfile* prof) const
{
	if(Profiles.IsIn(*prof))
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
	if(Docs.IsIn(*doc))
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
	return(Profiles.IsIn(*prof));
}


//------------------------------------------------------------------------------
bool GSubject::IsIn(GDoc* doc) const
{
	return(Docs.IsIn(*doc));
}


//------------------------------------------------------------------------------
void GSubject::ClearIdealGroup(tObjType type)
{
	switch(type)
	{
		case otCommunity:
			Community=0;
			break;
		case otTopic:
			Topic=0;
			break;
		default:
			break;
	}
}


//------------------------------------------------------------------------------
void GSubject::AssignIdealGroup(GCommunity* com)
{
	Community=com;
}


//------------------------------------------------------------------------------
void GSubject::AssignIdealGroup(GTopic* top)
{
	Topic=top;
}


//------------------------------------------------------------------------------
GCommunity* GSubject::GetIdealCommunity(void) const
{
	return(Community);
}


//------------------------------------------------------------------------------
GTopic* GSubject::GetIdealTopic(void) const
{
	return(Topic);
}


//------------------------------------------------------------------------------
size_t GSubject::GetNbIdealGroups(tObjType type) const
{
	size_t nb(0);

	switch(type)
	{
		case otProfile:
			if(Profiles.GetNb())
				nb++;
			break;
		case otDoc:
			if(Docs.GetNb())
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

	if(Docs.GetNb())
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
	RCursor<GProfile> Prof(Profiles);
	for(Prof.Start();!Prof.End();Prof.Next())
		if(com->IsIn(Prof()))
			tot++;
	return(tot);
}


//------------------------------------------------------------------------------
size_t GSubject::GetNbObjs(const GTopic* top) const
{
	size_t tot(0);
	RCursor<GDoc> Doc(Docs);
	for(Doc.Start();!Doc.End();Doc.Next())
		if(top->IsIn(Doc()))
			tot++;
	return(tot);
}


//------------------------------------------------------------------------------
size_t GSubject::GetNbDocs(const RContainer<GDoc,false,false>* docs) const
{
	size_t tot;
	RCursor<GDoc> sub(Docs);

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
	Profiles.InsertPtr(profile);
}


//------------------------------------------------------------------------------
R::RCursor<GProfile> GALILEI::GSubject::GetObjs(GProfile*) const
{
	return(R::RCursor<GProfile>(Profiles));
}


//------------------------------------------------------------------------------
R::RCursor<GDoc> GALILEI::GSubject::GetObjs(GDoc*) const
{
	return(R::RCursor<GDoc>(Docs));
}


//------------------------------------------------------------------------------
size_t GALILEI::GSubject::GetNbObjs(tObjType type) const
{
	switch(type)
	{
		case otDoc:
			return(Docs.GetNb());
		case otProfile:
			return(Profiles.GetNb());
		default:
			return(0);
	}
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
size_t GSubject::GetId(void) const
{
	return(Id);
}


//------------------------------------------------------------------------------
bool GSubject::IsUsed(void) const
{
	return(Used);
}


//------------------------------------------------------------------------------
void GSubject::SetUsed(GSession* session,RRandom* random,size_t nbdocs,GDoc** tmp,size_t nbprofiles,size_t& nbsocial)
{
	GProfile* prof;
	static const RString UserNames[]={"Pascal Francq","Alain Delchambre","Hugues Bersini","Francois Heinderyckx",
		"Faiza Gaultier","Marco Saerens","Marjorie Paternostre","David Wartel","Valery Vandaele","Nicolas Kumps","Julien Lamoral",
		"Sarah Rolfo","Jean-Baptiste Valsamis","Jean-Pierre Devroey","Herve Gilson"};

	// Verify that they are enough users -> If not, create new ones
	if(session->GetNbUsers()<nbprofiles+Profiles.GetNb())
	{
		size_t maxusers=sizeof(UserNames)/sizeof(char*);
		size_t newusers=nbprofiles+Profiles.GetNb()-session->GetNbUsers()+1;
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
	for(Cur.Start();(nbprofiles)&&(!Cur.End());Cur.Next())
	{
		if(Cur()->GetPtr(Name,false))
			continue;
		prof=new GProfile(Cur(),cNoRef,Name,cNoRef,RDate::GetToday(),RDate::GetToday(),RDate::GetToday(),nbsocial,5);
		session->AssignId(prof);
		session->InsertProfile(prof);
		if(nbsocial)
			nbsocial--;
		session->GetSubjects()->InsertProfileSubject(prof,Id);
		nbprofiles--;
	}

	// Select the documents
	if(!nbdocs) return;
	if(nbdocs>=AllDocs.GetNb())
		Docs=AllDocs;
	else
	{
		AllDocs.GetTab(tmp);
		random->RandOrder(tmp,AllDocs.GetNb());
		GDoc** ptr(tmp);
		for(size_t i=nbdocs+1;--i;ptr++)
			Docs.InsertPtr(*ptr);
	}
}


//------------------------------------------------------------------------------
void GSubject::ReInit(bool unselected)
{
	if(unselected)
		Used=false;
	Profiles.Clear();
	Docs.Clear();
	Community=0;
	Topic=0;
}


//------------------------------------------------------------------------------
GSubject::~GSubject(void)
{
}
