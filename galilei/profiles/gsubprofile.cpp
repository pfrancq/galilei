/*

	GALILEI Research Project

	GSubProfile.cpp

	Subprofile - Implementation.

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
// include files for GALILEI
#include <gsubprofile.h>
#include <gprofile.h>
#include <gdoc.h>
#include <gweightinfo.h>
#include <glang.h>
#include <ggroup.h>
#include <gsession.h>
#include <gstorage.h>
using namespace GALILEI;
using namespace R;



//------------------------------------------------------------------------------
//
//  GSubProfile
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GSubProfile::GSubProfile(GProfile *prof,unsigned int id,GLang *lang,unsigned int grpid,R::RDate a,R::RDate u,R::RDate c)
	: GWeightInfos(lang,60), Id(id), Profile(prof), GroupId(grpid), Attached(a), Updated(u), Computed(c), Fdbks(20,10)
{
	if(!Profile)
		throw GException("Subprofile "+RString::Number(id)+" has no parent profile");
	if((Id==cNoRef)&&(GSession::Get()))
		GSession::Get()->AssignId(this);
	Profile->InsertPtr(this);
	if((GroupId!=cNoRef)&&(GSession::Get()))
	{
		GGroup* grp=GSession::Get()->GetGroup(Lang,GroupId,false,false);
		if(grp)
			grp->InsertSubProfile(this);
	}
	GSession::Event(this,eObjNew);
}


//------------------------------------------------------------------------------
int GSubProfile::Compare(const GSubProfile& subprofile) const
{
	return(Id-subprofile.Id);
}


//------------------------------------------------------------------------------
int GSubProfile::Compare(const GSubProfile* subprofile) const
{
	return(Id-subprofile->Id);
}


//------------------------------------------------------------------------------
int GSubProfile::Compare(const unsigned int id) const
{
	return(Id-id);
}


//------------------------------------------------------------------------------
int GSubProfile::Compare(const GLang* lang) const
{
	if(!lang)
		return(1);
	return(Lang->Compare(lang));
}


//------------------------------------------------------------------------------
void GSubProfile::LoadInfos(void) const
{
	RContainer<GWeightInfo,false,true> Infos(1000,500);
	GSession* session=GSession::Get();
	if(session&&session->GetStorage())
		session->GetStorage()->LoadInfos(Infos,otSubProfile,Id);
	const_cast<GSubProfile*>(this)->Update(Lang,&Infos,false);
}


//------------------------------------------------------------------------------
void GSubProfile::InsertFdbk(GFdbk* fdbk)
{
	Fdbks.InsertPtr(fdbk);
	State=osModified;

	// If the document assessed was computed after the last computation
	// -> profile is considered as updated
	if(Computed<fdbk->GetComputed())
		Updated.SetToday();
}


//------------------------------------------------------------------------------
void GSubProfile::SetId(unsigned int id)
{
	if(id==cNoRef)
		throw GException("Cannot assign cNoRef to a subprofile");
	Id=id;
}


//------------------------------------------------------------------------------
void GSubProfile::SetGroup(unsigned int groupid)
{
	GroupId=groupid;
	if(GroupId!=cNoRef)
		Attached.SetToday();	
}


//------------------------------------------------------------------------------
RDate GSubProfile::GetAttached(void) const
{
	return(Attached);
}


//------------------------------------------------------------------------------
RDate GSubProfile::GetUpdated(void) const
{
	return(Updated);
}


//------------------------------------------------------------------------------
RDate GSubProfile::GetComputed(void) const
{
	return(Computed);
}


//------------------------------------------------------------------------------
bool GSubProfile::MustGroup(void) const
{
	return(Attached<Updated);
}


//------------------------------------------------------------------------------
bool GSubProfile::MustCompute(void) const
{
	if(!Fdbks.GetNb())
		return(false);
	if(Updated<Computed)
		return(false);
	return(true);
}


//------------------------------------------------------------------------------
unsigned int GSubProfile::GetCommonOKDocs(const GSubProfile* sub) const
{
	tDocAssessment f;
	GFdbk* cor;
	unsigned int nb;

	// Verify that the two profile have the same language
	if(Lang!=sub->Lang) return(0);
	nb=0;

	// Go through the document judged by the corresponding profile
	RCursor<GFdbk> fdbks=GetFdbks();
	for(fdbks.Start();!fdbks.End();fdbks.Next())
	{
		// If the document is not "good"  -> Nothing
		f=fdbks()->GetFdbk();
		if(!(f & djOK)) continue;
		// Look for the same document in the other profile. If not found or the
		// document is not "good" -> Nothing
		cor=sub->GetProfile()->GetFdbk(fdbks()->GetDocId());
		if(!cor) continue;
		f=cor->GetFdbk();
		if(!(f & djOK)) continue;

		// Increase the number of common documents
		nb++;
	}
	return(nb);
}


//------------------------------------------------------------------------------
unsigned int GSubProfile::GetCommonDocs(const GSubProfile* sub) const
{
	tDocAssessment f;
	GFdbk* cor;
	unsigned int nb;

	// Verify that the two profile have the same language
	if(Lang!=sub->Lang) return(0);
	nb=0;

	// Go through the document judged by the corresponding profile
	RCursor<GFdbk> Fdbks=GetFdbks();
	for(Fdbks.Start();!Fdbks.End();Fdbks.Next())
	{
		f=Fdbks()->GetFdbk();
		// Look for the same document in the other profile. If not found or the
		// document is not "good" -> Nothing
		cor=sub->GetProfile()->GetFdbk(Fdbks()->GetDocId());
		if(!cor) continue;
		f=cor->GetFdbk();

		// Increase the number of common documents
		nb++;
	}
	return(nb);
}


//------------------------------------------------------------------------------
unsigned int GSubProfile::GetCommonDiffDocs(const GSubProfile* sub) const
{
	tDocAssessment f;
	GFdbk* cor;
	unsigned int nb;
	bool bOK,bOK2;

	// Verify that the two profile have the same language
	if(Lang!=sub->Lang) return(0);
	nb=0;

	// Go through the document judged by the corresponding profile
	RCursor<GFdbk> Fdbks=GetFdbks();
	for(Fdbks.Start();!Fdbks.End();Fdbks.Next())
	{
		f=Fdbks()->GetFdbk();
		bOK=(f & djOK);

		// If the document was not judged by the other profile or have not the
		// same judgment -> Nothing
		cor=sub->GetProfile()->GetFdbk(Fdbks()->GetDocId());
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
RCursor<GFdbk> GSubProfile::GetFdbks(void) const
{
	return(RCursor<GFdbk>(Fdbks));
}


//------------------------------------------------------------------------------
void GSubProfile::Update(GLang* lang,R::RContainer<GWeightInfo,false,true>* infos,bool computed)
{
	// Remove its references
	if(computed&&Lang)
		DelRefs(otSubProfile);

	// Assign information
	GWeightInfos::Clear();
	Lang=lang;
	if(computed)
	{
		State=osUpdated;
		Computed.SetToday();

		// Update the group were it belongs
		if(GSession::Get())
			GSession::Get()->UpdateGroups(Id);
	}
	else
		State=osUpToDate;
	CopyInfos(infos);

	// Clear infos
	infos->Clear();

	// Update its references
	if(computed&&Lang)
		AddRefs(otSubProfile);

	// Emit an event that it was modified
	if(computed)
		GSession::Event(this,eObjModified);
}


//------------------------------------------------------------------------------
void GSubProfile::ClearFdbks(void)
{
	// Remove all assessments
	Fdbks.Clear();
}


//------------------------------------------------------------------------------
GSubProfile::~GSubProfile(void)
{
	GSession::Event(this,eObjDelete);
	try
	{
		// Remove it from its group if necessary
		if((GroupId!=cNoRef)&&(GSession::Get()))
		{
			GGroup* grp=GSession::Get()->GetGroup(Lang,GroupId);
			if(grp)
				grp->DeleteSubProfile(this);
		}

		// Remove its references
		if(Lang&&(State==osDelete))  // The object has modified the references count but was not saved
			DelRefs(otSubProfile);
	}
	catch(...)
	{
	}
}
