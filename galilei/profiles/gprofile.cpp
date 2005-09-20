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
#include <gprofile.h>
#include <gsession.h>
#include <guser.h>
#include <gsubprofile.h>
#include <gdoc.h>
using namespace GALILEI;
using namespace R;



//------------------------------------------------------------------------------
//
// class GFdbk
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GFdbk::GFdbk(unsigned int docid,GLang* lang,tDocAssessment fdbk,const RDate& when,const R::RDate& computed)
  : DocId(docid), Fdbk(fdbk), When(when), Computed(computed), Lang(lang)
{
}


//------------------------------------------------------------------------------
int GFdbk::Compare(const GFdbk& profdoc) const
{
	return(DocId-profdoc.DocId);
}


//------------------------------------------------------------------------------
int GFdbk::Compare(const GFdbk* profdoc) const
{
	return(DocId-profdoc->DocId);
}


//------------------------------------------------------------------------------
int GFdbk::Compare(const unsigned id) const
{
	return(DocId-id);
}


//------------------------------------------------------------------------------
void GFdbk::NewFdbk(tDocAssessment fdbk,const RDate& date)
{
	Fdbk=fdbk;
	When=date;
}


//------------------------------------------------------------------------------
RDate GFdbk::GetWhen(void) const
{
	return(When);
}


//------------------------------------------------------------------------------
RDate GFdbk::GetComputed(void) const
{
	return(Computed);
}


//------------------------------------------------------------------------------
bool GFdbk::MustUse(const GSubProfile* subprofile) const
{
	return((When>subprofile->GetComputed())||(Computed>subprofile->GetComputed()));
}


//------------------------------------------------------------------------------
void GFdbk::HasUpdate(GLang* lang)
{
	Computed.SetToday();
	Lang=lang;
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
		}
	}
	return(fdbk);
}


//------------------------------------------------------------------------------
GFdbk::~GFdbk(void)
{
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
	throw GException("Profile "+RString::Number(id)+" has no parent user");
	User->InsertPtr(this);
	GSession::Event(this,eObjCreated);
}


//------------------------------------------------------------------------------
int GProfile::Compare(const GProfile &profile) const
{
	return(Id-profile.Id);
}


//------------------------------------------------------------------------------
int GProfile::Compare(const RString& name) const
{
	return(Name.Compare(name));
}


//------------------------------------------------------------------------------
int GProfile::Compare(const unsigned int id) const
{
 	return(Id-id);
}


//------------------------------------------------------------------------------
void GProfile::SetId(unsigned int id)
{
	if(id==cNoRef)
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
		sub=new GSubProfile(this,cNoRef,lang,cNoRef,RDate::null,RDate::GetToday(),RDate::null);
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
unsigned int GProfile::GetNbAssessedDocs(void) const
{
	return(Fdbks.GetNb());
}


//------------------------------------------------------------------------------
RCursor<GFdbk> GProfile::GetFdbks(void) const
{
	return(RCursor<GFdbk>(Fdbks));
}


//------------------------------------------------------------------------------
RCursor<GSubProfile> GProfile::GetSubProfiles(void) const
{
	return(RCursor<GSubProfile>(*this));
}


//------------------------------------------------------------------------------
void GProfile::InsertFdbk(unsigned int docid,GLang* lang,tDocAssessment assess,const R::RDate& date,const R::RDate& update)
{
	Fdbks.InsertPtr(new GFdbk(docid,lang,assess,date,update));
}


//------------------------------------------------------------------------------
void GProfile::DeleteFdbk(unsigned int docid)
{
	Fdbks.DeletePtr(Fdbks.GetPtr<unsigned int>(docid));
}


//------------------------------------------------------------------------------
void GProfile::ClearFdbks(void)
{
	Fdbks.Clear();
}


//------------------------------------------------------------------------------
GFdbk* GProfile::GetFdbk(unsigned int docid) const
{
	return(Fdbks.GetPtr<unsigned int>(docid));
}


//------------------------------------------------------------------------------
void GProfile::HasUpdate(unsigned int docid,GLang* lang)
{
	GFdbk* fdbk=Fdbks.GetPtr(docid);
	if(fdbk)
		fdbk->HasUpdate(lang);
}


//------------------------------------------------------------------------------
void GProfile::DispatchFdbks(void)
{
	GSubProfile* sub;

	// Go through each subprofiles and clear the feedbacks
	RCursor<GSubProfile> SubProfiles(*this);
	for(SubProfiles.Start();!SubProfiles.End();SubProfiles.Next())
		SubProfiles()->ClearFdbks();

	GSession* session=GSession::Get();
	if(!session)
		return;

	// Go through each assessed document
	// If a language is defined -> put it in the corresponding subprofile.
	RCursor<GFdbk> Docs(Fdbks);
	for(Docs.Start();!Docs.End();Docs.Next())
	{
		if(!Docs()->GetLang()) continue;

		// Get the subprofile (if subprofile does not exist -> create it)
		sub=GetPtr<const GLang*>(Docs()->GetLang(),false);
		if(!sub)
		{
			sub=new GSubProfile(this,cNoRef,Docs()->GetLang(),cNoRef,RDate::null,RDate::GetToday(),RDate::null);
			GSession::Get()->InsertSubProfile(sub);
		}
		sub->InsertFdbk(Docs());
	}
}


//------------------------------------------------------------------------------
GProfile::~GProfile(void)
{
	GSession::Event(this,eObjDeleted);
}
