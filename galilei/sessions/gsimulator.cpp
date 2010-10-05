/*

	GALILEI Research Project

	GSubjects.cpp

	Simulator - Implementation.

	Copyright 2002-2010 by Pascal Francq (pascal@francq.info).
	Copyright 2002-2004 by Julien Lamoral.
	Copyright 2002-2004 by David Wartel.
	Copyright 2002-2008 by the Université Libre de Bruxelles (ULB).

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
#include <gsimulator.h>
#include <gsubject.h>
#include <gdoc.h>
#include <gsession.h>
#include <ggalileiapp.h>
#include <gstorage.h>
#include <glang.h>
#include <gprofile.h>
#include <gmeasure.h>
#include <gcommunity.h>
#include <gtopic.h>
#include <guser.h>
#include <glink.h>
#include <gsuggestion.h>
#include <gsugs.h>
#include <gcommunitycalc.h>
#include <gtopiccalc.h>
#include <gfdbk.h>
using namespace std;
using namespace R;
using namespace GALILEI;



//------------------------------------------------------------------------------
// User Names
static const RString UserNames[]={
		"Pascal Francq","Faiza Gaultier","Marco Saerens","Marjorie Paternostre",
		"David Wartel","Valery Vandaele","Nicolas Kumps","Julien Lamoral",
	    "Sarah Rolfo","Jean-Baptiste Valsamis","Marvin Minsky","John McCarthy",
	    "Edsger W. Dijkstra","Donald E. Knuth","Allen Newell","Herbert A. Simon",
	    "Niklaus Wirth"};



//------------------------------------------------------------------------------
//
// class GSimulator
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GSimulator::GSimulator(GSession* session)
	: Session(session), TmpTopics(0), TmpDocs(0),
	  NewDocs(100), SwitchRandom(0), NewProfiles(0)
{
	ApplyParams();
	Session->Simulator=this;
}


//------------------------------------------------------------------------------
void GSimulator::CreateConfig(RConfig* config)
{
	// Subjects
	config->InsertParam(new RParamValue("MaxDepth",0),"Simulator","Subjects");
	config->InsertParam(new RParamValue("ManualSubjects",false),"Simulator","Subjects");
	config->InsertParam(new RParamValue("Nb",100.0),"Simulator","Subjects","NbSubjects");
	config->InsertParam(new RParamValue("Percentage",true),"Simulator","Subjects","NbSubjects");
	config->InsertParam(new RParamValue("NbMinDocsSubject",50),"Simulator","Subjects");

	// Documents
	config->InsertParam(new RParamValue("Nb",100.0),"Simulator","Documents","DocsPerSubject");
	config->InsertParam(new RParamValue("Percentage",true),"Simulator","Documents","DocsPerSubject");
	config->InsertParam(new RParamValue("MultipleSubjects",false),"Simulator","Documents");
	config->InsertParam(new RParamValue("ClusterSelectedDocs",false),"Simulator","Documents");		 // Parameter of GSession
	config->InsertParam(new RParamValue("SwitchPerc",5.0),"Simulator","Documents");

	// Profiles
	config->InsertParam(new RParamValue("CreateProfiles",true),"Simulator","Profiles");
	config->InsertParam(new RParamValue("NbProfMin",2),"Simulator","Profiles");
	config->InsertParam(new RParamValue("NbProfMax",10),"Simulator","Profiles");
	config->InsertParam(new RParamValue("PercSocial",100.0),"Simulator","Profiles");
	config->InsertParam(new RParamValue("Nb",10.0),"Simulator","Profiles","Relevant");
	config->InsertParam(new RParamValue("Percentage",true),"Simulator","Profiles","Relevant");
	config->InsertParam(new RParamValue("Nb",10.0),"Simulator","Profiles","Fuzzy Relevant");
	config->InsertParam(new RParamValue("Percentage",true),"Simulator","Profiles","Fuzzy Relevant");
	config->InsertParam(new RParamValue("Nb",50.0),"Simulator","Profiles","Irrelevant");
	config->InsertParam(new RParamValue("Percentage",true),"Simulator","Profiles","Irrelevant");
	config->InsertParam(new RParamValue("PercErr",0.0),"Simulator","Profiles");
	config->InsertParam(new RParamValue("NbDocsAssess",30),"Simulator","Profiles");
}


//------------------------------------------------------------------------------
void GSimulator::ApplyParams(void)
{
	// Subjects
	MaxDepth=Session->GetUInt("MaxDepth","Simulator","Subjects");
	ManualSubjects=Session->GetBool("ManualSubjects","Simulator","Subjects");
	NbSubjects=Session->GetDouble("Nb","Simulator","Subjects","NbSubjects");
	RelSubjects=Session->GetBool("Percentage","Simulator","Subjects","NbSubjects");
	NbMinDocsSubject=Session->GetUInt("NbMinDocsSubject","Simulator","Subjects");

	// Documents
	NbDocsPerSubject=Session->GetDouble("Nb","Simulator","Documents","DocsPerSubject");
	PercNbDocsPerSubject=Session->GetBool("Percentage","Simulator","Documents","DocsPerSubject");
	MultipleSubjects=Session->GetBool("MultipleSubjects","Simulator","Documents");
	SwitchPerc=Session->GetDouble("SwitchPerc","Simulator","Documents");

	// Profiles
	CreateProfiles=Session->GetBool("CreateProfiles","Simulator","Profiles");
	NbProfMin=Session->GetUInt("NbProfMin","Simulator","Profiles");
	NbProfMax=Session->GetUInt("NbProfMax","Simulator","Profiles");
	PercSocial=Session->GetDouble("PercSocial","Simulator","Profiles");
	NbOK=Session->GetDouble("Nb","Simulator","Profiles","Relevant");
	RelOK=Session->GetBool("Percentage","Simulator","Profiles","Relevant");
	NbKO=Session->GetDouble("Nb","Simulator","Profiles","Fuzzy Relevant");
	RelKO=Session->GetBool("Percentage","Simulator","Profiles","Fuzzy Relevant");
	NbH=Session->GetDouble("Nb","Simulator","Profiles","Irrelevant");
	RelH=Session->GetBool("Percentage","Simulator","Profiles","Irrelevant");
	PercErr=Session->GetDouble("PercErr","Simulator","Profiles");
	NbDocsAssess=Session->GetUInt("NbDocsAssess","Simulator","Profiles");
}


//------------------------------------------------------------------------------
void GSimulator::StartSimulation(bool create)
{
	// Initialize the session and the subjects
	Session->ReInit();
	if(!TmpDocs)
		TmpDocs=new GDoc*[Session->GetMaxObjectPos(otDoc)+1];

	// Suppose no profiles added
	NewProfiles.Clear();

	// Apply Configuration
	ApplyParams();

	// Select the subjects
	if(create)
	{
		SelectSubjects();
		if(CreateProfiles)
			CreateAssessments();
	}
	else
	{
		// No subject is selected.
		RCursor<GSubject> Subjects(Session->GetSubjects());
		for(Subjects.Start();!Subjects.End();Subjects.Next())
			Subjects()->Used=false;
	}

	// Save the new situation if necessary
	if(Session->MustSaveResults())
	{
		// Clear the storages related to the current situation
		Session->GetStorage()->Clear(otUser);
		Session->GetStorage()->Clear(otProfile);
		Session->Clear(otProfile,otDescFile);
		Session->Clear(otProfile,otIndexFile);
		Session->GetStorage()->Clear(otFdbk);
		Session->GetStorage()->Clear(otCommunity);
		Session->Clear(otCommunity,otDescFile);
		Session->Clear(otCommunity,otIndexFile);
		Session->GetStorage()->Clear(otClass);
		Session->Clear(otClass,otDescFile);
		Session->Clear(otClass,otIndexFile);
		Session->Clear(otTopic,otDescFile);
		Session->Clear(otTopic,otIndexFile);

		// Save the elements changed
		RCursor<GSubject> Subjects(Session->GetSubjects());
		for(Subjects.Start();!Subjects.End();Subjects.Next())
			Session->GetStorage()->SaveSubject(Subjects());
		if(CreateProfiles)
		{
			RCursor<GUser> Users(Session->GetUsers());
			for(Users.Start();!Users.End();Users.Next())
				Session->GetStorage()->SaveUser(Users());
			RCursor<GProfile> Profiles(Session->GetProfiles());
			for(Profiles.Start();!Profiles.End();Profiles.Next())
				Session->GetStorage()->SaveProfile(Profiles());
		}
	}
}


//------------------------------------------------------------------------------
bool GSimulator::AddSubject(void)
{
	GSubject** ptr;
	size_t i;
	GSubject* newSubject;
	RCursor<GProfile> Prof;
	size_t maxDocsOK,maxDocsKO,maxDocsH;

	// Suppose no profiles added
	NewProfiles.Clear();

	// Apply Configuration
	ApplyParams();

	// Randomly mix the subjects in tab
	GSubject** tab(new GSubject*[Session->GetNbObjects(otSubject)]);
	Session->FillSubjects(tab);
	Session->GetRandom().RandOrder<GSubject*>(tab,Session->GetNbObjects(otSubject));

	// Find the first not used subject having at least NbMinDocsSubject documents and the correct depth.
	for(ptr=tab,i=Session->GetNbObjects(otSubject)+1,newSubject=0;--i;ptr++)
	{
		if((*ptr)->GetMaxDocs(MaxDepth)<NbMinDocsSubject) continue;
		if((*ptr)->IsUsed()) continue;
		if(MaxDepth&&((*ptr)->Depth>MaxDepth)) continue;
		newSubject=(*ptr);
		break;
	}

	// delete tab;
	delete[] tab;

	// If no subject found -> do nothing
	if(!newSubject) return(false);

	// Select the subject
	newSubject->Used=true;
	InitSubject(newSubject,true);

	// Copy the selected documents in TmpDocs;
	if(!TmpDocs)
		TmpDocs=new GDoc*[Session->GetMaxObjectPos(otDoc)+1];
	NbTmpDocs=Session->FillSelectedDocs(TmpDocs);

	// Number of documents to judged by each profile
	if(RelOK)
		maxDocsOK=static_cast<size_t>(static_cast<double>(newSubject->GetNbObjs(otDoc))*NbOK/100);
	else
		maxDocsOK=static_cast<size_t>(NbOK);
	if(RelKO)
		maxDocsKO=static_cast<size_t>(static_cast<double>(newSubject->GetNbObjs(otDoc))*NbKO/100);
	else
		maxDocsKO=static_cast<size_t>(NbKO);
	if(RelH)
		maxDocsH=static_cast<size_t>(static_cast<double>(maxDocsOK)*NbH/100);
	else
		maxDocsH=static_cast<size_t>(NbH);

	// Assess documents
	Prof=newSubject->GetObjs(static_cast<GProfile*>(0));
	for(Prof.Start();!Prof.End();Prof.Next())
		ProfileAssess(Prof(),newSubject,cNoRef,maxDocsOK,maxDocsKO,maxDocsH);

	if(Session->MustSaveResults())
	{
		RCursor<GUser> Users(Session->GetUsers());
		for(Users.Start();!Users.End();Users.Next())
			Session->GetStorage()->SaveUser(Users());
		RCursor<GProfile> Profiles(Session->GetProfiles());
		for(Profiles.Start();!Profiles.End();Profiles.Next())
			Session->GetStorage()->SaveProfile(Profiles());
	}
	return(true);
}


//------------------------------------------------------------------------------
void GSimulator::ShareDocuments(void)
{
	size_t i;
	GSugs Sugs(200);

	// Suppose no profiles added
	NewProfiles.Clear();

	// Apply Configuration
	ApplyParams();

	// Compute the enabled suggestions computing methods
	Session->ComputeSugs();

	// Go through the groups
	R::RCursor<GCommunity> Grps(Session->GetCommunities());
	for(Grps.Start();!Grps.End();Grps.Next())
	{
		// Go through the profiles contained in the group.
		RCursor<GProfile> Profile(Grps()->GetObjs());
		for(Profile.Start();!Profile.End();Profile.Next())
		{
			Sugs.SetAddresseeId(otProfile,Profile()->GetId());
			Session->GetStorage()->LoadSugs(Sugs);
			Sugs.ReOrder(GDocRanking::SortOrderRanking);

			RCursor<GSuggestion> Cur(Sugs);
			for(Cur.Start(),i=NbDocsAssess+1;(!Cur.End())&&(--i);Cur.Next())
			{
				GDoc* doc=Session->GetDoc(Cur()->GetDocId());
				if(!doc) continue;

				// Look if 'OK'
				if(Session->IsFromSubject(Session->GetDoc(Cur()->GetDocId()),Session->GetSubject(Profile())))
				{
					Session->InsertFdbk(Profile()->GetId(),Cur()->GetDocId(),GFdbk::ErrorFdbk(ftRelevant,PercErr,Session->GetRandom()),RDate::GetToday(),doc->GetUpdated());
				}
				else
				{
					// Look If 'KO'
					if(Session->IsFromParentSubject(Session->GetDoc(Cur()->GetDocId()),Session->GetSubject(Profile())))
					{
						Session->InsertFdbk(Profile()->GetId(),Cur()->GetDocId(),GFdbk::ErrorFdbk(ftFuzzyRelevant,PercErr,Session->GetRandom()),RDate::GetToday(),doc->GetUpdated());
					}
					else
					{
						// Must be H
						Session->InsertFdbk(Profile()->GetId(),Cur()->GetDocId(),GFdbk::ErrorFdbk(ftIrrelevant,PercErr,Session->GetRandom()),RDate::GetToday(),doc->GetUpdated());
					}
				}
			}
			Profile()->SetState(osModified);
		}
	}

	if(Session->MustSaveResults())
	{
		RCursor<GProfile> Profiles(Session->GetProfiles());
		for(Profiles.Start();!Profiles.End();Profiles.Next())
			Session->GetStorage()->SaveProfile(Profiles());
	}
}


//------------------------------------------------------------------------------
void GSimulator::AddAssessments(void)
{
	// Suppose no profiles added
	NewProfiles.Clear();

	// Apply Configuration
	ApplyParams();

	// Copy the selected documents in TmpDocs;
	if(!TmpDocs)
		TmpDocs=new GDoc*[Session->GetMaxObjectPos(otDoc)+1];
	NbTmpDocs=Session->FillSelectedDocs(TmpDocs);

	// Go through all the subjects which are used
	R::RCursor<GSubject> Subs(Session->GetSubjects());
	for(Subs.Start();!Subs.End();Subs.Next())
	{
		if(!Subs()->IsUsed()) continue;

		// Go through the profiles attached to the subject
		RCursor<GProfile> Prof(Subs()->GetObjs(static_cast<GProfile*>(0)));
		for(Prof.Start();!Prof.End();Prof.Next())
		{
			// Look if current profile has already assessed documents.
			if(!Prof()->GetNbAssessedDocs())
				continue;

			// Mix the documents and assess the first NbDocsAssess one
			Session->GetRandom().RandOrder<GDoc*>(TmpDocs,NbTmpDocs);
			ProfileAssess(Prof(),Subs(),NbDocsAssess,cNoRef,cNoRef,cNoRef);
		}
	}
	if(Session->MustSaveResults())
	{
		RCursor<GProfile> Profiles(Session->GetProfiles());
		for(Profiles.Start();!Profiles.End();Profiles.Next())
			Session->GetStorage()->SaveProfile(Profiles());
	}
}


//------------------------------------------------------------------------------
size_t GSimulator::AddProfiles(void)
{

	GSubject** ptr;
	size_t i;
	GSubject* usedSubject;
	R::RCursor<GCommunity> CurGrps;
	size_t maxDocsOK,maxDocsKO,maxDocsH;


	// Suppose no profiles added
	NewProfiles.Clear();

	// Apply Configuration
	ApplyParams();

	// Randomly mix the subjects in tab
	GSubject** tab(new GSubject*[Session->GetNbObjects(otSubject)]);
	Session->FillSubjects(tab);
	Session->GetRandom().RandOrder<GSubject*>(tab,Session->GetNbObjects(otSubject));

	// Find the first used subject
	for(ptr=tab,i=Session->GetNbObjects(otSubject)+1,usedSubject=0;--i;ptr++)
	{
		if(!(*ptr)->IsUsed()) continue;
		usedSubject=(*ptr);
		break;
	}

	// delete tab;
	delete[] tab;

	// If no subject found -> do nothing
	if(!usedSubject)
		return(0);

	InitSubject(usedSubject,false);

	// Copy the documents in TmpDocs;
	if(!TmpDocs)
		TmpDocs=new GDoc*[Session->GetMaxObjectPos(otDoc)+1];
	NbTmpDocs=Session->FillSelectedDocs(TmpDocs);

	// Number of documents to judged by each profile
	if(RelOK)
		maxDocsOK=static_cast<size_t>(static_cast<double>(usedSubject->GetNbObjs(otDoc))*NbOK/100);
	else
		maxDocsOK=static_cast<size_t>(NbOK);
	if(RelKO)
		maxDocsKO=static_cast<size_t>(static_cast<double>(usedSubject->GetNbObjs(otDoc))*NbKO/100);
	else
		maxDocsKO=static_cast<size_t>(NbKO);
	if(RelH)
		maxDocsH=static_cast<size_t>(static_cast<double>(maxDocsOK)*NbH/100);
	else
		maxDocsH=static_cast<size_t>(NbH);

	// Assess documents
	RCursor<GProfile> Prof(NewProfiles);
	for(Prof.Start();!Prof.End();Prof.Next())
		ProfileAssess(Prof(),usedSubject,cNoRef,maxDocsOK,maxDocsKO,maxDocsH);

	// Optional saving
	if(Session->MustSaveResults())
	{
		RCursor<GUser> Users(Session->GetUsers());
		for(Users.Start();!Users.End();Users.Next())
			Session->GetStorage()->SaveUser(Users());
		RCursor<GProfile> Profiles(NewProfiles);
		for(Profiles.Start();!Profiles.End();Profiles.Next())
			Session->GetStorage()->SaveProfile(Profiles());
	}

	// Return the number of created profiles
	return(NewProfiles.GetNb());
}


//------------------------------------------------------------------------------
void GSimulator::PerformDegradation(char what,int nb)
{
	// Suppose no profiles added
	NewProfiles.Clear();

	switch(what)
	{
		case 0:
			// Apply Configuration
			ApplyParams();
			if(SwitchRandom)
			{
				delete SwitchRandom;
				SwitchRandom=0;
			}
			SwitchRandom=new RRandom(RRandom::Good);

			// Initialize the elements
			if(!TmpDocs)
				TmpDocs=new GDoc*[Session->GetMaxObjectPos(otDoc)+1];
			if(!TmpTopics)
				TmpTopics=new GTopic*[Session->GetNbObjects(otSubject)+1];
			return;
			break;

		case 1:
			// Re-init the session
			Session->ReInit();

			// Copy the ideal clustering as the current one
			BuildIdealTopics();
			NbTmpDocs=Session->FillSelectedDocs(TmpDocs);
			SwitchRandom->RandOrder(TmpDocs,NbTmpDocs);
			SwitchPos=0;
			if(nb>0)
			{
				// nb splits
				size_t Nb(nb);
				if(Nb>Session->GetNbObjects(otTopic))
					Nb=Session->GetNbObjects(otTopic);
				Session->FillTopics(TmpTopics);
				SwitchRandom->RandOrder(TmpTopics,Nb);
				GTopic** ptr(TmpTopics);
				for(size_t i=0;i<Nb;ptr++)
				{
					if(Session->MustBreak()) return;
					if(!(*ptr))
						continue;
					i++;
					size_t split(SwitchRandom->GetValue((*ptr)->GetNbObjs()-1)+2);

					// The first 'split' elements will be copied to a new topic
					RCursor<GDoc> Docs((*ptr)->GetObjs());
					RContainer<GDoc,false,false> Dels(Docs.GetNb());
					for(Docs.Start();--split;Docs.Next())
						Dels.InsertPtr(Docs());
					Docs=Dels;
					GTopic* ptr2=new GTopic(Session,"Split "+RString::Number(i));
					Session->AssignId(ptr2);
					Session->Insert(ptr2);
					for(Docs.Start();!Docs.End();Docs.Next())
					{
						(*ptr)->DeleteObj(Docs());
						ptr2->InsertObj(Docs());
					}
				}
			}
			else if(nb<0)
			{
				// -nb merges
				size_t Nb(-nb);
				if(Nb>Session->GetNbObjects(otTopic)-2)
					Nb=Session->GetNbObjects(otTopic)-2;
				Session->FillTopics(TmpTopics);
				for(size_t i=0;i<Nb;i++)
				{
					if(Session->MustBreak()) return;
					SwitchRandom->RandOrder(TmpTopics,Nb+1);
					GTopic** ptr(TmpTopics);
					while(!(*ptr))
						ptr++;
					GTopic** ptr2=ptr+1;
					while(!(*ptr2))
						ptr2++;

					// Put elements of (*ptr) in (*ptr2)
					RCursor<GDoc> Docs((*ptr)->GetObjs());
					RContainer<GDoc,false,false> Dels(Docs.GetNb());
					for(Docs.Start();!Docs.End();Docs.Next())
						Dels.InsertPtr(Docs());
					Docs=Dels;
					for(Docs.Start();!Docs.End();Docs.Next())
					{
						(*ptr)->DeleteObj(Docs());
						(*ptr2)->InsertObj(Docs());
					}
					Session->Delete(*ptr);
					(*ptr)=0;

				}
			}
			break;

		case 2:
		{
			size_t IncPos(static_cast<size_t>((SwitchPerc/100.0)*static_cast<double>(NbTmpDocs)));
			if(IncPos+SwitchPos>NbTmpDocs)
				IncPos=NbTmpDocs-SwitchPos;
			if(!IncPos)
				return;
			IncPos++;
			size_t MaxId(Session->GetMaxObjectId(otTopic));
			for(GDoc** ptr=&TmpDocs[SwitchPos];--IncPos;SwitchPos++,ptr++)
			{
				size_t ActGrp=(*ptr)->GetGroupId();
				size_t NewGrp(ActGrp);
				while((NewGrp==ActGrp)||(!Session->GetTopic(NewGrp,false,true)))
					NewGrp=SwitchRandom->GetValue(MaxId)+1;
				Session->GetTopic(ActGrp)->DeleteObj(*ptr);
				Session->GetTopic(NewGrp)->InsertObj(*ptr);
			}
			break;
		}

		default:
			throw GException("'"+RString::Number(what)+"' is a wrong parameter for GSimulator::StartDegradation");
	}
}


//------------------------------------------------------------------------------
void GSimulator::SetManualUsed(GSubject* subject,bool used)
{
	subject->Used=used;
	if(Session->MustSaveResults())
		Session->GetStorage()->SaveSubject(subject);
}


//------------------------------------------------------------------------------
void GSimulator::InitSubject(GSubject* subject,bool selectdocs)
{
	// Number of (social) profiles to create
	size_t nbprof;
	size_t nbsocial;
	if(CreateProfiles)
	{
		nbprof=Session->GetCurrentRandomValue(NbProfMax-NbProfMin+1)+NbProfMin;
		nbsocial=static_cast<size_t>(static_cast<double>(nbprof)*PercSocial/100);
	}
	else
		nbprof=nbsocial=0;


	// Verify that they are enough users -> If not, create new ones
	if(CreateProfiles)
	{
		if(Session->GetNbObjects(otUser)<nbprof+subject->GetNbObjs(otProfile))
		{
			size_t maxusers=sizeof(UserNames)/sizeof(char*);
			size_t newusers=nbprof+subject->GetNbObjs(otProfile)-Session->GetNbObjects(otUser)+1;
			for(size_t i=Session->GetNbObjects(otUser);(--newusers)&&(i<maxusers);i++)
			{
				GUser* user(new GUser(Session,cNoRef,UserNames[i],UserNames[i],Session->GetNbObjects(otSubject)));
				Session->AssignId(user);
				Session->Insert(user);
			}
			for(newusers++;--newusers;)
			{
				RString Usr("User"+RString::Number(Session->GetNbObjects(otUser)+1));
				GUser* user=new GUser(Session,cNoRef,Usr,Usr,Session->GetNbObjects(otSubject));
				Session->AssignId(user);
				Session->Insert(user);
			}
		}
	}

	// For each user without a profile in this topic (i.e. having the same name), create one.
	RCursor<GUser> Cur(Session->GetUsers());
	for(Cur.Start();(nbprof)&&(!Cur.End());Cur.Next())
	{
		if(Cur()->GetPtr(subject->Name,false))
			continue;
		GProfile* prof(new GProfile(Session,Cur(),ptInterest,subject->Name,nbsocial));
		Session->AssignId(prof);
		Session->Insert(prof);
		NewProfiles.InsertPtr(prof);
		if(nbsocial)
			nbsocial--;
		Session->Insert(prof,subject->GetId());
		nbprof--;
	}

	// If no documents to select -> the method has finished its job
	if(!selectdocs) return;

	// Compute the number of documents to select for this subject
	size_t nbdocs(0);
	size_t subjectdocs(subject->GetMaxDocs(MaxDepth));
	if(PercNbDocsPerSubject)
		nbdocs=static_cast<size_t>(NbDocsPerSubject*subjectdocs);
	else
	{
		if(NbDocsPerSubject)
			nbdocs=NbDocsPerSubject;
		else
			nbdocs=subjectdocs;
	}
	if(!nbdocs) return;

	// Select the documents of the subjects in TmpDocs
	NbTmpDocs=0;
	subject->FillDocs(TmpDocs,NbTmpDocs,MaxDepth);
	if(nbdocs>NbTmpDocs)
		nbdocs=NbTmpDocs;
	else
		Session->GetRandom().RandOrder(TmpDocs,NbTmpDocs);  // Randomize the documents

	// Choose the first nbdocs documents
	GDoc** ptr(TmpDocs);
	for(size_t i=nbdocs+1;--i;ptr++)
	{
		if((!MultipleSubjects)&&Session->GetSubject(*ptr)) continue;
		Session->Insert(*ptr,cNoRef,subject->GetId());
	}
}


//------------------------------------------------------------------------------
void GSimulator::SelectSubjects(void)
{
	// If the subjects must be selected automatically -> Select the necessary one
	if(!ManualSubjects)
	{
		size_t ToSelect;
		GSubject** ptr;
		size_t i;

		// Randomly mix the subjects in tab
		GSubject** tab(new GSubject*[Session->GetNbObjects(otSubject)]);
		Session->FillSubjects(tab);
		Session->GetRandom().RandOrder<GSubject*>(tab,Session->GetNbObjects(otSubject));

		// Compute the number of subjects to select
		if(RelSubjects)
			ToSelect=static_cast<size_t>((static_cast<double>(Session->GetNbObjects(otSubject))*NbSubjects)/100)+1;
		else
			ToSelect=static_cast<size_t>(NbSubjects);
		if(ToSelect>Session->GetNbObjects(otSubject))
			ToSelect=Session->GetNbObjects(otSubject);

		// Parse all subjects and select the correct number
		for(ptr=tab,i=Session->GetNbObjects(otSubject)+1;(--i)&&ToSelect;ptr++)
		{
			// Suppose the subject is not selected
			(*ptr)->Used=false;

			// Verify that there is enough documents to select the subject, and the the subject has the right depth
			if((*ptr)->GetMaxDocs(MaxDepth)<NbMinDocsSubject) continue;
			if(MaxDepth&&((*ptr)->Depth>MaxDepth)) continue;

			// OK, the subject can be used
			(*ptr)->Used=true;
			ToSelect--;
		}

		// The rest of the subjects are un-used
		for(;i--;ptr++)
			(*ptr)->Used=false;

		// delete tab;
		delete[] tab;
	}

	// For each selected subject -> Select the number of necessary profiles and documents (if necessary)
	RCursor<GSubject> Cur(Session->GetSubjects());
	for(Cur.Start();!Cur.End();Cur.Next())
		if(Cur()->IsUsed())
			InitSubject(Cur(),true);
}


//------------------------------------------------------------------------------
void GSimulator::CreateAssessments(void)
{
	size_t maxDocsOK,maxDocsKO,maxDocsH;

	// Copy the selected documents in TmpDocs;
	NbTmpDocs=Session->FillSelectedDocs(TmpDocs);

	// Assess documents
	RCursor<GSubject> Subs(Session->GetSubjects());
	for(Subs.Start();!Subs.End();Subs.Next())
	{
		if(!Subs()->IsUsed())
			continue;

		// Number of documents to assessed by each profile of the current subject
		if(RelOK)
			maxDocsOK=static_cast<size_t>(static_cast<double>(Subs()->GetNbObjs(otDoc))*NbOK/100);
		else
			maxDocsOK=static_cast<size_t>(NbOK);
		if(RelKO)
			maxDocsKO=static_cast<size_t>(static_cast<double>(Subs()->GetNbObjs(otDoc))*NbKO/100);
		else
			maxDocsKO=static_cast<size_t>(NbKO);
		if(RelH)
			maxDocsH=static_cast<size_t>(static_cast<double>(maxDocsOK)*NbH/100);
		else
			maxDocsH=static_cast<size_t>(NbH);

		// Simulate the assessments of a given profile for a given subject
		RCursor<GProfile> Prof(Subs()->GetObjs(static_cast<GProfile*>(0)));
		for(Prof.Start();!Prof.End();Prof.Next())
			ProfileAssess(Prof(),Subs(),cNoRef,maxDocsOK,maxDocsKO,maxDocsH);
	}
}


//------------------------------------------------------------------------------
void GSimulator::ProfileAssess(GProfile* prof,GSubject* sub,size_t max,size_t maxDocsOK,size_t maxDocsKO,size_t maxDocsH)
{
	size_t nbDocsOK,nbDocsKO,nbDocsH;
	size_t i;
	GDoc** ptr;

	// Mix the documents
	Session->GetRandom().RandOrder<GDoc*>(TmpDocs,NbTmpDocs);

	// Go trough the documents to create the assessments
	if(max>NbTmpDocs)
		i=NbTmpDocs+1;
	else
		i=max+1;
	for(ptr=TmpDocs,nbDocsOK=maxDocsOK,nbDocsKO=maxDocsKO,nbDocsH=maxDocsH;(--i)&&((nbDocsOK)||(nbDocsKO)||(nbDocsH));ptr++)
	{
		// Pointer cannot be null
		if(!(*ptr))
			continue;

		// Document must be defined
		if(!((*ptr)->IsDefined()))
			continue;

		// Verify that the document is not already assigned to the profile
		if(prof->GetFdbk((*ptr)->GetId())) continue;

		// Look if 'OK'
		if(Session->IsFromSubject((*ptr),sub))
		{
			if(nbDocsOK)
			{
				nbDocsOK--;
				Session->InsertFdbk(prof->GetId(),(*ptr)->GetId(),GFdbk::ErrorFdbk(ftRelevant,PercErr,Session->GetRandom()),RDate::GetToday(),(*ptr)->GetUpdated());
			}
		}
		else
		{
			// Look If 'KO'
			if(Session->IsFromParentSubject((*ptr),sub))
			{
				if(nbDocsKO)
				{
					nbDocsKO--;
					Session->InsertFdbk(prof->GetId(),(*ptr)->GetId(),GFdbk::ErrorFdbk(ftFuzzyRelevant,PercErr,Session->GetRandom()),RDate::GetToday(),(*ptr)->GetUpdated());
				}
			}
			else
			{
				// Must be H
				if(nbDocsH)
				{
					nbDocsH--;
					Session->InsertFdbk(prof->GetId(),(*ptr)->GetId(),GFdbk::ErrorFdbk(ftIrrelevant,PercErr,Session->GetRandom()),RDate::GetToday(),(*ptr)->GetUpdated());
				}
			}
		}
	}
}


//------------------------------------------------------------------------------
template<class cGroup,class cObj,class cCalc>
	void GSimulator::CopyIdealGroups(tObjType objtype,tObjType grouptype,cCalc* calc)
{
	cGroup* grp;

	// Clear current clustering
	Session->Clear(grouptype);

	// Go through each subjects
	R::RCursor<GSubject> Grps(Session->GetSubjects());
	for(Grps.Start();!Grps.End();Grps.Next())
	{
		// Clear the groups associated to the subject
		Grps()->ClearIdealGroup(grouptype);

		// If the subject has no objects or is not selected -> next one.
		if((!Grps()->IsUsed())||(!Grps()->GetNbObjs(objtype)))
			continue;

		// Create a new group in groups and associated with the current groups
		grp=new cGroup(Session,Grps()->GetName());
		Session->AssignId(grp);
		Session->Insert(grp);
		Grps()->AssignIdeal(grp);

		// Go through each object (verify that each object can be assigned only once)
		RCursor<cObj> Objs=Grps()->GetObjs(static_cast<cObj*>(0));
		for(Objs.Start();!Objs.End();Objs.Next())
			if(!Objs()->GetGroupId())
				grp->InsertObj(Objs());

		// Compute Description
		if(calc)
		{
			calc->Compute(grp);
			grp->Update(Session,calc->Infos);
		}
	}
}


//-----------------------------------------------------------------------------
void GSimulator::BuildClass(GSubject* subject,GClass* parent)
{
	// Create the class
	GClass* Class(new GClass(Session,"Subject "+RString::Number(subject->GetId())+" ("+subject->GetName()+")"));
	Session->Insert(parent,Class);

	// Build the vector representing its concepts
	Session->AssignInfos(Class,subject->GetVector());

	// Create sub-classes
	RCursor<GSubject> Cur(subject->GetSubjects());
	for(Cur.Start();!Cur.End();Cur.Next())
		BuildClass(Cur(),Class);
}


//------------------------------------------------------------------------------
void GSimulator::BuildIdealCommunities(void)
{
	GCommunityCalc* CalcDesc(GALILEIApp->GetCurrentPlugIn<GCommunityCalc>("CommunityCalc"));
	if(!CalcDesc)
		ThrowGException("No current plug-in to compute the communities");

	CopyIdealGroups<GCommunity,GProfile,GCommunityCalc>(otProfile,otCommunity,CalcDesc);
	if(Session->MustSaveResults())
	{
		Session->GetStorage()->Clear(otCommunity);
		RCursor<GCommunity> Groups(Session->GetCommunities());
		for(Groups.Start();!Groups.End();Groups.Next())
		{
			if(Groups()->GetVector().IsDefined())
				Session->SaveInfos(Groups()->GetVector(),otCommunity,Groups()->BlockId,Groups()->Id);

			Session->GetStorage()->SaveCommunity(Groups());
			Groups()->SetState(osSaved);
		}
	}
}


//------------------------------------------------------------------------------
void GSimulator::BuildIdealTopics(void)
{
	GTopicCalc* CalcDesc(GALILEIApp->GetCurrentPlugIn<GTopicCalc>("TopicCalc"));
	if(!CalcDesc)
		ThrowGException("No current plug-in to compute the topics");

	CopyIdealGroups<GTopic,GDoc,GTopicCalc>(otDoc,otTopic,CalcDesc);
	if(Session->MustSaveResults())
	{
		Session->GetStorage()->Clear(otTopic);
		RCursor<GTopic> Topics(Session->GetTopics());
		for(Topics.Start();!Topics.End();Topics.Next())
		{
			if(Topics()->GetVector().IsDefined())
				Session->SaveInfos(Topics()->GetVector(),otTopic,Topics()->BlockId,Topics()->Id);
			Session->GetStorage()->SaveTopic(Topics());
			Topics()->SetState(osSaved);
		}
	}
}


//------------------------------------------------------------------------------
void GSimulator::BuildIdealLeafTopics(void)
{
	Session->SetDescType(sdNames);
	RContainer<GWeightInfo,false,true> Infos(60);

	// Clear current topics clustering
	Session->Clear(otTopic);

	// Go through each subjects
	R::RCursor<GSubject> Grps(Session->GetSubjects());
	for(Grps.Start();!Grps.End();Grps.Next())
	{
		// Clear the groups associated to the subject
		Grps()->ClearIdealGroup(otTopic);

		// If the subject is not a final one, has no objects or is not selected -> next one.
		if((!Grps()->IsUsed())||(Grps()->GetNbSubjects()))
			continue;

		// Create a new group in groups and associated with the current groups
		GTopic* grp(new GTopic(Session,Grps()->GetName()));
		Session->AssignId(grp);
		Session->Insert(grp);
		Grps()->AssignIdeal(grp);

		// Update the topic.
		grp->Update(Session,Grps()->GetVector());

		// Save the results if necessary
		if(Session->MustSaveResults())
		{
			Session->GetStorage()->Clear(otTopic);
			RCursor<GTopic> Topics(Session->GetTopics());
			for(Topics.Start();!Topics.End();Topics.Next())
			{
				if(Topics()->GetVector().IsDefined())
					Session->SaveInfos(Topics()->GetVector(),otTopic,Topics()->BlockId,Topics()->Id);
				Session->GetStorage()->SaveTopic(Topics());
				Topics()->SetState(osSaved);
			}
		}
	}
}


//------------------------------------------------------------------------------
void GSimulator::BuildIdealClasses(void)
{
	Session->SetDescType(sdNames);

	// Clear current classes
	Session->ForceReCompute(otClass);
	RCursor<GSubject> Top(Session->GetTopSubjects());
	for(Top.Start();!Top.End();Top.Next())
		BuildClass(Top(),0);
}


//------------------------------------------------------------------------------
void GSimulator::BuildIdealDocsClasses(void)
{
	Session->SetDescType(sdDocs);

	// Clear current classes
	Session->ForceReCompute(otClass);
	RCursor<GSubject> Top(Session->GetTopSubjects());
	for(Top.Start();!Top.End();Top.Next())
		BuildClass(Top(),0);
}


//------------------------------------------------------------------------------
GSimulator::~GSimulator(void)
{
	delete[] TmpTopics;
	delete[] TmpDocs;
	delete SwitchRandom;
}
