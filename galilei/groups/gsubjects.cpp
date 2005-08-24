/*

	GALILEI Research Project

	GSubjects.cpp

	Subjects - Implementation.

	Copyright 2002-2003 by the Universit�Libre de Bruxelles.

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
#include <gpluginmanagers.h>
#include <gstorage.h>
#include <glang.h>
#include <gprofile.h>
#include <gsubprofile.h>
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
//  GSubjects::GroupLang
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
class GSubjects::GroupLang
{
public:
	GLang* Lang;
	GGroup* Group;

	GroupLang(GLang* lang,GGroup* group) : Lang(lang), Group(group) {}
	int Compare(const GroupLang& grp) const { return(Lang->Compare(*grp.Lang));}
	int Compare(const GLang* lang) const { return(Lang->Compare(lang));}
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
	double PercOK;                                                       // Percentage of relevant documents to create.
	double PercKO;                                                       // Percentage of fuzzy relevant documents to create.
	double PercH;                                                        // Percentage of irrelevant documents to create (Computed as a percentage of the relevant documents created).
	double PercErr;                                                      //  Percentage of documents assessed whith an error.
	double PercSubjects;                                                 // Percentage of subjects to use.
	unsigned int NbMinDocsSubject;                                       // Minimal number of documents in a subject to use it.
	unsigned int NbProfMax;                                              //  Maximal number of profiles to create in a subject.
	unsigned int NbProfMin;                                              // Minimal number of profiles to create in a subject.
	double PercSocial;                                                   // Percentage of profiles that are social.
	GDoc** tmpDocs;                                                      // Temporary Array of documents.
	unsigned int NbDocs;                                                 // Number of documents actually managed.
	R::RContainer<GFdbk,false,false> NewDocs;                            // New documents to assess.
	R::RContainer<GSubProfile,false,true> LastAdded;                     // Lastest added subprofiles.
	unsigned int NbDocsAssess;                                           // Number of documents to be assessed during a feeddback process.
	double Precision;                                                    // Precision of the clustering.
	double Recall;                                                       // Recall of the clustering.
	double Total;                                                        // Total comparaison between for the clustering.
	R::RContainer<GroupScore,true,true> GroupsScore;                     // Score of a given group.
	R::RContainer<GSubject,false,false> Profiles;                        // Subjects for the profiles.
	R::RContainer<R::RContainer<GSubject,false,false>,true,false> Docs;  // Subjects for the documents.
	bool SaveSimulation;                                                 // Must the simulation be saved?

	Intern(GSession* session) :  Session(session),
	  tmpDocs(0), NbDocs(0), NewDocs(NbDocs), LastAdded(50,25),
	  GroupsScore(100,50), Profiles(1000), Docs(5000), SaveSimulation(false)
	{
	}

	~Intern(void)
	{
//		delete IdealGroups;
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
	: RTree<GSubject,true,false>(100,50), GParams("Subjects"), Data(0)
{
	Data=new Intern(session);
	GParams::InsertPtr(new GParamDouble("PercOK",10.0));
	GParams::InsertPtr(new GParamDouble("PercKO",10.0));
	GParams::InsertPtr(new GParamDouble("PercH",50.0));
	GParams::InsertPtr(new GParamDouble("PercErr",0.0));
	GParams::InsertPtr(new GParamUInt("NbProfMin",2));
	GParams::InsertPtr(new GParamUInt("NbProfMax",10));
	GParams::InsertPtr(new GParamDouble("PercSocial",100.0));
	GParams::InsertPtr(new GParamDouble("PercSubjects",100.0));
	GParams::InsertPtr(new GParamUInt("NbMinDocsSubject",50));
	GParams::InsertPtr(new GParamUInt("NbDocsAssess",30));
	Apply();
}


//------------------------------------------------------------------------------
void GSubjects::Apply(void)
{
	Data->PercOK=GetDouble("PercOK");
	Data->PercKO=GetDouble("PercKO");
	Data->PercH=GetDouble("PercH");
	Data->PercErr=GetDouble("PercErr");
	Data->NbProfMin=GetUInt("NbProfMin");
	Data->NbProfMax=GetUInt("NbProfMax");
	Data->PercSocial=GetDouble("PercSocial");
	Data->PercSubjects=GetDouble("PercSubjects");
	Data->NbMinDocsSubject=GetUInt("NbMinDocsSubject");
	Data->NbDocsAssess=GetUInt("NbDocsAssess");
}


//------------------------------------------------------------------------------
void GSubjects::ReInit(void)
{
	// Re-init the subjects
	RCursor<GSubject> Subjects(*this);
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
	tab=new GSubject*[RTree<GSubject,true,false>::GetMaxPos()+1];
	RTree<GSubject,true,false>::GetTab(tab);
	Data->Session->GetRandom()->RandOrder<GSubject*>(tab,GetNbNodes());

	// Choose the first percgrp subjects having at least NbMinDocsSubject documents.
	compt=static_cast<unsigned int>((GetNbNodes()*Data->PercSubjects)/100)+1;
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
	RCursor<GSubject> Subs;
	RCursor<GProfile> Prof;
	unsigned int nbprof,nbsocial;
	unsigned int maxDocsOK,maxDocsKO,maxDocsH;
	RCursor<GFactoryLang> CurLang;
	RContainer<GroupLang,true,true> Groups(20);          // Groups created for a given subject

	// Init Part
	Data->LastAdded.Clear();
	if(!Data->tmpDocs)
		Data->tmpDocs=new GDoc*[Data->Session->GetMaxPosDoc()+1];

	// Go through all the subjects which are used
	Subs.Set(*this);
	for(Subs.Start();!Subs.End();Subs.Next())
	{
		if(!Subs()->IsUsed())
			continue;

		// Clear the created groups
		Groups.Clear();

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

		// Number of documents to judged by each subprofile
		maxDocsOK=static_cast<unsigned int>(Subs()->GetNbDocs()*Data->PercOK/100);
		maxDocsKO=static_cast<unsigned int>(Subs()->GetNbDocs()*Data->PercKO/100);
		maxDocsH=static_cast<unsigned int>(maxDocsOK*Data->PercH/100);

		// Assess documents
		Prof=Subs()->GetProfiles();
		for(Prof.Start();!Prof.End();Prof.Next())
			ProfileAssess(Groups,Prof(),Subs(),maxDocsOK,maxDocsKO,maxDocsH);
	}
}


//------------------------------------------------------------------------------
void GSubjects::ProfileAssess(RContainer<GroupLang,true,true>& groups,GProfile* prof,GSubject* sub,unsigned int maxDocsOK,unsigned int maxDocsKO,unsigned int maxDocsH)
{
	unsigned int nbDocsOK,nbDocsKO,nbDocsH;
	unsigned int i;
	GDoc** ptr;
	RContainer<GLang,false,true> Langs(20);         // Langs already used
	GSubProfile* subprofile;

	// Mix the documents
	Data->Session->GetRandom()->RandOrder<GDoc*>(Data->tmpDocs,Data->NbDocs);

	// Go trought the documents to create the judgements
	for(i=Data->NbDocs+1,ptr=Data->tmpDocs,nbDocsOK=maxDocsOK,nbDocsKO=maxDocsKO,nbDocsH=maxDocsH;(--i)&&((nbDocsOK)||(nbDocsKO)||(nbDocsH));ptr++)
	{
		// Pointer cannot be null
		if(!(*ptr))
			continue;

		// Language of the document cannot be null
		GLang* Lang=(*ptr)->GetLang();
		if(!Lang)
			continue;
		if(!Langs.IsIn(Lang))
		{
/*			GroupLang* Grp=groups.GetPtr<GLang*>(Lang);
			if(!Grp)
			{
				GGroup* NewGrp;

				// A ideal group must be created
				Data->IdealGroups->InsertGroup(NewGrp=new GGroup(Data->IdealGroups->GetNbGroups(),Lang,false,RDate(""),RDate("")));
				groups.InsertPtr(Grp=new GroupLang(Lang,NewGrp));
			}

			// and the corresponding subprofile inserted
			Grp->Group->InsertSubProfile(prof->GetInsertSubProfile(Lang,Data->Session));*/
			Data->LastAdded.InsertPtr(subprofile=prof->GetInsertSubProfile(Lang,Data->Session));
			sub->InsertSubProfile(subprofile);
			Langs.InsertPtr(Lang);
		}

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
GSubject* GSubjects::GetIdealGroup(GSubProfile* sub) const
{
//	return(Data->IdealGroups->GetGroup(sub));
	GSubject* subject;

	RCursor<GSubject> Cur(*Top);
	for(Cur.Start(),subject=0;(!Cur.End())&&(!subject);Cur.Next())
		subject=Cur()->GetIdealGroup(sub);
	return(subject);
}


//------------------------------------------------------------------------------
void GSubjects::ComputeRecallPrecision(void)
{
	R::RCursor<GroupScore> Grp;
	RCursor<GSubProfile> Sub;
	//GGroup* thGrp;
	GSubject* thGrp;
	unsigned int NbGrp;
	unsigned int InGrp;
	unsigned int InthGrp;
	unsigned int NbProf;
	unsigned int nbsub;

	Data->Precision=Data->Recall=0.0;
	NbProf=0;
	Grp.Set(Data->GroupsScore);
	for(Grp.Start();!Grp.End();Grp.Next())
	{
		NbGrp=Grp()->Group->GetNbSubProfiles();
		NbProf+=NbGrp;
		Grp()->Precision=Grp()->Recall=0.0;
		if(!NbGrp) continue;
		if(NbGrp==1)
		{
			Sub=Grp()->Group->GetSubProfiles();
			Sub.Start();
			thGrp=GetIdealGroup(Sub());
//			if((!thGrp)||(thGrp->GetLang()!=Grp()->Group->GetLang()))
//				return;
			if(!thGrp)
				continue;
			nbsub=thGrp->GetNbSubProfiles(Grp()->Group->GetLang());
			if(!nbsub)
				continue;
			Grp()->Precision=1.0;
			if(nbsub==1)
				Grp()->Recall=1.0;
			Data->Precision+=Grp()->Precision;
			Data->Recall+=Grp()->Recall;
		}
		else
		{
			Sub=Grp()->Group->GetSubProfiles();
			for(Sub.Start();!Sub.End();Sub.Next())
			{
				thGrp=GetIdealGroup(Sub());
//				if((!thGrp)||(!thGrp->GetLang()!=Grp()->Group->GetLang()))
//					continue;
				if(!thGrp)
					continue;
				nbsub=thGrp->GetNbSubProfiles(Grp()->Group->GetLang());
				if(!nbsub)
					continue;
				if(nbsub==1)
				{
					Grp()->Recall+=1.0;
				}
				else
				{
					InthGrp=thGrp->GetNbSubProfiles(Grp()->Group)-1;
					if(InthGrp)
						Grp()->Precision+=((double)(InthGrp))/((double)(NbGrp-1));
					InGrp=Grp()->Group->GetNbSubProfiles(thGrp)-1;
					if(InGrp)
						Grp()->Recall+=((double)(InGrp))/((double)(nbsub-1));
				}
			}
			Data->Precision+=Grp()->Precision;
			Data->Recall+=Grp()->Recall;
			Grp()->Precision/=NbGrp;
			Grp()->Recall/=NbGrp;
		}
	}
	if(NbProf)
	{
		Data->Precision/=(double)NbProf;
		Data->Recall/=(double)NbProf;
	}
}


//-----------------------------------------------------------------------------
unsigned int GSubjects::GetNbIdealGroups(const GLang* lang) const
{
	unsigned int nb;

	RCursor<GSubject> Cur(*Top);
	for(Cur.Start(),nb=0;!Cur.End();Cur.Next())
		nb+=Cur()->GetNbIdealGroups(lang);
	return(nb);
}


//-----------------------------------------------------------------------------
void GSubjects::ComputeTotal(void)
{
//	RCursor<GGroup> GroupsIdeal;                 // Pointer to the ideal groups for a given language
	RCursor<GGroup> GroupsComputed;               // Pointer to the computed groups for a given language
	GGroup* GroupComputed;                        // Pointer to a computed group
	unsigned int NbRows,NbCols;                   // Rows and Cols for the current language for matrix
	unsigned int MaxRows,MaxCols;                 // Maximal Rows and Cols for matrix allocation
	unsigned int NbSubProfiles;                   // Total Number of subprofiles
	unsigned int NbTot;
	R::RCursor<GFactoryLang> Langs;
	GLang* lang;
	unsigned int col;
	double a,b,c,d,num,den,subtotal;
	double* VectorRows;                           // Sum of the rows of the matrix
	double* VectorCols;                           // Sum of the columns of the matrix
	double* VectorColsTemp;                       // temp sum of the columns of the matrix
	double* ptr;
	RCursor<GSubProfile> Sub;

	// Init part
	Data->Total=0.0;
	NbSubProfiles=0;

	// Go through the languages to define the maximal sizes and allocate the matrix
	MaxRows=MaxCols=0;
	Langs=(dynamic_cast<GLangManager*>(GPluginManagers::PluginManagers.GetManager("Lang")))->GetFactories();
	for(Langs.Start();!Langs.End();Langs.Next())
	{
		lang=Langs()->GetPlugin();
		if(!lang) continue;
//		GroupsIdeal=Data->IdealGroups->GetGroups(lang);
		NbRows=GetNbIdealGroups(lang);
		NbCols=Data->Session->GetGroups(lang).GetNb();
		if(NbRows>MaxRows) MaxRows=NbRows;
		if(NbCols>MaxCols) MaxCols=NbCols;
	}
	if((!MaxRows)||(!MaxCols))
		return;
	VectorRows=new double[MaxRows];
	VectorCols=new double[MaxCols];
	VectorColsTemp=new double[MaxCols];

	// Take the total for each languages multiplied by the number of
	// subprofiles in the idealgroup for this language.
	for(Langs.Start();!Langs.End();Langs.Next())
	{
		lang=Langs()->GetPlugin();
		if(!lang)
			continue;

		// Compute number of elements in ideal and computed groups.
		// and assign the groups to the current language.
		//GroupsIdeal=Data->IdealGroups->GetGroups(lang);
		//NbRows=GroupsIdeal.GetNb();
		NbRows=GetNbIdealGroups(lang);
		GroupsComputed=Data->Session->GetGroups(lang);
		NbCols=GroupsComputed.GetNb();
		if((!NbRows)||(!NbCols))
			continue;

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
		RCursor<GSubject> GroupsIdeal(*this);
		for(GroupsIdeal.Start(),NbTot=0;!GroupsIdeal.End();GroupsIdeal.Next())
		{
			if(!GroupsIdeal()->GetNbSubProfiles(lang))
				continue;
			memset(VectorColsTemp,0,NbCols*sizeof(double));
			Sub=GroupsIdeal()->GetSubProfiles(lang);
			for(Sub.Start();!Sub.End();Sub.Next())
			{
				VectorRows[row]++;
				NbTot++;
				GroupComputed=Data->Session->GetGroup(Sub()->GetGroupId());
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
		NbSubProfiles+=NbTot;
		Data->Total+=subtotal*NbTot;
	}

	// Compute Total
	if(NbSubProfiles)
		Data->Total=Data->Total/static_cast<double>(NbSubProfiles);
	else
		Data->Total=0.0;

	//delete the vectors
	if (VectorRows) delete[] VectorRows;
	if (VectorCols) delete[] VectorCols;
	if (VectorColsTemp) delete[] VectorColsTemp;
}


//------------------------------------------------------------------------------
void GSubjects::CreateIdeal(bool save)
{
	// Apply Config
	Apply();

	Data->SaveSimulation=save;

	// Re-init the session
	Data->Session->ReInit();

	// Remove the ideal clustering
//	GetIdealGroups()->ClearGroups();

	if(!Data->tmpDocs)
		Data->tmpDocs=new GDoc*[Data->Session->GetMaxPosDoc()+1];
	ChooseSubjects();
	CreateSet();
	if(Data->SaveSimulation)
	{
		Data->Session->GetStorage()->SaveFdbks(Data->Session);
//		Data->Session->GetStorage()->SaveIdealGroupment(Data->IdealGroups);
	}
}


//------------------------------------------------------------------------------
void GSubjects::FdbksCycle(void)
{
	R::RCursor<GGroup> Grps;
	RCursor<GSubProfile> SubProfile;
	unsigned int i;

	// Apply Config
	Apply();

	// Go through the languages
	Grps=Data->Session->GetGroups();
	for(Grps.Start();!Grps.End();Grps.Next())
	{
		// Go through the subprofile contained in the group.
		SubProfile=Grps()->GetSubProfiles();
		for(SubProfile.Start();!SubProfile.End();SubProfile.Next())
		{
			Grps()->NotJudgedDocsRelList(&Data->NewDocs,SubProfile(),Data->Session);
			RCursor<GFdbk> Cur(Data->NewDocs);
			for(Cur.Start(),i=Data->NbDocsAssess+1;(!Cur.End())&&(--i);Cur.Next())
			{
				GDoc* doc=Data->Session->GetDoc(Cur()->GetDocId());
				if(!doc)
					continue;
				// Look if 'OK'
				if(IsFromSubject(Cur()->GetDocId(),GetSubject(SubProfile())))
				{
					Data->Session->InsertFdbk(SubProfile()->GetProfile()->GetId(),Cur()->GetDocId(),GFdbk::ErrorJudgment(djOK,Data->PercErr,Data->Session->GetRandom()),RDate::GetToday(),doc->GetUpdated());
				}
				else
				{
					// Look If 'KO'
					if(IsFromParentSubject(Cur()->GetDocId(),GetSubject(SubProfile())))
					{
						Data->Session->InsertFdbk(SubProfile()->GetProfile()->GetId(),Cur()->GetDocId(),GFdbk::ErrorJudgment(djKO,Data->PercErr,Data->Session->GetRandom()),RDate::GetToday(),doc->GetUpdated());
					}
					else
					{
						// Must be H
						Data->Session->InsertFdbk(SubProfile()->GetProfile()->GetId(),Cur()->GetDocId(),GFdbk::ErrorJudgment(djOutScope,Data->PercErr,Data->Session->GetRandom()),RDate::GetToday(),doc->GetUpdated());
					}
				}
			}
			SubProfile()->SetState(osModified);
		}
	}

	if(Data->SaveSimulation)
		Data->Session->GetStorage()->SaveFdbks(Data->Session);
}


//------------------------------------------------------------------------------
void GSubjects::AddAssessments(void)
{
	R::RCursor<GSubject> Subs;
	RCursor<GProfile> Prof;
	unsigned int i;
	GDoc** ptr;

	// Apply Config
	Apply();

	// Go through all the subjects which are used
	Subs.Set(*this);
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
	if(Data->SaveSimulation)
	{
		Data->Session->GetStorage()->SaveFdbks(Data->Session);
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
	RContainer<GroupLang,true,true> Groups(20);          // Groups created for a given subject

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

	// Number of documents to judged by each subprofile
	maxDocsOK=static_cast<unsigned int>(newSubject->GetNbDocs()*Data->PercOK/100);
	maxDocsKO=static_cast<unsigned int>(newSubject->GetNbDocs()*Data->PercKO/100);
	maxDocsH=static_cast<unsigned int>(maxDocsOK*Data->PercH/100);

	// Assess documents
	Prof=newSubject->GetProfiles();
	for(Prof.Start();!Prof.End();Prof.Next())
		ProfileAssess(Groups,Prof(),newSubject,maxDocsOK,maxDocsKO,maxDocsH);

	if(Data->SaveSimulation)
	{
		Data->Session->GetStorage()->SaveFdbks(Data->Session);
//		Data->Session->GetStorage()->SaveIdealGroupment(Data->IdealGroups);
	}
	return(true);
}


//------------------------------------------------------------------------------
unsigned int GSubjects::AddProfiles(void)
{
	GSubject** tab;
	GSubject** ptr;
	unsigned int i;
	unsigned int nbprof, nbsocial, nbprofilescreated;
	GSubject* usedSubject;
	RCursor<GProfile> Prof;
	R::RCursor<GGroup> CurGrps;
	GGroup* Grp;
	unsigned int maxDocsOK,maxDocsKO,maxDocsH;
	RCursor<GSubProfile> Sub;
	RContainer<GroupLang,true,true> Groups(20);          // Groups created for a given subject

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

	// Catch all the ideal groups for this subject
	// It is suppose that the subject of the group is the subject of any
	// subprofile contained in it.
/*	RCursor<GFactoryLang> Langs=(dynamic_cast<GLangManager*>(GPluginManagers::PluginManagers.GetManager("Lang")))->GetFactories();
	for(Langs.Start();!Langs.End();Langs.Next())
	{
		GLang* lang=Langs()->GetPlugin();
		if(!lang) continue;
		CurGrps=Data->IdealGroups->GetGroups(lang);
		for(CurGrps.Start(),Grp=0;!CurGrps.End();CurGrps.Next())
		{
			Sub=CurGrps()->GetSubProfiles();
			Sub.Start();
			if(Sub.End()) continue;
			if(GetSubject(Sub())->GetId()==usedSubject->GetId())
			{
				Groups.InsertPtr(new GroupLang(lang,CurGrps()));
				break;
			}
		}
	}*/

	// Copy the documents of the same language of the session in Docs;
	Data->NbDocs=Data->Session->FillDocs(Data->tmpDocs);

	// Number of documents to judged by each subprofile
	maxDocsOK=static_cast<unsigned int>(usedSubject->GetNbDocs()*Data->PercOK/100);
	maxDocsKO=static_cast<unsigned int>(usedSubject->GetNbDocs()*Data->PercKO/100);
	maxDocsH=static_cast<unsigned int>(maxDocsOK*Data->PercH/100);

	usedSubject->SetUsed(Data->Session,nbprof,nbsocial);

	// Assess documents
	Prof=usedSubject->GetProfiles();
	for(Prof.Start();!Prof.End();Prof.Next())
		ProfileAssess(Groups,Prof(),usedSubject,maxDocsOK,maxDocsKO,maxDocsH);

	// optional saving
	if(Data->SaveSimulation)
	{
		Data->Session->GetStorage()->SaveFdbks(Data->Session);
//		Data->Session->GetStorage()->SaveIdealGroupment(Data->IdealGroups);
	}

	//returns the number of created profiles
	return(nbprofilescreated);
}


//------------------------------------------------------------------------------
double GSubjects::ComputePercAss(void)
{
	RCursor<GSubProfile> Cur1;
	R::RCursor<GProfile> Cur2;
	GSubProfile* Sub;
	unsigned int nb;
	double PercAss;
	GLang* Lang;

	Cur1.Set(Data->LastAdded);
	Cur2=Data->Session->GetProfiles();
	if(Cur1.GetNb()<1)
	{
		PercAss=1.0;
		return(1.0);
	}

	// Go through the new created subprofiles
	for(Cur1.Start(),PercAss=0.0,nb=0;!Cur1.End();Cur1.Next())
	{
		Lang=Cur1()->GetLang();

		// Go through all the subprofiles
		for(Cur2.Start();!Cur2.End();Cur2.Next())
		{
			// Get the subprofile corresponding to Lang and verify
			// that it is defined
			Sub=Cur2()->GetSubProfile(Lang);
			if(!Sub->IsDefined()) continue;

			// If same subprofile, skip it.
			if(Cur1()==Sub) continue;

			// If both subprofiles are not related to the same topic, skip it.
			if(GetSubject(Cur1())!=GetSubject(Sub)) continue;

			// Make comparaisons
			nb++;
			if(Cur1()->GetGroupId()==Sub->GetGroupId()) PercAss+=1.0;
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
	return(RTree<GSubject,true,false>::GetPtr<unsigned int>(id));
}


//------------------------------------------------------------------------------
void GSubjects::Clear(void)
{
	RTree<GSubject,true,false>::Clear();
	Data->Profiles.Clear();
}


//------------------------------------------------------------------------------
void GSubjects::Compare(void)
{
	R::RCursor<GGroup> Cur;

	Data->GroupsScore.Clear();
	Cur=Data->Session->GetGroups();
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		Data->GroupsScore.InsertPtr(new GroupScore(Cur()));
	}
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
/*GGroups* GSubjects::GetIdealGroups(void) const
{
	if(!Data->IdealGroups)
	{
		Data->IdealGroups=new GGroups(Data->Session->GetStorage(),0);
		Data->Session->GetStorage()->LoadIdealGroupment(Data->Session);
	}
	return(Data->IdealGroups);
}*/


//------------------------------------------------------------------------------
void GSubjects::InsertProfileSubject(GProfile* profile,unsigned int subjectid)
{
	GSubject* subject=RTree<GSubject,true,false>::GetPtr<unsigned int>(subjectid);

	if(!subject)
		return;
	Data->Profiles.InsertPtrAt(subject,profile->GetId(),true);
	subject->Insert(profile);
}


//------------------------------------------------------------------------------
void GSubjects::InsertSubProfileSubject(GSubProfile* subprofile,unsigned int subjectid)
{
	GSubject* subject=RTree<GSubject,true,false>::GetPtr<unsigned int>(subjectid);

	if(!subject)
		return;
	subject->InsertSubProfile(subprofile);
}


//------------------------------------------------------------------------------
GSubject* GSubjects::GetSubject(GSubProfile* sub)
{
	if(!sub)
		return(0);
	return(Data->Profiles[sub->GetProfile()->GetId()]);
}


//------------------------------------------------------------------------------
void GSubjects::InsertDocSubject(GDoc* doc,unsigned int subjectid)
{
	GSubject* subject=RTree<GSubject,true,false>::GetPtr<unsigned int>(subjectid);
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
	if(!s->Parent)
		return(false);
	if(Data->Docs.GetMaxPos()<docid)
		return(false);
	R::RContainer<GSubject,false,false>* line=Data->Docs[docid];
	if(!line)
		return(false);
	R::RCursor<GSubject> Sub;

	Sub.Set(*s->Parent);
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
