/*

	GALILEI Research Project

	GSubjects.cpp

	Subjects - Implementation.

	Copyright 2002-2008 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).
		David Wartel (dwartel@ulb.ac.be).
		Julien Lamoral (jlamoral@ulb.ac.be).

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
#include <gsubjects.h>
#include <gsubject.h>
#include <gdoc.h>
#include <gsession.h>
#include <ggalileiapp.h>
#include <gstorage.h>
#include <glang.h>
#include <gprofile.h>
#include <gmeasure.h>
using namespace R;
using namespace GALILEI;



//------------------------------------------------------------------------------
//
//  GGroupId
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
class GGroupId
{
public:
	int GrpId;       // Group Identificator.
	int position;    // Position of the group.

	// Constructor and Compare methods.
	GGroupId(int RealId,int Position) : GrpId(RealId), position(Position) {}
	int Compare(const int ID) const {return(GrpId-ID);}
	int Compare(const GGroupId& grp) const {return(GrpId-grp.GrpId);}
};



//------------------------------------------------------------------------------
//
//  GroupScore
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
class GroupScore
{
public:
	GGroup* Group;                 // Pointer to the group.
	double Precision;              // Precision inside the group.
	double Recall;                 // Recall inside the group.

	// Constructor and Compare methods.
	GroupScore(GGroup* grp) : Group(grp), Precision(0.0), Recall(0.0) {}
	int Compare(const GroupScore& grp) const {return(Group->Compare(*grp.Group));}
	int Compare(const GGroup* grp) const {return(Group->Compare(grp));}
};



//------------------------------------------------------------------------------
//
// class GSubjects::Intern
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
class GSubjects::Intern
{
public:

	GSession* Session;                                                   // Session.
	double NbOK;                                                         // Nb of relevant documents to create.
	bool RelOK;                                                          // Number of Ok are relative or absolut.
	double NbKO;                                                         // Nb of fuzzy relevant documents to create.
	bool RelKO;                                                          // Number of KO are relative or absolut.
	double NbH;                                                          // Nb of irrelevant documents to create.
	bool RelH;                                                           // Number of H are relative (computed as a percentage of the relevant documents created) or absolut.
	double PercErr;                                                      // Percentage of documents assessed whith an error.
	double NbSubjects;                                                   // Percentage of subjects to use.
	bool RelSubjects;                                                    // Number of subjects are relative or absolut.
	unsigned int NbMinDocsSubject;                                       // Minimal number of documents in a subject to use it.
	unsigned int NbProfMax;                                              // Maximal number of profiles to create in a subject.
	unsigned int NbProfMin;                                              // Minimal number of profiles to create in a subject.
	double PercSocial;                                                   // Percentage of profiles that are social.
	GDoc** tmpDocs;                                                      // Temporary Array of documents.
	unsigned int NbDocs;                                                 // Number of documents actually managed.
	R::RContainer<GFdbk,false,false> NewDocs;                            // New documents to assess.
	R::RContainer<GProfile,false,true> LastAdded;                        // Lastest added profiles.
	unsigned int NbDocsAssess;                                           // Number of documents to be assessed during a feeddback process.
	double Precision;                                                    // Precision of the clustering.
	double Recall;                                                       // Recall of the clustering.
	double Total;                                                        // Total comparaison between for the clustering.
	R::RContainer<GroupScore,true,true> GroupsScore;                     // Score of a given group.
	R::RContainer<GSubject,false,false> Profiles;                        // Subjects for the profiles.
	R::RContainer<R::RContainer<GSubject,false,false>,true,false> Docs;  // Subjects for the documents.

	Intern(GSession* session) :  Session(session),
	  tmpDocs(0), NbDocs(0), NewDocs(NbDocs), LastAdded(50,25),
	  GroupsScore(100,50), Profiles(1000), Docs(5000)
	{
	}

	~Intern(void)
	{
		delete[] tmpDocs;
	}
};



//------------------------------------------------------------------------------
//
// class GSubjects
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GSubjects::GSubjects(GSession* session)
	: RTree<GSubject,true,false>(100,50), Data(0)
{
	Data=new Intern(session);
	InsertNode(0,new GSubject(0,"Subjects",false));
	Apply();
}


//------------------------------------------------------------------------------
void GSubjects::CreateConfig(void)
{
	GALILEIApp->GetGALILEIConfig()->InsertParam(new RParamValue("NbOK",10.0),"Subjects");
	GALILEIApp->GetGALILEIConfig()->InsertParam(new RParamValue("RelOK",true),"Subjects");
	GALILEIApp->GetGALILEIConfig()->InsertParam(new RParamValue("NbKO",10.0),"Subjects");
	GALILEIApp->GetGALILEIConfig()->InsertParam(new RParamValue("RelKO",true),"Subjects");
	GALILEIApp->GetGALILEIConfig()->InsertParam(new RParamValue("NbH",50.0),"Subjects");
	GALILEIApp->GetGALILEIConfig()->InsertParam(new RParamValue("RelH",true),"Subjects");
	GALILEIApp->GetGALILEIConfig()->InsertParam(new RParamValue("PercErr",0.0),"Subjects");
	GALILEIApp->GetGALILEIConfig()->InsertParam(new RParamValue("NbProfMin",2),"Subjects");
	GALILEIApp->GetGALILEIConfig()->InsertParam(new RParamValue("NbProfMax",10),"Subjects");
	GALILEIApp->GetGALILEIConfig()->InsertParam(new RParamValue("PercSocial",100.0),"Subjects");
	GALILEIApp->GetGALILEIConfig()->InsertParam(new RParamValue("NbSubjects",100.0),"Subjects");
	GALILEIApp->GetGALILEIConfig()->InsertParam(new RParamValue("RelSubjects",true),"Subjects");
	GALILEIApp->GetGALILEIConfig()->InsertParam(new RParamValue("NbMinDocsSubject",50),"Subjects");
	GALILEIApp->GetGALILEIConfig()->InsertParam(new RParamValue("NbDocsAssess",30),"Subjects");
}


//------------------------------------------------------------------------------
void GSubjects::Apply(void)
{
	Data->NbOK=GALILEIApp->GetGALILEIConfig()->GetDouble("NbOK","Subjects");
	Data->RelOK=GALILEIApp->GetGALILEIConfig()->GetBool("RelOK","Subjects");
	Data->NbKO=GALILEIApp->GetGALILEIConfig()->GetDouble("NbKO","Subjects");
	Data->RelKO=GALILEIApp->GetGALILEIConfig()->GetBool("RelKO","Subjects");
	Data->NbH=GALILEIApp->GetGALILEIConfig()->GetDouble("NbH","Subjects");
	Data->RelH=GALILEIApp->GetGALILEIConfig()->GetBool("RelH","Subjects");
	Data->PercErr=GALILEIApp->GetGALILEIConfig()->GetDouble("PercErr","Subjects");
	Data->NbProfMin=GALILEIApp->GetGALILEIConfig()->GetUInt("NbProfMin","Subjects");
	Data->NbProfMax=GALILEIApp->GetGALILEIConfig()->GetUInt("NbProfMax","Subjects");
	Data->PercSocial=GALILEIApp->GetGALILEIConfig()->GetDouble("PercSocial","Subjects");
	Data->NbSubjects=GALILEIApp->GetGALILEIConfig()->GetDouble("NbSubjects","Subjects");
	Data->RelSubjects=GALILEIApp->GetGALILEIConfig()->GetBool("RelSubjects","Subjects");
	Data->NbMinDocsSubject=GALILEIApp->GetGALILEIConfig()->GetUInt("NbMinDocsSubject","Subjects");
	Data->NbDocsAssess=GALILEIApp->GetGALILEIConfig()->GetUInt("NbDocsAssess","Subjects");
}


//------------------------------------------------------------------------------
void GSubjects::ReInit(void)
{
	// Re-init the subjects
	RCursor<GSubject> Subjects(GetNodes());
	for(Subjects.Start();!Subjects.End();Subjects.Next())
		Subjects()->ReInit();
}


//------------------------------------------------------------------------------
void GSubjects::ChooseSubjects(void)
{
	R::RCursor<GSubject> Subs;
	unsigned int compt;
	unsigned int nbprof,nbsocial;
	GSubject** tab(0);
	GSubject** ptr;
	unsigned int i;

	// Randomly mix the subjects in tab
	tab=new GSubject*[GetNbNodes()];
	RTree<GSubject,true,false>::GetTab(tab);
	Data->Session->GetRandom()->RandOrder<GSubject*>(tab,GetNbNodes());

	// Choose the first percgrp subjects having at least NbMinDocsSubject documents.
	if(Data->RelSubjects)
		compt=static_cast<unsigned int>((GetNbNodes()*Data->NbSubjects)/100)+1;
	else
		compt=static_cast<unsigned int>(Data->NbSubjects);
	for(ptr=tab,i=GetNbNodes()+1;(--i)&&compt;ptr++)
	{
		// Verify that there is enough documents
		if((*ptr)->GetNbDocs()<Data->NbMinDocsSubject) continue;

		// Number of (social) profiles that will assess documents
		nbprof=Data->Session->GetCurrentRandomValue(Data->NbProfMax-Data->NbProfMin+1)+Data->NbProfMin;
		nbsocial=static_cast<unsigned int>(nbprof*Data->PercSocial/100);

		// Set the topic to used and create profiles
		(*ptr)->SetUsed(Data->Session,nbprof,nbsocial);
		compt--;
	}

	// delete tab;
	delete[] tab;
}


//------------------------------------------------------------------------------
void GSubjects::CreateSet(void)
{
	RCursor<GProfile> Prof;
	unsigned int nbprof,nbsocial;
	unsigned int maxDocsOK,maxDocsKO,maxDocsH;
	RCursor<GFactoryLang> CurLang;

	// Init Part
	Data->LastAdded.Clear();
	if(!Data->tmpDocs)
		Data->tmpDocs=new GDoc*[Data->Session->GetMaxPosDoc()+1];

	// Go through all the subjects which are used
	RCursor<GSubject> Subs(GetNodes());
	for(Subs.Start();!Subs.End();Subs.Next())
	{
		if(!Subs()->IsUsed())
			continue;

		// Copy the documents of the session in Docs;
		Data->NbDocs=Data->Session->FillDocs(Data->tmpDocs);

		// Number of profiles that will assess documents
		if(Data->NbProfMax>Subs()->GetNbProfiles())
			Data->NbProfMax=Subs()->GetNbProfiles();
		if(Data->NbProfMin>Subs()->GetNbProfiles())
			Data->NbProfMin=Subs()->GetNbProfiles();
		nbprof=Data->Session->GetCurrentRandomValue(Data->NbProfMax-Data->NbProfMin+1)+Data->NbProfMin;

		// Number of profiles that are social
		nbsocial=static_cast<unsigned int>(nbprof*Data->PercSocial/100);

		// Number of documents to judged by each profile
		if(Data->RelOK)
			maxDocsOK=static_cast<unsigned int>(Subs()->GetNbDocs()*Data->NbOK/100);
		else
			maxDocsOK=static_cast<unsigned int>(Data->NbOK);
		if(Data->RelKO)
			maxDocsKO=static_cast<unsigned int>(Subs()->GetNbDocs()*Data->NbKO/100);
		else
			maxDocsKO=static_cast<unsigned int>(Data->NbKO);
		if(Data->RelH)
			maxDocsH=static_cast<unsigned int>(maxDocsOK*Data->NbH/100);
		else
			maxDocsH=static_cast<unsigned int>(Data->NbH);

		// Assess documents
		Prof=Subs()->GetProfiles();
		for(Prof.Start();!Prof.End();Prof.Next())
			ProfileAssess(Prof(),Subs(),maxDocsOK,maxDocsKO,maxDocsH);
	}
}


//------------------------------------------------------------------------------
void GSubjects::ProfileAssess(GProfile* prof,GSubject* sub,unsigned int maxDocsOK,unsigned int maxDocsKO,unsigned int maxDocsH)
{
	unsigned int nbDocsOK,nbDocsKO,nbDocsH;
	unsigned int i;
	GDoc** ptr;

	// Mix the documents
	Data->Session->GetRandom()->RandOrder<GDoc*>(Data->tmpDocs,Data->NbDocs);
	
	// Go trought the documents to create the judgements
	for(i=Data->NbDocs+1,ptr=Data->tmpDocs,nbDocsOK=maxDocsOK,nbDocsKO=maxDocsKO,nbDocsH=maxDocsH;(--i)&&((nbDocsOK)||(nbDocsKO)||(nbDocsH));ptr++)
	{
		// Pointer cannot be null
		if(!(*ptr))
			continue;

		// Document must be defined
		if(!((*ptr)->IsDefined()))
			continue;

		// Look if 'OK'
		if(IsFromSubject((*ptr),sub))
		{
			if(nbDocsOK)
			{
				nbDocsOK--;
				Data->Session->InsertFdbk(prof->GetId(),(*ptr)->GetId(),GFdbk::ErrorJudgment(djOK,Data->PercErr,Data->Session->GetRandom()),RDate::GetToday(),(*ptr)->GetUpdated());
			}
		}
		else
		{
			// Look If 'KO'
			if(IsFromParentSubject((*ptr),sub))
			{
				if(nbDocsKO)
				{
					nbDocsKO--;
					Data->Session->InsertFdbk(prof->GetId(),(*ptr)->GetId(),GFdbk::ErrorJudgment(djKO,Data->PercErr,Data->Session->GetRandom()),RDate::GetToday(),(*ptr)->GetUpdated());
				}
			}
			else
			{
				// Must be H
				if(nbDocsH)
				{
					nbDocsH--;
					Data->Session->InsertFdbk(prof->GetId(),(*ptr)->GetId(),GFdbk::ErrorJudgment(djOutScope,Data->PercErr,Data->Session->GetRandom()),RDate::GetToday(),(*ptr)->GetUpdated());
				}
			}
		}
	}
}


//------------------------------------------------------------------------------
GSubject* GSubjects::GetIdealGroup(GProfile* prof) const
{
	GSubject* subject(0);
	RCursor<GSubject> Cur(Top->GetNodes());
	for(Cur.Start();(!Cur.End())&&(!subject);Cur.Next())
		subject=Cur()->GetIdealGroup(prof);
	return(subject);
}


//------------------------------------------------------------------------------
GSubject* GSubjects::GetIdealGroup(GDoc* doc) const
{
	GSubject* subject;

	RCursor<GSubject> Cur(Top->GetNodes());
	for(Cur.Start(),subject=0;(!Cur.End())&&(!subject);Cur.Next())
		subject=Cur()->GetIdealGroup(doc);
	return(subject);
}


//------------------------------------------------------------------------------
void GSubjects::ComputeRecallPrecision(void)
{
	GSubject* ThGrp;        // Subject of the corresponding ideal group.
	size_t InGrp;           // Number of profiles in the computed group.
	size_t ElseInGrp;       // Number of profiles in the computed group that are also in the ideal one.
	size_t InThGrp;         // Number of profiles in the corresponding ideal group.
	size_t ElseInThGrp;     // Number of profiles in the corresponding ideal group that are also in the computed one.
	size_t NbProf;          // Number of profiles grouped.

	Data->Precision=Data->Recall=0.0;
	NbProf=0;
	R::RCursor<GroupScore> Grp(Data->GroupsScore);
	for(Grp.Start();!Grp.End();Grp.Next())
	{
		InGrp=Grp()->Group->GetNbProfiles();
		NbProf+=InGrp;
		Grp()->Precision=Grp()->Recall=0.0;
		RAssert(InGrp);
		RCursor<GProfile> Prof(Grp()->Group->GetProfiles());
		if(InGrp==1)
		{			
			Prof.Start();
			ThGrp=GetIdealGroup(Prof());
			RAssert(ThGrp);
			InThGrp=ThGrp->GetNbProfiles();
			RAssert(InThGrp);
			Grp()->Precision=1.0;
			if(InThGrp==1)
				Grp()->Recall=1.0;
			Data->Precision+=Grp()->Precision;
			Data->Recall+=Grp()->Recall;
		}
		else
		{
			for(Prof.Start();!Prof.End();Prof.Next())
			{
				ThGrp=GetIdealGroup(Prof());
				RAssert(ThGrp);
				InThGrp=ThGrp->GetNbProfiles();
				RAssert(InThGrp);			
				if(InThGrp==1)
					Grp()->Recall+=1.0;
				else
				{
					ElseInThGrp=ThGrp->GetNbProfiles(Grp()->Group)-1;
					Grp()->Precision+=((double)(ElseInThGrp))/((double)(InGrp-1));
					ElseInGrp=Grp()->Group->GetNbProfiles(ThGrp)-1;
					Grp()->Recall+=((double)(ElseInGrp))/((double)(InThGrp-1));
				}
			}
			Data->Precision+=Grp()->Precision;
			Data->Recall+=Grp()->Recall;
			Grp()->Precision/=(double)InGrp;
			Grp()->Recall/=(double)InGrp;
		}
	}
	if(NbProf)
	{
		Data->Precision/=(double)NbProf;
		Data->Recall/=(double)NbProf;
	}
}


//-----------------------------------------------------------------------------
size_t GSubjects::GetNbIdealGroups(void) const
{
	unsigned int nb;

	RCursor<GSubject> Cur(Top->GetNodes());
	for(Cur.Start(),nb=0;!Cur.End();Cur.Next())
		nb+=Cur()->GetNbIdealGroups();
	return(nb);
}


//-----------------------------------------------------------------------------
size_t GSubjects::GetNbTopicsDocs(void) const
{
	unsigned int nb;

	RCursor<GSubject> Cur(Top->GetNodes());
	for(Cur.Start(),nb=0;!Cur.End();Cur.Next())
		nb+=Cur()->GetNbTopicsDocs();
	return(nb);
}


//-----------------------------------------------------------------------------
void GSubjects::ComputeTotal(void)
{
	GGroup* GroupComputed;                        // Pointer to a computed group
	unsigned int NbRows,NbCols;                   // Rows and Cols for the current language for matrix
	unsigned int MaxRows,MaxCols;                 // Maximal Rows and Cols for matrix allocation
	unsigned int NbProfiles;                      // Total Number of profiles
	unsigned int NbTot;
	unsigned int col;
	double a,b,c,d,num,den,subtotal;
	double* VectorRows;                           // Sum of the rows of the matrix
	double* VectorCols;                           // Sum of the columns of the matrix
	double* VectorColsTemp;                       // temp sum of the columns of the matrix
	double* ptr;

	// Init part
	Data->Total=0.0;
	NbProfiles=0;

	// Go through the languages to define the maximal sizes and allocate the matrix
	MaxRows=MaxCols=0;
	NbRows=GetNbIdealGroups();
	NbCols=Data->Session->GetGroups().GetNb();
	if(NbRows>MaxRows) MaxRows=NbRows;
	if(NbCols>MaxCols) MaxCols=NbCols;
	if((!MaxRows)||(!MaxCols))
		return;
	VectorRows=new double[MaxRows];
	VectorCols=new double[MaxCols];
	VectorColsTemp=new double[MaxCols];

	// Compute number of elements in ideal and computed groups.
	// and assign the groups to the current language.
	//GroupsIdeal=Data->IdealGroups->GetGroups(lang);
	//NbRows=GroupsIdeal.GetNb();
	NbRows=GetNbIdealGroups();
	RCursor<GGroup> GroupsComputed(Data->Session->GetGroups());
	NbCols=GroupsComputed.GetNb();
	if((!NbRows)||(!NbCols))
		return;

	// Construction of the container for relation between id and column in the matrix.
	RContainer<GGroupId,true,true> GroupsId(NbCols,NbCols/2);
	for(GroupsComputed.Start(),col=0;!GroupsComputed.End();GroupsComputed.Next())
		GroupsId.InsertPtr(new GGroupId((GroupsComputed())->GetId(),col++));

	// Initialisation of the variable used for computing the subtotal
	a=b=c=d=0.0;

	// Initalisation of the vectors
	memset(VectorRows,0,NbRows*sizeof(double));
	memset(VectorCols,0,NbCols*sizeof(double));

	// For each group of ideal group and for each subprofiles in this group
	// -> Compute the differents terms of the total
	int row,position;
	row=0;
	//for(GroupsIdeal.Start(),NbTot=0;!GroupsIdeal.End();GroupsIdeal.Next())
	RCursor<GSubject> GroupsIdeal(GetNodes());
	for(GroupsIdeal.Start(),NbTot=0;!GroupsIdeal.End();GroupsIdeal.Next())
	{
		if(!GroupsIdeal()->GetNbProfiles())
			continue;
		memset(VectorColsTemp,0,NbCols*sizeof(double));
		RCursor<GProfile> Prof(GroupsIdeal()->GetProfiles());
		for(Prof.Start();!Prof.End();Prof.Next())
		{
			if(Prof()->GetGroupId()==cNoRef)
				continue;
			VectorRows[row]++;
			NbTot++;
			GroupComputed=Data->Session->GetGroup(Prof()->GetGroupId());
			if(!GroupComputed)
				continue;
			position=GroupsId.GetPtr(GroupComputed->GetId())->position;
			VectorCols[position]++;
			VectorColsTemp[position]++;
		}
		row++;
		for(col=NbCols+1,ptr=VectorColsTemp;--col;ptr++)
			a+=(((*ptr)*((*ptr)-1))/2);
	}

	for(col=NbCols+1,ptr=VectorCols;--col;ptr++)
		b+=(((*ptr)*((*ptr)-1))/2);
	for(row=NbRows+1,ptr=VectorRows;--row;ptr++)
		c+=(((*ptr)*((*ptr)-1))/2);
	d=(NbTot*(NbTot-1))/2;
	num=a-((b*c)/d);
	den=(0.5*(b+c))-(b*c/d);
	if(den)
		subtotal=num/den;
	else
		subtotal=1.0;
	NbProfiles+=NbTot;
	Data->Total+=subtotal*NbTot;

	// Compute Total
	if(NbProfiles)
		Data->Total=Data->Total/static_cast<double>(NbProfiles);
	else
		Data->Total=0.0;

	//delete the vectors
	if (VectorRows) delete[] VectorRows;
	if (VectorCols) delete[] VectorCols;
	if (VectorColsTemp) delete[] VectorColsTemp;
}


//------------------------------------------------------------------------------
void GSubjects::CreateIdeal(void)
{
	// Apply Config
	Apply();

	// Re-init the session
	Data->Session->ReInit();

	if(!Data->tmpDocs)
		Data->tmpDocs=new GDoc*[Data->Session->GetMaxPosDoc()+1];
	ChooseSubjects();
	CreateSet();
	if(Data->Session->MustSaveResults())
	{
		Data->Session->GetStorage()->Clear(otUser);
		Data->Session->GetStorage()->Clear(otProfile);
		Data->Session->GetStorage()->Clear(otFdbk);
		Data->Session->GetStorage()->Clear(otGroup);
		RCursor<GUser> Users(Data->Session->GetUsers());
		for(Users.Start();!Users.End();Users.Next())
			Data->Session->GetStorage()->SaveUser(Users());
		RCursor<GProfile> Profiles(Data->Session->GetProfiles());
		for(Profiles.Start();!Profiles.End();Profiles.Next())
			Data->Session->GetStorage()->SaveProfile(Profiles());
	}
}


//------------------------------------------------------------------------------
void GSubjects::DocumentSharing(void)
{	
	unsigned int i;

	// Apply Config
	Apply();

	// Similarities
	GMeasure* ProfilesDocsSims=GALILEIApp->GetManager<GMeasureManager>("Measures")->GetCurrentMethod("Profiles/Documents Similarities");

	// Go through the groups
	R::RCursor<GGroup> Grps(Data->Session->GetGroups());
	for(Grps.Start();!Grps.End();Grps.Next())
	{
		// Go through the profiles contained in the group.
		RCursor<GProfile> Profile(Grps()->GetProfiles());
		for(Profile.Start();!Profile.End();Profile.Next())
		{
			Grps()->NotJudgedDocsRelList(ProfilesDocsSims,Data->NewDocs,Profile(),Data->Session);
			RCursor<GFdbk> Cur(Data->NewDocs);
			for(Cur.Start(),i=Data->NbDocsAssess+1;(!Cur.End())&&(--i);Cur.Next())
			{				
				GDoc* doc=Data->Session->GetDoc(Cur()->GetDocId());
				if(!doc)
					continue;
			
				// Look if 'OK'
				if(IsFromSubject(Cur()->GetDocId(),GetSubject(Profile())))
				{
					Data->Session->InsertFdbk(Profile()->GetId(),Cur()->GetDocId(),GFdbk::ErrorJudgment(djOK,Data->PercErr,Data->Session->GetRandom()),RDate::GetToday(),doc->GetUpdated());
				}
				else
				{
					// Look If 'KO'
					if(IsFromParentSubject(Cur()->GetDocId(),GetSubject(Profile())))
					{
						Data->Session->InsertFdbk(Profile()->GetId(),Cur()->GetDocId(),GFdbk::ErrorJudgment(djKO,Data->PercErr,Data->Session->GetRandom()),RDate::GetToday(),doc->GetUpdated());
					}
					else
					{
						// Must be H
						Data->Session->InsertFdbk(Profile()->GetId(),Cur()->GetDocId(),GFdbk::ErrorJudgment(djOutScope,Data->PercErr,Data->Session->GetRandom()),RDate::GetToday(),doc->GetUpdated());
					}
				}
			}
			Profile()->SetState(osModified);
		}
	}
	
	if(Data->Session->MustSaveResults())
	{
		RCursor<GProfile> Profiles(Data->Session->GetProfiles());
		for(Profiles.Start();!Profiles.End();Profiles.Next())
			Data->Session->GetStorage()->SaveProfile(Profiles());
	}
}


//------------------------------------------------------------------------------
void GSubjects::AddAssessments(void)
{
	RCursor<GProfile> Prof;
	unsigned int i;
	GDoc** ptr;

	// Apply Config
	Apply();

	// Go through all the subjects which are used
	R::RCursor<GSubject> Subs(GetNodes());
	for(Subs.Start();!Subs.End();Subs.Next())
	{
		if(!Subs()->IsUsed()) continue;

		// Copy the documents of the session in Docs;
		Data->NbDocs=Data->Session->FillDocs(Data->tmpDocs);

		// Go through the profiles attached to the subject
		Prof=Subs()->GetProfiles();
		for(Prof.Start();!Prof.End();Prof.Next())
		{
			// Look if current profile has already judged documents.
			if(!Prof()->GetNbAssessedDocs())
				continue;

			// Mix the documents
			Data->Session->GetRandom()->RandOrder<GDoc*>(Data->tmpDocs,Data->NbDocs);

			// Go trought the documents to create the judgements
			for(i=Data->NbDocsAssess+1,ptr=Data->tmpDocs;--i;ptr++)
			{
				// Verify that the document is not already assigned to the profile
				if(Prof()->GetFdbk((*ptr)->GetId())) continue;

				// Look if 'OK'
				if(IsFromSubject((*ptr),Subs()))
				{
					Data->Session->InsertFdbk(Prof()->GetId(),(*ptr)->GetId(),GFdbk::ErrorJudgment(djOK,Data->PercErr,Data->Session->GetRandom()),RDate::GetToday(),(*ptr)->GetUpdated());
				}
				else
				{
					// Look If 'KO'
					if(IsFromParentSubject((*ptr),Subs()))
					{
						Data->Session->InsertFdbk(Prof()->GetId(),(*ptr)->GetId(),GFdbk::ErrorJudgment(djKO,Data->PercErr,Data->Session->GetRandom()),RDate::GetToday(),(*ptr)->GetUpdated());
					}
					else
					{
						Data->Session->InsertFdbk(Prof()->GetId(),(*ptr)->GetId(),GFdbk::ErrorJudgment(djOutScope,Data->PercErr,Data->Session->GetRandom()),RDate::GetToday(),(*ptr)->GetUpdated());
					}
				}
			}
		}
	}
	if(Data->Session->MustSaveResults())
	{
		RCursor<GProfile> Profiles(Data->Session->GetProfiles());
		for(Profiles.Start();!Profiles.End();Profiles.Next())
			Data->Session->GetStorage()->SaveProfile(Profiles());
	}
}


//------------------------------------------------------------------------------
bool GSubjects::AddTopic(void)
{
	GSubject** tab;
	GSubject** ptr;
	unsigned int i;
	GSubject* newSubject;
	RCursor<GProfile> Prof;
	unsigned int nbprof,nbsocial;
	unsigned int maxDocsOK,maxDocsKO,maxDocsH;

	// Apply Config
	Apply();

	// Randomly mix the subjects in tab
	tab=new GSubject*[GetNbNodes()];
	RTree<GSubject,true,false>::GetTab(tab);
	Data->Session->GetRandom()->RandOrder<GSubject*>(tab,GetNbNodes());

	// Find the first not used subject having at least NbMinDocsSubject documents.
	for(ptr=tab,i=GetNbNodes()+1,newSubject=0;--i;ptr++)
	{
		if((*ptr)->GetNbDocs()<Data->NbMinDocsSubject) continue;
		if((*ptr)->IsUsed()) continue;
		newSubject=(*ptr);
		break;
	}

	// delete tab;
	delete[] tab;

	// If no subject found -> do nothing
	if(!newSubject) return(false);

	// Copy the documents of the same language of the session in Docs;
	Data->NbDocs=Data->Session->FillDocs(Data->tmpDocs);

	// Number of (social) subprofiles that will judge documents
	nbprof=Data->Session->GetCurrentRandomValue(Data->NbProfMax-Data->NbProfMin+1)+Data->NbProfMin;
	nbsocial=static_cast<unsigned int>(nbprof*Data->PercSocial/100);

	newSubject->SetUsed(Data->Session,nbprof,nbsocial);

	// Number of documents to judged by each profile
	if(Data->RelOK)
		maxDocsOK=static_cast<unsigned int>(newSubject->GetNbDocs()*Data->NbOK/100);
	else
		maxDocsOK=static_cast<unsigned int>(Data->NbOK);
	if(Data->RelKO)
		maxDocsKO=static_cast<unsigned int>(newSubject->GetNbDocs()*Data->NbKO/100);
	else
		maxDocsKO=static_cast<unsigned int>(Data->NbKO);
	if(Data->RelH)
		maxDocsH=static_cast<unsigned int>(maxDocsOK*Data->NbH/100);
	else
		maxDocsH=static_cast<unsigned int>(Data->NbH);


	// Assess documents
	Prof=newSubject->GetProfiles();
	for(Prof.Start();!Prof.End();Prof.Next())
		ProfileAssess(Prof(),newSubject,maxDocsOK,maxDocsKO,maxDocsH);

	if(Data->Session->MustSaveResults())
	{
		RCursor<GUser> Users(Data->Session->GetUsers());
		for(Users.Start();!Users.End();Users.Next())
			Data->Session->GetStorage()->SaveUser(Users());
		RCursor<GProfile> Profiles(Data->Session->GetProfiles());
		for(Profiles.Start();!Profiles.End();Profiles.Next())
			Data->Session->GetStorage()->SaveProfile(Profiles());
	}
	return(true);
}


//------------------------------------------------------------------------------
unsigned int GSubjects::AddProfiles(void)
{
	GSubject** tab;
	GSubject** ptr;
	unsigned int i;
	unsigned int nbprof, nbsocial;
	GSubject* usedSubject;
	R::RCursor<GGroup> CurGrps;
	unsigned int maxDocsOK,maxDocsKO,maxDocsH;
	RCursor<GProfile> Prof;

	// Apply Config
	Apply();

	//Randomly choose the number of (social) profiles.
	nbprof=Data->Session->GetCurrentRandomValue(Data->NbProfMax-Data->NbProfMin+1)+Data->NbProfMin;
	nbsocial=static_cast<unsigned int>(nbprof*Data->PercSocial/100);

	// Randomly mix the subjects in tab
	tab=new GSubject*[GetNbNodes()];
	RTree<GSubject,true,false>::GetTab(tab);
	Data->Session->GetRandom()->RandOrder<GSubject*>(tab,GetNbNodes());

	// Find the first used subject having at least NbMinDocsSubject documents.
	for(ptr=tab,i=GetNbNodes()+1,usedSubject=0;--i;ptr++)
	{
		if(!(*ptr)->IsUsed()) continue;  //check if the subject is already in use..
		usedSubject=(*ptr);
		break;
	}

	// delete tab;
	delete[] tab;

	// If no subject found -> do nothing
	if(!usedSubject)
		return(0);

	// Copy the documents of the same language of the session in Docs;
	Data->NbDocs=Data->Session->FillDocs(Data->tmpDocs);

	// Number of documents to judged by each profile
	if(Data->RelOK)
		maxDocsOK=static_cast<unsigned int>(usedSubject->GetNbDocs()*Data->NbOK/100);
	else
		maxDocsOK=static_cast<unsigned int>(Data->NbOK);
	if(Data->RelKO)
		maxDocsKO=static_cast<unsigned int>(usedSubject->GetNbDocs()*Data->NbKO/100);
	else
		maxDocsKO=static_cast<unsigned int>(Data->NbKO);
	if(Data->RelH)
		maxDocsH=static_cast<unsigned int>(maxDocsOK*Data->NbH/100);
	else
		maxDocsH=static_cast<unsigned int>(Data->NbH);


	usedSubject->SetUsed(Data->Session,nbprof,nbsocial);

	// Assess documents
	Prof=usedSubject->GetProfiles();
	for(Prof.Start();!Prof.End();Prof.Next())
		ProfileAssess(Prof(),usedSubject,maxDocsOK,maxDocsKO,maxDocsH);

	// optional saving
	if(Data->Session->MustSaveResults())
	{
		RCursor<GProfile> Profiles(Data->Session->GetProfiles());
		for(Profiles.Start();!Profiles.End();Profiles.Next())
			Data->Session->GetStorage()->SaveProfile(Profiles());
	}

	// Return the number of created profiles
	return(Prof.GetNb());
}


//------------------------------------------------------------------------------
double GSubjects::ComputePercAss(void)
{
	RCursor<GProfile> Cur1;
	RCursor<GProfile> Cur2;
	unsigned int nb;
	double PercAss;

	Cur1.Set(Data->LastAdded);
	Cur2=Data->Session->GetProfiles();
	if(Cur1.GetNb()<1)
	{
		PercAss=1.0;
		return(1.0);
	}

	// Go through the new created profiles
	for(Cur1.Start(),PercAss=0.0,nb=0;!Cur1.End();Cur1.Next())
	{
			// Go through all the profiles
		for(Cur2.Start();!Cur2.End();Cur2.Next())
		{
			// Get the profile corresponding to Lang and verify
			// that it is defined
			if(!Cur2()->IsDefined()) continue;

			// If same subprofile, skip it.
			if(Cur1()==Cur2()) continue;

			// If both subprofiles are not related to the same topic, skip it.
			if(GetSubject(Cur1())!=GetSubject(Cur2())) continue;

			// Make comparaisons
			nb++;
			if(Cur1()->GetGroupId()==Cur2()->GetGroupId()) PercAss+=1.0;
		}
	}
	if(!nb)
		PercAss=1.0;
	else
		PercAss/=nb;
	return(PercAss);
}


//------------------------------------------------------------------------------
void GSubjects::ClearLastAdded(void)
{
	Data->LastAdded.Clear();
}


//------------------------------------------------------------------------------
GSubject* GSubjects::GetSubject(unsigned int id)
{
	return(RTree<GSubject,true,false>::GetNode<unsigned int>(id,false));
}


//------------------------------------------------------------------------------
void GSubjects::Clear(void)
{
	RTree<GSubject,true,false>::Clear();
	Data->Profiles.Clear();
	InsertNode(0,new GSubject(0,"Subjects",false));
}


//------------------------------------------------------------------------------
void GSubjects::Compare(void)
{
	Data->GroupsScore.Clear();
	RCursor<GGroup> Cur(Data->Session->GetGroups());
	for(Cur.Start();!Cur.End();Cur.Next())
		Data->GroupsScore.InsertPtr(new GroupScore(Cur()));
	ComputeRecallPrecision();
	ComputeTotal();
}


//------------------------------------------------------------------------------
double GSubjects::GetPrecision(void) const
{
	return(Data->Precision);
}


//------------------------------------------------------------------------------
double GSubjects::GetRecall(void) const
{
	return(Data->Recall);
}


//------------------------------------------------------------------------------
double GSubjects::GetTotal(void) const
{
	return(Data->Total);
}


//------------------------------------------------------------------------------
double GSubjects::GetPrecision(GGroup* grp) const
{
	GroupScore* g=Data->GroupsScore.GetPtr<const GGroup*>(grp);

	if(!g) return(0.0);
	return(g->Precision);
}


//------------------------------------------------------------------------------
double GSubjects::GetRecall(GGroup* grp) const
{
	GroupScore* g=Data->GroupsScore.GetPtr<const GGroup*>(grp);

	if(!g) return(0.0);
	return(g->Recall);
}


//------------------------------------------------------------------------------
void GSubjects::InsertProfileSubject(GProfile* profile,unsigned int subjectid)
{
	GSubject* subject=RTree<GSubject,true,false>::GetNode<unsigned int>(subjectid,false);

	if(!subject)
		return;
	Data->Profiles.InsertPtrAt(subject,profile->GetId(),true);
	subject->Insert(profile);
}



//------------------------------------------------------------------------------
GSubject* GSubjects::GetSubject(GProfile* prof)
{
	RCursor<GSubject> Subjects(GetNodes());
	for(Subjects.Start();!Subjects.End();Subjects.Next())
		if(Subjects()->IsIn(prof))
			return(Subjects());
	return(0);
}


//------------------------------------------------------------------------------
void GSubjects::InsertDocSubject(GDoc* doc,unsigned int subjectid)
{
	GSubject* subject=RTree<GSubject,true,false>::GetNode<unsigned int>(subjectid,false);
	if(!subject)
		return;
	R::RContainer<GSubject,false,false>* line;
	if(Data->Docs.GetMaxPos()<doc->GetId())
	{
		line=new R::RContainer<GSubject,false,false>(10,5);
		Data->Docs.InsertPtrAt(line,doc->GetId(),true);
	}
	else
	{
		line=Data->Docs[doc->GetId()];
		if(!line)
		{
			line=new R::RContainer<GSubject,false,false>(10,5);
			Data->Docs.InsertPtrAt(line,doc->GetId(),true);
		}
	}
	line->InsertPtr(subject);
	subject->Insert(doc);
}


//------------------------------------------------------------------------------
bool GSubjects::IsFromSubject(GDoc* doc,const GSubject* s)
{
	return(IsFromSubject(doc->GetId(),s));
}


//------------------------------------------------------------------------------
bool GSubjects::IsFromSubject(unsigned int docid,const GSubject* s)
{
	if(Data->Docs.GetMaxPos()<docid)
		return(false);
	R::RContainer<GSubject,false,false>* line=Data->Docs[docid];
	if(!line)
		return(false);
	return(line->IsIn(*s));
}


//------------------------------------------------------------------------------
bool GSubjects::IsFromParentSubject(GDoc* doc,const GSubject* s)
{
	return(IsFromParentSubject(doc->GetId(),s));
}


//------------------------------------------------------------------------------
bool GSubjects::IsFromParentSubject(unsigned int docid,const GSubject* s)
{
	// Verify that a parent exist and that it is not the root node
	if(!s->Parent)
		return(false);
	if(!s->Parent->Parent)
		return(false);
	
	
	if(Data->Docs.GetMaxPos()<docid)
		return(false);
	R::RContainer<GSubject,false,false>* line=Data->Docs[docid];
	if(!line)
		return(false);

	R::RCursor<GSubject> Sub(s->GetParent()->GetNodes());
	for(Sub.Start();!Sub.End();Sub.Next())
	{
		if(Sub()==s)
			continue;
		if(line->IsIn(*Sub()))
			return(true);
	}
	return(false);
}


//------------------------------------------------------------------------------
R::RCursor<GSubject> GSubjects::GetSubjects(GDoc* doc)
{
	return(GetSubjects(doc->GetId()));
}


//------------------------------------------------------------------------------
R::RCursor<GSubject> GSubjects::GetSubjects(unsigned int docid)
{
	R::RCursor<GSubject> cur;
	R::RContainer<GSubject,false,false>* line=Data->Docs[docid];
	if(line)
		cur.Set(*line);
	return(cur);
}


//------------------------------------------------------------------------------
unsigned int GSubjects::GetNbSubjects(GDoc* doc)
{
	return(GetNbSubjects(doc->GetId()));
}


//------------------------------------------------------------------------------
unsigned int GSubjects::GetNbSubjects(unsigned int docid)
{
	R::RContainer<GSubject,false,false>* line=Data->Docs[docid];
	if(!line)
		return(0);
	return(line->GetNb());
}


//------------------------------------------------------------------------------
GSubjects::~GSubjects(void)
{
	delete Data;
}
