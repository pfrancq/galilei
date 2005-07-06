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
#include <docs/gdocproxy.h>
#include <sessions/gsession.h>
#include <sessions/gstorage.h>
#include <infos/glang.h>
#include <infos/glangmanager.h>
#include <profiles/gprofile.h>
#include <profiles/gsubprofile.h>
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
	int Compare(const GroupScore& grp) const {return(Group->Compare(*grp.Group));}
	int Compare(const GGroup* grp) const {return(Group->Compare(grp));}
};


//------------------------------------------------------------------------------
//
//  GroupLang
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
// class GSubjects
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GSubjects::GSubjects(GSession* session) throw(std::bad_alloc)
	: RTree<GSubject,true,false>(100,50), GParams("Subjects"), Session(session),
	  tmpDocs(0), NbDocs(0), NewDocs(NbDocs), LastAdded(50,25), IdealGroups(0),
	  GroupsScore(100,50), Profiles(1000), Docs(5000)
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
	R::RCursor<GSubject> Subs;
	unsigned int compt;
	GSubject** tab(0);
	GSubject** ptr;
	unsigned int i;

	// Suppose all subjects are not chosen
	Subs.Set(*this);
	for(Subs.Start();!Subs.End();Subs.Next())
		Subs()->SetUsed(false);

	// Randomly mix the subjects in tab
	tab=new GSubject*[RTree<GSubject,true,false>::GetMaxPos()+1];
	RTree<GSubject,true,false>::GetTab(tab);
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
	RCursor<GSubject> Subs;
	RCursor<GProfile> Prof;
	unsigned int nbprof,nbsocial;
	unsigned int maxDocsOK,maxDocsKO,maxDocsH;
	RCursor<GFactoryLang> CurLang;
	RContainer<GroupLang,true,true> Groups(20);          // Groups created for a given subject

	// Init Part
	LastAdded.Clear();
	if(!tmpDocs)
		tmpDocs=new GDoc*[Session->GetMaxPosDoc()+1];

	// Go through all the subjects which are used
	Subs.Set(*this);
	for(Subs.Start();!Subs.End();Subs.Next())
	{
		if(!Subs()->IsUsed())
			continue;

		// Clear the created groups
		Groups.Clear();

		// Copy the documents of the session in Docs;
		NbDocs=Session->FillDocs(tmpDocs);

		// Number of profiles that will assess documents
		if(NbProfMax>Subs()->GetNbProfiles())
			NbProfMax=Subs()->GetNbProfiles();
		if(NbProfMin>Subs()->GetNbProfiles())
			NbProfMin=Subs()->GetNbProfiles();
		nbprof=Session->GetCurrentRandomValue(NbProfMax-NbProfMin+1)+NbProfMin;

		// Number of profiles that are social
		nbsocial=static_cast<unsigned int>(nbprof*PercSocial/100);

		// Number of documents to judged by each subprofile
		maxDocsOK=static_cast<unsigned int>(Subs()->GetNbDocs()*PercOK/100);
		maxDocsKO=static_cast<unsigned int>(Subs()->GetNbDocs()*PercKO/100);
		maxDocsH=static_cast<unsigned int>(maxDocsOK*PercH/100);

		// Go through the nbprof first profiles attached to the subject
		Prof=Subs()->GetProfilesCursor();
		for(Prof.Start(),nbprof++;(!Prof.End())&&(--nbprof);Prof.Next())
		{
			// Look if current subprofile is social or not
			if(nbsocial)
			{
				Prof()->SetSocial(true);
				nbsocial--;
			}
			else
				Prof()->SetSocial(false);

			// Judges documents
			ProfileAssess(Groups,Prof(),Subs(),maxDocsOK,maxDocsKO,maxDocsH);
		}
	}
}


//------------------------------------------------------------------------------
void GSubjects::ProfileAssess(RContainer<GroupLang,true,true>& groups,GProfile* prof,GSubject* sub,unsigned int maxDocsOK,unsigned int maxDocsKO,unsigned int maxDocsH)
{
	unsigned int nbDocsOK,nbDocsKO,nbDocsH;
	unsigned int i;
	GDoc** ptr;
	RContainer<GLang,false,true> Langs(20);         // Langs already used

	// Mix the documents
	Session->GetRandom()->RandOrder<GDoc*>(tmpDocs,NbDocs);

	// Go trought the documents to create the judgements
	for(i=NbDocs+1,ptr=tmpDocs,nbDocsOK=maxDocsOK,nbDocsKO=maxDocsKO,nbDocsH=maxDocsH;(--i)&&((nbDocsOK)||(nbDocsKO)||(nbDocsH));ptr++)
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
			GroupLang* Grp=groups.GetPtr<GLang*>(Lang);
			if(!Grp)
			{
				GGroup* NewGrp;

				// A ideal group must be created
				IdealGroups->InsertGroup(NewGrp=new GGroup(IdealGroups->GetNbGroups(),Lang,false));
				groups.InsertPtr(Grp=new GroupLang(Lang,NewGrp));
			}

			// and the corresponding subprofile inserted
			Grp->Group->InsertSubProfile(prof->GetInsertSubProfile(Lang,Session));
			LastAdded.InsertPtr(prof->GetInsertSubProfile(Lang,Session));
			Langs.InsertPtr(Lang);
		}

		// Look if 'OK'
		if(IsFromSubject((*ptr),sub))
		{
			if(nbDocsOK)
			{
				nbDocsOK--;
				Session->InsertFdbk(prof->GetId(),(*ptr)->GetId(),GFdbk::ErrorJudgment(djOK,PercErr,Session->GetRandom()),RDate::GetToday());
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
					Session->InsertFdbk(prof->GetId(),(*ptr)->GetId(),GFdbk::ErrorJudgment(djKO,PercErr,Session->GetRandom()),RDate::GetToday());
				}
			}
			else
			{
				// Must be H
				if(nbDocsH)
				{
					nbDocsH--;
					Session->InsertFdbk(prof->GetId(),(*ptr)->GetId(),GFdbk::ErrorJudgment(djOutScope,PercErr,Session->GetRandom()),RDate::GetToday());
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
	R::RCursor<GroupScore> Grp;
	RCursor<GSubProfile> Sub;
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
			if((!thGrp)||(thGrp->GetLang()!=Grp()->Group->GetLang()))
				continue;
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
				if((!thGrp)||(thGrp->GetLang()!=Grp()->Group->GetLang()))
					continue;
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
	RCursor<GGroup> GroupsIdeal;                 // Pointer to the ideal groups for a given language
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
	Total=0.0;
	NbSubProfiles=0;

	// Go through the languages to define the maximal sizes and allocate the matrix
	MaxRows=MaxCols=0;
	Langs=(dynamic_cast<GLangManager*>(GPluginManager::GetManager("Lang")))->GetLangsCursor();
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
		GroupsIdeal=IdealGroups->GetGroupsCursor(lang);
		NbRows=GroupsIdeal.GetNb();
		GroupsComputed=Session->GetGroupsCursor(lang);
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
		for(GroupsIdeal.Start(),NbTot=0;!GroupsIdeal.End();GroupsIdeal.Next())
		{
			memset(VectorColsTemp,0,NbCols*sizeof(double));
			Sub=GroupsIdeal()->GetSubProfilesCursor();
			for(Sub.Start();!Sub.End();Sub.Next())
			{
				VectorRows[row]++;
				NbTot++;
				GroupComputed=Sub()->GetGroup();
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
		Total+=subtotal*NbTot;
	}

	// Compute Total
	if(NbSubProfiles)
		Total=Total/static_cast<double>(NbSubProfiles);
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
	if(!tmpDocs)
		tmpDocs=new GDoc*[Session->GetMaxPosDoc()+1];
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
	R::RCursor<GGroup> Grps;
	RCursor<GSubProfile> SubProfile;
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
			RCursor<GFdbk> Cur(NewDocs);
			for(Cur.Start(),i=NbDocsAssess+1;(!Cur.End())&&(--i);Cur.Next())
			{
				// Look if 'OK'
				if(IsFromSubject(Cur()->GetDoc()->GetId(),GetSubject(SubProfile())))
				{
					Session->InsertFdbk(SubProfile()->GetProfile()->GetId(),Cur()->GetDoc()->GetId(),GFdbk::ErrorJudgment(djOK,PercErr,Session->GetRandom()),RDate::GetToday());
				}
				else
				{
					// Look If 'KO'
					if(IsFromParentSubject(Cur()->GetDoc()->GetId(),GetSubject(SubProfile())))
					{
						Session->InsertFdbk(SubProfile()->GetProfile()->GetId(),Cur()->GetDoc()->GetId(),GFdbk::ErrorJudgment(djKO,PercErr,Session->GetRandom()),RDate::GetToday());
					}
					else
					{
						// Must be H
						Session->InsertFdbk(SubProfile()->GetProfile()->GetId(),Cur()->GetDoc()->GetId(),GFdbk::ErrorJudgment(djOutScope,PercErr,Session->GetRandom()),RDate::GetToday());
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
		NbDocs=Session->FillDocs(tmpDocs);

		// Go through the profiles attached to the subject
		Prof=Subs()->GetProfilesCursor();
		for(Prof.Start();!Prof.End();Prof.Next())
		{
			// Look if current profile has already judged documents.
			if(!Prof()->GetNbAssessedDocs())
				continue;

			// Mix the documents
			Session->GetRandom()->RandOrder<GDoc*>(tmpDocs,NbDocs);

			// Go trought the documents to create the judgements
			for(i=NbDocsAssess+1,ptr=tmpDocs;--i;ptr++)
			{
				// Verify that the document is not already assigned to the profile
				if(Prof()->GetFdbk((*ptr)->GetId())) continue;

				// Look if 'OK'
				if(IsFromSubject((*ptr),Subs()))
				{
					Session->InsertFdbk(Prof()->GetId(),(*ptr)->GetId(),GFdbk::ErrorJudgment(djOK,PercErr,Session->GetRandom()),RDate::GetToday());
				}
				else
				{
					// Look If 'KO'
					if(IsFromParentSubject((*ptr),Subs()))
					{
						Session->InsertFdbk(Prof()->GetId(),(*ptr)->GetId(),GFdbk::ErrorJudgment(djKO,PercErr,Session->GetRandom()),RDate::GetToday());
					}
					else
					{
						Session->InsertFdbk(Prof()->GetId(),(*ptr)->GetId(),GFdbk::ErrorJudgment(djOutScope,PercErr,Session->GetRandom()),RDate::GetToday());
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
	RCursor<GProfile> Prof;
	unsigned int nbprof,nbsocial;
	unsigned int maxDocsOK,maxDocsKO,maxDocsH;
	RContainer<GroupLang,true,true> Groups(20);          // Groups created for a given subject

	// Apply Config
	Apply();

	// Randomly mix the subjects in tab
	tab=new GSubject*[GetNbNodes()];
	RTree<GSubject,true,false>::GetTab(tab);
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

	// Copy the documents of the same language of the session in Docs;
	NbDocs=Session->FillDocs(tmpDocs);

	// Number of subprofiles that will judge documents
	if(NbProfMax>newSubject->GetNbProfiles())
		NbProfMax=newSubject->GetNbProfiles();
	if(NbProfMin>newSubject->GetNbProfiles())
		NbProfMin=newSubject->GetNbProfiles();
	nbprof=Session->GetCurrentRandomValue(NbProfMax-NbProfMin+1)+NbProfMin;

	// Number of profiles that are social
	nbsocial=static_cast<unsigned int>(nbprof*PercSocial/100);

	// Number of documents to judged by each subprofile
	maxDocsOK=static_cast<unsigned int>(newSubject->GetNbDocs()*PercOK/100);
	maxDocsKO=static_cast<unsigned int>(newSubject->GetNbDocs()*PercKO/100);
	maxDocsH=static_cast<unsigned int>(maxDocsOK*PercH/100);

	// Go through the nbprof first profiles attached to the subject
	Prof=newSubject->GetProfilesCursor();
	for(Prof.Start(),nbprof++;(!Prof.End())&&(--nbprof);Prof.Next())
	{
		// Look if current subprofile is social or not
		if(nbsocial)
		{
			Prof()->SetSocial(true);
			nbsocial--;
		}
		else
			Prof()->SetSocial(false);

		// Judges documents
		ProfileAssess(Groups,Prof(),newSubject,maxDocsOK,maxDocsKO,maxDocsH);
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
	RCursor<GProfile> Prof;
	R::RCursor<GGroup> CurGrps;
	GGroup* Grp;
	unsigned int maxDocsOK,maxDocsKO,maxDocsH;
	RCursor<GSubProfile> Sub;
	RContainer<GroupLang,true,true> Groups(20);          // Groups created for a given subject

	// Apply Config
	Apply();

	//Randomly choose the number of profiles.
	nbprof=Session->GetCurrentRandomValue(NbProfMax-NbProfMin+1)+NbProfMin;

	// Randomly mix the subjects in tab
	tab=new GSubject*[GetNbNodes()];
	RTree<GSubject,true,false>::GetTab(tab);
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

	// Catch all the ideal groups for this subject
	// It is suppose that the subject of the group is the subject of any
	// subprofile contained in it.
	RCursor<GFactoryLang> Langs=(dynamic_cast<GLangManager*>(GPluginManager::GetManager("Lang")))->GetLangsCursor();
	for(Langs.Start();!Langs.End();Langs.Next())
	{
		GLang* lang=Langs()->GetPlugin();
		if(!lang) continue;
		CurGrps=IdealGroups->GetGroupsCursor(lang);
		for(CurGrps.Start(),Grp=0;!CurGrps.End();CurGrps.Next())
		{
			Sub=CurGrps()->GetSubProfilesCursor();
			Sub.Start();
			if(Sub.End()) continue;
			if(GetSubject(Sub())->GetId()==usedSubject->GetId())
			{
				Groups.InsertPtr(new GroupLang(lang,CurGrps()));
				break;
			}
		}
	}

	// Copy the documents of the same language of the session in Docs;
	NbDocs=Session->FillDocs(tmpDocs);

	// Number of profiles that are social
	nbsocial=static_cast<unsigned int>(nbprof*PercSocial/100);

	// Number of documents to judged by each subprofile
	maxDocsOK=static_cast<unsigned int>(usedSubject->GetNbDocs()*PercOK/100);
	maxDocsKO=static_cast<unsigned int>(usedSubject->GetNbDocs()*PercKO/100);
	maxDocsH=static_cast<unsigned int>(maxDocsOK*PercH/100);

	// Go through the nbprof first subprofiles attached to the subject
	nbprofilescreated=0;
	Prof=usedSubject->GetProfilesCursor();
	Prof.Start();
	while((!Prof.End())&&(nbprofilescreated<nbprof))
	{
		// Check if the profile is free
		if(Prof()->GetNbAssessedDocs())
		{
			Prof.Next(); continue;
		}
		// Look if current profile is social or not
		if(nbsocial)
		{
			Prof()->SetSocial(true);
			nbsocial--;
		}
		else
			Prof()->SetSocial(false);

		// Judges documents
		ProfileAssess(Groups,Prof(),usedSubject,maxDocsOK,maxDocsKO,maxDocsH);

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
	RCursor<GSubProfile> Cur1;
	R::RCursor<GProfile> Cur2;
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
			if(GetSubject(Cur1())!=GetSubject(Sub)) continue;

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
	Profiles.Clear();
}


//------------------------------------------------------------------------------
void GSubjects::Compare(void)
{
	R::RCursor<GGroup> Cur;

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
void GSubjects::InsertProfileSubject(GProfile* profile,unsigned int subjectid)
{
	GSubject* subject=RTree<GSubject,true,false>::GetPtr<unsigned int>(subjectid);

	if(!subject)
		return;
	Profiles.InsertPtrAt(subject,profile->GetId(),true);
	subject->Insert(profile);
}


//------------------------------------------------------------------------------
GSubject* GSubjects::GetSubject(GSubProfile* sub)
{
	if(!sub)
		return(0);
	return(Profiles[sub->GetProfile()->GetId()]);
}


//------------------------------------------------------------------------------
void GSubjects::InsertDocSubject(GDoc* doc,unsigned int subjectid)
{
	GSubject* subject=RTree<GSubject,true,false>::GetPtr<unsigned int>(subjectid);
	if(!subject)
		return;
	R::RContainer<GSubject,false,false>* line;
	if(Docs.GetMaxPos()<doc->GetId())
	{
		line=new R::RContainer<GSubject,false,false>(10,5);
		Docs.InsertPtrAt(line,doc->GetId(),true);
	}
	else
	{
		line=Docs[doc->GetId()];
		if(!line)
		{
			line=new R::RContainer<GSubject,false,false>(10,5);
			Docs.InsertPtrAt(line,doc->GetId(),true);
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
	if(Docs.GetMaxPos()<docid)
		return(false);
	R::RContainer<GSubject,false,false>* line=Docs[docid];
	if(!line)
		return(false);
	return(line->IsIn(s));
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
	if(Docs.GetMaxPos()<docid)
		return(false);
	R::RContainer<GSubject,false,false>* line=Docs[docid];
	if(!line)
		return(false);
	R::RCursor<GSubject> Sub;

	Sub.Set(*s->Parent);
	for(Sub.Start();!Sub.End();Sub.Next())
	{
		if(Sub()==s)
			continue;
		if(line->IsIn(Sub()))
			return(true);
	}
	return(false);
}


//------------------------------------------------------------------------------
R::RCursor<GSubject> GSubjects::GetSubjectCursor(GDoc* doc)
{
	return(GetSubjectCursor(doc->GetId()));
}


//------------------------------------------------------------------------------
R::RCursor<GSubject> GSubjects::GetSubjectCursor(unsigned int docid)
{
	R::RCursor<GSubject> cur;
	R::RContainer<GSubject,false,false>* line=Docs[docid];
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
	R::RContainer<GSubject,false,false>* line=Docs[docid];
	if(!line)
		return(0);
	return(line->GetNb());
}


//------------------------------------------------------------------------------
GSubjects::~GSubjects(void)
{
	if(IdealGroups)
		delete IdealGroups;
	if(tmpDocs)
		delete[] tmpDocs;
}
