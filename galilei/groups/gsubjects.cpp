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
#include <groups/gsubjects.h>
#include <groups/gsubject.h>
#include <groups/ggroups.h>
#include <docs/gdoc.h>
#include <sessions/gsession.h>
#include <sessions/gstorage.h>
#include <infos/glang.h>
#include <infos/glangmanager.h>
#include <profiles/gprofile.h>
#include <profiles/gsubprofile.h>
#include <profiles/gprofdoc.h>
using namespace R;
using namespace GALILEI;



//------------------------------------------------------------------------------
//
//  GGroupId
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
class GSubjects::GGroupId
{
public:
	int GrpId;       // Group Identificator.
	int position;    // Position of the group.

	// Constructor and Compare methods.
	GGroupId(int RealId,int Position) : GrpId(RealId), position(Position) {}
	int Compare(const GGroupId* grp) const {return(GrpId-grp->GrpId);}
	int Compare(const int ID) const {return(GrpId-ID);}
	int Compare(const GGroupId& grp) const {return(GrpId-grp.GrpId);}
};



//------------------------------------------------------------------------------
//
//  GroupScore
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
class GSubjects::GroupScore
{
public:
	GGroup* Group;                 // Pointer to the group.
	double Precision;              // Precision inside the group.
	double Recall;                 // Recall inside the group.

	// Constructor and Compare methods.
	GroupScore(GGroup* grp) : Group(grp), Precision(0.0), Recall(0.0) {}
	int Compare(const GroupScore* grp) const {return(Group->Compare(grp->Group));}
	int Compare(const GGroup* grp) const {return(Group->Compare(grp));}
};



//------------------------------------------------------------------------------
//
// class GSubjects
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GSubjects::GSubjects(GSession* session) throw(std::bad_alloc)
	: RTree<GSubject,true,false>(100,50), GParams("Subjects"), Session(session),
	  Docs(0), NbDocs(0), NewDocs(NbDocs), LastAdded(50,25), IdealGroups(0),
	  GroupsScore(100,50)
{
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
	GParams::InsertPtr(new GParamBool("idf",true));
	IdealGroups=new GGroups(100);
	Apply();
}


//------------------------------------------------------------------------------
void GSubjects::Apply(void)
{
	PercOK=GetDouble("PercOK");
	PercKO=GetDouble("PercKO");
	PercH=GetDouble("PercH");
	PercErr=GetDouble("PercErr");
	NbProfMin=GetUInt("NbProfMin");
	NbProfMax=GetUInt("NbProfMax");
	PercSocial=GetDouble("PercSocial");
	PercSubjects=GetDouble("PercSubjects");
	NbMinDocsSubject=GetUInt("NbMinDocsSubject");
	NbDocsAssess=GetUInt("NbDocsAssess");
	IFF=GetBool("idf");
}


//------------------------------------------------------------------------------
void GSubjects::ChooseSubjects(void) throw(std::bad_alloc)
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

	// Choose the first percgrp subjects having at least NbMinDocsSubject documents.
	compt=static_cast<unsigned int>((GetNbNodes()*PercSubjects)/100)+1;
	for(ptr=tab,i=GetNbNodes()+1;(--i)&&compt;ptr++)
	{
		if((*ptr)->GetNbDocs()<NbMinDocsSubject) continue;
		(*ptr)->SetUsed(true);
		compt--;
	}

	// delete tab;
	delete[] tab;
}


//------------------------------------------------------------------------------
void GSubjects::CreateSet(void) throw(std::bad_alloc)
{
	GSubjectCursor Subs;
	GSubProfileCursor Prof;
	unsigned int nbprof,nbsocial;
//	GGroups* CurGrps;
	GGroup* Grp;
	unsigned int maxDocsOK,maxDocsKO,maxDocsH;
	GFactoryLangCursor CurLang;
//	GLang* lang;

	// Init Part
	LastAdded.Clear();
	IdealGroups->Clear();
	CurLang=Session->GetLangs()->GetLangsCursor();
/*	for(CurLang.Start();!CurLang.End();CurLang.Next())
	{
		lang=CurLang()->GetPlugin();
		if(!lang) continue;
		IdealGroups.InsertPtr(new GGroups(lang));
	}*/
	if(!Docs)
		Docs=new GDoc*[Session->GetNbDocs()];

//	Session->ClearFdbks();
	Session->ReInit(false);
//	Session->ClearSubProfilesGroups();

	// Go through all the subjects which are used
	Subs.Set(this);
	for(Subs.Start();!Subs.End();Subs.Next())
	{
		if(!Subs()->IsUsed()) continue;

		// Create an ideal group for this subject
		IdealGroups->InsertGroup(Grp=new GGroupVector(IdealGroups->GetNbGroups(),Subs()->GetLang(),false));

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
			Grp->InsertSubProfile(Prof());

			// Judges documents
			ProfileJudges(Prof()->GetProfile(),Subs(),maxDocsOK,maxDocsKO,maxDocsH);
		}
	}
}


//------------------------------------------------------------------------------
void GSubjects::ProfileJudges(GProfile* prof,GSubject* sub,unsigned int maxDocsOK,unsigned int maxDocsKO,unsigned int maxDocsH) throw(std::bad_alloc)
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


//------------------------------------------------------------------------------
GGroup* GSubjects::GetIdealGroup(GSubProfile* sub) const throw(GException)
{
	return(IdealGroups->GetGroup(sub));
}


//------------------------------------------------------------------------------
void GSubjects::ComputeRecallPrecision(void)
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
		NbGrp=Grp()->Group->GetNbSubProfiles();
		NbProf+=NbGrp;
		Grp()->Precision=Grp()->Recall=0.0;
		if(!NbGrp) continue;
		if(NbGrp==1)
		{
			Sub=Grp()->Group->GetSubProfilesCursor();
			Sub.Start();
			thGrp=GetIdealGroup(Sub());
			if((!thGrp)||(thGrp->GetLang()!=Grp()->Group->GetLang())) continue;
			Grp()->Precision=1.0;
			if(thGrp->GetNbSubProfiles()==1)
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
				if(thGrp->GetNbSubProfiles()==1)
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
						Grp()->Recall+=((double)(InGrp))/((double)(thGrp->GetNbSubProfiles()-1));
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
void GSubjects::ComputeTotal(void)
{
	GGroupCursor GroupsIdeal;                     // Pointer to the ideal groups for a given language
	GGroupCursor GroupsComputed;                  // Pointer to the computed groups for a given language
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
	GSubProfileCursor Sub;

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
		GroupsIdeal=IdealGroups->GetGroupsCursor(lang);
		NbRows=GroupsIdeal.GetNb();
		NbCols=Session->GetGroupsCursor(lang).GetNb();
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
		GroupsIdeal=IdealGroups->GetGroupsCursor(lang);
		NbRows=GroupsIdeal.GetNb();
		GroupsComputed=Session->GetGroupsCursor(lang);
		NbCols=GroupsComputed.GetNb();
		if((!NbRows)||(!NbCols)) continue;

		// Construction of the container for relation between id and column in the matrix.
		RContainer<GGroupId,true,true> GroupsId(NbCols,NbCols/2);
		for(GroupsComputed.Start(),col=0;!GroupsComputed.End();GroupsComputed.Next())
			GroupsId.InsertPtr(new GGroupId((GroupsComputed())->GetId(),col++));

		//Initialisation of the variable used for computing the subtotal
		a=b=c=d=0.0;

		// Initalisation of the vectors
		memset(VectorRows,0,NbRows*sizeof(double));
		memset(VectorCols,0,NbCols*sizeof(double));

		//for each group of ideal group and for each profiles in this group compute the differents terms of the total
		int row,position;
		row=0;
		for(GroupsIdeal.Start(),NbTot=0;!GroupsIdeal.End();GroupsIdeal.Next())
		{
			memset(VectorColsTemp,0,NbCols*sizeof(double));
			Sub=GroupsIdeal()->GetSubProfilesCursor();
			for(Sub.Start();!Sub.End();Sub.Next())
			{
				GroupComputed=0;
				for(GroupsComputed.Start();!GroupsComputed.End()&&!GroupComputed;GroupsComputed.Next())
					if((GroupsComputed)()->IsIn(Sub()))
						GroupComputed=(GroupsComputed)();
				//GroupComputed=(*GroupIdeal)()->GetGroup();//GroupsComputed->GetGroup((*GroupIdeal)());
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


//------------------------------------------------------------------------------
void GSubjects::CreateIdeal(bool Save) throw(std::bad_alloc)
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
		Session->GetStorage()->SaveFdbks(Session);
		Session->GetStorage()->SaveIdealGroupment(IdealGroups);
	}
}


//------------------------------------------------------------------------------
void GSubjects::FdbksCycle(bool Save) throw(std::bad_alloc)
{
	GGroupCursor Grps;
	GSubProfileCursor SubProfile;
	unsigned int i;

	// Apply Config
	Apply();

	// Go through the languages
	Grps=Session->GetGroupsCursor();
	for(Grps.Start();!Grps.End();Grps.Next())
	{
		// Go through the subprofile contained in the group.
		SubProfile=Grps()->GetSubProfilesCursor();
		for(SubProfile.Start();!SubProfile.End();SubProfile.Next())
		{
			Grps()->NotJudgedDocsRelList(&NewDocs,SubProfile(),Session);
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

	if(Save)
		Session->GetStorage()->SaveFdbks(Session);
}


//------------------------------------------------------------------------------
void GSubjects::AddAssessments(bool Save) throw(std::bad_alloc)
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
			if(!Prof()->GetNbAssessedDocs()) continue;

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
		Session->GetStorage()->SaveFdbks(Session);
	}
}


//------------------------------------------------------------------------------
bool GSubjects::AddTopic(bool Save) throw(std::bad_alloc)
{
	GSubject** tab;
	GSubject** ptr;
	unsigned int i;
	GSubject* newSubject;
	GSubProfileCursor Prof;
	unsigned int nbprof,nbsocial;
//	GGroups* CurGrps;
	GGroup* Grp;
	unsigned int maxDocsOK,maxDocsKO,maxDocsH;

	// Apply Config
	Apply();

	// Randomly mix the subjects in tab
	tab=new GSubject*[GetNbNodes()];
	memcpy(tab,RTree<GSubject,true,false>::Tab,sizeof(GSubject*)*(GetNbNodes()));
	Session->GetRandom()->RandOrder<GSubject*>(tab,GetNbNodes());

	// Find the first not used subject having at least NbMinDocsSubject documents.
	for(ptr=tab,i=GetNbNodes()+1,newSubject=0;--i;ptr++)
	{
		if((*ptr)->GetNbDocs()<NbMinDocsSubject) continue;
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
	IdealGroups->InsertGroup(Grp=new GGroupVector(IdealGroups->GetNbGroups(),newSubject->GetLang(),false));

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
		Grp->InsertSubProfile(Prof());
		LastAdded.InsertPtr(Prof());

		// Judges documents
		ProfileJudges(Prof()->GetProfile(),newSubject,maxDocsOK,maxDocsKO,maxDocsH);
	}
	if(Save)
	{
		Session->GetStorage()->SaveFdbks(Session);
		Session->GetStorage()->SaveIdealGroupment(IdealGroups);
	}
	return(true);
}


//------------------------------------------------------------------------------
unsigned int GSubjects::AddProfiles(bool Save) throw(std::bad_alloc)
{
	GSubject** tab;
	GSubject** ptr;
	unsigned int i;
	unsigned int nbprof, nbsocial, nbprofilescreated;
	GSubject* usedSubject;
	GSubProfileCursor Prof;
	GGroupCursor CurGrps;
	GGroup* Grp;
	unsigned int maxDocsOK,maxDocsKO,maxDocsH;
	GSubProfileCursor Sub;

	// Apply Config
	Apply();

	//Randomly choose the number of profiles.
	nbprof=Session->GetCurrentRandomValue(NbProfMax-NbProfMin+1)+NbProfMin;

	// Randomly mix the subjects in tab
	tab=new GSubject*[GetNbNodes()];
	memcpy(tab,RTree<GSubject,true,false>::Tab,sizeof(GSubject*)*(GetNbNodes()));
	Session->GetRandom()->RandOrder<GSubject*>(tab,GetNbNodes());

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
	if(!usedSubject) return 0;

	// Catch the ideal group for this subject
	// It is suppose that the subject of the group is the subject of any
	// subprofile contained in it.
	CurGrps=IdealGroups->GetGroupsCursor(usedSubject->GetLang());
	for(CurGrps.Start(),Grp=0;!CurGrps.End();CurGrps.Next())
	{
		Sub=CurGrps()->GetSubProfilesCursor();
		Sub.Start();
		if(Sub.End()) continue;
		if(Sub()->GetSubject()->GetId()==usedSubject->GetId())
		{
			Grp=CurGrps();
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
		if (Prof()->GetProfile()->GetNbAssessedDocs(usedSubject->GetLang()))
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
		Grp->InsertSubProfile(Prof());
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
		Session->GetStorage()->SaveFdbks(Session);
		Session->GetStorage()->SaveIdealGroupment(IdealGroups);
	}

	//returns the number of created profiles
	return(nbprofilescreated);
}


//------------------------------------------------------------------------------
double GSubjects::ComputePercAss(void)
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


//------------------------------------------------------------------------------
void GSubjects::ClearLastAdded(void) throw(std::bad_alloc)
{
	LastAdded.Clear();
}


//------------------------------------------------------------------------------
GSubject* GSubjects::GetSubject(unsigned int id)
{
	return(RTree<GSubject,true,false>::GetPtr<unsigned int>(id));
}


//------------------------------------------------------------------------------
void GSubjects::Clear(void) throw(std::bad_alloc)
{
	RTree<GSubject,true,false>::Clear();
}


//------------------------------------------------------------------------------
void GSubjects::Compare(void)
{
	GGroupCursor Cur;

	GroupsScore.Clear();
	Cur=Session->GetGroupsCursor();
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		GroupsScore.InsertPtr(new GroupScore(Cur()));
	}
	ComputeRecallPrecision();
	ComputeTotal();
}


//------------------------------------------------------------------------------
double GSubjects::GetPrecision(GGroup* grp)
{
	GroupScore* g=GroupsScore.GetPtr<const GGroup*>(grp);

	if(!g) return(0.0);
	return(g->Precision);
}


//------------------------------------------------------------------------------
double GSubjects::GetRecall(GGroup* grp)
{
	GroupScore* g=GroupsScore.GetPtr<const GGroup*>(grp);

	if(!g) return(0.0);
	return(g->Recall);
}


//------------------------------------------------------------------------------
GGroups* GSubjects::GetIdealGroups(void)
{
	return(IdealGroups);
}


//------------------------------------------------------------------------------
GSubjects::~GSubjects(void)
{
	if(IdealGroups)
		delete IdealGroups;
	if(Docs)
		delete[] Docs;
}
