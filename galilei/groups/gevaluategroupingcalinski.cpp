/*

	GALILEI Research Project

	GEvaluateGroupingCalinski.cpp

	Compare a ideal groupement with a computed one - Implementation

	Copyright 2002 by the Université Libre de Bruxelles.

	Authors:
		David Wartel (dwartel@ulb.ac.be).

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
#include <groups/gevaluategroupingcalinski.h>
#include <tests/ggroupsevaluate.h>
#include <tests/ggroupevaluate.h>
#include <groups/ggroup.h>
#include <groups/ggroups.h>
#include <profiles/gsubprofile.h>
using namespace GALILEI;


//-----------------------------------------------------------------------------
//
//  GEvaluateGroupingCalinski
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GALILEI::GEvaluateGroupingCalinski::GEvaluateGroupingCalinski(GSession* s
			,RContainer<GGroupsEvaluate,unsigned int,false,false>* groups)  throw(bad_alloc)
	: GEvaluateGrouping("Calinski Index", s, groups, 1)
{

}


//-----------------------------------------------------------------------------
double GALILEI::GEvaluateGroupingCalinski::Run(void)
{
	return(CalcCalinski());
}


//-----------------------------------------------------------------------------
double GALILEI::GEvaluateGroupingCalinski::CalcCalinski()
{                                 
	unsigned int mean;
	double refsum=-1.0, ssw=0.0, ssb=0.0;
	double sum;
	double cal;
	int n;

	n=0;
	for (Groups->Start(); !Groups->End(); Groups->Next())
	{
		GGroupsEvaluate* grs=(*Groups)();
		for (grs->Start(); !grs->End();grs->Next())
		{
		n+=(*grs)()->NbProfDoc();
		}
	}

	for (Groups->Start(); !Groups->End(); Groups->Next())
	{
		GGroupsEvaluate* grs=(*Groups)();
		for (grs->Start(); !grs->End();grs->Next())
		{
			GGroupEvaluate* gr=(*grs)();
			for(gr->Start(); !gr->End();gr->Next())
			{
				sum=gr->SumSimilarity(gr->Current());
				if (sum>refsum)
				{
					refsum=sum;
					mean=gr->Current();
				}
			}
		}
	}
	// ssw & ssb calculation
	for (Groups->Start(); !Groups->End(); Groups->Next())
	{
		if ((*Groups)()->NbPtr==0) continue;
		for ((*Groups)()->Start(); !(*Groups)()->End(); (*Groups)()->Next())
		{
			GGroupEvaluate* gr=(*(*Groups)())();
			unsigned int center=gr->RelevantSubDoc();
			for (gr->Start(); !gr->End(); gr->Next())
			{
				double dist=(1.0)-gr->Similarity(center,gr->Current());
				ssw=ssw+(dist*dist);
			}
			double dist2=(1.0)-(gr->Similarity(mean,center));
			ssb=ssb+((gr->NbPtr())*(dist2*dist2));
		}
	}

	//index calculation
//	cal=(ssb/(k-1))/(ssw/(n-k));
	cal=ssb/ssw;
	return(cal);
}



//-----------------------------------------------------------------------------
int GALILEI::GEvaluateGroupingCalinski::GetNbGroups(void)
{
	unsigned int k=0;

	for (Groups->Start(); !Groups->End(); Groups->Next())
		k+=(*Groups)()->NbPtr;
	return(k);
}



//-----------------------------------------------------------------------------
GALILEI::GEvaluateGroupingCalinski::~GEvaluateGroupingCalinski(void)
{
}
