/*

	GALILEI Research Project

	GCompareGrouping.cpp

	Compare a ideal groupement with a computed one - Implementation

	(C) 2002 by P. Francq and J. Lamoral.

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
//include files for GALILEI
#include<groups/gcomparegrouping.h>
#include<groups/ggroup.h>
#include<groups/ggroups.h>
#include<langs/glangs.h>
#include<langs/glang.h>
#include<profiles/gprofile.h>
#include<profiles/gsubprofile.h>
#include<sessions/gsession.h>
#include<sessions/gslot.h>
using namespace GALILEI;



//-----------------------------------------------------------------------------
//
//  GGroupId
//
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
class GALILEI::GCompareGrouping::GGroupId
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
class GALILEI::GCompareGrouping::GroupScore
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
//  GCompareGrouping
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GALILEI::GCompareGrouping::GCompareGrouping(GSession* s,RContainer<GGroups,unsigned int,true,true>* groups) throw(bad_alloc)
	: Session(s), Groups(groups), Precision(0.0), Recall(0.0), GroupsScore(0)
{
	GGroupsCursor Cur;
	GGroupCursor Grp;

	GroupsScore=new RContainer<GroupScore,unsigned int,true,true>(100,50);
	Cur=s->GetGroupsCursor();
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		Grp=Cur()->GetGroupCursor();
		for(Grp.Start();!Grp.End();Grp.Next())
			GroupsScore->InsertPtr(new GroupScore(Grp()));
	}
}


//-----------------------------------------------------------------------------
GGroup* GALILEI::GCompareGrouping::GetIdealGroup(GSubProfile* sub) const
{
	GGroups* grps;

	grps=Groups->GetPtr<const GLang*>(sub->GetLang());
	if(!grps) return(0);
	return(grps->GetGroup(sub));
}


//-----------------------------------------------------------------------------
void GALILEI::GCompareGrouping::ComputeRecallPrecision(GSlot* /*rec*/)
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
			Sub=Grp()->Group->GetSubProfileCursor();
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
void GALILEI::GCompareGrouping::ComputeTotal(GSlot* /*rec*/)
{
	GGroups* GroupsIdeal;                         // Pointer to the ideal groups for a given language
	GGroups* GroupsComputed;                      // Pointer to the computed groups for a given language
	GGroup* GroupIdeal;                           // Pointer to a ideal group
	GGroup* GroupComputed;                        // Pointer to a computed group
	unsigned int NbRows,NbCols;                   // Rows and Cols for the current language for matrix
	unsigned int MaxRows,MaxCols;                 // Maximal Rows and Cols for matrix allocation
	unsigned int NbProfiles;                      // Total Number of profiles
	unsigned int NbTot;
	GLangCursor Langs;
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
	Langs=Session->GetLangsCursor();
	for(Langs.Start();!Langs.End();Langs.Next())
	{
		GroupsIdeal=Groups->GetPtr<GLang*>(Langs());
		if(GroupsIdeal)
			NbRows=GroupsIdeal->NbPtr;
		else
			NbRows=0;
		NbCols=Session->GetGroups(Langs())->NbPtr;
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
		// Compute number of elements in ideal and computed groups.
		// and assign the groups to the current language.
		GroupsIdeal=Groups->GetPtr<GLang*>(Langs());
		if(GroupsIdeal)
			NbRows=GroupsIdeal->NbPtr;
		else
			NbRows=0;
		GroupsComputed=Session->GetGroups(Langs());
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
	Total=Total/NbProfiles;

	//delete the vectors
	if (VectorRows) delete[] VectorRows;
	if (VectorCols) delete[] VectorCols;
	if (VectorColsTemp) delete[] VectorColsTemp;
}


//-----------------------------------------------------------------------------
void GALILEI::GCompareGrouping::Compare(GSlot* rec)
{
	ComputeRecallPrecision(rec);
	ComputeTotal(rec);
}


//-----------------------------------------------------------------------------
double GALILEI::GCompareGrouping::GetPrecision(GGroup* grp)
{
	GroupScore* g=GroupsScore->GetPtr<const GGroup*>(grp);

	if(!g) return(0.0);
	return(g->Precision);
}


//-----------------------------------------------------------------------------
double GALILEI::GCompareGrouping::GetRecall(GGroup* grp)
{
	GroupScore* g=GroupsScore->GetPtr<const GGroup*>(grp);

	if(!g) return(0.0);
	return(g->Recall);
}


//-----------------------------------------------------------------------------
GALILEI::GCompareGrouping::~GCompareGrouping(void)
{
	if(GroupsScore) delete GroupsScore;
}
