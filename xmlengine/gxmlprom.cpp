/*

	GALILEI Research Project

	GIRProm.cpp

	PROMETHEE Kernel for Grouping - Implementation.

	Copyright 2001-2002 by the Universit� Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).

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
// include files for R Project
#include <rpromsol.h>
#include <rpromcriterion.h>
#include <vector>
using namespace R;


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <gxmlprom.h>
#include <gxmlfrag.h>
using namespace GALILEI;
using namespace std;


//-----------------------------------------------------------------------------
//
// class GPlugin
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

//RPromKernel("GALILEIXMLL",3,5) le 3 devra changer pour �tre calcul� automatiquement selon le nombre de frag � trier
//-----------------------------------------------------------------------------
GALILEI::GXmlProm::GXmlProm(GXmlParams* p) throw(bad_alloc)
	: RPromKernel("GALILEIXMLL",3,5), Params(p), CritDocSc(0),CritTfief(0),CritDis(0), CritSpecif(0), Sols(0)
{
	//if(Params->ParamsDocSc.Weight)
		AddCriterion(CritDocSc= new RPromLinearCriterion(RPromCriterion::Maximize,Params->ParamsDocSc, "DocSc"));
		
	//if(Params->ParamsTfief.Weight) pas encore calcul�
		AddCriterion(CritTfief= new RPromLinearCriterion(RPromCriterion::Maximize,Params->ParamsTfief,"Tfief"));

	//if(Params->ParamsType.Weight)
		//CritType=NewCriterion(RPromCriterion::Maximize,"Type",Params->ParamsType);

	//if(Params->ParamsDis.Weight)
		AddCriterion(CritDis= new RPromLinearCriterion(RPromCriterion::Minimize,Params->ParamsDis,"Dis"));

	//if(Params->ParamsOcc.Weight)pas encore calcul�
		//CritOcc=NewCriterion(RPromCriterion::Maximize,"Occurrence",Params->ParamsOcc);

	//if(Params->ParamsSpecif.Weight)
		AddCriterion(CritSpecif= new RPromLinearCriterion(RPromCriterion::Minimize,Params->ParamsSpecif,"Specificity"));


}
//-----------------------------------------------------------------------------
void GALILEI::GXmlProm::Assign(RPromSol* s,XNode* n)
{
	RPromKernel::Assign(s,CritDocSc,n->CritDocSc);
	RPromKernel::Assign(s,CritTfief,n->CritTfief);
	//RPromKernel::Assign(s,CritType,n->CritType);
	RPromKernel::Assign(s,CritDis,n->CritDis);
//	RPromKernel::Assign(s,CritOcc,n->CritOcc);
	RPromKernel::Assign(s,CritSpecif,n->CritSpecif);
	
}
//-----------------------------------------------------------------------------
void GALILEI::GXmlProm::Assign(RPromSol* s, double criteria_values[4])
{
	RPromKernel::Assign(s,CritDocSc,criteria_values[0]);
	RPromKernel::Assign(s,CritTfief,criteria_values[1]);
	//RPromKernel::Assign(s,CritType,criteria_values[2]);
	RPromKernel::Assign(s,CritDis,criteria_values[2]);
//	RPromKernel::Assign(s,CritOcc,n->CritOcc);
	RPromKernel::Assign(s,CritSpecif,criteria_values[3]);
	
}