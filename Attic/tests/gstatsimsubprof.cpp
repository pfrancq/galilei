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
	double MinIntra;
	double MaxExtra;

	//Initialization
	MeanIntraM=0.0;
	MeanExtraM=0.0;
	Overlap=0.0;

	int ComptIntra=0;
	int ComptExtra=0;
	int ComptOverlap=0;

	// For each language
	for(IdealGroups->Start();!IdealGroups->End();IdealGroups->Next())
	{
		GGroupCursor Cur2=(*IdealGroups)()->GetGroupCursor();
		// For each SubGroup
		for(Cur2.Start();!Cur2.End();Cur2.Next())
		{
			// Temp initialization
			MinIntra=1.0;
			MaxExtra=0.0;

			GGroup * Group=Cur2();
			GSubProfileCursor SubCur1= Group->GetSubProfileCursor();
			GSubProfileCursor SubCur2= Group->GetSubProfileCursor();
			// for each subprofiles in thos group compare whith all subprofile in this group.
			for(SubCur1.Start();!SubCur1.End();SubCur1.Next())
			{
				for(SubCur2.Start();!SubCur2.End();SubCur2.Next())
				{
					double temp;
					if(Global) temp=SubCur1()->GlobalSimilarity(SubCur2());
					else temp=SubCur1()->Similarity(SubCur2());
					if(SubCur1()->GetId()!=SubCur2()->GetId())
					{
						MeanIntraM+=temp;
						ComptIntra++;
						if(temp<MinIntra) MinIntra=temp;
					}
				}
			}
			GGroupCursor Cur=(*IdealGroups)()->GetGroupCursor();
			// Compare each subprofile sof the group whith all the subprofiles in the session.
			for(Cur.Start();!Cur.End();Cur.Next())
			{
				GGroup* GroupExtra=Cur();
				// not compare with the same group...
				if(GroupExtra->GetId()!=Group->GetId())
				{
					GSubProfileCursor SubCur1= Group->GetSubProfileCursor();
					GSubProfileCursor SubCur2= GroupExtra->GetSubProfileCursor();
					for(SubCur1.Start();!SubCur1.End();SubCur1.Next())
					{
						for(SubCur2.Start();!SubCur2.End();SubCur2.Next())
						{
							double temp;
							if(Global) temp=SubCur1()->GlobalSimilarity(SubCur2());
							else temp=SubCur1()->Similarity(SubCur2());
							MeanExtraM+=temp;
							ComptExtra++;
							if((temp>MaxExtra)&&(temp<0.99)) MaxExtra=temp;
						}
					}
				}
			}
			ComptOverlap++;
			if(MaxExtra>MinIntra) Overlap+=(MaxExtra-MinIntra);
		}
	}
	MeanIntraM/=ComptIntra;
	MeanExtraM/=ComptExtra;
	if(ComptOverlap>0)
		Overlap/=ComptOverlap;
	Rie=(MeanIntraM-MeanExtraM)/MeanIntraM;
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
