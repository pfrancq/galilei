/*

	GALILEI Research Project

	GSatSimProfGrp.cpp

	Calc the similarity between subprofiles and groups using the ideal groupment - Implementation.

	Copyright 2002 by the Université Libre de Bruxelles.

	Authors:
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
#include <tests/gstatsimprofgrp.h>
#include <tests/ggroupevaluate.h>
#include <tests/ggroupevaluatesubprof.h>
#include <tests/ggroupsevaluate.h>
#include <tests/gstatsimid.h>
#include <sessions/gsession.h>
#include <infos/giwordsweights.h>
#include <infos/giwordweight.h>
#include <profiles/gprofilecalc.h>
#include <profiles/gprofile.h>
#include <profiles/gsubprofile.h>
#include <groups/ggroups.h>
#include <groups/ggroup.h>
#include <groups/ggroupcalc.h>
#include <groups/ggroupvector.h>
#include <docs/gdoc.h>
using namespace GALILEI;




//-----------------------------------------------------------------------------
//
// class GStatSimProfGrp
//
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
GALILEI::GStatSimProfGrp::GStatSimProfGrp(GSession* ses,RContainer<GGroups,unsigned int,true,true>* ideal)
	: Session(ses), IdealGroups(ideal),Global(true),relevant(true)
{
}


//-----------------------------------------------------------------------------
void GALILEI::GStatSimProfGrp::Run(void)
{
	double MeanIntraAll;
	double MeanExtraAll;
	double MinIntra;
	double MaxExtra;
	double MeanIntraMAll;
	double MeanExtraMAll;
	double overlap;
	double nbprofiles;

	MeanIntraMAll=0.0;
	MeanExtraMAll=0.0;
	overlap=0.0;
	nbprofiles=0.0;

	// David
	GGroupCalc* CalcMethod;
	if(relevant)
		Session->GetGroupCalcMng()->SetCurrentMethod("Protype");
	else
		Session->GetGroupCalcMng()->SetCurrentMethod("Relevant");
	CalcMethod=Session->GetGroupCalcMng()->GetCurrentMethod();


	int nbtot=0;

	for(IdealGroups->Start();!IdealGroups->End();IdealGroups->Next())
	{
		GGroupCursor Cur=(*IdealGroups)()->GetGroupCursor();
		GGroupCursor Cur2=(*IdealGroups)()->GetGroupCursor();
		for(Cur.Start();!Cur.End();Cur.Next())
		{
			//le groupe de sous profile
			GGroup* Group=Cur();
			GSubProfileCursor subcur= Group->GetSubProfileCursor();

			for(subcur.Start();!subcur.End();subcur.Next())
			{
				GSubProfile* SubProf=subcur();
				nbprofiles++;

				MinIntra=10;
				MaxExtra=0;

				int comptiall=0;
				int compteall=0;

				MeanIntraAll=0.0;
				MeanExtraAll=0.0;

				for(Cur2.Start();!Cur2.End();Cur2.Next())
				{
					GGroup* Group2=Cur2();
					CalcMethod->Compute(Group2);
					// Same Group
					if(Group2->GetId()==Group->GetId())
					{
					 	double similarity;
						if(Global)	similarity=static_cast<GGroupVector*>(Group)->GlobalSimilarity(SubProf);
						else similarity=static_cast<GGroupVector*>(Group)->Similarity(SubProf);
						if((similarity<10)&&(similarity>-10))
						{
							if(similarity<MinIntra) MinIntra=similarity;
							comptiall++;
							MeanIntraAll+=similarity;
						} 
					}
					//	Different group
					else
					{
					 	double similarity;
						if(Global)	similarity=static_cast<GGroupVector*>(Group2)->GlobalSimilarity(SubProf);
						else similarity=static_cast<GGroupVector*>(Group2)->Similarity(SubProf);
						if((similarity<10)&&(similarity>-10))
						{
							if(similarity>MaxExtra) MaxExtra=similarity;
							compteall++;
							MeanExtraAll+=similarity;
						}
					}
				}

				if(comptiall>0)
				{
					MeanIntraAll/=comptiall;
					MeanExtraAll/=compteall;

					if(MaxExtra>=MinIntra) overlap++;
					MeanIntraMAll+=MeanIntraAll;
					MeanExtraMAll+=MeanExtraAll;
					nbtot++;
				}
			}
		}
	}	
	overlap/=nbprofiles;
	cout<<"MeanIntraMAll/nbtot"<<" "<<"MeanExtraMAll/nbtot"<<" "<<"RIE"<<"overlapp"<<endl;
	cout<<MeanIntraMAll/nbtot<<" "<<MeanExtraMAll/nbtot<<" "<<((MeanIntraMAll/nbtot)-(MeanExtraMAll/nbtot))/(MeanIntraMAll/nbtot)<<" "<<overlap<<endl;
}


//-----------------------------------------------------------------------------
char* GALILEI::GStatSimProfGrp::GetSettings(void)
{
	static char tmp[100];
	char c;
	char c1;

	if(Global) c='1'; else c='0';
	if(relevant) c1='1'; else c1='0';
	sprintf(tmp,"%c %c",c,c1);
	return(tmp);
}


//-----------------------------------------------------------------------------
void GALILEI::GStatSimProfGrp::SetSettings(const char* s)
{
	char c;
	char c1;

		//mettre le rerelevant
	if(!(*s)) return;
	sscanf(s,"%c %c",&c,&c1);
	if(c=='1') Global=true; else Global=false;
	if(c1=='1') relevant=true; else relevant=false;
}


//-----------------------------------------------------------------------------
GALILEI::GStatSimProfGrp::~GStatSimProfGrp(void)
{
}
