/*

	GALILEI Research Project

	GSubjectTree.cpp

	Tree of Subjects - Implementation.

	Copyright 2002 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).
		David Wartel (dwartel@ulb.ac.be).
		Julien Lamoral (jlamoral@ulb.ac.be).

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
#include <groups/gsubjecttree.h>
#include <groups/gsubject.h>
#include <groups/ggroups.h>
#include <docs/gdoc.h>
#include <sessions/gsession.h>
#include <infos/glang.h>
#include <infos/glangmanager.h>
#include <profiles/gprofile.h>
#include <profiles/gsubprofile.h>
#include <profiles/gprofdoc.h>
using namespace R;
using namespace GALILEI;



//-----------------------------------------------------------------------------
//
//  GGroupId
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
class GSubjectTree::GGroupId
{
public:
	/**
	*the id of the groupment.
	*/
	int GrpId;

	/**
	*the position of the group.
	*/
	int position;

	/**
	* Construct the Real ID .
	* @Real Id              the id of the groupment.
	*/
	GGroupId(int RealId,int Position) : GrpId(RealId), position(Position) {}

	int Compare(const GGroupId* grp) const {return(GrpId-grp->GrpId);}

	int Compare(const int ID) const {return(GrpId-ID);}

	int Compare(const GGroupId& grp) const {return(GrpId-grp.GrpId);}

};



//-----------------------------------------------------------------------------
//
//  GroupScore
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
class GSubjectTree::GroupScore
{
public:
	GGroup* Group;
	double Precision;
	double Recall;

	GroupScore(GGroup* grp) : Group(grp), Precision(0.0), Recall(0.0) {}
	int Compare(const GroupScore* grp) const {return(Group->Compare(grp->Group));}
	int Compare(const GGroup* grp) const {return(Group->Compare(grp));}
};



//-----------------------------------------------------------------------------
//
// class GSubjectTree
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GSubjectTree::GSubjectTree(GSession* session)
	: RTree<GSubject,true,false>(100,50),GParams("Subjects"), Session(session),
	  Docs(0), NbDocs(0), NewDocs(NbDocs), LastAdded(50,25), IdealGroups(100,50),
	  GroupsScore(100,50)
{
	GParams::InsertPtr(new GParamDouble("PercOK",10.0));
	GParams::InsertPtr(new GParamDouble("PercKO",10.0));
	GParams::InsertPtr(new GParamDouble("PercH",50.0));
	GParams::InsertPtr(new GParamDouble("PercErr",0.0));
	GParams::InsertPtr(new GParamUInt("NbProfMin",2));
	GParams::InsertPtr(new GParamUInt("NbProfMax",10));
	GParams::InsertPtr(new GParamDouble("PercSocial",100.0));
	GParams::InsertPtr(new GParamDouble("PercGrp",100.0));
	GParams::InsertPtr(new GParamUInt("NbMinDocsGrp",50));
	GParams::InsertPtr(new GParamUInt("NbDocsAssess",30));
	GParams::InsertPtr(new GParamBool("idf",true));
	Apply();
}


//-----------------------------------------------------------------------------
void GSubjectTree::Apply(void)
{
	PercOK=GetDouble("PercOK");
	PercKO=GetDouble("PercKO");
	PercH=GetDouble("PercH");
	PercErr=GetDouble("PercErr");
	NbProfMin=GetUInt("NbProfMin");
	NbProfMax=GetUInt("NbProfMax");
	PercSocial=GetDouble("PercSocial");
	PercGrp=GetDouble("PercGrp");
	NbMinDocsGrp=GetUInt("NbMinDocsGrp");
	NbDocsAssess=GetUInt("NbDocsAssess");
	Global=GetBool("idf");
}


//-----------------------------------------------------------------------------
void GSubjectTree::ChooseSubjects(void)
{
	GSubjectCursor Subs;
	unsigned int compt;
	GSubject** tab;
	GSubject** ptr;
	unsigned int i;

	// Suppose all subjects are not chosen
	Subs.Set(this);
	for(Subs.Start();!Subs.End();Subs.Next())
		Subs()->SetUsed(false);

	// Randomly mix the subjects in tab
	tab=new GSubject*[GetNbNodes()];
	memcpy(tab,RTree<GSubject,true,false>::Tab,sizeof(GSubject*)*(GetNbNodes()));
	Session->GetRandom()->RandOrder<GSubject*>(tab,GetNbNodes());

	// Choose the first percgrp subjects having at least NbMinDocsGrp documents.
	compt=static_cast<unsigned int>((GetNbNodes()*PercGrp)/100)+1;
	for(ptr=tab,i=GetNbNodes()+1;(--i)&&compt;ptr++)
	{
		if((*ptr)->GetNbDocs()<NbMinDocsGrp) continue;
		(*ptr)->SetUsed(true);
		compt--;
	}

	// delete tab;
	delete[] tab;
}


//-----------------------------------------------------------------------------
void GSubjectTree::CreateSet(void)
{
	GSubjectCursor Subs;
	GSubProfileCursor Prof;
	unsigned int nbprof,nbsocial;
	GGroups* CurGrps;
	GGroup* Grp;
	unsigned int maxDocsOK,maxDocsKO,maxDocsH;
	GFactoryLangCursor CurLang;
	GLang* lang;

	// Init Part
	LastAdded.Clear();
	IdealGroups.Clear();
	CurLang=Session->GetLangs()->GetLangsCursor();
	for(CurLang.Start();!CurLang.End();CurLang.Next())
	{
		lang=CurLang()->GetPlugin();
		if(!lang) continue;
		IdealGroups.InsertPtr(new GGroups(lang));
	}
	if(!Docs)
		Docs=new GDoc*[Session->GetNbDocs()];

//	Session->ClearFdbks();
//	Session->ReInit();
//	Session->ClearSubProfilesGroups();

	// Go through all the subjects which are used
	Subs.Set(this);
	for(Subs.Start();!Subs.End();Subs.Next())
	{
		if(!Subs()->IsUsed()) continue;

		// Create an ideal group for this subject
		CurGrps=IdealGroups.GetPtr<const GLang*>(Subs()->GetLang());
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
		nbsocial=static_cast<unsigned int>(nbprof*PercSocial/100);

		// Number of documents to judged by each subprofile
		maxDocsOK=static_cast<unsigned int>(Subs()->GetNbDocs()*PercOK/100);
		maxDocsKO=static_cast<unsigned int>(Subs()->GetNbDocs()*PercKO/100);
		maxDocsH=static_cast<unsigned int>(maxDocsOK*PercH/100);

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
}


//-----------------------------------------------------------------------------
void GSubjectTree::ProfileJudges(GProfile* prof,GSubject* sub,unsigned int maxDocsOK,unsigned int maxDocsKO,unsigned int maxDocsH)
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
				Session->InsertFdbk(prof,*ptr,GProfDoc::ErrorJudgment(djOK,PercErr,Session->GetRandom()),RDate::GetToday());
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
					Session->InsertFdbk(prof,*ptr,GProfDoc::ErrorJudgment(djKO,PercErr,Session->GetRandom()),RDate::GetToday());
				}
			}
			else
			{
				// Must be H
				if(nbDocsH)
				{
					nbDocsH--;
					Session->InsertFdbk(prof,*ptr,GProfDoc::ErrorJudgment(djOutScope,PercErr,Session->GetRandom()),RDate::GetToday());
				}
			}
		}
	}
}


//-----------------------------------------------------------------------------
GGroup* GSubjectTree::GetIdealGroup(GSubProfile* sub) const
{
	GGroups* grps;

	grps=IdealGroups.GetPtr<const GLang*>(sub->GetLang());
	if(!grps) return(0);
	return(grps->GetGroup(sub));
}


//-----------------------------------------------------------------------------
void GSubjectTree::ComputeRecallPrecision(GSlot* /*rec*/)
{
	GroupScoreCursor Grp;
	GSubProfileCursor Sub;
	GGroup* thGrp;
	unsigned int NbGrp;
	unsigned int InGrp;
	unsigned int InthGrp;
	unsigned int NbProf;

	Precision=Recall=0.0;
	NbProf=0;
	Grp.Set(GroupsScore);
	for(Grp.Start();!Grp.End();Grp.Next())
	{
		NbGrp=Grp()->Group->NbPtr;
		NbProf+=NbGrp;
		Grp()->Precision=Grp()->Recall=0.0;
		if(!NbGrp) continue;
		if(NbGrp==1)
		{
			thGrp=GetIdealGroup(Grp()->Group->Tab[0]);
			if((!thGrp)||(thGrp->GetLang()!=Grp()->Group->GetLang())) continue;
			Grp()->Precision=1.0;
			if(thGrp->NbPtr==1)
				Grp()->Recall=1.0;
			Precision+=Grp()->Precision;
			Recall+=Grp()->Recall;
		}
		else
		{
			Sub=Grp()->Group->GetSubProfilesCursor();
			for(Sub.Start();!Sub.End();Sub.Next())
			{
				thGrp=GetIdealGroup(Sub());
				if((!thGrp)||(thGrp->GetLang()!=Grp()->Group->GetLang())) continue;
				if(thGrp->NbPtr==1)
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
						Grp()->Recall+=((double)(InGrp))/((double)(thGrp->NbPtr-1));
				}
			}
			Precision+=Grp()->Precision;
			Recall+=Grp()->Recall;
			Grp()->Precision/=NbGrp;
			Grp()->Recall/=NbGrp;
		}
	}
	if(NbProf)
	{
		Precision/=(double)NbProf;
		Recall/=(double)NbProf;
	}
}


//-----------------------------------------------------------------------------
void GSubjectTree::ComputeTotal(GSlot* /*rec*/)
{
	GGroups* GroupsIdeal;                         // Pointer to the ideal groups for a given language
	GGroups* GroupsComputed;                      // Pointer to the computed groups for a given language
	GGroup* GroupIdeal;                           // Pointer to a ideal group
	GGroup* GroupComputed;                        // Pointer to a computed group
	unsigned int NbRows,NbCols;                   // Rows and Cols for the current language for matrix
	unsigned int MaxRows,MaxCols;                 // Maximal Rows and Cols for matrix allocation
	unsigned int NbProfiles;                      // Total Number of profiles
	unsigned int NbTot;
	GFactoryLangCursor Langs;
	GLang* lang;
	unsigned int col;
	double a,b,c,d,num,den,subtotal;
	double* VectorRows;                           // Sum of the rows of the matrix
	double* VectorCols;                           // Sum of the columns of the matrix
	double* VectorColsTemp;                       // temp sum of the columns of the matrix
	double* ptr;

	// Init part
	Total=0.0;
	NbProfiles=0;

	// Go through the languages to define the maximal sizes and allocate the matrix
	MaxRows=MaxCols=0;
	Langs=Session->GetLangs()->GetLangsCursor();
	for(Langs.Start();!Langs.End();Langs.Next())
	{
		lang=Langs()->GetPlugin();
		if(!lang) continue;
		GroupsIdeal=IdealGroups.GetPtr<GLang*>(lang);
		if(GroupsIdeal)
			NbRows=GroupsIdeal->NbPtr;
		else
			NbRows=0;
		NbCols=Session->GetGroups(lang)->NbPtr;
		if(NbRows>MaxRows) MaxRows=NbRows;
		if(NbCols>MaxCols) MaxCols=NbCols;
	}
	if((!MaxRows)||(!MaxCols)) return;
	VectorRows=new double[MaxRows];
	VectorCols=new double[MaxCols];
	VectorColsTemp=new double[MaxCols];

	// we take the total for each languages multiplied by the number of subprofiles
	// in the idealgroup for this language.
	for(Langs.Start();!Langs.End();Langs.Next())
	{
		lang=Langs()->GetPlugin();
		if(!lang) continue;

		// Compute number of elements in ideal and computed groups.
		// and assign the groups to the current language.
		GroupsIdeal=IdealGroups.GetPtr<GLang*>(lang);
		if(GroupsIdeal)
			NbRows=GroupsIdeal->NbPtr;
		else
			NbRows=0;
		GroupsComputed=Session->GetGroups(lang);
		NbCols=GroupsComputed->NbPtr;
		if((!NbRows)||(!NbCols)) continue;

		// Construction of the container for relation between id and column in the matrix.
		RContainer<GGroupId,unsigned int,true,true> GroupsId(NbCols,NbCols/2);
		for(GroupsComputed->Start(),col=0;!GroupsComputed->End();GroupsComputed->Next())
			GroupsId.InsertPtr(new GGroupId(((*GroupsComputed)())->GetId(),col++));

		//Initialisation of the variable used for computing the subtotal
		a=b=c=d=0.0;

		// Initalisation of the vectors
		memset(VectorRows,0,NbRows*sizeof(double));
		memset(VectorCols,0,NbCols*sizeof(double));

		//for each group of ideal group and for each profiles in this group compute the differents terms of the total
		int row,position;
		row=0;
		for(GroupsIdeal->Start(),NbTot=0;!GroupsIdeal->End();GroupsIdeal->Next())
		{
			memset(VectorColsTemp,0,NbCols*sizeof(double));
			GroupIdeal=(*GroupsIdeal)();
			for(GroupIdeal->Start();!GroupIdeal->End();GroupIdeal->Next())
			{
				GroupComputed=GroupsComputed->GetGroup((*GroupIdeal)());
				if(GroupComputed)
				{
					position=GroupsId.GetPtr(GroupComputed->GetId())->position;
					VectorCols[position]++;
					VectorColsTemp[position]++;
				}
				VectorRows[row]++;
				NbTot++;
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
		Total+=subtotal*NbTot;
	}

	// Compute Total
	if(NbProfiles)
		Total=Total/(double)NbProfiles;
	else
		Total=0.0;

	//delete the vectors
	if (VectorRows) delete[] VectorRows;
	if (VectorCols) delete[] VectorCols;
	if (VectorColsTemp) delete[] VectorColsTemp;
}


//-----------------------------------------------------------------------------
void GSubjectTree::CreateIdeal(bool Save)
{
	// Apply Config
	Apply();

	// re-init the session
	Session->ReInit(Save);
	if(!Docs)
		Docs=new GDoc*[Session->GetNbDocs()];
	ChooseSubjects();
	CreateSet();
	if(Save)
	{
		Session->SaveFdbks();
		Session->SaveIdealGroupment(&IdealGroups);
	}
}


//-----------------------------------------------------------------------------
void GSubjectTree::FdbksCycle(bool Save)
{
	GGroupsCursor Grps;
	GSubProfileCursor SubProfile;
	GGroupCursor Grp;
	unsigned int i;

	// Apply Config
	Apply();

	// Go through the languages
	Grps=Session->GetGroupsCursor();
	for(Grps.Start();!Grps.End();Grps.Next())
	{
		// Go through the groups of the current language.
		Grp=Grps()->GetGroupCursor();
		for(Grp.Start();!Grp.End();Grp.Next())
		{
			// Go through the subprofile contained in the group.
			SubProfile=Grp()->GetSubProfilesCursor();
			for(SubProfile.Start();!SubProfile.End();SubProfile.Next())
			{
				Grp()->NotJudgedDocsRelList(&NewDocs,SubProfile(),Global);
				for(NewDocs.Start(),i=NbDocsAssess+1;(!NewDocs.End())&&(--i);NewDocs.Next())
				{
					// Look if 'OK'
					if(NewDocs()->GetDoc()->IsFromSubject(SubProfile()->GetSubject()))
					{
						Session->InsertFdbk(SubProfile()->GetProfile(),NewDocs()->GetDoc(),GProfDoc::ErrorJudgment(djOK,PercErr,Session->GetRandom()),RDate::GetToday());
					}
					else
					{
						// Look If 'KO'
						if(NewDocs()->GetDoc()->IsFromParentSubject(SubProfile()->GetSubject()))
						{
							Session->InsertFdbk(SubProfile()->GetProfile(),NewDocs()->GetDoc(),GProfDoc::ErrorJudgment(djKO,PercErr,Session->GetRandom()),RDate::GetToday());
						}
						else
						{
							// Must be H
							Session->InsertFdbk(SubProfile()->GetProfile(),NewDocs()->GetDoc(),GProfDoc::ErrorJudgment(djOutScope,PercErr,Session->GetRandom()),RDate::GetToday());
						}
					}
				}
				SubProfile()->SetState(osModified);
			}
		}
	}

	if(Save)
		Session->SaveFdbks();
}


//-----------------------------------------------------------------------------
void GSubjectTree::AddAssessments(bool Save)
{
	GSubjectCursor Subs;
	GSubProfileCursor Prof;
	unsigned int i;
	GDoc** ptr;

	// Apply Config
	Apply();

	// Go through all the subjects which are used
	Subs.Set(this);
	for(Subs.Start();!Subs.End();Subs.Next())
	{
		if(!Subs()->IsUsed()) continue;

		// Copy the documents of the same language of the session in Docs;
		NbDocs=Session->FillDocs(Docs,Subs()->GetLang());

		// Go through the subprofiles attached to the subject
		Prof=Subs()->GetSubProfilesCursor();
		for(Prof.Start();!Prof.End();Prof.Next())
		{
			// Look if current subprofile has already judged documents.
			if(!Prof()->GetNbJudgedDocs()) continue;

			// Mix the documents
			Session->GetRandom()->RandOrder<GDoc*>(Docs,NbDocs);

			// Go trought the documents to create the judgements
			for(i=NbDocsAssess+1,ptr=Docs;--i;ptr++)
			{
				// Verify that the document is not already assigned to the profile
				if(Prof()->GetProfile()->GetFeedback(*ptr)) continue;

				// Look if 'OK'
				if((*ptr)->IsFromSubject(Subs()))
				{
					Session->InsertFdbk(Prof()->GetProfile(),*ptr,GProfDoc::ErrorJudgment(djOK,PercErr,Session->GetRandom()),RDate::GetToday());
				}
				else
				{
					// Look If 'KO'
					if((*ptr)->IsFromParentSubject(Subs()))
					{
						Session->InsertFdbk(Prof()->GetProfile(),*ptr,GProfDoc::ErrorJudgment(djKO,PercErr,Session->GetRandom()),RDate::GetToday());
					}
					else
					{
						Session->InsertFdbk(Prof()->GetProfile(),*ptr,GProfDoc::ErrorJudgment(djOutScope,PercErr,Session->GetRandom()),RDate::GetToday());
					}
				}
			}
		}
	}
	if(Save)
	{
		Session->SaveFdbks();
	}
}


//-----------------------------------------------------------------------------
bool GSubjectTree::AddTopic(bool Save)
{
	GSubject** tab;
	GSubject** ptr;
	unsigned int i;
	GSubject* newSubject;
	GSubProfileCursor Prof;
	unsigned int nbprof,nbsocial;
	GGroups* CurGrps;
	GGroup* Grp;
	unsigned int maxDocsOK,maxDocsKO,maxDocsH;

	// Apply Config
	Apply();

	// Randomly mix the subjects in tab
	tab=new GSubject*[GetNbNodes()];
	memcpy(tab,RTree<GSubject,true,false>::Tab,sizeof(GSubject*)*(GetNbNodes()));
	Session->GetRandom()->RandOrder<GSubject*>(tab,GetNbNodes());

	// Find the first not used subject having at least NbMinDocsGrp documents.
	for(ptr=tab,i=GetNbNodes()+1,newSubject=0;--i;ptr++)
	{
		if((*ptr)->GetNbDocs()<NbMinDocsGrp) continue;
		if((*ptr)->IsUsed()) continue;
		newSubject=(*ptr);
		break;
	}

	// delete tab;
	delete[] tab;

	// If no subject found -> do nothing
	if(!newSubject) return(false);
	newSubject->SetUsed(true);

	// Create an ideal group for this subject
	CurGrps=IdealGroups.GetPtr<const GLang*>(newSubject->GetLang());
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
	nbsocial=static_cast<unsigned int>(nbprof*PercSocial/100);

	// Number of documents to judged by each subprofile
	maxDocsOK=static_cast<unsigned int>(newSubject->GetNbDocs()*PercOK/100);
	maxDocsKO=static_cast<unsigned int>(newSubject->GetNbDocs()*PercKO/100);
	maxDocsH=static_cast<unsigned int>(maxDocsOK*PercH/100);

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

		// Insert current subprofile in the ideal group and are hold as last inserted.
		Grp->InsertPtr(Prof());
		LastAdded.InsertPtr(Prof());

		// Judges documents
		ProfileJudges(Prof()->GetProfile(),newSubject,maxDocsOK,maxDocsKO,maxDocsH);
	}
	if(Save)
	{
		Session->SaveFdbks();
		Session->SaveIdealGroupment(&IdealGroups);
	}
	return(true);
}


//-----------------------------------------------------------------------------
unsigned int GSubjectTree::AddProfiles(bool Save)
{
	GSubject** tab;
	GSubject** ptr;
	unsigned int i;
	unsigned int nbprof, nbsocial, nbprofilescreated;
	GSubject* usedSubject;
	GSubProfileCursor Prof;
	GGroups* CurGrps;
	GGroup* Grp;
	unsigned int maxDocsOK,maxDocsKO,maxDocsH;

	// Apply Config
	Apply();

	//Randomly choose the number of profiles.
	nbprof=Session->GetCurrentRandomValue(NbProfMax-NbProfMin+1)+NbProfMin;

	// Randomly mix the subjects in tab
	tab=new GSubject*[GetNbNodes()];
	memcpy(tab,RTree<GSubject,true,false>::Tab,sizeof(GSubject*)*(GetNbNodes()));
	Session->GetRandom()->RandOrder<GSubject*>(tab,GetNbNodes());

	// Find the first used subject having at least NbMinDocsGrp documents.
	for(ptr=tab,i=GetNbNodes()+1,usedSubject=0;--i;ptr++)
	{
		if(!(*ptr)->IsUsed()) continue;  //check if the subject is already in use..
		usedSubject=(*ptr);
		break;
	}

	// delete tab;
	delete[] tab;

	// If no subject found -> do nothing
	if(!usedSubject) return 0;

	// catch the ideal group for this subject
	CurGrps=IdealGroups.GetPtr<const GLang*>(usedSubject->GetLang());
	for(CurGrps->Start(),Grp=0;!CurGrps->End();CurGrps->Next())
	{
		if ( (*(*CurGrps)()->Tab)->GetSubject()->GetId()==usedSubject->GetId())
		{
			Grp=(*CurGrps)();
			break;
		}
	}

	// Copy the documents of the same language of the session in Docs;
	NbDocs=Session->FillDocs(Docs,usedSubject->GetLang());

	// Number of profiles that are social
	nbsocial=static_cast<unsigned int>(nbprof*PercSocial/100);

	// Number of documents to judged by each subprofile
	maxDocsOK=static_cast<unsigned int>(usedSubject->GetNbDocs()*PercOK/100);
	maxDocsKO=static_cast<unsigned int>(usedSubject->GetNbDocs()*PercKO/100);
	maxDocsH=static_cast<unsigned int>(maxDocsOK*PercH/100);

	// Go through the nbprof first subprofiles attached to the subject
	nbprofilescreated=0;
	Prof=usedSubject->GetSubProfilesCursor();
	Prof.Start();
	while((!Prof.End())&&(nbprofilescreated<nbprof))
	{
		// check if the profile is free
		if (Prof()->GetProfile()->GetNbJudgedDocs(usedSubject->GetLang()))
		{
			Prof.Next(); continue;
		}
		// Look if current subprofile is social or not
		if(nbsocial)
		{
			Prof()->GetProfile()->SetSocial(true);
			nbsocial--;
		}
		else
			Prof()->GetProfile()->SetSocial(false);

		// Insert current subprofile in the ideal group and are hold as last inserted.
		Grp->InsertPtr(Prof());
		LastAdded.InsertPtr(Prof());

		// Judges documents
		ProfileJudges(Prof()->GetProfile(),usedSubject,maxDocsOK,maxDocsKO,maxDocsH);

		//increment Prof and number of created profiles
		Prof.Next();
		nbprofilescreated++;
	}

	// optional saving
	if(Save)
	{
		Session->SaveFdbks();
		Session->SaveIdealGroupment(&IdealGroups);
	}

	//returns the number of created profiles
	return(nbprofilescreated);
}


//-----------------------------------------------------------------------------
double GSubjectTree::ComputePercAss(void)
{
	GSubProfileCursor Cur1;
	GProfileCursor Cur2;
	GSubProfile* Sub;
	unsigned int nb;
	double PercAss;
	GLang* Lang;

	Cur1.Set(LastAdded);
	Cur2=Session->GetProfilesCursor();
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
			if(Cur1()->GetSubject()!=Sub->GetSubject()) continue;

			// Make comparaisons
			nb++;
			if(Cur1()->GetGroup()==Sub->GetGroup()) PercAss+=1.0;
		}
	}
	if(!nb)
		PercAss=1.0;
	else
		PercAss/=nb;
	return(PercAss);
}


//-----------------------------------------------------------------------------
void GSubjectTree::ClearLastAdded(void)
{
	LastAdded.Clear();
}


//-----------------------------------------------------------------------------
GSubject* GSubjectTree::GetSubject(unsigned int id)
{
	return(RTree<GSubject,true,false>::GetPtr<unsigned int>(id));
}


//-----------------------------------------------------------------------------
void GSubjectTree::ClearSubjects(void)
{
	RTree<GSubject,true,false>::Clear();
}


//-----------------------------------------------------------------------------
void GSubjectTree::Compare(GSlot* rec)
{
	GGroupsCursor Cur;
	GGroupCursor Grp;

	GroupsScore.Clear();
	Cur=Session->GetGroupsCursor();
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		Grp=Cur()->GetGroupCursor();
		for(Grp.Start();!Grp.End();Grp.Next())
			GroupsScore.InsertPtr(new GroupScore(Grp()));
	}
	ComputeRecallPrecision(rec);
	ComputeTotal(rec);
}


//-----------------------------------------------------------------------------
double GSubjectTree::GetPrecision(GGroup* grp)
{
	GroupScore* g=GroupsScore.GetPtr<const GGroup*>(grp);

	if(!g) return(0.0);
	return(g->Precision);
}


//-----------------------------------------------------------------------------
double GSubjectTree::GetRecall(GGroup* grp)
{
	GroupScore* g=GroupsScore.GetPtr<const GGroup*>(grp);

	if(!g) return(0.0);
	return(g->Recall);
}


//-----------------------------------------------------------------------------
RContainer<GGroups,unsigned int,true,true>* GSubjectTree::GetIdealGroups(void)
{
	return(&IdealGroups);
}


//-----------------------------------------------------------------------------
GGroupsCursor& GSubjectTree::GetIdealGroupsCursor(void)
{
	GGroupsCursor *cur=GGroupsCursor::GetTmpCursor();
	cur->Set(IdealGroups);
	return(*cur);
}


//-----------------------------------------------------------------------------
GSubjectTree::~GSubjectTree(void)
{
	if(Docs)
		delete[] Docs;
}
