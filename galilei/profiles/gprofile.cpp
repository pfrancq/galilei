/*

 GALILEI Research Project

 GProfile.cpp

 Profile - Implementation.

 Copyright 2001-2008 by the Université libre de Bruxelles.

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
#include <gdoc.h>
#include <gweightinfo.h>
#include <gstorage.h>
#include <gcommunity.h>
using namespace GALILEI;
using namespace R;



//------------------------------------------------------------------------------
//
// class GFdbk
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GFdbk::GFdbk(unsigned int docid,tDocAssessment fdbk,const RDate& when,const R::RDate& computed)
  : DocId(docid), Fdbk(fdbk), When(when), Computed(computed)
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
bool GFdbk::MustUse(const GProfile* profile) const
{
	return((When>profile->GetComputed())||(Computed>profile->GetComputed()));
}


//------------------------------------------------------------------------------
void GFdbk::HasUpdate(void)
{
	Computed.SetToday();
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
GProfile::GProfile(GUser* usr,unsigned int id,const R::RString name,unsigned int grpid,R::RDate a,R::RDate u,R::RDate c,bool s,unsigned int nbf)
  : GWeightInfos(60), User(usr),Id(id), Name(name),
    Fdbks(nbf+nbf/2,nbf/2), Social(s), Updated(u), Computed(c), GroupId(grpid), Attached(a)
{
	if(!User)
		throw GException("Profile "+RString::Number(id)+" has no parent user");
	User->InsertPtr(this);

	// Verify if the group is existing in memory
	if((GroupId!=cNoRef)&&(GSession::Get()))
	{
		GCommunity* grp=GSession::Get()->GetCommunity(GroupId,false,false);
		if(grp)
			grp->InsertObj(this);
	}

	if(Id!=cNoRef)
		GSession::Event(this,eObjNew);
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
void GProfile::LoadInfos(void) const
{
	RContainer<GWeightInfo,false,true> Infos(1000,500);
	GSession* session=GSession::Get();
	if(session&&session->GetStorage())
		session->GetStorage()->LoadInfos(Infos,otProfile,Id);
	if(Infos.GetNb())
		const_cast<GProfile*>(this)->Update(Infos,false);
}


//------------------------------------------------------------------------------
void GProfile::SetId(unsigned int id)
{
	if(id==cNoRef)
		throw GException("Cannot assign cNoRef to a profile");
	Id=id;
	GSession::Event(this,eObjNew);
}


//------------------------------------------------------------------------------
RString GProfile::GetName(void) const
{
	return(Name);
}


//------------------------------------------------------------------------------
void GProfile::SetName(const RString& name)
{
	Name=name;
}


//------------------------------------------------------------------------------
void GProfile::SetSocial(bool social)
{
	Social=social;
}


//------------------------------------------------------------------------------
bool GProfile::MustCompute(void) const
{
	if(!Fdbks.GetNb())
		return(false);
	if(Updated<Computed)
		return(false);
	return(true);
}


//------------------------------------------------------------------------------
void GProfile::SetGroup(unsigned int groupid)
{
	GroupId=groupid;
	if(GroupId!=cNoRef)
		Attached.SetToday();
}


//------------------------------------------------------------------------------
RDate GProfile::GetAttached(void) const
{
	return(Attached);
}


//------------------------------------------------------------------------------
size_t GProfile::GetCommonOKDocs(const GProfile* prof) const
{
	tDocAssessment f;
	GFdbk* cor;
	size_t nb=0;

	// Go through the document judged by the corresponding profile
	RCursor<GFdbk> fdbks=GetFdbks();
	for(fdbks.Start();!fdbks.End();fdbks.Next())
	{
		// If the document is not "good"  -> Nothing
		f=fdbks()->GetFdbk();
		if(!(f & djOK)) continue;
		// Look for the same document in the other profile. If not found or the
		// document is not "good" -> Nothing
		cor=prof->GetFdbk(fdbks()->GetDocId());
		if(!cor) continue;
		f=cor->GetFdbk();
		if(!(f & djOK)) continue;

		// Increase the number of common documents
		nb++;
	}
	return(nb);
}


//------------------------------------------------------------------------------
size_t GProfile::GetCommonDocs(const GProfile* prof) const
{
	tDocAssessment f;
	GFdbk* cor;
	size_t nb=0;

	// Go through the document judged by the corresponding profile
	RCursor<GFdbk> Fdbks=GetFdbks();
	for(Fdbks.Start();!Fdbks.End();Fdbks.Next())
	{
		f=Fdbks()->GetFdbk();
		// Look for the same document in the other profile. If not found or the
		// document is not "good" -> Nothing
		cor=prof->GetFdbk(Fdbks()->GetDocId());
		if(!cor) continue;
		f=cor->GetFdbk();

		// Increase the number of common documents
		nb++;
	}
	return(nb);
}


//------------------------------------------------------------------------------
size_t GProfile::GetCommonDiffDocs(const GProfile* prof) const
{
	tDocAssessment f;
	GFdbk* cor;
	size_t nb=0;
	bool bOK,bOK2;

	// Go through the document judged by the corresponding profile
	RCursor<GFdbk> Fdbks=GetFdbks();
	for(Fdbks.Start();!Fdbks.End();Fdbks.Next())
	{
		f=Fdbks()->GetFdbk();
		bOK=(f & djOK);

		// If the document was not judged by the other profile or have not the
		// same judgment -> Nothing
		cor=prof->GetFdbk(Fdbks()->GetDocId());
		if(!cor) continue;
		f=cor->GetFdbk();
		bOK2=(f & djOK);
		if(bOK==bOK2) continue;

		// Increase the number of common documents with different judgement
		nb++;
	}
	return(nb);
}


//------------------------------------------------------------------------------
size_t GProfile::GetNbAssessedDocs(void) const
{
	return(Fdbks.GetNb());
}


//------------------------------------------------------------------------------
RCursor<GFdbk> GProfile::GetFdbks(void) const
{
	return(RCursor<GFdbk>(Fdbks));
}


//------------------------------------------------------------------------------
void GProfile::InsertFdbk(unsigned int docid,tDocAssessment assess,const R::RDate& date,const R::RDate& update)
{
	GFdbk* fdbk;

	Fdbks.InsertPtr(fdbk=new GFdbk(docid,assess,date,update));
	State=osModified;

	// If the document assessed was computed after the last computation
	// -> profile is considered as updated
//	if(Computed<fdbk->GetComputed())
		Updated.SetToday();
}


//------------------------------------------------------------------------------
void GProfile::DeleteFdbk(unsigned int docid)
{
	GFdbk* fdbk;

	Fdbks.DeletePtr(fdbk=Fdbks.GetPtr<unsigned int>(docid));
	State=osModified;

	// If the document assessed was computed after the last computation
	// -> profile is considered as updated
//	if(Computed<fdbk->GetComputed())
		Updated.SetToday();
}


//------------------------------------------------------------------------------
void GProfile::Update(R::RContainer<GWeightInfo,false,true>& infos,bool computed)
{
	// Remove its references
	if(computed)
		DelRefs(otProfile);

	// Assign information
	GWeightInfos::Clear();
	if(computed)
	{
		State=osUpdated;
		Computed.SetToday();

		// Update the group were it belongs
		if(GSession::Get())
			GSession::Get()->UpdateCommunity(this);
	}
	else
		State=osUpToDate;
	CopyInfos(&infos);

	// Clear infos
	infos.Clear();

	// Update its references
	if(computed)
		AddRefs(otProfile);

	// Emit an event that it was modified
	if(computed)
		GSession::Event(this,eObjModified);
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
void GProfile::HasUpdate(unsigned int docid)
{
	GFdbk* fdbk=Fdbks.GetPtr(docid);
	if(fdbk)
		fdbk->HasUpdate();
}


//------------------------------------------------------------------------------
GProfile::~GProfile(void)
{
	GSession::Event(this,eObjDelete);
	try
	{
		// Remove it from its group if necessary
		if((GroupId!=cNoRef)&&(GSession::Get()))
		{
			GCommunity* grp=GSession::Get()->GetCommunity(GroupId);
			if(grp)
				grp->DeleteObj(this);
		}

		// Remove its references
		if(State==osDelete)  // The object has modified the references count but was not saved
			DelRefs(otProfile);
	}
	catch(...)
	{
	}
}
