/*

	GALILEI Research Project

	GSubProfile.cpp

	Sub-Profile - Implementation.

	Copyright 2001 by the Université Libre de Bruxelles.

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
// include files for GALILEI
#include<profiles/gsubprofile.h>
#include<profiles/gprofile.h>
#include<profiles/gprofdoc.h>
#include<infos/glang.h>
#include<groups/ggroup.h>

using namespace GALILEI;
using namespace R;



//-----------------------------------------------------------------------------
//
//  GSubProfile
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GALILEI::GSubProfile::GSubProfile(GProfile *prof,unsigned int id,GLang *lang,GGroup* grp,const char* a,tObjState state, const char* c) throw(bad_alloc)
  :  Id(id), Profile(prof), Lang(lang), Group(grp), State(state), Attached(a),  Computed(c), Fdbks(20,10)
{

#if GALILEITEST
		Subject=0;
	#endif
	Profile->InsertPtr(this);
	if(grp)
		grp->InsertPtr(this);
}


//-----------------------------------------------------------------------------
int GALILEI::GSubProfile::Compare(const unsigned int id) const
{
	return(Id-id);
}


//-----------------------------------------------------------------------------
int GALILEI::GSubProfile::Compare(const GLang* lang) const
{
	return(Lang->Compare(lang));
}


//-----------------------------------------------------------------------------
int GALILEI::GSubProfile::Compare(const GSubProfile& subprofile) const
{
	return(Id-subprofile.Id);
}


//-----------------------------------------------------------------------------
int GALILEI::GSubProfile::Compare(const GSubProfile* subprofile) const
{
	return(Id-subprofile->Id);
}


//-----------------------------------------------------------------------------
void GALILEI::GSubProfile::AddJudgement(GProfDoc* j) throw(bad_alloc)
{
	Fdbks.InsertPtr(j);
	State=osModified;
}


//-----------------------------------------------------------------------------
void GALILEI::GSubProfile::RemoveJudgement(GProfDoc* j) throw(bad_alloc)
{
	Fdbks.DeletePtr(j);
	State=osModified;
}


//-----------------------------------------------------------------------------
void GALILEI::GSubProfile::ClearFdbks(void)
{
	Fdbks.Clear();
}


//-----------------------------------------------------------------------------
void GALILEI::GSubProfile::SetState(tObjState state)
{
	GGroup* grp;
	State=state;

	// If the profile was updated -> the groups containing the subprofiles
	// are modified.
	if(State==osUpdated)
	{
		grp=GetGroup();
		if(grp)
			grp->SetState(osModified);
	}
}

//-----------------------------------------------------------------------------
void GALILEI::GSubProfile::SetGroup(GGroup* grp)
{
	Group=grp;
	if(grp)
		Attached.SetToday();
}


//-----------------------------------------------------------------------------
bool GALILEI::GSubProfile::IsUpdated(void) const
{
	return(Attached<Updated);
}


//-----------------------------------------------------------------------------
unsigned int GALILEI::GSubProfile::GetCommonOKDocs(const GSubProfile* prof)
{
	tDocJudgement f;
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


//-----------------------------------------------------------------------------
unsigned int GALILEI::GSubProfile::GetCommonDocs(const GSubProfile* prof)
{
	tDocJudgement f;
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


//-----------------------------------------------------------------------------
unsigned int GALILEI::GSubProfile::GetCommonDiffDocs(const GSubProfile* prof)
{
	tDocJudgement f;
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


//-----------------------------------------------------------------------------
GProfDocCursor& GALILEI::GSubProfile::GetProfDocCursor(void)
{
	GProfDocCursor *cur=GProfDocCursor::GetTmpCursor();
	cur->Set(Fdbks);
	return(*cur);
}


//-----------------------------------------------------------------------------
GProfDoc* GALILEI::GSubProfile::GetFeedback(const GDoc* doc) const
{
	return(Fdbks.GetPtr<const GDoc*>(doc));
}


//-----------------------------------------------------------------------------
double GALILEI::GSubProfile::Similarity(const GDoc*) const
{
	return(0.0);
}


//-----------------------------------------------------------------------------
double GALILEI::GSubProfile::GlobalSimilarity(const GDoc*) const
{
	return(0.0);
}


//-----------------------------------------------------------------------------
double GALILEI::GSubProfile::Similarity(const GSubProfile*) const
{
	return(0.0);
}


//-----------------------------------------------------------------------------
double GALILEI::GSubProfile::GlobalSimilarity(const GSubProfile*) const
{
	return(0.0);
}


//-----------------------------------------------------------------------------
double GALILEI::GSubProfile::Similarity(const GGroup*) const
{
	return(0.0);
}


//-----------------------------------------------------------------------------
double GALILEI::GSubProfile::GlobalSimilarity(const GGroup*) const
{
	return(0.0);
}

#if GALILEITEST
//-----------------------------------------------------------------------------
void GALILEI::GSubProfile::SetSubject(GSubject* s)
{
	Subject=s;
}


//-----------------------------------------------------------------------------
GSubject* GALILEI::GSubProfile::GetSubject(void) const
{
	return(Subject);
}
#endif


//-----------------------------------------------------------------------------
void GALILEI::GSubProfile::UpdateFinished(void)
{
	State=osUpdated;
	Computed.SetToday();
}


//-----------------------------------------------------------------------------
GALILEI::GSubProfile::~GSubProfile(void)
{
}
