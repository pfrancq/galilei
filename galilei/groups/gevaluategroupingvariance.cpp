/*

	GALILEI Research Project

	GEvaluateGroupingVariance.cpp

	Compare a ideal groupement with a computed one - Implementation

	Copyright 2002 by the Université Libre de Bruxelles.

	Authors:
		David Wartel (dwartel@ulb.ac.be).
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
#include <groups/gevaluategroupingvariance.h>
#include <tests/ggroupevaluate.h>
#include <tests/ggroupsevaluate.h>
#include <profiles/gsubprofile.h>
using namespace GALILEI;
using namespace R;



//-----------------------------------------------------------------------------
//
//  GEvaluateGroupingVariance
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GALILEI::GEvaluateGroupingVariance::GEvaluateGroupingVariance(GSession* s
			,RContainer<GGroupsEvaluate,unsigned int,false,false>* groups)  throw(bad_alloc)
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
	double var=0;
	int nbsubprofiles=0;
	// calculation of variance
	for (Groups->Start(); !Groups->End(); Groups->Next())
	{
		if ((*Groups)()->NbPtr==0) continue;
		for ((*Groups)()->Start(); !(*Groups)()->End(); (*Groups)()->Next())
		{
			GGroupEvaluate* grp=(*(*Groups)())() ;
			unsigned int s1= grp->RelevantSubDoc();
			for (grp->Start(); !grp->End(); grp->Next())
			{
				double dist=(1.0)-grp->Similarity(s1,grp->Current());
				if((dist<=2)&&(dist>-2))
				{
					var+=dist;
					nbsubprofiles++;
				}
			}
		}
	}
	var/=nbsubprofiles;
	return(var);
}



//-----------------------------------------------------------------------------
GALILEI::GEvaluateGroupingVariance::~GEvaluateGroupingVariance(void)
{
}
