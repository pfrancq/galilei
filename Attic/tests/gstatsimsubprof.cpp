/*

	GALILEI Research Project

	GSatSimSubProf.cpp

	Calc the similarity between subprofiles using the ideal groupment - Implementation.

	Copyright 2002 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).
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
#include <tests/ggroupevaluate.h>
#include <tests/ggroupevaluatesubprof.h>
#include <tests/ggroupsevaluate.h>
#include <tests/gstatsimid.h>
#include <groups/ggroups.h>
#include <groups/ggroup.h>
#include <sessions/gsession.h>
#include <profiles/gprofile.h>
#include <infos/giwordsweights.h>
#include <infos/giwordweight.h>
#include <profiles/gprofilecalc.h>
#include <tests/gstatsimsubprof.h>
using namespace GALILEI;



//-----------------------------------------------------------------------------
//
// class GStatSimSubProf
//
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
GALILEI::GStatSimSubProf::GStatSimSubProf(GSession* ses,RContainer<GGroups,unsigned int,true,true>* ideal)
	: Session(ses), IdealGroups(ideal),Global(true)
{
}


//-----------------------------------------------------------------------------
void GALILEI::GStatSimSubProf::Run(void)
{

	//The container of StatSimilarities.
	RContainer<GSatSimId,unsigned int,true,true> StatsSim (100,50);

	for(IdealGroups->Start();!IdealGroups->End();IdealGroups->Next())
	{
		GGroupCursor Cur2=(*IdealGroups)()->GetGroupCursor();
		// For each SubGroup
		for(Cur2.Start();!Cur2.End();Cur2.Next())
		{
			GGroup * Group=Cur2();
			GSubProfileCursor SubCur1= Group->GetSubProfileCursor();
			for(SubCur1.Start();!SubCur1.End();SubCur1.Next())
			{
				GSatSimId* temp=new GSatSimId(SubCur1()->GetId(),Group->GetId());
				StatsSim.InsertPtr(temp);
			}
		}
	}

	// Calculation of SimId Contianer
	for(IdealGroups->Start();!IdealGroups->End();IdealGroups->Next())
	{
		GGroupCursor Cur2=(*IdealGroups)()->GetGroupCursor();
		for(Cur2.Start();!Cur2.End();Cur2.Next())
		{
			GGroup * Group=Cur2();
			GSubProfileCursor SubCur1= Group->GetSubProfileCursor();
			for(SubCur1.Start();!SubCur1.End();SubCur1.Next())
			{
				GSatSimId* temp1=StatsSim.GetPtr(SubCur1()->GetId());
				for(StatsSim.Start();!StatsSim.End();StatsSim.Next())
				{
					double simtemp1temp2;
					GSatSimId* temp2=StatsSim();
					if(Global) simtemp1temp2= Session->GetSubProfile(temp1->GetId())->GlobalSimilarity(Session->GetSubProfile(temp2->GetId()));
					else simtemp1temp2=Session->GetSubProfile(temp1->GetId())->Similarity(Session->GetSubProfile(temp2->GetId()));
					if(temp1->GetId()!=temp2->GetId())
					{
						temp1->SetSim(simtemp1temp2,temp2->GetGrpId());
						temp2->SetSim(simtemp1temp2,temp1->GetGrpId());
					}
				}
			}
		}
	}

	//Initialization
	MeanIntraM=0.0;
	MeanExtraM=0.0;
	MeanIntraMin=0.0;
	MeanExtraMax=0.0;
	Overlap=0.0;
	GrpOverlap=0.0;

	for(IdealGroups->Start();!IdealGroups->End();IdealGroups->Next())
	{
		GGroupCursor Cur2=(*IdealGroups)()->GetGroupCursor();
		for(Cur2.Start();!Cur2.End();Cur2.Next())
		{
			bool grpover=false;
			for(StatsSim.Start();!StatsSim.End();StatsSim.Next())
			{
				GSatSimId* temp=StatsSim();
				if(temp->GetGrpId()==Cur2()->GetId())
				{
					if(temp->GetMinIntra()<temp->GetMaxInter())
					{
						grpover=true;
					}
				}
			}
			if(grpover) GrpOverlap++;
		}
	}

	int ComptOverlap=0;
	int CompProf=0;

	for(StatsSim.Start();!StatsSim.End();StatsSim.Next())
	{
		GSatSimId* temp=StatsSim();
		MeanIntraM+=temp->GetMeanIntra();
		MeanExtraM+=temp->GetMeanInter();
		CompProf++;
		MeanIntraMin+=temp->GetMinIntra();
		MeanExtraMax+=temp->GetMaxInter();
		if(temp->GetMinIntra()<temp->GetMaxInter())
		{
			ComptOverlap++;
		}
		
	}
	
	MeanIntraM/=CompProf;
	MeanExtraM/=CompProf;
	Overlap=100*ComptOverlap/CompProf;
	Rie=(MeanIntraM-MeanExtraM)/MeanIntraM;

	//Calculation of J.
	double SW=0;
	double Mb=1;
	for(IdealGroups->Start();!IdealGroups->End();IdealGroups->Next())
	{
		GGroupCursor Cur2=(*IdealGroups)()->GetGroupCursor();
		// For each SubGroup
		for(Cur2.Start();!Cur2.End();Cur2.Next())
		{
			double SWtemp=0;
			GGroup * Group=Cur2();
			GSubProfile* relevant= Group->RelevantSubProfile(Global);
			GSubProfileCursor SubCur1= Group->GetSubProfileCursor();
			for(SubCur1.Start();!SubCur1.End();SubCur1.Next())
			{
				if(Global)
					SWtemp+=(1-relevant->GlobalSimilarity(SubCur1()))*(1-relevant->GlobalSimilarity(SubCur1()));
				else
					SWtemp+=(1-relevant->Similarity(SubCur1()))*(1-relevant->GlobalSimilarity(SubCur1()));
			}
			SW+=SWtemp;
		}
	}
	
	for(IdealGroups->Start();!IdealGroups->End();IdealGroups->Next())
	{
		GGroupCursor Cur2=(*IdealGroups)()->GetGroupCursor();
		GGroupCursor Cur1=(*IdealGroups)()->GetGroupCursor();
		for(Cur2.Start();!Cur2.End();Cur2.Next())
		{
			GGroup* Group2=Cur2();
			GSubProfile* relevant= Group2->RelevantSubProfile(Global);
			for(Cur1.Start();!Cur1.End();Cur1.Next())
			{
				GGroup* Group1=Cur1();
				GSubProfile* relevant1=Group1->RelevantSubProfile(Global);
				if(relevant1->GetId()!=relevant->GetId())
				{
					double temp;
					if(Global)
						temp=(1-relevant->GlobalSimilarity(relevant1))*(1-relevant->GlobalSimilarity(relevant1));
					else
						temp=(1-relevant->Similarity(relevant1))*(1-relevant->GlobalSimilarity(relevant1));
					if(temp<Mb) Mb=temp;
				}
			}
		}
	}
	J=Mb/SW;


}


//-----------------------------------------------------------------------------
char* GALILEI::GStatSimSubProf::GetSettings(void)
{
	static char tmp[100];
	char c;

	if(Global) c='1'; else c='0';
	sprintf(tmp,"%c",c);
	return(tmp);

}


//-----------------------------------------------------------------------------
void GALILEI::GStatSimSubProf::SetSettings(const char* s)
{
	char c;

	if(!(*s)) return;
	sscanf(s,"%c",&c);
	if(c=='1') Global=true; else Global=false;
}


//-----------------------------------------------------------------------------
GALILEI::GStatSimSubProf::~GStatSimSubProf(void)
{
}
