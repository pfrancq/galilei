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
#include <gcommunity.h>
#include <gtopic.h>
using namespace std;
using namespace R;
using namespace GALILEI;



//------------------------------------------------------------------------------
//
//  GCommunityId
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
class GCommunityId
{
public:
	int GrpId;       // Group Identifier.
	int position;    // Position of the group.

	// Constructor and Compare methods.
	GCommunityId(int RealId,int Position) : GrpId(RealId), position(Position) {}
	int Compare(const int ID) const {return(GrpId-ID);}
	int Compare(const GCommunityId& grp) const {return(GrpId-grp.GrpId);}
};



//------------------------------------------------------------------------------
//
//  GSubjects::GroupScore
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------

 template<class cGroup>
	class GSubjects::GroupScore
{
public:
	cGroup* Group;              // Pointer to the group.
	double Precision;              // Precision inside the group.
	double Recall;                 // Recall inside the group.

	// Constructor and Compare methods.
	GroupScore(cGroup* grp) : Group(grp), Precision(0.0), Recall(0.0) {}
	int Compare(const GroupScore& grp) const {return(Group->Compare(*grp.Group));}
	int Compare(const GCommunity* grp) const {return(Group->Compare(grp));}
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
	double NbOK;                                                         // Number of relevant documents to create.
	bool RelOK;                                                          // Number of Ok are relative or absolute.
	double NbKO;                                                         // Number of fuzzy relevant documents to create.
	bool RelKO;                                                          // Number of KO are relative or absolute.
	double NbH;                                                          // Number of irrelevant documents to create.
	bool RelH;                                                           // Number of H are relative (computed as a percentage of the relevant documents created) or absolute.
	double PercErr;                                                      // Percentage of documents assessed with an error.
	double NbSubjects;                                                   // Percentage of subjects to use.
	bool RelSubjects;                                                    // Number of subjects are relative or absolute.
	size_t NbMinDocsSubject;                                             // Minimal number of documents in a subject to use it.
	size_t NbProfMax;                                                    // Maximal number of profiles to create in a subject.
	size_t NbProfMin;                                                    // Minimal number of profiles to create in a subject.
	double PercSocial;                                                   // Percentage of profiles that are social.
	GDoc** tmpDocs;                                                      // Temporary Array of documents.
	size_t NbDocs;                                                       // Number of documents actually managed.
	R::RContainer<GFdbk,false,false> NewDocs;                            // New documents to assess.
	R::RContainer<GProfile,false,true> LastAdded;                        // Latest added profiles.
	size_t NbDocsAssess;                                                 // Number of documents to be assessed during a feedback process.
	double CommunityPrecision;                                           // Precision of the profiles clustering.
	double CommunityRecall;                                              // Recall of the profiles clustering.
	double CommunityTotal;                                               // Total comparison between for the profiles clustering.
	double TopicPrecision;                                               // Precision of the documents clustering.
	double TopicRecall;                                                  // Recall of the documents clustering.
	double TopicTotal;                                                   // Total comparison between for the documents clustering.
	R::RContainer<GroupScore<GCommunity>,true,true> CommunitiesScore;    // Score of computed communities.
	R::RContainer<GroupScore<GTopic>,true,true> TopicsScore;             // Score of computed topics.
	R::RContainer<GSubject,false,false> Profiles;                        // Subjects for the profiles.
	R::RContainer<R::RContainer<GSubject,false,false>,true,false> Docs;  // Subjects for the documents.

	Intern(GSession* session) :  Session(session),
	  tmpDocs(0), NbDocs(0), NewDocs(NbDocs), LastAdded(50,25),
	  CommunitiesScore(100,50), TopicsScore(100,50), Profiles(1000), Docs(5000)
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
	size_t compt;
	size_t nbprof,nbsocial;
	GSubject** tab(0);
	GSubject** ptr;
	size_t i;

	// Randomly mix the subjects in tab
	tab=new GSubject*[GetNbNodes()];
	RTree<GSubject,true,false>::GetTab(tab);
	Data->Session->GetRandom()->RandOrder<GSubject*>(tab,GetNbNodes());

	// Choose the first percgrp subjects having at least NbMinDocsSubject documents.
	if(Data->RelSubjects)
		compt=static_cast<size_t>((GetNbNodes()*Data->NbSubjects)/100)+1;
	else
		compt=static_cast<size_t>(Data->NbSubjects);
	for(ptr=tab,i=GetNbNodes()+1;(--i)&&compt;ptr++)
	{
		// Verify that there is enough documents
		if((*ptr)->GetNbObjs(otDoc)<Data->NbMinDocsSubject) continue;

		// Number of (social) profiles that will assess documents
		nbprof=Data->Session->GetCurrentRandomValue(Data->NbProfMax-Data->NbProfMin+1)+Data->NbProfMin;
		nbsocial=static_cast<size_t>(nbprof*Data->PercSocial/100);

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
	size_t nbprof,nbsocial;
	size_t maxDocsOK,maxDocsKO,maxDocsH;
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
		if(Data->NbProfMax>Subs()->GetNbObjs(otProfile))
			Data->NbProfMax=Subs()->GetNbObjs(otProfile);
		if(Data->NbProfMin>Subs()->GetNbObjs(otProfile))
			Data->NbProfMin=Subs()->GetNbObjs(otProfile);
		nbprof=Data->Session->GetCurrentRandomValue(Data->NbProfMax-Data->NbProfMin+1)+Data->NbProfMin;

		// Number of profiles that are social
		nbsocial=static_cast<size_t>(nbprof*Data->PercSocial/100);

		// Number of documents to judged by each profile
		if(Data->RelOK)
			maxDocsOK=static_cast<size_t>(Subs()->GetNbObjs(otDoc)*Data->NbOK/100);
		else
			maxDocsOK=static_cast<size_t>(Data->NbOK);
		if(Data->RelKO)
			maxDocsKO=static_cast<size_t>(Subs()->GetNbObjs(otDoc)*Data->NbKO/100);
		else
			maxDocsKO=static_cast<size_t>(Data->NbKO);
		if(Data->RelH)
			maxDocsH=static_cast<size_t>(maxDocsOK*Data->NbH/100);
		else
			maxDocsH=static_cast<size_t>(Data->NbH);

		// Assess documents
		Prof=Subs()->GetObjs(static_cast<GProfile*>(0));
		for(Prof.Start();!Prof.End();Prof.Next())
			ProfileAssess(Prof(),Subs(),maxDocsOK,maxDocsKO,maxDocsH);
	}
}


//------------------------------------------------------------------------------
void GSubjects::ProfileAssess(GProfile* prof,GSubject* sub,size_t maxDocsOK,size_t maxDocsKO,size_t maxDocsH)
{
	size_t nbDocsOK,nbDocsKO,nbDocsH;
	size_t i;
	GDoc** ptr;

	// Mix the documents
	Data->Session->GetRandom()->RandOrder<GDoc*>(Data->tmpDocs,Data->NbDocs);

	// Go trough the documents to create the assessments
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
template<class cGroup,class cObj>
	void GSubjects::ComputeRecallPrecision(tObjType type,RCursor<GroupScore<cGroup> >& grp,double& recall,double& precision)
{
	GSubject* ThGrp;        // Subject of the corresponding ideal group.
	size_t InGrp;           // Number of profiles in the computed group.
	size_t ElseInGrp;       // Number of profiles in the computed group that are also in the ideal one.
	size_t InThGrp;         // Number of profiles in the corresponding ideal group.
	size_t ElseInThGrp;     // Number of profiles in the corresponding ideal group that are also in the computed one.
	size_t NbProf;          // Number of profiles grouped.

	precision=recall=0.0;
	NbProf=0;
	for(grp.Start();!grp.End();grp.Next())
	{
		InGrp=grp()->Group->GetNbObjs();
		NbProf+=InGrp;
		grp()->Precision=grp()->Recall=0.0;
		RAssert(InGrp);
		RCursor<cObj> Prof(grp()->Group->GetObjs());
		if(InGrp==1)
		{
			Prof.Start();
			ThGrp=GetIdealGroup(Prof());
			RAssert(ThGrp);
			InThGrp=ThGrp->GetNbObjs(type);
			RAssert(InThGrp);
			grp()->Precision=1.0;
			if(InThGrp==1)
				grp()->Recall=1.0;
			precision+=grp()->Precision;
			recall+=grp()->Recall;
		}
		else
		{
			for(Prof.Start();!Prof.End();Prof.Next())
			{
				ThGrp=GetIdealGroup(Prof());
				RAssert(ThGrp);
				InThGrp=ThGrp->GetNbObjs(type);
				RAssert(InThGrp);
				if(InThGrp==1)
					grp()->Recall+=1.0;
				else
				{
					ElseInThGrp=ThGrp->GetNbObjs(grp()->Group)-1;
					grp()->Precision+=((double)(ElseInThGrp))/((double)(InGrp-1));
					ElseInGrp=grp()->Group->GetNbObjs(ThGrp)-1;
					grp()->Recall+=((double)(ElseInGrp))/((double)(InThGrp-1));
				}
			}
			precision+=grp()->Precision;
			recall+=grp()->Recall;
			grp()->Precision/=(double)InGrp;
			grp()->Recall/=(double)InGrp;
		}
	}
	if(NbProf)
	{
		precision/=(double)NbProf;
		recall/=(double)NbProf;
	}
}


//-----------------------------------------------------------------------------
size_t GSubjects::GetNbIdealGroups(tObjType type) const
{
	size_t nb;

	RCursor<GSubject> Cur(Top->GetNodes());
	for(Cur.Start(),nb=0;!Cur.End();Cur.Next())
		nb+=Cur()->GetNbIdealGroups(type);
	return(nb);
}


//-----------------------------------------------------------------------------
size_t GSubjects::GetNbTopicsDocs(void) const
{
	size_t nb;

	RCursor<GSubject> Cur(Top->GetNodes());
	for(Cur.Start(),nb=0;!Cur.End();Cur.Next())
		nb+=Cur()->GetNbTopicsDocs();
	return(nb);
}


//-----------------------------------------------------------------------------
template<class cGroup,class cObj>
	void GSubjects::ComputeTotal(tObjType objtype,tObjType grouptype,R::RCursor<cGroup>& grps,double& total)
{
	cGroup* GroupComputed;                  // Pointer to a computed group
	size_t NbRows,NbCols;                   // Rows and Cols for the matrix
	size_t MaxRows,MaxCols;                 // Maximal Rows and Cols for matrix allocation
	size_t NbObjs;                          // Total Number of objects
	size_t NbTot;
	size_t col;
	double a,b,c,d,num,den,subtotal;
	double* VectorRows;                           // Sum of the rows of the matrix
	double* VectorCols;                           // Sum of the columns of the matrix
	double* VectorColsTemp;                       // Temporary sum of the columns of the matrix
	double* ptr;

	// Initialization part
	total=0.0;
	NbObjs=0;

	// Go through the languages to define the maximal sizes and allocate the matrix
	MaxRows=MaxCols=0;
	NbRows=GetNbIdealGroups(objtype);
	NbCols=Data->Session->GetNbElements(grouptype);
	if((!NbRows)||(!NbCols))
		return;
	if(NbRows>MaxRows) MaxRows=NbRows;
	if(NbCols>MaxCols) MaxCols=NbCols;
	if((!MaxRows)||(!MaxCols))
		return;
	VectorRows=new double[MaxRows];
	VectorCols=new double[MaxCols];
	VectorColsTemp=new double[MaxCols];

	// Construction of the container for relation between id and column in the matrix.
	RContainer<GCommunityId,true,true> GroupsId(NbCols,NbCols/2);
	for(grps.Start(),col=0;!grps.End();grps.Next())
		GroupsId.InsertPtr(new GCommunityId(grps()->GetId(),col++));

	// Initialization of the variable used for computing the total
	a=b=c=d=0.0;

	// Initialization of the vectors
	memset(VectorRows,0,NbRows*sizeof(double));
	memset(VectorCols,0,NbCols*sizeof(double));

	// For each group of ideal group and for each object in this group
	// -> Compute the different terms of the total
	int row,position;
	row=0;
	//for(GroupsIdeal.Start(),NbTot=0;!GroupsIdeal.End();GroupsIdeal.Next())
	RCursor<GSubject> GroupsIdeal(GetNodes());
	for(GroupsIdeal.Start(),NbTot=0;!GroupsIdeal.End();GroupsIdeal.Next())
	{
		if(!GroupsIdeal()->GetNbObjs(objtype))
			continue;
		memset(VectorColsTemp,0,NbCols*sizeof(double));
		RCursor<cObj> Prof(GroupsIdeal()->GetObjs(static_cast<cObj*>(0)));
		for(Prof.Start();!Prof.End();Prof.Next())
		{
			if(Prof()->GetGroupId()==cNoRef)
				continue;
			VectorRows[row]++;
			NbTot++;
			GroupComputed=static_cast<cGroup*>(Data->Session->GetElement(grouptype,Prof()->GetGroupId()));
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
	NbObjs+=NbTot;
	total+=subtotal*NbTot;

	// Compute Total
	if(NbObjs)
		total=total/static_cast<double>(NbObjs);
	else
		total=0.0;

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
		Data->Session->GetStorage()->Clear(otCommunity);
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
	size_t i;

	// Apply Configuration
	Apply();

	// Similarities
	GMeasure* ProfilesDocsSims=GALILEIApp->GetManager<GMeasureManager>("Measures")->GetCurrentMethod("Documents/Profiles Similarities");

	// Go through the groups
	R::RCursor<GCommunity> Grps(Data->Session->GetCommunities());
	for(Grps.Start();!Grps.End();Grps.Next())
	{
		// Go through the profiles contained in the group.
		RCursor<GProfile> Profile(Grps()->GetObjs());
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
	size_t i;
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
		Prof=Subs()->GetObjs(static_cast<GProfile*>(0));
		for(Prof.Start();!Prof.End();Prof.Next())
		{
			// Look if current profile has already judged documents.
			if(!Prof()->GetNbAssessedDocs())
				continue;

			// Mix the documents
			Data->Session->GetRandom()->RandOrder<GDoc*>(Data->tmpDocs,Data->NbDocs);

			// Go trough the documents to create the assessments
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
	size_t i;
	GSubject* newSubject;
	RCursor<GProfile> Prof;
	size_t nbprof,nbsocial;
	size_t maxDocsOK,maxDocsKO,maxDocsH;

	// Apply Configuration
	Apply();

	// Randomly mix the subjects in tab
	tab=new GSubject*[GetNbNodes()];
	RTree<GSubject,true,false>::GetTab(tab);
	Data->Session->GetRandom()->RandOrder<GSubject*>(tab,GetNbNodes());

	// Find the first not used subject having at least NbMinDocsSubject documents.
	for(ptr=tab,i=GetNbNodes()+1,newSubject=0;--i;ptr++)
	{
		if((*ptr)->GetNbObjs(otDoc)<Data->NbMinDocsSubject) continue;
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

	// Number of (social) profiles that will judge documents
	nbprof=Data->Session->GetCurrentRandomValue(Data->NbProfMax-Data->NbProfMin+1)+Data->NbProfMin;
	nbsocial=static_cast<size_t>(nbprof*Data->PercSocial/100);

	newSubject->SetUsed(Data->Session,nbprof,nbsocial);

	// Number of documents to judged by each profile
	if(Data->RelOK)
		maxDocsOK=static_cast<size_t>(newSubject->GetNbObjs(otDoc)*Data->NbOK/100);
	else
		maxDocsOK=static_cast<size_t>(Data->NbOK);
	if(Data->RelKO)
		maxDocsKO=static_cast<size_t>(newSubject->GetNbObjs(otDoc)*Data->NbKO/100);
	else
		maxDocsKO=static_cast<size_t>(Data->NbKO);
	if(Data->RelH)
		maxDocsH=static_cast<size_t>(maxDocsOK*Data->NbH/100);
	else
		maxDocsH=static_cast<size_t>(Data->NbH);


	// Assess documents
	Prof=newSubject->GetObjs(static_cast<GProfile*>(0));
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
size_t GSubjects::AddProfiles(void)
{
	GSubject** tab;
	GSubject** ptr;
	size_t i;
	size_t nbprof, nbsocial;
	GSubject* usedSubject;
	R::RCursor<GCommunity> CurGrps;
	size_t maxDocsOK,maxDocsKO,maxDocsH;
	RCursor<GProfile> Prof;

	// Apply Config
	Apply();

	//Randomly choose the number of (social) profiles.
	nbprof=Data->Session->GetCurrentRandomValue(Data->NbProfMax-Data->NbProfMin+1)+Data->NbProfMin;
	nbsocial=static_cast<size_t>(nbprof*Data->PercSocial/100);

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
		maxDocsOK=static_cast<size_t>(usedSubject->GetNbObjs(otDoc)*Data->NbOK/100);
	else
		maxDocsOK=static_cast<size_t>(Data->NbOK);
	if(Data->RelKO)
		maxDocsKO=static_cast<size_t>(usedSubject->GetNbObjs(otDoc)*Data->NbKO/100);
	else
		maxDocsKO=static_cast<size_t>(Data->NbKO);
	if(Data->RelH)
		maxDocsH=static_cast<size_t>(maxDocsOK*Data->NbH/100);
	else
		maxDocsH=static_cast<size_t>(Data->NbH);


	usedSubject->SetUsed(Data->Session,nbprof,nbsocial);

	// Assess documents
	Prof=usedSubject->GetObjs(static_cast<GProfile*>(0));
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
	size_t nb;
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
			// Verify that the profile is defined
			if(!Cur2()->IsDefined()) continue;

			// If same profile, skip it.
			if(Cur1()==Cur2()) continue;

			// If both profiles are not related to the same topic, skip it.
			if(GetSubject(Cur1())!=GetSubject(Cur2())) continue;

			// Make comparisons
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
GSubject* GSubjects::GetSubject(size_t id)
{
	return(RTree<GSubject,true,false>::GetNode(id,false));
}


//------------------------------------------------------------------------------
void GSubjects::Clear(void)
{
	RTree<GSubject,true,false>::Clear();
	Data->Profiles.Clear();
	InsertNode(0,new GSubject(0,"Subjects",false));
}


//------------------------------------------------------------------------------
void GSubjects::Compare(tObjType type)
{
	switch(type)
	{
		case otCommunity:
		{
			Data->CommunitiesScore.Clear();
			RCursor<GCommunity> Cur(Data->Session->GetCommunities());
			for(Cur.Start();!Cur.End();Cur.Next())
				Data->CommunitiesScore.InsertPtr(new GroupScore<GCommunity>(Cur()));
			RCursor<GroupScore<GCommunity> > Grp(Data->CommunitiesScore);
			ComputeRecallPrecision<GCommunity,GProfile>(otProfile,Grp,Data->CommunityRecall,Data->CommunityPrecision);
			RCursor<GCommunity> GroupsComputed(Data->Session->GetCommunities());
			ComputeTotal<GCommunity,GProfile>(otProfile,otCommunity,GroupsComputed,Data->CommunityTotal);
			break;
		}
		case otTopic:
		{
			Data->TopicsScore.Clear();
			RCursor<GTopic> Cur(Data->Session->GetTopics());
			for(Cur.Start();!Cur.End();Cur.Next())
				Data->TopicsScore.InsertPtr(new GroupScore<GTopic>(Cur()));
			RCursor<GroupScore<GTopic> > Grp(Data->TopicsScore);
			ComputeRecallPrecision<GTopic,GDoc>(otDoc,Grp,Data->TopicRecall,Data->TopicPrecision);
			RCursor<GTopic> GroupsComputed(Data->Session->GetTopics());
			ComputeTotal<GTopic,GDoc>(otDoc,otTopic,GroupsComputed,Data->TopicTotal);
			break;
		}
		default:
			throw GException("Type is not supported");
	}
}


//------------------------------------------------------------------------------
double GSubjects::GetPrecision(tObjType type) const
{
	switch(type)
	{
		case otCommunity:
			return(Data->CommunityPrecision);
		case otTopic:
			return(Data->TopicPrecision);
		default:
			return(0.0);
	}
}


//------------------------------------------------------------------------------
double GSubjects::GetRecall(tObjType type) const
{
	switch(type)
	{
		case otCommunity:
			return(Data->CommunityRecall);
		case otTopic:
			return(Data->TopicRecall);
		default:
			return(0.0);
	}
}


//------------------------------------------------------------------------------
double GSubjects::GetTotal(tObjType type) const
{
	switch(type)
	{
		case otCommunity:
			return(Data->CommunityTotal);
		case otTopic:
			return(Data->TopicTotal);
		default:
			return(0.0);
	}
}


//------------------------------------------------------------------------------
double GSubjects::GetPrecision(GCommunity* com) const
{
	GroupScore<GCommunity>* g=Data->CommunitiesScore.GetPtr(com);
	if(!g) return(0.0);
	return(g->Precision);
}


//------------------------------------------------------------------------------
double GSubjects::GetRecall(GCommunity* com) const
{
	GroupScore<GCommunity>* g=Data->CommunitiesScore.GetPtr(com);
	if(!g) return(0.0);
	return(g->Recall);
}


//------------------------------------------------------------------------------
double GSubjects::GetPrecision(GTopic* top) const
{
	GroupScore<GTopic>* g=Data->TopicsScore.GetPtr(top);
	if(!g) return(0.0);
	return(g->Precision);
}


//------------------------------------------------------------------------------
double GSubjects::GetRecall(GTopic* top) const
{
	GroupScore<GTopic>* g=Data->TopicsScore.GetPtr(top);
	if(!g) return(0.0);
	return(g->Recall);
}


//------------------------------------------------------------------------------
void GSubjects::InsertProfileSubject(GProfile* profile,size_t subjectid)
{
	GSubject* subject=RTree<GSubject,true,false>::GetNode(subjectid,false);

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
void GSubjects::InsertDocSubject(GDoc* doc,size_t subjectid)
{
	GSubject* subject=RTree<GSubject,true,false>::GetNode<size_t>(subjectid,false);
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
bool GSubjects::IsFromSubject(size_t docid,const GSubject* s)
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
bool GSubjects::IsFromParentSubject(size_t docid,const GSubject* s)
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
R::RCursor<GSubject> GSubjects::GetSubjects(size_t docid)
{
	R::RCursor<GSubject> cur;
	R::RContainer<GSubject,false,false>* line=Data->Docs[docid];
	if(line)
		cur.Set(*line);
	return(cur);
}


//------------------------------------------------------------------------------
size_t GSubjects::GetNbSubjects(GDoc* doc)
{
	return(GetNbSubjects(doc->GetId()));
}


//------------------------------------------------------------------------------
size_t GSubjects::GetNbSubjects(size_t docid)
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
