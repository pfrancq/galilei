/*

	GALILEI Research Project

	GEvaluateGroupingIntraMinInter.cpp

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
#include <groups/gevaluategroupingintramininter.h>
#include <tests/ggroupsevaluate.h>
#include <tests/ggroupevaluate.h>
#include <groups/ggroup.h>
#include <groups/ggroups.h>
#include <profiles/gsubprofile.h>
using namespace GALILEI;
using namespace R;



//-----------------------------------------------------------------------------
//
//  GEvaluateGroupingIntraMinInter
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GALILEI::GEvaluateGroupingIntraMinInter::GEvaluateGroupingIntraMinInter(GSession* s
			,RContainer<GGroupsEvaluate,unsigned int,false,false>* groups)  throw(bad_alloc)
	: GEvaluateGrouping("Intra / Min(Inter)", s, groups, 3)
{

}


//-----------------------------------------------------------------------------
double GALILEI::GEvaluateGroupingIntraMinInter::Run(void)
{
	return(CalcIntraMinInter());
}



//-----------------------------------------------------------------------------
double GALILEI::GEvaluateGroupingIntraMinInter::CalcIntraMinInter()
{
	double intra=0.0, inter=1.0;
	int nbsubprofiles=0;
	unsigned int i,j,s1,s2;
	
	int* protos;
	protos=new int [GetNbGroups()];
	for (int i=0; i<GetNbGroups();i++)
	protos[i]=-1;

	// calculation of 'intra'
	i=0;
	for (Groups->Start();!Groups->End();Groups->Next())
	{
		if ((*Groups)()->NbPtr==0) continue;
		for ((*Groups)()->Start(); !(*Groups)()->End(); (*Groups)()->Next())
		{
			GGroupEvaluate* grp=(*(*Groups)())();
			unsigned int s=grp->RelevantSubDoc();
			protos[i]=s;
			i++;
			for (grp->Start(); !grp->End(); grp->Next())
			{
				double dist=(1.0)-(grp->Similarity(s,grp->Current()));
				intra=intra+(dist*dist);
				nbsubprofiles++;
			}
		}
	}
	intra/=nbsubprofiles;

	// calculation of 'min(inter)'

	for(s1=0, i=GetNbGroups();--i;s1++)
	{
		for(s2=s1+1,j=i+1;--j;s2++)
		{
			for (Groups->Start();!Groups->End();Groups->Next())
			{
				if ((*Groups)()->NbPtr==0) continue;
				for ((*Groups)()->Start(); !(*Groups)()->End(); (*Groups)()->Next())
				{
					GGroupEvaluate* grp=(*(*Groups)())() ;
					if(grp->IsIn(protos[s2]))
					{
						double dist=(1.0)-grp->Similarity(protos[s1],protos[s2]);
						dist=dist*dist;
						if(dist<=inter&&dist!=0)
						{
							inter=dist;
						}
 					}
				}
			}
		}
	}
	return(intra/(inter*GetNbGroups()));
	
}



//-----------------------------------------------------------------------------
int GALILEI::GEvaluateGroupingIntraMinInter::GetNbGroups(void)
{
	unsigned int k=0;

	for (Groups->Start(); !Groups->End(); Groups->Next())
		k+=(*Groups)()->NbPtr;
	return(k);
}



//-----------------------------------------------------------------------------
GALILEI::GEvaluateGroupingIntraMinInter::~GEvaluateGroupingIntraMinInter(void)
{
}
