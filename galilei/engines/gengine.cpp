/*

	GALILEI Research Project

	GEngine.cpp

	Search Engine - Implementation.

	Copyright 2003-2015 by Pascal Francq (pascal@francq.info).
	Copyright 2003-2004 by Valery Vandaele.
	Copyright 2003-2008 Université Libre de Bruxelles (ULB).

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



//------------------------------------------------------------------------------
// include files for ANSI C/C++
#include <stdio.h>
#include <iostream>
#include <cstdlib>


//------------------------------------------------------------------------------
// include files for GALILEI
#include <gengine.h>
#include <ggalileiapp.h>
#include <gcomputerank.h>
using namespace GALILEI;
using namespace R;
using namespace std;



//------------------------------------------------------------------------------
//
// class GEngine
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GEngine::GEngine(GSession* session,GPlugInFactory* fac)
	: GPlugIn(session,fac), Weight(1.0), Ranking(0)
{
}


//------------------------------------------------------------------------------
void GEngine::ApplyConfig(void)
{
	RankingMethod=FindParam<RParamValue>("RankingMethod")->Get();
	if(!RankingMethod.IsEmpty())
	{
		Ranking=GALILEIApp->GetPlugIn<GComputeRank>("ComputeRank",RankingMethod,0);
	}
}


//------------------------------------------------------------------------------
void GEngine::CreateConfig(void)
{
	InsertParam(new RParamValue("RankingMethod","None","Name of the ranking method."));
}


//------------------------------------------------------------------------------
GEngine::~GEngine(void)
{
}
