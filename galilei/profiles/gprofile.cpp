/*

	GALILEI Research Project

	GProfile.cpp

	Profile - Implementation.

	Copyright 2001-2004 by the Université libre de Bruxelles.

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
#include <docs/gdocproxy.h>
using namespace GALILEI;
using namespace R;



//------------------------------------------------------------------------------
//
// class GFdbk
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GFdbk::GFdbk(unsigned int id,tDocAssessment fdbk,RDate& updated)
  : Doc(New<GDocProxy>(id)), Fdbk(fdbk), Updated(updated)
{
}


//------------------------------------------------------------------------------
int GFdbk::Compare(const GFdbk& profdoc) const
{
	return(Doc->GetId()-profdoc.Doc->GetId());
}


//------------------------------------------------------------------------------
int GFdbk::Compare(const GFdbk* profdoc) const
{
	return(Doc->GetId()-profdoc->Doc->GetId());
}


//------------------------------------------------------------------------------
int GFdbk::Compare(const unsigned id) const
{
	return(Doc->GetId()-id);
}


//------------------------------------------------------------------------------
void GFdbk::UpdateFdbk(tDocAssessment fdbk,RDate date)
{
	Fdbk=fdbk;
	Updated=date;
}


//------------------------------------------------------------------------------
RDate GFdbk::GetUpdated(void) const
{
	return(Updated);
}


//------------------------------------------------------------------------------
tDocAssessment GFdbk::ErrorJudgment(tDocAssessment fdbk,double PercErr,RRandom* rand)
{
	double random=rand->Value()*100+1.0;

	// If there is Random change the judgment.
	if(random<PercErr)
	{
		random=rand->Value()*100+1.0;;
		switch(fdbk & djMaskJudg)
		{
			case djOK:
				if(random<25.0)
					return(djOutScope);
				else
					return(djKO);
			case djKO:
				if(random<50.0)
					return(djOK);
				else
					return(djOutScope);
			case djOutScope:
				if(random<25.0)
					return(djOK);
				else
					return(djKO);
				break;
		}
	}
	return(fdbk);
}


//------------------------------------------------------------------------------
GFdbk::~GFdbk(void)
{
	if(Doc)
		delete Doc;
}



//------------------------------------------------------------------------------
//
//  GProfile
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GProfile::GProfile(GUser* usr,unsigned int id,const char* name,bool s,unsigned int nb,unsigned int nbf)
  : RContainer<GSubProfile,false,true>(nb,nb/2), User(usr),Id(id), Name(name),
    Fdbks(nbf+nbf/2,nbf/2), Social(s)
{
	if(!User)
		throw GException("Profile "+itou(id)+" has no parent user");
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
unsigned int GProfile::GetNbAssessedDocs(const GLang* lang) const
{
	return(GetSubProfile(lang)->GetNbAssessedDocs());
}


//------------------------------------------------------------------------------
RCursor<GFdbk> GProfile::GetFdbks(void)
{
	RCursor<GFdbk> cur(Fdbks);
	return(cur);
}


//------------------------------------------------------------------------------
RCursor<GSubProfile> GProfile::GetSubProfilesCursor(void)
{
	RCursor<GSubProfile> cur(this);
	return(cur);
}


//------------------------------------------------------------------------------
void GProfile::InsertFdbk(unsigned int id,tDocAssessment assess,R::RDate date) throw(std::bad_alloc)
{
	Fdbks.InsertPtr(new GFdbk(id,assess,date));
}


//------------------------------------------------------------------------------
void GProfile::DeleteFdbk(unsigned int id) throw(std::bad_alloc)
{
	Fdbks.DeletePtr(Fdbks.GetPtr<unsigned int>(id));
}


//------------------------------------------------------------------------------
void GProfile::ClearFdbks(void)
{
	Fdbks.Clear();
}


//------------------------------------------------------------------------------
GFdbk* GProfile::GetFdbk(unsigned int id) const
{
	return(Fdbks.GetPtr<unsigned int>(id));
}


//------------------------------------------------------------------------------
void GProfile::HasUpdate(unsigned int,bool)
{
/*	GDoc* doc=GSession::Get()->GetDoc(id);*/
}


//------------------------------------------------------------------------------
void GProfile::Update(void)
{
	GSubProfile* sub;
	GLang* lang;

	// Go through each subprofiles and clear the feedbacks
	RCursor<GSubProfile> SubProfiles(this);
	for(SubProfiles.Start();!SubProfiles.End();SubProfiles.Next())
		SubProfiles()->ClearFdbks();

	// Go through each assessed document
	// If a language is defined -> put it in the corresponding subprofile.
	RCursor<GFdbk> Docs(Fdbks);
	for(Docs.Start();!Docs.End();Docs.Next())
	{
		lang=Docs()->GetDoc()->GetLang();
		if((!lang)||(!Docs()->GetDoc()->IsDefined())) continue;

		// Get the subprofile (if subprofile does not exist -> create it).
		sub=GetPtr<const GLang*>(lang,false);
		if(!sub)
		{
			sub=new GSubProfile(GSession::Get(),this,lang);
			GSession::Get()->InsertSubProfile(sub);
		}

		// Add the assessment
		sub->InsertFdbk(Docs());
	}
}


//------------------------------------------------------------------------------
GProfile::~GProfile(void)
{
}
