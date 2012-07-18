/*

	GALILEI Research Project

	GProfile.cpp

	Profile - Implementation.

	Copyright 2001-2012 by Pascal Francq (pascal@francq.info).
	Copyright 2001-2008 by the Université Libre de Bruxelles (ULB).

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
#include <gprofile.h>
#include <gsession.h>
#include <guser.h>
#include <gdoc.h>
#include <gstorage.h>
#include <gcommunity.h>
#include <gfdbk.h>
#include <gdescriptionobject.hh>
using namespace GALILEI;
using namespace R;
using namespace std;



//------------------------------------------------------------------------------
//
//  GProfile
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void GProfile::PrivateInit(void)
{
	SetState(osNew);
	SaveDesc();
	AddRefs(Session,otProfile);
	SetId(cNoRef);
}


//------------------------------------------------------------------------------
GProfile::GProfile(GSession* session,GUser* usr,tProfileType type,const R::RString name,bool s)
  : GDescriptionObject<GProfile>(session,cNoRef,0,otProfile,name,osNew), User(usr), Type(type),
    Fdbks(100,50), Social(s), Updated(RDate::GetToday()), Computed(RDate::Null),
    GroupId(0), Attached(RDate::Null), Score(0.0), Level(0)
{
	if(!User)
		ThrowGException("New profile has no parent user");
	User->InsertPtr(this);

	// Verify if the community exists in memory
	if(GroupId)
	{
		GCommunity* grp=Session->GetObj(pCommunity,GroupId,false,false);
		if(grp)
			grp->InsertObj(this);
	}
}


//------------------------------------------------------------------------------
GProfile::GProfile(GSession* session,GUser* usr,tProfileType type,size_t id,size_t blockid,const R::RString name,size_t grpid,RDate a,RDate u,RDate c,bool s,double score,char level,size_t nbf)
  : GDescriptionObject<GProfile>(session,id,blockid,otProfile,name,osNew), User(usr), Type(type),
    Fdbks(nbf+nbf/2,nbf/2), Social(s), Updated(u), Computed(c),
    GroupId(grpid), Attached(a), Score(score), Level(level)
{
	if(!User)
		ThrowGException("Profile "+RString::Number(id)+" has no parent user");
	User->InsertPtr(this);

	// Verify if the community exists in memory
	if(GroupId)
	{
		GCommunity* grp=Session->GetObj(pCommunity,GroupId,false,false);
		if(grp)
			grp->InsertObj(this);
	}
}


//------------------------------------------------------------------------------
int GProfile::Compare(const GProfile& profile) const
{
	return(CompareIds(Id,profile.Id));
}


//------------------------------------------------------------------------------
int GProfile::Compare(const GProfile* profile) const
{
	return(CompareIds(Id,profile->Id));
}


//------------------------------------------------------------------------------
int GProfile::Compare(const RString& name) const
{
	return(Name.Compare(name));
}


//------------------------------------------------------------------------------
int GProfile::Compare(const size_t id) const
{
 	return(CompareIds(Id,id));
}


//------------------------------------------------------------------------------
void GProfile::ClearInfos(bool disk)
{
	// Clear the information
	GDescriptionObject<GProfile>::Clear(disk);

	// Make sure that it will be re-computed
	Computed=RDate::Null;
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
	if(Fdbks.GetNb()&&(Updated>Computed))
		return(true);
	return(false);
}


//------------------------------------------------------------------------------
void GProfile::SetGroup(size_t groupid)
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
void GProfile::SetConfidence(double score,char level)
{
	Score=score;
	Level=level;
}


//------------------------------------------------------------------------------
double GProfile::GetAgreementRatio(const GProfile* prof,size_t nbmin) const
{
	double nbcommon(0.0), nbagree(0.0);

	// Go through the document assessed by the profile
	RCursor<GFdbk> fdbks(GetFdbks());
	for(fdbks.Start();!fdbks.End();fdbks.Next())
	{
		// Look for the same document in the other profile.
		GFdbk* f1(fdbks());
		GFdbk* f2(prof->GetFdbk(f1->GetDocId()));
		if(!f2) continue;

		nbcommon+=1.0;  // A common document
		if((f1->GetFdbk()==ftRelevant)&&(f2->GetFdbk()==ftRelevant))
			nbagree+=1.0;
	}
	if(nbcommon<nbmin)
		return(0.0);
	return(nbagree/nbcommon);
}


//------------------------------------------------------------------------------
double GProfile::GetDisagreementRatio(const GProfile* prof,size_t nbmin) const
{
	double nbcommon(0.0), nbdisagree(0.0);

	// Go through the document assessed by the profile
	RCursor<GFdbk> fdbks(GetFdbks());
	for(fdbks.Start();!fdbks.End();fdbks.Next())
	{
		// Look for the same document in the other profile.
		GFdbk* f1(fdbks());
		GFdbk* f2(prof->GetFdbk(f1->GetDocId()));
		if(!f2) continue;

		nbcommon+=1.0;  // A common document
		if(((f1->GetFdbk()==ftRelevant)&&(f2->GetFdbk()!=ftRelevant))||((f1->GetFdbk()!=ftRelevant)&&(f2->GetFdbk()==ftRelevant)))
			nbdisagree+=1.0;
	}
	if(nbcommon<nbmin)
		return(0.0);
	return(nbdisagree/nbcommon);
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
bool GProfile::InsertFdbk(size_t docid,tFdbkType fdbk,const R::RDate& done)
{
	if(fdbk==ftUnknown)
		ThrowGException("Cannot add an unknown feedback to profile +'"+RString::Number(Id)+"' for document '"+RString::Number(docid)+"'");

	bool NewFdbk(false);

	// Create only one feedback per document and it is is latest
	GFdbk* Fdbk(Fdbks.GetInsertPtr(docid));
	if(Fdbk->Fdbk==ftUnknown)
		NewFdbk=true;
	Fdbk->Fdbk=fdbk;
	if(done>Fdbk->Done)
		Fdbk->Done=done;

	// If the feedback was made after the last computation, the profile is
	// considered as to be updated
	if(Computed<done)
	{
		Updated.SetToday();
		State=osModified;
	}

	return(NewFdbk);
}


//------------------------------------------------------------------------------
void GProfile::DeleteFdbk(size_t docid)
{
	GFdbk* fdbk(Fdbks.GetPtr<size_t>(docid));
	if(!fdbk)
		return;

	Fdbks.DeletePtr(*fdbk);
	State=osModified;

	// Profile is considered as updated
	Updated.SetToday();
}


//------------------------------------------------------------------------------
void GProfile::Update(GSession* session,GDescription& desc,bool delref)
{
	// Remove its references
	if(delref)
	{
		DelRefs(Session,otProfile);
		if(Session->DoCreateIndex(pProfile))
			session->UpdateIndex(pProfile,desc,Id,false);
	}

	// Assign information
	State=osUpdated;
	Computed.SetToday();
	GDescription::operator=(desc);

	// Update the group were it belongs
	Session->UpdateCommunity(this);

	// Update its references
	AddRefs(Session,otProfile);
	if(Session->DoCreateIndex(pProfile))
		session->UpdateIndex(pProfile,desc,Id,true);

	desc.Clear(); // Clear the description

	// Emit an event that it was modified
	Emit(GEvent::eObjModified);
}


//------------------------------------------------------------------------------
void GProfile::ClearFdbks(void)
{
	Fdbks.Clear();
}


//------------------------------------------------------------------------------
GFdbk* GProfile::GetFdbk(size_t docid) const
{
	return(Fdbks.GetPtr(docid));
}


//------------------------------------------------------------------------------
void GProfile::WasUpdated(size_t docid)
{
	GFdbk* fdbk=Fdbks.GetPtr(docid);
	if(fdbk)
		Updated.SetToday();
}


//------------------------------------------------------------------------------
GProfile::~GProfile(void)
{
	// Remove it from its group if necessary
	if(GroupId)
	{
		GCommunity* grp=Session->GetObj(pCommunity,GroupId);
		if(grp)
			grp->DeleteObj(this);
	}
}
