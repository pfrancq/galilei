/*

	GALILEI Research Project

	GSatSimDocGrp.cpp

	Similarities between Documents and Groups - Implementation.

	Copyright 2002 by the Universit� Libre de Bruxelles.

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
#include <tests/ggroupsevaluate.h>
#include <tests/gstatsimdocgrp.h>
#include <sessions/gsession.h>
#include <infos/giwordsweights.h>
#include <infos/giwordweight.h>
#include <profiles/gprofile.h>
#include <groups/ggroups.h>
#include <groups/ggroup.h>
#include <groups/ggroupcalcgravitation.h>
#include <groups/ggroupcalcrelevant.h>
#include <groups/ggroupvector.h>
#include <docs/gdoc.h>
#include <langs/glang.h>
#if GALILEITEST
	#include <groups/gsubject.h>
#endif
using namespace GALILEI;
using namespace R;



//-----------------------------------------------------------------------------
//
// class SimGroup
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
class GALILEI::GStatSimDocGrp::SimGroup
{
public:
	bool Good;
	double Sim;

	SimGroup(bool g,double s) : Good(g), Sim(s) {}
	int Compare(const SimGroup&) const {return(-1);}
	int Compare(const SimGroup*) const {return(-1);}
};



//-----------------------------------------------------------------------------
//
// class Local
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
class GALILEI::GStatSimDocGrp::LocalStat
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
// class GStatSimDocGrp
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GALILEI::GStatSimDocGrp::GStatSimDocGrp(GSession* ses,RTextFile* f,bool g,bool l)
	: Session(ses), Global(g), Local(l), File(f), Sub(100,50), DocsL(25000,10000), DocsG(25000,10000)
{
	if(File)
	{
		(*File)<<"GId";
		if(Global) (*File)<<"RieGlobal\tR0_Global\tR10_Global\tR20_Global\tR30_Global\tR40_Global\tR50_Global\tR60_Global\tR70_Global\tR80_Global\tR90_Global\tR100_Global";
		if(Local) (*File)<<"RieLocal\tR0_Local\tR10_Local\tR20_Local\tR30_Local\tR40_Local\tR50_Local\tR60_Local\tR70_Local\tR80_Local\tR90_Local\tR100_Local";
		(*File)<<endl;
	}
}


//--------------------------------------------------------------------------
int GALILEI::GStatSimDocGrp::sortOrder(const void *a,const void *b)
{

  double af=(*((SimGroup**)(a)))->Sim;
  double bf=(*((SimGroup**)(b)))->Sim;

  if(af==bf) return(0);
  if(af>bf)
    return(-1);
  else
    return(1);
}


//-----------------------------------------------------------------------------
void GALILEI::GStatSimDocGrp::Run(void)
{
#if GALILEITEST
	GGroupsCursor Grps;
	GGroupCursor Grp1;
	GDocCursor Doc2;
	double SimIntraL;
	double SimExtraL;
	double SimIntraG;
	double SimExtraG;
	double tmp;
	unsigned int nbIntra;
	unsigned int nbExtra;
	unsigned int nbGrp;
	bool Same;
	double MinIntraL;
	double MaxExtraL;
	double MinIntraG;
	double MaxExtraG;
	LocalStat* t;
	unsigned int level;
	unsigned int nbGood;
	unsigned int nb;
	unsigned int nbDocs;
	double SPrecisionL[11];
	double SPrecisionG[11];
	GSubject* Subject;

	//Initialization
	OverlapL=MeanExtraML=MeanIntraML=0.0;
	OverlapG=MeanExtraMG=MeanIntraMG=0.0;
	memset(PrecisionL,0x00,11*sizeof(double));
	memset(PrecisionG,0x00,11*sizeof(double));
	nbGrp=0;
	Sub.Clear();

	// Go through the groups
	Grps=Session->GetGroupsCursor();
	for(Grps.Start();!Grps.End();Grps.Next())
	{
		// Init cursors
		Grp1=Grps()->GetGroupCursor();
		Doc2=Session->GetDocsCursor(Grps()->GetLang());

		// Go through each group
		for(Grp1.Start();!Grp1.End();Grp1.Next())
		{
			// Verify if group is empty
			if(Grp1()->IsEmpty()) continue;
			nbGrp++;

			// Suppose subject of the group is subject of the first subprofile
			// contained
			Subject=Grp1()->Tab[0]->GetSubject();
		
			// Init Local Data
			SimIntraG=SimExtraG=SimIntraL=SimExtraL=0.0;
			nbIntra=nbExtra=0;
			MinIntraG=MinIntraL=1.1;
			MaxExtraG=MaxExtraL=-1.1;
			DocsL.Clear();
			DocsG.Clear();

			// Go through to documents
			for(Doc2.Start();!Doc2.End();Doc2.Next())
			{
				// Look if Same topic
				Same=Doc2()->IsFromSubject(Subject);
				if(Same)
					nbIntra++;
				else
					nbExtra++;

				if(Global)
				{
					tmp=Grp1()->GlobalSimilarity(Doc2());
					DocsG.InsertPtr(new SimGroup(tmp,Same));
					if(Same)
					{
						SimIntraG+=tmp;
						if(tmp<MinIntraG) MinIntraG=tmp;
					}
					else
					{
						SimExtraG+=tmp;
						if(tmp>MaxExtraG) MaxExtraG=tmp;
					}
				}

				if(Local)
				{
					tmp=Grp1()->Similarity(Doc2());
					DocsL.InsertPtr(new SimGroup(tmp,Same));
					if(Same)
					{
						SimIntraL+=tmp;
						if(tmp<MinIntraL) MinIntraL=tmp;
					}
					else
					{
						SimExtraL+=tmp;
						if(tmp>MaxExtraL) MaxExtraL=tmp;
					}
				}
			}

			// Compute Overlap
			t=Sub.GetInsertPtr<GSubject*>(Subject);
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

			// Ordered similarities
			qsort(static_cast<void*>(DocsG.Tab),DocsG.NbPtr,sizeof(SimGroup*),sortOrder);
			qsort(static_cast<void*>(DocsL.Tab),DocsL.NbPtr,sizeof(SimGroup*),sortOrder);

			// Compute Precision at Recall Level 0%
			if(Global)
			{
				if(DocsG.Tab[0]->Good)
					SPrecisionG[0]=1.0;
				else
					SPrecisionG[0]=0.0;
				PrecisionG[0]+=SPrecisionG[0];
			}
			if(Local)
			{
				if(DocsL.Tab[0]->Good)
					SPrecisionL[0]=1.0;
				else
					SPrecisionL[0]=0.0;
				PrecisionL[0]+=SPrecisionL[0];
			}

			// Compute Precision at Recall Level 10% -> 100%
			for(level=1;level<11;level++)
			{
					nbDocs=static_cast<unsigned int>(0.1*level*Subject->GetNbDocs());

					// Parse Global
					if(Global)
					{
						for(DocsG.Start(),nb=0,nbGood=0;(nbGood<nbDocs)&&(!DocsG.End());DocsG.Next(),nb++)
						{
							if(DocsG()->Good)
								nbGood++;
						}
						if(nb)
							SPrecisionG[level]=static_cast<double>(nbGood)/static_cast<double>(nb);
						else
							SPrecisionG[level]=1.0;
						PrecisionG[level]+=SPrecisionG[level];
					}

					// Parse Local
					if(Local)
					{
						for(DocsL.Start(),nb=0,nbGood=0;(nbGood<nbDocs)&&(!DocsL.End());DocsL.Next(),nb++)
						{
							if(DocsL()->Good)
								nbGood++;
						}
						if(nb!=0)
							SPrecisionL[level]=static_cast<double>(nbGood)/static_cast<double>(nb);
						else
							SPrecisionL[level]=1.0;
						PrecisionL[level]+=SPrecisionL[level];
					}
				}

			// Compute Rie for document Grp1
			if(File)
			{
				(*File)<<Grp1()->GetId();
				if(Global)
				{
					SimIntraG/=nbIntra;
					SimExtraG/=nbExtra;
					MeanIntraMG+=SimIntraG;
					MeanExtraMG+=SimExtraG;
					tmp=(SimIntraG-SimExtraG)/SimIntraG;
					(*File)<<tmp;
					for(level=0;level<11;level++)
						(*File)<<SPrecisionG[level];
				}
				if(Local)
				{
					SimIntraL/=nbIntra;
					SimExtraL/=nbExtra;
					MeanIntraML+=SimIntraL;
					MeanExtraML+=SimExtraL;
					tmp=(SimIntraL-SimExtraL)/SimIntraL;
					(*File)<<tmp;
					for(level=0;level<11;level++)
						(*File)<<SPrecisionL[level];
				}
				(*File)<<endl;
			}
		}
	}
	if(Global)
	{
		MeanIntraMG/=nbGrp;
		MeanExtraMG/=nbGrp;
		RieG=(MeanIntraMG-MeanExtraMG)/MeanIntraMG;
	}
	if(Local)
	{
		MeanIntraML/=nbGrp;
		MeanExtraML/=nbGrp;
		RieL=(MeanIntraML-MeanExtraML)/MeanIntraML;
	}
	OverlapL/=nbGrp;
	OverlapG/=nbGrp;
	for(Sub.Start(),GOverlapG=0,GOverlapL=0;!Sub.End();Sub.Next())
	{
		if(Sub()->OverlapG) GOverlapG++;
		if(Sub()->OverlapL) GOverlapL++;
	}
	GOverlapG/=Sub.NbPtr;
	GOverlapL/=Sub.NbPtr;
	for(level=0;level<11;level++)
	{
		if(Local) PrecisionL[level]/=nbGrp;
		if(Global) PrecisionG[level]/=nbGrp;
	}
#endif
}


//-----------------------------------------------------------------------------
GALILEI::GStatSimDocGrp::~GStatSimDocGrp(void)
{
}
