/*

	GALILEI Research Project

	GIRProm.cpp

	PROMETHEE Kernel for Grouping - Implementation.

	Copyright 2001-2002 by the Universit�Libre de Bruxelles.

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
using namespace R;


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <girprom.h>
#include <gir.h>
#include <gchromoir.h>
using namespace GALILEI;
using namespace std;



//-----------------------------------------------------------------------------
//
// class GPlugin
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
GALILEI::GIRProm::GIRProm(GIRParams* p)
	: RPromKernel("GALILEI",20,10), Params(p),CritSimJ(0), CritAgreement(0),
	  CritDisagreement(0)
{
	// Init Criterion and Solutions of the PROMETHEE part
	CritSimJ=NewCriterion(Maximize,"J (Sim)",Params->ParamsSim.P,Params->ParamsSim.Q,Params->ParamsSim.Weight);
	CritAgreement=NewCriterion(Maximize,"Agreement",Params->ParamsAgreement.P,Params->ParamsAgreement.Q,Params->ParamsAgreement.Weight);
	CritDisagreement=NewCriterion(Minimize,"Disagreement",Params->ParamsDisagreement.P,Params->ParamsDisagreement.Q,Params->ParamsDisagreement.Weight);
}


//-----------------------------------------------------------------------------
void GALILEI::GIRProm::Assign(RPromSol* s,GChromoIR* c)
{
	RPromKernel::Assign(s,CritSimJ,c->CritSimJ);
	RPromKernel::Assign(s,CritAgreement,c->CritAgreement);
	RPromKernel::Assign(s,CritDisagreement,c->CritDisagreement);
}
