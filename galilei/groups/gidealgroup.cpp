/*

	GALILEI Research Project

	GIdealGroup.cpp

	Get the jugments for the profiles

	Copyright 2002 by the Université Libre de Bruxelles.

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
// inluce files for R-Project
#include <rio/rtextfile.h>
using namespace RIO;
#include <rstd/rcontainer.h>
using namespace RStd;


//-----------------------------------------------------------------------------
//include specific files
#include <iostream.h>
#include <stdlib.h>


//-----------------------------------------------------------------------------
// includes files for GALILEI
#include <langs/glang.h>
#include <infos/giwordlist.h>
#include <profiles/guser.h>
#include <profiles/gprofile.h>
#include <profiles/gsubprofile.h>
#include <profiles/gsubprofilevector.h>
#include <profiles/gprofdoc.h>
#include <docs/gdoc.h>
#include <docs/gdocs.h>
#include <groups/gsubject.h>
#include <groups/gidealgroup.h>
#include <groups/ggroupvector.h>
#include <groups/ggroups.h>
#include <groups/gsubjecttree.h>
#include <sessions/gsession.h>
#include <profiles/gusers.h>
#include <groups/gsubject.h>
#include <tests/ggroupsevaluate.h>
using namespace GALILEI;


//-----------------------------------------------------------------------------
//
//  GIdealGroup
//
//-----------------------------------------------------------------------------

GALILEI::GIdealGroup::GIdealGroup(GSession* session)
	: Session(session), Docs(0), NbDocs(0)
{
	RTimeDate::RDate date;

	PercOK=10;
	PercKO=10;
	PercHS=50;
	PercErr=0;
	NbProfMax=10;
	NbProfMin=2;
	PercSocial=100;
	PercGrp=100;
	NbDocPerGrp=50;
	sprintf(today,"%u-%u-%u",date.GetYear(),date.GetMonth(),date.GetDay());
}


//-----------------------------------------------------------------------------
void GALILEI::GIdealGroup::Run(bool Save)
{
	if(!Docs)
		Docs=new GDoc*[Session->GetNbDocs()];
	ChooseSubjects();
	CreateSet();
	if(Save)
	{ 
		Session->SaveFdbks();  
		Session->SaveIdealGroupment(Session->GetIdealGroups());
	}
}


//-----------------------------------------------------------------------------
void GALILEI::GIdealGroup::ChooseSubjects(void)
{
	GSubjectCursor Subs;
	unsigned int compt;
	GSubject** tab;
	GSubject** ptr;
	unsigned int i;
	GSubjectTree* Subjects;

	// Suppose all subjects are not chosen
	Subjects=Session->GetSubjects();
	Subs.Set(Subjects);
	for(Subs.Start();!Subs.End();Subs.Next())
		Subs()->SetUsed(false);

	// Randomly mix the subjects in tab
	tab=new GSubject*[Subjects->NbPtr];
	memcpy(tab,Subjects->Tab,sizeof(GSubject*)*(Subjects->NbPtr));
	Session->GetRandom()->RandOrder<GSubject*>(tab,Subjects->NbPtr);

	// Choose the first percgrp subjects having at least NbDocPerGrp documents.
	for(ptr=tab,i=Subjects->NbPtr+1,compt=static_cast<unsigned int>((Subjects->NbPtr*PercGrp)/100)+1;(--i)&&compt;ptr++)
	{
		if((*ptr)->GetNbDocs()<NbDocPerGrp) continue;
		(*ptr)->SetUsed(true);
		compt--;
	}

	// delete tab;
	delete[] tab;
}


//-----------------------------------------------------------------------------
void GALILEI::GIdealGroup::CreateSet(void)
{
	GSubjectCursor Subs;
	GSubProfileCursor Prof;
	unsigned int nbprof,nbsocial;
	RStd::RContainer<GGroups,unsigned int,true,true>* IdealGroups;
	GGroups* CurGrps;
	GGroup* Grp;
	unsigned int maxDocsOK,maxDocsKO,maxDocsH;
	GLangCursor CurLang;

	// Init Part
	IdealGroups=Session->GetIdealGroups();
	IdealGroups->Clear();
	CurLang=Session->GetLangsCursor();
	for(CurLang.Start();!CurLang.End();CurLang.Next())
		IdealGroups->InsertPtr(new GGroups(CurLang()));
	if(!Docs)
		Docs=new GDoc*[Session->GetNbDocs()];
	Session->ClearFdbks();
	Session->ClearSubProfilesGroups();

	// Go through all the subjects which are used
	Subs.Set(Session->GetSubjects());
	for(Subs.Start();!Subs.End();Subs.Next())
	{
		if(!Subs()->IsUsed()) continue;

		// Create an ideal group for this subject
		CurGrps=IdealGroups->GetPtr<const GLang*>(Subs()->GetLang());
		CurGrps->InsertPtr(Grp=new GGroupVector(CurGrps->NbPtr,Subs()->GetLang()));

		// Copy the documents of the same language of the session in Docs;
		NbDocs=Session->FillDocs(Docs,Subs()->GetLang());

		// Number of subprofiles that will judge documents
		if(NbProfMax>Subs()->GetNbSubProfiles())
			NbProfMax=Subs()->GetNbSubProfiles();
		if(NbProfMin>Subs()->GetNbSubProfiles())
			NbProfMin=Subs()->GetNbSubProfiles();
		nbprof=Session->GetCurrentRandomValue(NbProfMax-NbProfMin+1)+NbProfMin;

		// Number of profiles that are social
		nbsocial=static_cast<unsigned int>(nbprof*PercSocial/100)+1;

		// Number of documents to judged by each subprofile
		maxDocsOK=static_cast<unsigned int>(Subs()->GetNbDocs()*PercOK/100);
		maxDocsKO=static_cast<unsigned int>(Subs()->GetNbDocs()*PercKO/100);
		maxDocsH=static_cast<unsigned int>(maxDocsOK*PercHS/100);

		// Go through the nbprof first subprofiles attached to the subject
		Prof=Subs()->GetSubProfilesCursor();
		for(Prof.Start(),nbprof++;(!Prof.End())&&(--nbprof);Prof.Next())
		{
			// Look if current subprofile is social or not
			if(nbsocial)
			{
				Prof()->GetProfile()->SetSocial(true);
				nbsocial--;
			}
			else
				Prof()->GetProfile()->SetSocial(false);

			// Insert current subprofile in the ideal group
			Grp->InsertPtr(Prof());

			// Judges documents
			ProfileJudges(Prof()->GetProfile(),Subs(),maxDocsOK,maxDocsKO,maxDocsH);
		}
	}
	cout<<"Ideal"<<IdealGroups->Tab[0]->NbPtr+IdealGroups->Tab[1]->NbPtr<<endl;
}


//-----------------------------------------------------------------------------
void GALILEI::GIdealGroup::ProfileJudges(GProfile* prof,GSubject* sub,unsigned int maxDocsOK,unsigned int maxDocsKO,unsigned int maxDocsH)
{
	unsigned int nbDocsOK,nbDocsKO,nbDocsH;
	unsigned int i;
	GDoc** ptr;

	// Mix the documents
	Session->GetRandom()->RandOrder<GDoc*>(Docs,NbDocs);

	// Go trought the documents to create the judgements
	for(i=NbDocs+1,ptr=Docs,nbDocsOK=maxDocsOK,nbDocsKO=maxDocsKO,nbDocsH=maxDocsH;(--i)&&((nbDocsOK)||(nbDocsKO)||(nbDocsH));ptr++)
	{
		// Look if 'OK'
		if((*ptr)->IsFromSubject(sub))
		{
			if(nbDocsOK)
			{
				nbDocsOK--;
				Session->InsertFdbk(prof,*ptr,GProfDoc::ErrorJudgment(djOK,PercErr,Session->GetRandom()),today);
			}
		}
		else
		{
			// Look If 'KO'
			if((*ptr)->IsFromParentSubject(sub))
			{
				if(nbDocsKO)
				{
					nbDocsKO--;
					Session->InsertFdbk(prof,*ptr,GProfDoc::ErrorJudgment(djKO,PercErr,Session->GetRandom()),today);
				}
			}
			else
			{
				// Must be H
				if(nbDocsH)
				{
					nbDocsH--;
					Session->InsertFdbk(prof,*ptr,GProfDoc::ErrorJudgment(djOutScope,PercErr,Session->GetRandom()),today);
				}
			}
		}
	}
}


//-----------------------------------------------------------------------------
void GALILEI::GIdealGroup::AddJudgement(bool Save)
{
	GSubject** tab;
	GSubject** ptr;
	unsigned int i;
	GSubjectTree* Subjects;
	GSubject* newSubject;
	GSubProfileCursor Prof;
	unsigned int nbprof,nbsocial;
	GGroups* CurGrps;
	GGroup* Grp;
	unsigned int maxDocsOK,maxDocsKO,maxDocsH;

	// Randomly mix the subjects in tab
	Subjects=Session->GetSubjects();
	tab=new GSubject*[Subjects->NbPtr];
	memcpy(tab,Subjects->Tab,sizeof(GSubject*)*(Subjects->NbPtr));
	Session->GetRandom()->RandOrder<GSubject*>(tab,Subjects->NbPtr);

	// Find the first not used subject having at least NbDocPerGrp documents.
	for(ptr=tab,i=Subjects->NbPtr+1,newSubject=0;--i;ptr++)
	{
		if((*ptr)->GetNbDocs()<NbDocPerGrp) continue;
		if((*ptr)->IsUsed()) continue;
		newSubject=(*ptr);
		break;
	}

	// delete tab;
	delete[] tab;

	// If no subject found -> do nothing
	if(!newSubject) return;
	newSubject->SetUsed(true);

	// Create an ideal group for this subject
	CurGrps=Session->GetIdealGroups()->GetPtr<const GLang*>(newSubject->GetLang());
	CurGrps->InsertPtr(Grp=new GGroupVector(CurGrps->NbPtr,newSubject->GetLang()));

	// Copy the documents of the same language of the session in Docs;
	NbDocs=Session->FillDocs(Docs,newSubject->GetLang());

	// Number of subprofiles that will judge documents
	if(NbProfMax>newSubject->GetNbSubProfiles())
		NbProfMax=newSubject->GetNbSubProfiles();
	if(NbProfMin>newSubject->GetNbSubProfiles())
		NbProfMin=newSubject->GetNbSubProfiles();
	nbprof=Session->GetCurrentRandomValue(NbProfMax-NbProfMin+1)+NbProfMin;

	// Number of profiles that are social
	nbsocial=static_cast<unsigned int>(nbprof*PercSocial/100)+1;

	// Number of documents to judged by each subprofile
	maxDocsOK=static_cast<unsigned int>(newSubject->GetNbDocs()*PercOK/100);
	maxDocsKO=static_cast<unsigned int>(newSubject->GetNbDocs()*PercKO/100);
	maxDocsH=static_cast<unsigned int>(maxDocsOK*PercHS/100);

	// Go through the nbprof first subprofiles attached to the subject
	Prof=newSubject->GetSubProfilesCursor();
	for(Prof.Start(),nbprof++;(!Prof.End())&&(--nbprof);Prof.Next())
	{
		// Look if current subprofile is social or not
		if(nbsocial)
		{
			Prof()->GetProfile()->SetSocial(true);
			nbsocial--;
		}
		else
			Prof()->GetProfile()->SetSocial(false);
		
		// Insert current subprofile in the ideal group
		Grp->InsertPtr(Prof());

		// Judges documents
		ProfileJudges(Prof()->GetProfile(),newSubject,maxDocsOK,maxDocsKO,maxDocsH);
	}
	if(Save)
	{
		Session->SaveFdbks();
		Session->SaveIdealGroupment(Session->GetIdealGroups());
	}
}


//-----------------------------------------------------------------------------
const char* GALILEI::GIdealGroup::GetSettings(void)
{
	static char tmp[100];

	sprintf(tmp,"%f %f %f %i %u %u %f %f %f %u",PercOK,PercKO,PercHS,Rand,NbProfMin,NbProfMax,PercSocial,PercErr,PercGrp,NbDocPerGrp);

	return(tmp);
}


//-----------------------------------------------------------------------------
void GALILEI::GIdealGroup::SetSettings(const char* s)
{
	unsigned int a;
	unsigned int b;
	if(!s) return;
	sscanf(s,"%lf %lf %lf %i %u %u %lf %lf %lf %u",&PercOK,&PercKO,&PercHS,&Rand,&a,&b,&PercSocial,&PercErr,&PercGrp,&NbDocPerGrp);
	if (a<=b) NbProfMin=a;
	if (b<=Session->GetNbUsers()) NbProfMax=b;
	if (b==0) NbProfMax=Session->GetNbUsers();

	if(Rand!=0)
		Session->SetCurrentRandom(Rand);
}


//-----------------------------------------------------------------------------
GALILEI::GIdealGroup::~GIdealGroup(void)
{
	if(Docs)
		delete[] Docs;
}
