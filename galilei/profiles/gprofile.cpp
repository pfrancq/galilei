/*

	GALILEI Research Project

	GProfile.cpp

	Profile - Implementation.

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
// include filess for GALILEI
#include <profiles/gprofile.h>
#include <sessions/gsession.h>
#include <profiles/guser.h>
#include <profiles/gsubprofile.h>
#include <profiles/gprofdoc.h>
#include <docs/gdoc.h>
using namespace GALILEI;
using namespace R;



//------------------------------------------------------------------------------
//
//  GProfile
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GProfile::GProfile(GUser *usr,unsigned int id,const char* name,bool s,unsigned int nb,unsigned int nbf) throw(std::bad_alloc)
  : RContainer<GSubProfile,false,true>(nb,nb/2), User(usr),Id(id),Name(name),
    Fdbks(nbf+nbf/2,nbf/2), Social(s)
{
	User->InsertPtr(this);
}


//------------------------------------------------------------------------------
int GProfile::Compare(const GProfile &profile) const
{
	return(Id-profile.Id);
}


//------------------------------------------------------------------------------
int GProfile::Compare(const GProfile *profile) const
{
	return(Id-profile->Id);
}


//------------------------------------------------------------------------------
int GProfile::Compare(const unsigned int id) const
{
	return(Id-id);
}


#if GALILEITEST
//------------------------------------------------------------------------------
void GProfile::SetSubject(GSubject* s)
{
	Subject=s;
}


//------------------------------------------------------------------------------
GSubject* GProfile::GetSubject(void) const
{
	return(Subject);
}
#endif


//------------------------------------------------------------------------------
void GProfile::SetId(unsigned int id) throw(GException)
{
	if(Id==cNoRef)
		throw GException("Cannot assign cNoRef to a profile");
	Id=id;
}


//------------------------------------------------------------------------------
RString GProfile::GetName(void) const
{
	return(Name);
}


//------------------------------------------------------------------------------
void GProfile::SetSocial(bool social)
{
	Social=social;
}


//------------------------------------------------------------------------------
GSubProfile* GProfile::GetSubProfile(const GLang* lang) const
{
	// If no lang -> return null
	if(!lang) return(0);
	return(GetPtr<const GLang*>(lang,false));
}


//------------------------------------------------------------------------------
GSubProfile* GProfile::GetInsertSubProfile(GLang* lang,GSession* s)
{
	GSubProfile* sub;

	// If no lang
	if(!lang) return(0);

	sub=GetPtr<const GLang*>(lang,false);

	// If subprofile does not exist -> create it
	if(!sub)
	{
		sub=new GSubProfile(s,this,lang);
		s->InsertSubProfile(sub);
	}

	return(sub);
}


//------------------------------------------------------------------------------
GProfDoc* GProfile::GetFeedback(const GDoc* doc) const
{
	GLang* lang;

	lang=doc->GetLang();
	if( lang)
		return(GetSubProfile(lang)->GetFeedback(doc));
	else
		return(Fdbks.GetPtr<const GDoc*>(doc));
}


//------------------------------------------------------------------------------
unsigned int GProfile::GetNbAssessedDocs(const GLang* lang) const
{
	return(GetSubProfile(lang)->GetNbAssessedDocs());
}


//------------------------------------------------------------------------------
GProfDocCursor GProfile::GetProfDocCursor(void)
{
	GProfDocCursor cur(Fdbks);
	return(cur);
}


//------------------------------------------------------------------------------
GSubProfileCursor GProfile::GetSubProfilesCursor(void)
{
	GSubProfileCursor cur(this);
	return(cur);
}


//------------------------------------------------------------------------------
void GProfile::ClearFdbks(void)
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


//------------------------------------------------------------------------------
void GProfile::AddAssessment(GProfDoc* j,GSession* s) throw(std::bad_alloc)
{
	GLang* l;

	l=j->GetDoc()->GetLang();
	if(!l)
	{
		Fdbks.InsertPtr(j);
	}
	else
	{
		GetInsertSubProfile(l,s)->AddAssessment(j);
	}
}


//------------------------------------------------------------------------------
void GProfile::DispatchFdbks(GProfDoc* profdoc, GLang* oldlang, GSession* s)
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
		sub->AddAssessment(profdoc);
		sub->SetState(osModified);
		Fdbks.DeletePtr(profdoc);
		return;
	}

	//if the new lang is not defined and the old one is defined.
	if((!lang)&&oldlang)
	{
		Fdbks.InsertPtr(profdoc);
		sub=GetInsertSubProfile(oldlang,s);
		sub->RemoveAssessment(profdoc);
		sub->SetState(osModified);
		return;
	}

	// if the two langs are defined (but different)
	sub=GetInsertSubProfile(lang,s);
	sub->AddAssessment(profdoc);
	sub->SetState(osModified);
	sub=GetSubProfile(oldlang);
	sub->RemoveAssessment(profdoc);
	sub->SetState(osModified);
}


//------------------------------------------------------------------------------
void GProfile::Modify(GSession* session,GDocRef& profdoc,GLang* newlang,GLang* oldlang)
{
	GSubProfile* sub;

	// If no languages are defined -> nothing to do
	if((!newlang)&&(!oldlang))
		return;

	// If a new language is defined -> modify the corresponding subprofile
	if(newlang)
	{
		sub=GetInsertSubProfile(newlang,session);
		sub->SetState(osModified);
	}

	// If the languages are identical -> nothing else to do
	if(newlang==oldlang)
		return;

	#warning profdoc must be create/delete

	// If the old language was defined -> delete the assessment on the document
	if(oldlang)
	{
		GSubProfile* sub2=GetInsertSubProfile(oldlang,session);
//		subs->RemoveAssessment(profdoc);
		sub2->SetState(osModified);
	}

	if(!newlang)
	{
		// if the new language is not defined -> create an assessment in the profile
//		Fdbks.InsertPtr(profdoc);
	}
	else
	{
		// if the new language is defined -> create an asessment in the subprofile
//		sub->AddAssessment(profdoc);
	}
}


//------------------------------------------------------------------------------
GProfile::~GProfile(void)
{
}
