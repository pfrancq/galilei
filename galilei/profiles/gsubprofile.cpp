/*

	GALILEI Research Project

	GSubProfile.cpp

	Subprofile - Implementation.

	Copyright 2001-2003 by the Université Libre de Bruxelles.

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



//------------------------------------------------------------------------------
// include files for GALILEI
#include <profiles/gsubprofile.h>
#include <profiles/gprofile.h>
#include <profiles/gprofdoc.h>
#include <docs/gdoc.h>
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
  :  Id(id), Profile(prof), Lang(lang), Group(grp), Attached(a), Updated(u), Computed(c), Fdbks(20,10)
{

	#if GALILEITEST
		Subject=0;
	#endif
	if(Updated>Computed)
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
  :  Id(cNoRef), Profile(prof), Lang(lang), Group(0), State(osCreated), Attached(""), Updated(""), Computed(""), Fdbks(20,10)
{
	#if GALILEITEST
		Subject=0;
	#endif
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
void GSubProfile::AddAssessment(GProfDoc* j) throw(std::bad_alloc)
{
	Fdbks.InsertPtr(j);

	// The profile is updated only if a document assessed was updated later
	if(j->GetDoc()->GetUpdated()>Updated)
	{
		State=osModified;
		Updated.SetToday();
	}
}


//------------------------------------------------------------------------------
void GSubProfile::RemoveAssessment(GProfDoc* j) throw(std::bad_alloc)
{
	Fdbks.DeletePtr(j);

	// When an assessment is removed -> the profile is always updated
	State=osModified;
	Updated.SetToday();
}


//------------------------------------------------------------------------------
void GSubProfile::ClearFdbks(void) throw(std::bad_alloc)
{
	Fdbks.Clear();
}


//------------------------------------------------------------------------------
void GSubProfile::SetId(unsigned int id) throw(GException)
{
	if(Id==cNoRef)
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
	return(false);
}


//------------------------------------------------------------------------------
void GSubProfile::SetGroup(GGroup* grp)
{
	Group=grp;
	if(grp)
		Attached.SetToday();
}


//------------------------------------------------------------------------------
RDate& GSubProfile::GetAttached(void) const
{
	RDate* d=RDate::GetDate();

	(*d)=Attached;
	return(*d);
}


//------------------------------------------------------------------------------
RDate& GSubProfile::GetUpdated(void) const
{
	RDate* d=RDate::GetDate();

	(*d)=Updated;
	return(*d);
}


//------------------------------------------------------------------------------
R::RDate& GSubProfile::GetComputed(void) const
{
	RDate* d=RDate::GetDate();

	(*d)=Computed;
	return(*d);
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
	GProfDoc* cor;
	unsigned int nb;

	// Verify that the two profile have the same language
	if(Lang!=prof->Lang) return(0);
	nb=0;

	// Go through the document judged by the corresponding profile
	GProfDocCursor fdbks=GetProfDocCursor();
	for(fdbks.Start();!fdbks.End();fdbks.Next())
	{
		// If the document is not "good"  -> Nothing
		f=fdbks()->GetFdbk();
		if(!(f & djOK)) continue;
		// Look for the same document in the other profile. If not found or the
		// document is not "good" -> Nothing
		cor=prof->GetFeedback(fdbks()->GetDoc());
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
	GProfDoc* cor;
	unsigned int nb;

	// Verify that the two profile have the same language
	if(Lang!=prof->Lang) return(0);
	nb=0;

	// Go through the document judged by the corresponding profile
	GProfDocCursor Fdbks=GetProfDocCursor();
	for(Fdbks.Start();!Fdbks.End();Fdbks.Next())
	{
		f=Fdbks()->GetFdbk();
		// Look for the same document in the other profile. If not found or the
		// document is not "good" -> Nothing
		cor=prof->GetFeedback(Fdbks()->GetDoc());
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
	GProfDoc* cor;
	unsigned int nb;
	bool bOK,bOK2;

	// Verify that the two profile have the same language
	if(Lang!=prof->Lang) return(0);
	nb=0;

	// Go through the document judged by the corresponding profile
	GProfDocCursor Fdbks=GetProfDocCursor();
	for(Fdbks.Start();!Fdbks.End();Fdbks.Next())
	{
		f=Fdbks()->GetFdbk();
		bOK=(f & djOK);

		// If the document was not judged by the other profile or have not the
		// same judgment -> Nothing
		cor=prof->GetFeedback(Fdbks()->GetDoc());
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
GProfDocCursor& GSubProfile::GetProfDocCursor(void)
{
	GProfDocCursor *cur=GProfDocCursor::GetTmpCursor();
	cur->Set(Fdbks);
	return(*cur);
}


//------------------------------------------------------------------------------
GProfDoc* GSubProfile::GetFeedback(const GDoc* doc) const
{
	return(Fdbks.GetPtr<const GDoc*>(doc));
}


//------------------------------------------------------------------------------
double GSubProfile::Similarity(const GDoc*) const
{
	return(0.0);
}


//------------------------------------------------------------------------------
double GSubProfile::SimilarityIFF(const GDoc*) const throw(GException)
{
	return(0.0);
}


//------------------------------------------------------------------------------
double GSubProfile::Similarity(const GSubProfile*) const
{
	return(0.0);
}


//------------------------------------------------------------------------------
double GSubProfile::SimilarityIFF(const GSubProfile*) const throw(GException)
{
	return(0.0);
}


//------------------------------------------------------------------------------
double GSubProfile::Similarity(const GGroup*) const
{
	return(0.0);
}


//------------------------------------------------------------------------------
double GSubProfile::SimilarityIFF(const GGroup*) const throw(GException)
{
	return(0.0);
}


#if GALILEITEST
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
#endif


//------------------------------------------------------------------------------
void GSubProfile::UpdateFinished(void)
{
	State=osUpdated;
	Computed.SetToday();
}


//------------------------------------------------------------------------------
GSubProfile::~GSubProfile(void) throw(GException)
{
}
