/*

	GALILEI Research Project

	GSatSimSubProf.cpp

	Similarities between Subprofiles - Implementation.

	Copyright 2002 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).

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
#include <langs/glang.h>
#include <sessions/gsession.h>
#include <profiles/gprofile.h>
#include <infos/giwordsweights.h>
#include <infos/giwordweight.h>
#include <profiles/gprofilecalc.h>
#include <tests/gstatsimsubprof.h>
#if GALILEITEST
	#include <groups/gsubject.h>
#endif
using namespace GALILEI;



//-----------------------------------------------------------------------------
//
// class Local
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
class GALILEI::GStatSimSubProf::LocalStat
{
public:
	GSubject* Sub;
	bool OverlapL;
	bool OverlapG;

	LocalStat(GSubject* s) : Sub(s), OverlapL(false), OverlapG(false) {}
	int Compare(const LocalStat* l) const {return(Sub->Compare(l->Sub));}
	int Compare(const GSubject* s) const {return(Sub->Compare(s));}
};



//-----------------------------------------------------------------------------
//
// class GStatSimSubProf
//
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
GALILEI::GStatSimSubProf::GStatSimSubProf(GSession* ses,R::RTextFile* f,bool g,bool l)
	: Session(ses), Global(g), Local(l), File(f), Sub(100,50)
{
	if(File)
	{
		(*File)<<"SubProfileId";
		if(Global) (*File)<<"RieGlobal";
		if(Local) (*File)<<"RieLocal";
		(*File)<<endl;
	}
}


//-----------------------------------------------------------------------------
void GALILEI::GStatSimSubProf::Run(void)
{
#if GALILEITEST
	GLangCursor Langs;
	GSubProfileCursor Sub1;
	GSubProfileCursor Sub2;
	GIWordWeightCursor v1;
	double SimIntraL;
	double SimExtraL;
	double SimIntraG;
	double SimExtraG;
	double tmp;
	unsigned int nbIntra;
	unsigned int nbExtra;
	unsigned int nbSubProf;
	bool Same;
	double MinIntraL;
	double MaxExtraL;
	double MinIntraG;
	double MaxExtraG;
	double minG,minL,maxG,maxL;
	LocalStat* t;

	//Initialization
	OverlapL=MeanExtraML=MeanIntraML=0.0;
	OverlapG=MeanExtraMG=MeanIntraMG=0.0;
	nbSubProf=0;
	Sub.Clear();
	minG=minL=1.1;
	maxG=maxL=-1.1;

	// Go through the languages
	Langs=Session->GetLangsCursor();
	for(Langs.Start();!Langs.End();Langs.Next())
	{
		Sub1=Session->GetSubProfilesCursor(Langs());
		Sub2=Session->GetSubProfilesCursor(Langs());

		// Go through each subprofile
		for(Sub1.Start();!Sub1.End();Sub1.Next())
		{
			// Verify if subprofile defined
			if(!Sub1()->IsDefined()) continue;
			nbSubProf++;

			// Init Local Data
			SimIntraG=SimExtraG=SimIntraL=SimExtraL=0.0;
			nbIntra=nbExtra=0;
			MinIntraG=MinIntraL=1.1;
			MaxExtraG=MaxExtraL=-1.1;

			// Go through to other subprofiles
			for(Sub2.Start();!Sub2.End();Sub2.Next())
			{
				if(Sub1()==Sub2()) continue;
				if(!Sub2()->IsDefined()) continue;

				// Look if Same topic
				Same=(Sub1()->GetSubject()==Sub2()->GetSubject());
				if(Same)
					nbIntra++;
				else
					nbExtra++;

				if(Global)
				{
					tmp=Sub1()->GlobalSimilarity(Sub2());
					if(Same)
					{
						SimIntraG+=tmp;
						if(tmp<MinIntraG) MinIntraG=tmp;
						if(tmp<minG)
						{
							minG=tmp;
							cout<<"similar : "<<Sub1()->GetId()<<" - "<<Sub2()->GetId()<<" = "<<tmp<<endl;
							v1=Sub1()->GetVectorCursor();
							for(v1.Start();!v1.End();v1.Next())
								cout<<v1()->GetId()<<" ";
							cout<<endl;
							v1=Sub2()->GetVectorCursor();
							for(v1.Start();!v1.End();v1.Next())
								cout<<v1()->GetId()<<" ";
							cout<<endl;
						}
					}
					else
					{
						SimExtraG+=tmp;
						if(tmp>MaxExtraG) MaxExtraG=tmp;
						if(tmp>maxG)
						{
							maxG=tmp;
							cout<<"not similar : "<<Sub1()->GetId()<<" - "<<Sub2()->GetId()<<" = "<<tmp<<endl;
							v1=Sub1()->GetVectorCursor();
							for(v1.Start();!v1.End();v1.Next())
								cout<<v1()->GetId()<<" ";
							cout<<endl;
							v1=Sub2()->GetVectorCursor();
							for(v1.Start();!v1.End();v1.Next())
								cout<<v1()->GetId()<<" ";
							cout<<endl;
						}
					}
				}

				if(Local)
				{
					tmp=Sub1()->Similarity(Sub2());
					if(Same)
					{
						SimIntraL+=tmp;
						if(tmp<MinIntraL) MinIntraL=tmp;
						if(tmp<minL) minL=tmp;
					}
					else
					{
						SimExtraL+=tmp;
						if(tmp>MaxExtraL) MaxExtraL=tmp;
						if(tmp>maxL) maxL=tmp;
					}
				}
			}

			// Compute Overlap
			t=Sub.GetInsertPtr<GSubject*>(Sub1()->GetSubject());
			if(Global&&(MaxExtraG>MinIntraG))
			{
				OverlapG++;
				t->OverlapG=true;
			}
			if(Local&&(MaxExtraL>MinIntraL))
			{
				OverlapL++;
				t->OverlapL=true;
			}

			// Compute Rie for document Sub1
			if(File)
			{
				(*File)<<Sub1()->GetId();
				if(Global)
				{
					SimIntraG/=nbIntra;
					SimExtraG/=nbExtra;
					MeanIntraMG+=SimIntraG;
					MeanExtraMG+=SimExtraG;
					tmp=(SimIntraG-SimExtraG)/SimIntraG;
					(*File)<<tmp;
				}
				if(Local)
				{
					SimIntraL/=nbIntra;
					SimExtraL/=nbExtra;
					MeanIntraML+=SimIntraL;
					MeanExtraML+=SimExtraL;
					tmp=(SimIntraL-SimExtraL)/SimIntraL;
					(*File)<<tmp;
				}
				(*File)<<endl;
			}
		}
	}
	if(Global)
	{
		MeanIntraMG/=nbSubProf;
		MeanExtraMG/=nbSubProf;
		RieG=(MeanIntraMG-MeanExtraMG)/MeanIntraMG;
	}
	if(Local)
	{
		MeanIntraML/=nbSubProf;
		MeanExtraML/=nbSubProf;
		RieL=(MeanIntraML-MeanExtraML)/MeanIntraML;
	}
	OverlapL/=nbSubProf;
	OverlapG/=nbSubProf;
	for(Sub.Start(),GOverlapG=0,GOverlapL=0;!Sub.End();Sub.Next())
	{
		if(Sub()->OverlapG) GOverlapG++;
		if(Sub()->OverlapL) GOverlapL++;
	}
	GOverlapG/=Sub.NbPtr;
	GOverlapL/=Sub.NbPtr;
	cout<<"MaxSimInterG : "<<maxG<<"  MinSimIntraG : "<<minG<<"  MaxSimInterL : "<<maxL<<"  MinSimIntraL : "<<minL<<endl;
#endif
}


//-----------------------------------------------------------------------------
GALILEI::GStatSimSubProf::~GStatSimSubProf(void)
{
}
