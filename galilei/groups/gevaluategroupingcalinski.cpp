/*

	GALILEI Research Project

	GEvaluateGroupingCalinski.cpp

	Compare a ideal groupement with a computed one - Implementation

	(C) 2002 by David Wartel

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
#include<groups/gevaluategroupingcalinski.h>
#include<groups/ggroup.h>
#include<groups/ggroups.h>
#include<profiles/gsubprofile.h>
using namespace GALILEI;


//-----------------------------------------------------------------------------
//
//  GEvaluateGroupingCalinski
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GALILEI::GEvaluateGroupingCalinski::GEvaluateGroupingCalinski(GSession* s
			,RContainer<GGroups,unsigned int,true,true>* groups)  throw(bad_alloc)
	: GEvaluateGrouping("Calinski Index", s, groups, 1)
{

}


//-----------------------------------------------------------------------------
double GALILEI::GEvaluateGroupingCalinski::Run(void)
{
	return(CalcCalinski());
}



//-----------------------------------------------------------------------------
GSubProfile* GALILEI::GEvaluateGroupingCalinski::RelevantSubProfile(GGroup* grp)
{
	GSubProfile* finalsub;
	GSubProfile* sub1;
	double refsum=0.0;
	for(grp->Start(); !grp->End(); grp->Next())
	{
		sub1 = (*grp)();
		double sum=GroupSumSimilarity(sub1, grp);
		if (sum>=refsum)
		{
			finalsub=sub1;
			refsum=sum;
		}	
	}
	return(finalsub);
}


//-----------------------------------------------------------------------------
double GALILEI::GEvaluateGroupingCalinski::GroupSumSimilarity(GSubProfile * s, GGroup *grp)
{
	double sum=0.0;
	GSubProfile** sub1;
	unsigned i;
	for(sub1=grp->Tab,i=grp->NbPtr;--i;sub1++)
		sum=sum+s->Similarity((*sub1));
	return(sum);
}



//-----------------------------------------------------------------------------
double GALILEI::GEvaluateGroupingCalinski::CalcCalinski()
{                                 
	GSubProfile* mean;
	double refsum=0.0, ssw=0.0, ssb=0.0;
	double cal;
	unsigned int k=GetNbGroups();
	int n=SubProfiles.NbPtr;

	for (SubProfiles.Start();!SubProfiles.End(); SubProfiles.Next())
	{
		double sum=SumSimilarity((SubProfiles)());
		if (sum>refsum)
		{
			refsum=sum;
			mean=(SubProfiles)();
		} 
	}

   // ssw & ssb calculation 
	for (Groups->Start(); !Groups->End(); Groups->Next())
	{
		if ((*Groups)()->NbPtr==0) continue;
		for ((*Groups)()->Start(); !(*Groups)()->End(); (*Groups)()->Next())
		{
			GGroup* gr=(*(*Groups)())();
			GSubProfile* center= RelevantSubProfile(gr);
			for (gr->Start(); !gr->End(); gr->Next())
			{
				double dist=(1.0)-(*gr)()->Similarity(center);
				ssw=ssw+(dist*dist);
			}
			double dist2=(1.0)-(center->Similarity(mean));
			ssb=ssb+((gr->NbPtr)*(dist2*dist2));
		}
	}

	//index calculation
	cal=(ssb/(k-1))/(ssw/(n-k)); 
	return(cal);
}


//-----------------------------------------------------------------------------
double GALILEI::GEvaluateGroupingCalinski::SumSimilarity(GSubProfile * s)
{
	double sum=0.0;
	unsigned i;
	GSubProfile** sub1;

	for (sub1=SubProfiles.Tab,i=SubProfiles.NbPtr;--i;sub1++)
		sum=sum+s->Similarity((*sub1));
	return(sum);
}

//-----------------------------------------------------------------------------
int GALILEI::GEvaluateGroupingCalinski::GetNbGroups(void)
{
	unsigned int k=0;

	for (Groups->Start(); !Groups->End(); Groups->Next())
		k+=(*Groups)()->NbPtr; 
	cout << "nb groupes:="<<k<<endl;
	return(k);
}



//-----------------------------------------------------------------------------
GALILEI::GEvaluateGroupingCalinski::~GEvaluateGroupingCalinski(void)
{
}
