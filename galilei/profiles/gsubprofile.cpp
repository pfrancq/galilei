/*

	GALILEI Research Project

	GSubProfile.cpp

	Subprofile - Implementation.

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
// include files for GALILEI
#include <profiles/gsubprofile.h>
#include <profiles/gprofile.h>
#include <docs/gdocproxy.h>
#include <infos/gweightinfo.h>
#include <infos/glang.h>
#include <groups/ggroup.h>
#include <sessions/gsession.h>
using namespace GALILEI;
using namespace R;



//------------------------------------------------------------------------------
//
//  GSubProfile
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GSubProfile::GSubProfile(GProfile *prof,unsigned int id,GLang *lang,GGroup* grp,const char* a,const char* u, const char* c) throw(std::bad_alloc)
	: GWeightInfos(60), Id(id), Profile(prof), Lang(lang), Group(grp), Attached(a), Updated(u), Computed(c), Fdbks(20,10)
{
	Subject=0;
	if(Updated>=Computed)
	{
		if(Computed==RDate::null)
			State=osCreated;
		else
			State=osModified;
	}
	else
		State=osUpToDate;
	Profile->InsertPtr(this);
	if(grp)
		grp->InsertSubProfile(this);
}


//------------------------------------------------------------------------------
GSubProfile::GSubProfile(GSession* session,GProfile *prof,GLang *lang) throw(std::bad_alloc)
	: GWeightInfos(60), Id(cNoRef), Profile(prof), Lang(lang), Group(0), State(osCreated), Attached(""), Updated(""), Computed(""), Fdbks(20,10)
{
	Subject=0;
	session->AssignId(this);
	Profile->InsertPtr(this);
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
	return(Lang->Compare(lang));
}


//------------------------------------------------------------------------------
void GSubProfile::InsertFdbk(GFdbk* fdbk) throw(std::bad_alloc)
{
	Fdbks.InsertPtr(fdbk);
}


//------------------------------------------------------------------------------
/*void GSubProfile::InsertFdbk(GFdbk* j) throw(std::bad_alloc)
{
	Fdbks.InsertPtr(j);

	// The profile is modified only if a assessment was made later
	if(j->GetUpdated()>=Updated)
	{
		State=osModified;
		Updated.SetToday();
	}
}


//------------------------------------------------------------------------------
void GSubProfile::DeleteFdbk(GFdbk* j) throw(std::bad_alloc)
{
	Fdbks.DeletePtr(j);

	// When an assessment is removed -> the profile is always modified
	State=osModified;
	Updated.SetToday();
}*/


//------------------------------------------------------------------------------
void GSubProfile::ClearFdbks(void) throw(std::bad_alloc,GException)
{
	RemoveRefs();
	GWeightInfos::Clear();

	// When all assessments are removed -> the profile is always modified
	Fdbks.Clear();
	State=osModified;
	Updated.SetToday();
}


//------------------------------------------------------------------------------
void GSubProfile::SetId(unsigned int id) throw(GException)
{
	if(id==cNoRef)
		throw GException("Cannot assign cNoRef to a subprofile");
	Id=id;
}


//------------------------------------------------------------------------------
void GSubProfile::SetState(tObjState state)
{
	GGroup* grp;
	State=state;

	// If the profile was updated
	//   -> the group containing the subprofile is modified.
	//   -> the subprofile was just computed
	if(State==osUpdated)
	{
		grp=GetGroup();
		if(grp)
			grp->SetState(osModified);
	}
}


//------------------------------------------------------------------------------
bool GSubProfile::IsDefined(void) const
{
	return(NbPtr);
}


//------------------------------------------------------------------------------
void GSubProfile::SetGroup(GGroup* grp)
{
	Group=grp;
	if(grp)
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
bool GSubProfile::IsUpdated(void) const
{
	return(Attached<Updated);
}


//------------------------------------------------------------------------------
unsigned int GSubProfile::GetCommonOKDocs(const GSubProfile* prof)
{
	tDocAssessment f;
	GFdbk* cor;
	unsigned int nb;

	// Verify that the two profile have the same language
	if(Lang!=prof->Lang) return(0);
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
		cor=prof->GetProfile()->GetFdbk(fdbks()->GetDoc()->GetId());
		if(!cor) continue;
		f=cor->GetFdbk();
		if(!(f & djOK)) continue;

		// Increase the number of common documents
		nb++;
	}
	return(nb);
}


//------------------------------------------------------------------------------
unsigned int GSubProfile::GetCommonDocs(const GSubProfile* prof)
{
	tDocAssessment f;
	GFdbk* cor;
	unsigned int nb;

	// Verify that the two profile have the same language
	if(Lang!=prof->Lang) return(0);
	nb=0;

	// Go through the document judged by the corresponding profile
	RCursor<GFdbk> Fdbks=GetFdbks();
	for(Fdbks.Start();!Fdbks.End();Fdbks.Next())
	{
		f=Fdbks()->GetFdbk();
		// Look for the same document in the other profile. If not found or the
		// document is not "good" -> Nothing
		cor=prof->GetProfile()->GetFdbk(Fdbks()->GetDoc()->GetId());
		if(!cor) continue;
		f=cor->GetFdbk();

		// Increase the number of common documents
		nb++;
	}
	return(nb);
}


//------------------------------------------------------------------------------
unsigned int GSubProfile::GetCommonDiffDocs(const GSubProfile* prof)
{
	tDocAssessment f;
	GFdbk* cor;
	unsigned int nb;
	bool bOK,bOK2;

	// Verify that the two profile have the same language
	if(Lang!=prof->Lang) return(0);
	nb=0;

	// Go through the document judged by the corresponding profile
	RCursor<GFdbk> Fdbks=GetFdbks();
	for(Fdbks.Start();!Fdbks.End();Fdbks.Next())
	{
		f=Fdbks()->GetFdbk();
		bOK=(f & djOK);

		// If the document was not judged by the other profile or have not the
		// same judgment -> Nothing
		cor=prof->GetProfile()->GetFdbk(Fdbks()->GetDoc()->GetId());
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
RCursor<GFdbk> GSubProfile::GetFdbks(void)
{
	RCursor<GFdbk> cur(Fdbks);
	return(cur);
}


//------------------------------------------------------------------------------
double GSubProfile::Similarity(const GDoc* doc) const
{
	return(GWeightInfos::Similarity(doc));
}


//------------------------------------------------------------------------------
double GSubProfile::SimilarityIFF(const GDoc* doc) const throw(GException)
{
	return(GWeightInfos::SimilarityIFF(doc,otDocSubProfile,Lang));
}


//------------------------------------------------------------------------------
double GSubProfile::Similarity(const GDocProxy* doc) const
{
	return(GWeightInfos::Similarity(doc->GetInfos()));
}


//------------------------------------------------------------------------------
double GSubProfile::SimilarityIFF(const GDocProxy* doc) const throw(GException)
{
	return(GWeightInfos::SimilarityIFF(doc->GetInfos(),otDocSubProfile,Lang));
}


//------------------------------------------------------------------------------
double GSubProfile::Similarity(const GSubProfile* sub) const
{
	return(GWeightInfos::Similarity(sub));
}


//------------------------------------------------------------------------------
double GSubProfile::SimilarityIFF(const GSubProfile* sub) const throw(GException)
{
	return(GWeightInfos::SimilarityIFF(sub,otSubProfile,Lang));
}


//------------------------------------------------------------------------------
double GSubProfile::Similarity(const GGroup* grp) const
{
	return(GWeightInfos::Similarity(grp));
}


//------------------------------------------------------------------------------
double GSubProfile::SimilarityIFF(const GGroup* grp) const throw(GException)
{
	return(GWeightInfos::SimilarityIFF(grp,otSubProfileGroup,Lang));
}


//------------------------------------------------------------------------------
GWeightInfoCursor GSubProfile::GetWeightInfoCursor(void)
{
	GWeightInfoCursor cur(this);
	return(cur);
}


//------------------------------------------------------------------------------
unsigned int GSubProfile::GetNbNoNull(void) const
{
	return(NbPtr);
}


//------------------------------------------------------------------------------
void GSubProfile::UpdateRefs(void) const throw(GException)
{
	AddRefs(otSubProfile,Lang);
}


//------------------------------------------------------------------------------
void GSubProfile::RemoveRefs(void) const throw(GException)
{
	DelRefs(otSubProfile,Lang);
}


//------------------------------------------------------------------------------
void GSubProfile::SetSubject(GSubject* s)
{
	Subject=s;
}


//------------------------------------------------------------------------------
GSubject* GSubProfile::GetSubject(void) const
{
	return(Subject);
}


//------------------------------------------------------------------------------
void GSubProfile::UpdateFinished(void)
{
	State=osUpdated;
	Computed.SetToday();
}


//------------------------------------------------------------------------------
GSubProfile::~GSubProfile(void)
{
	try
	{
		RemoveRefs();
	}
	catch(...)
	{
	}
}
