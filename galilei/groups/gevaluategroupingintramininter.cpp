/*

	GALILEI Research Project

	GEvaluateGroupingIntraMinInter.cpp

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
#include<groups/gevaluategroupingintramininter.h>
#include<groups/ggroup.h>
#include<groups/ggroups.h>
#include<profiles/gsubprofile.h>
using namespace GALILEI;


//-----------------------------------------------------------------------------
//
//  GEvaluateGroupingIntraMinInter
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GALILEI::GEvaluateGroupingIntraMinInter::GEvaluateGroupingIntraMinInter(GSession* s
			,RContainer<GGroups,unsigned int,true,true>* groups)  throw(bad_alloc)
	: GEvaluateGrouping("Intra / Min(Inter)", s, groups, 3)
{

}


//-----------------------------------------------------------------------------
double GALILEI::GEvaluateGroupingIntraMinInter::Run(void)
{
	return(CalcIntraMinInter());
}



//-----------------------------------------------------------------------------
GSubProfile* GALILEI::GEvaluateGroupingIntraMinInter::RelevantSubProfile(GGroup* grp)
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
double GALILEI::GEvaluateGroupingIntraMinInter::GroupSumSimilarity(GSubProfile * s, GGroup *grp)
{
	double sum=0.0;
	GSubProfile** sub1;
	unsigned i;

	for(sub1=grp->Tab,i=grp->NbPtr;--i;sub1++)
		sum=sum+s->Similarity((*sub1));
	return(sum);
}



//-----------------------------------------------------------------------------
double GALILEI::GEvaluateGroupingIntraMinInter::CalcIntraMinInter()
{                                 
	double intra, inter=1.0;
	int nbsubprofiles=0;
	GSubProfile **s1, **s2;
	unsigned int i, j;

	RContainer<GSubProfile,unsigned int,false,false>* protos;
	protos = new RContainer<GSubProfile,unsigned int,false,false> (25, 10);

	// calculation of 'intra'
	for (Groups->Start(); !Groups->End(); Groups->Next())
	{
		if ((*Groups)()->NbPtr==0) continue;
		for ((*Groups)()->Start(); !(*Groups)()->End(); (*Groups)()->Next())
		{
			GGroup* grp=(*(*Groups)())() ;
			GSubProfile* s1= RelevantSubProfile(grp);
			protos->InsertPtr(s1);
			for (grp->Start(); !grp->End(); grp->Next())
			{
				double dist=(1.0)-((*grp)()->Similarity(s1));
				intra=intra+(dist*dist);
				nbsubprofiles++;
			}
		}
	}
	intra/=nbsubprofiles;

	// calculation of 'min(inter)'
	
	for(s1=protos->Tab,i=protos->NbPtr;--i;s1++)
	{
		for(s2=s1+1,j=i+1;--j;s2++)
		{
			double dist=((1-0)-(*s1)->Similarity(*s2));
			dist=dist*dist;
			if(dist<=inter)
				inter=dist;
		}
	}

	return(intra/(inter*GetNbGroups()));
	
}


//-----------------------------------------------------------------------------
double GALILEI::GEvaluateGroupingIntraMinInter::SumSimilarity(GSubProfile * s)
{
	double sum=0.0;
	GSubProfile** sub1;
	unsigned i;

	for (sub1=SubProfiles.Tab,i=SubProfiles.NbPtr;--i;sub1++)
		sum=sum+s->Similarity((*sub1));
	return(sum);
}

//-----------------------------------------------------------------------------
int GALILEI::GEvaluateGroupingIntraMinInter::GetNbGroups(void)
{
	unsigned int k=0;

	for (Groups->Start(); !Groups->End(); Groups->Next())
		k+=(*Groups)()->NbPtr; 
	cout << "nb groupes:="<<k<<endl;

	return(k);
}



//-----------------------------------------------------------------------------
GALILEI::GEvaluateGroupingIntraMinInter::~GEvaluateGroupingIntraMinInter(void)
{
}
