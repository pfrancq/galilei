/*

	GALILEI Research Project

	GSatSimDoc.cpp

	Similarities between Documents - Implementation.

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
#include <sessions/gsession.h>
#include <langs/glang.h>
#include <infos/giwordsweights.h>
#include <infos/giwordweight.h>
#include <profiles/gprofilecalc.h>
#include <tests/gstatsimdoc.h>
#include <docs/gdoc.h>
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
class GALILEI::GStatSimDoc::LocalStat
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
// class GStatSimDoc
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GALILEI::GStatSimDoc::GStatSimDoc(GSession* ses,RIO::RTextFile* f,bool g,bool l)
	: Session(ses), Global(g), Local(l), File(f), Sub(100,50)
{
	if(File)
	{
		(*File)<<"DocId";
		if(Global) (*File)<<"RieGlobal";
		if(Local) (*File)<<"RieLocal";
		(*File)<<endl;
	}
}


//-----------------------------------------------------------------------------
void GALILEI::GStatSimDoc::Run(void)
{
#if GALILEITEST
	GLangCursor Langs;
	GDocCursor Doc1;
	GDocCursor Doc2;
	GSubjectCursor Sub1;
	double SimIntraL;
	double SimExtraL;
	double SimIntraG;
	double SimExtraG;
	double tmp;
	unsigned int nbIntra;
	unsigned int nbExtra;
	unsigned int nbDocs;
	bool Same;
	double MinIntraL;
	double MaxExtraL;
	double MinIntraG;
	double MaxExtraG;
	LocalStat* t;

	//Initialization
	OverlapL=MeanExtraML=MeanIntraML=0.0;
	OverlapG=MeanExtraMG=MeanIntraMG=0.0;
	nbDocs=0;
	Sub.Clear();

	// Go through the languages
	Langs=Session->GetLangsCursor();
	for(Langs.Start();!Langs.End();Langs.Next())
	{
		Doc1=Session->GetDocsCursor(Langs());
		Doc2=Session->GetDocsCursor(Langs());

		// Go through each document
		for(Doc1.Start();!Doc1.End();Doc1.Next())
		{
			nbDocs++;

			// Init Local Data
			SimIntraG=SimExtraG=SimIntraL=SimExtraL=0.0;
			nbIntra=nbExtra=0;
			Sub1=Doc1()->GetSubjectCursor();
			MinIntraG=MinIntraL=1.1;
			MaxExtraG=MaxExtraL=-1.1;

			// Go through to other documents
			for(Doc2.Start();!Doc2.End();Doc2.Next())
			{
				if(Doc1()==Doc2()) continue;

				// Look if Same topic
				Same=false;
				for(Sub1.Start();(!Sub1.End())&&(!Same);Sub1.Next())
					if(Doc2()->IsFromSubject(Sub1()))
						Same=true;
				if(Same)
					nbIntra++;
				else
					nbExtra++;

				if(Global)
				{
					tmp=Doc1()->GlobalSimilarity(Doc2());
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
					tmp=Doc1()->Similarity(Doc2());
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
				for(Sub1.Start();(!Sub1.End())&&(!Same);Sub1.Next())
				{
					t=Sub.GetInsertPtr<GSubject*>(Sub1());
				}

			if(Global&&(MaxExtraG>MinIntraG))
			{
				OverlapG++;
				for(Sub1.Start();(!Sub1.End())&&(!Same);Sub1.Next())
				{
					t=Sub.GetInsertPtr<GSubject*>(Sub1());
					t->OverlapG=true;
				}
			}
			if(Local&&(MaxExtraL>MinIntraL))
			{
				OverlapL++;
				for(Sub1.Start();(!Sub1.End())&&(!Same);Sub1.Next())
				{
					t=Sub.GetInsertPtr<GSubject*>(Sub1());
					t->OverlapL=true;
				}
			}

			// Compute Rie for document Doc1
			if(File)
			{
				(*File)<<Doc1()->GetId();
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
		MeanIntraMG/=nbDocs;
		MeanExtraMG/=nbDocs;
		RieG=(MeanIntraMG-MeanExtraMG)/MeanIntraMG;
	}
	if(Local)
	{
		MeanIntraML/=nbDocs;
		MeanExtraML/=nbDocs;
		RieL=(MeanIntraML-MeanExtraML)/MeanIntraML;
	}
	OverlapL/=nbDocs;
	OverlapG/=nbDocs;
	for(Sub.Start(),GOverlapG=0,GOverlapL=0;!Sub.End();Sub.Next())
	{
		if(Sub()->OverlapG) GOverlapG++;
		if(Sub()->OverlapL) GOverlapL++;
	}
	GOverlapG/=Sub.NbPtr;
	GOverlapL/=Sub.NbPtr;
#endif
}


//-----------------------------------------------------------------------------
GALILEI::GStatSimDoc::~GStatSimDoc(void)
{
}
