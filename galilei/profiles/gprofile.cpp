/*

	GALILEI Research Project

	GProfile.cpp

	Profile - Implementation.

	Copyright 2001-2002 by the Université Libre de Bruxelles.

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



//-----------------------------------------------------------------------------
// include filess for GALILEI
#include <profiles/gprofile.h>
#include <sessions/gsession.h>
#include <profiles/guser.h>
#include <profiles/gsubprofile.h>
#include <profiles/gprofdoc.h>
#include <groups/ggroup.h>
#include <docs/gdoc.h>
using namespace GALILEI;
using namespace R;



//-----------------------------------------------------------------------------
//
//  GProfile
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GALILEI::GProfile::GProfile(GUser *usr,unsigned int id,const char* name,bool s,unsigned int nb,unsigned int nbf) throw(bad_alloc)
  : RContainer<GSubProfile,unsigned,false,true>(nb,nb/2), User(usr),Id(id),Name(name),
    Fdbks(nbf+nbf/2,nbf/2), Social(s)
{

	User->InsertPtr(this);
}


//-----------------------------------------------------------------------------
int GALILEI::GProfile::Compare(const unsigned int &id) const
{
	return(Id-id);
}


//-----------------------------------------------------------------------------
int GALILEI::GProfile::Compare(const GProfile &profile) const
{
  return(Id-profile.Id);
}


//-----------------------------------------------------------------------------
int GALILEI::GProfile::Compare(const GProfile *profile) const
{
  return(Id-profile->Id);
}


//-----------------------------------------------------------------------------
GSubProfile* GALILEI::GProfile::GetSubProfile(const GLang* lang) const
{
	// If no lang -> return null
	if(!lang) return(0);
	return(GetPtr<const GLang*>(lang,false));
}


//-----------------------------------------------------------------------------
GSubProfile* GALILEI::GProfile::GetInsertSubProfile(GLang* lang,GSession* s)
{
	GSubProfile* sub;

	// If no lang
	if(!lang) return(0);

	sub=GetPtr<const GLang*>(lang,false);

	// If subprofile does not exist -> create it
	if(!sub)
		sub=s->NewSubProfile(this,lang);
	return(sub);
}


//-----------------------------------------------------------------------------
GProfDoc* GALILEI::GProfile::GetFeedback(const GDoc* doc) const
{
	GLang* lang;

	lang=doc->GetLang();
	if( lang)
		return(GetSubProfile(lang)->GetFeedback(doc));
	else
		return(Fdbks.GetPtr<const GDoc*>(doc));
}


//-----------------------------------------------------------------------------
unsigned int GALILEI::GProfile::GetNbJudgedDocs(const GLang* lang) const
{
	return(GetSubProfile(lang)->GetNbJudgedDocs());
}


//-----------------------------------------------------------------------------
GProfDocCursor& GALILEI::GProfile::GetProfDocCursor(void)
{
	GProfDocCursor *cur=GProfDocCursor::GetTmpCursor();
	cur->Set(Fdbks);
	return(*cur);
}


//-----------------------------------------------------------------------------
GSubProfileCursor& GALILEI::GProfile::GetSubProfilesCursor(void)
{
	GSubProfileCursor *cur=GSubProfileCursor::GetTmpCursor();
	cur->Set(this);
	return(*cur);
}


//-----------------------------------------------------------------------------
void GALILEI::GProfile::ClearFdbks(void)
{
	GSubProfileCursor Cur;

	Fdbks.Clear();
	Cur.Set(this);
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		Cur()->ClearFdbks();
		Cur()->SetState(osModified);
	}
}


//-----------------------------------------------------------------------------
void GALILEI::GProfile::AddJudgement(GProfDoc* j,GSession* s) throw(bad_alloc)
{
	GLang* l;
	GSubProfile* sub;

	l=j->GetDoc()->GetLang();
	if(!l)
	{
		Fdbks.InsertPtr(j);
//		SetState(osModified);
	}
	else
	{
		GetInsertSubProfile(l,s)->AddJudgement(j);
	}
}


//-----------------------------------------------------------------------------
void GALILEI::GProfile::DispatchFdbks(GProfDoc* profdoc, GLang* oldlang, GSession* s)
{
	GLang* lang;
	GSubProfile* sub;

	// Get new lang
	lang=profdoc->GetDoc()->GetLang();

	// If no lang -> return
	if((!lang)&&(!oldlang)) return;

	// if the old lang is not defined and the new lang is defined.
	if((!oldlang)&&lang)
	{
		sub=GetInsertSubProfile(lang,s);
		sub->AddJudgement(profdoc);
		sub->SetState(osModified);
		Fdbks.DeletePtr(profdoc);
		return;
	}

	//if the new lang is not defined and the old one is defined.
	if((!lang)&&oldlang)
	{
		Fdbks.InsertPtr(profdoc);
		sub=GetInsertSubProfile(oldlang,s);
		sub->RemoveJudgement(profdoc);
		sub->SetState(osModified);
		return;
	}

	// if the two langs are defined (but different)
	sub=GetInsertSubProfile(lang,s);
	sub->AddJudgement(profdoc);
	sub->SetState(osModified);
	sub=GetSubProfile(oldlang);
	sub->RemoveJudgement(profdoc);
	sub->SetState(osModified);
}



#if GALILEITEST
//-----------------------------------------------------------------------------
void GALILEI::GProfile::SetSubject(GSubject* s)
{
	Subject=s;
}


//-----------------------------------------------------------------------------
GSubject* GALILEI::GProfile::GetSubject(void) const
{
	return(Subject);
}
#endif
