/*

	GALILEI Research Project

	GEvaluateGroupingVariance.cpp

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
#include<groups/gevaluategroupingvariance.h>
#include<groups/ggroup.h>
#include<groups/ggroups.h>
#include<profiles/gsubprofile.h>

using namespace GALILEI;


//-----------------------------------------------------------------------------
//
//  GEvaluateGroupingVariance
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GALILEI::GEvaluateGroupingVariance::GEvaluateGroupingVariance(GSession* s
			,RContainer<GGroups,unsigned int,true,true>* groups)  throw(bad_alloc)
	: GEvaluateGrouping("Variance", s, groups, 2)
{

}


//-----------------------------------------------------------------------------
double GALILEI::GEvaluateGroupingVariance::Run(void)
{
	return(CalcVariance());
}



//-----------------------------------------------------------------------------
double GALILEI::GEvaluateGroupingVariance::CalcVariance()
{                                 
	double var;
	int nbsubprofiles=0;
	// calculation of variance
	for (Groups->Start(); !Groups->End(); Groups->Next())
	{
		if ((*Groups)()->NbPtr==0) continue;
		for ((*Groups)()->Start(); !(*Groups)()->End(); (*Groups)()->Next())
		{
			GGroup* grp=(*(*Groups)())() ;
			GSubProfile* s1= RelevantSubProfile(grp);
			for (grp->Start(); !grp->End(); grp->Next())
			{
				double dist=(1.0)-((*grp)()->Similarity(s1));
				var+=dist;
				nbsubprofiles++;
			}
		}
	}
	return(var);
}

//-----------------------------------------------------------------------------
GSubProfile* GALILEI::GEvaluateGroupingVariance::RelevantSubProfile(GGroup* grp)
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
double GALILEI::GEvaluateGroupingVariance::GroupSumSimilarity(GSubProfile * s, GGroup *grp)
{
	double sum=0.0;
	GSubProfile** sub1;
	unsigned i;

	for(sub1=grp->Tab,i=grp->NbPtr;--i;sub1++)
		sum=sum+s->Similarity((*sub1));
	return(sum);
}


//-----------------------------------------------------------------------------
GALILEI::GEvaluateGroupingVariance::~GEvaluateGroupingVariance(void)
{
}
