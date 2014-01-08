/*

	GALILEI Research Project

	Gravitation_Topic.cpp

	Topic Description Computing Method - Implementation.

	Copyright 2008-2014 by Pascal Francq (pascal@francq.info).

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
// include files for ANSI C/C++
#include <stdlib.h>


//-----------------------------------------------------------------------------
//include files for GALILEI
#include <gtopic.h>
#include <gsession.h>
#include <gdoc.h>


//-----------------------------------------------------------------------------
//include files for current project
#include "gravitation_topic.h"
using namespace GALILEI;
using namespace R;
using namespace std;



//-----------------------------------------------------------------------------
//
//  class GTopicCalcGravitation
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GTopicCalcGravitation::GTopicCalcGravitation(GSession* session,GPlugInFactory* fac)
	: GTopicCalc(session,fac), GDescriptionFilter(),
	  LMax(60), LMin(0), Method(0),
	  Docs(session)
{
}


//-----------------------------------------------------------------------------
void GTopicCalcGravitation::ApplyConfig(void)
{
	LMax=FindParam<RParamValue>("LMax")->GetUInt();
	LMin=FindParam<RParamValue>("LMin")->GetUInt();
	RString MethodParam(FindParam<RParamValue>("Method")->Get());
	if(MethodParam=="Centroid Method")
		Method=1;
	else if(MethodParam=="Prototype Method")
		Method=2;
	else
		Method=0;
}


//-----------------------------------------------------------------------------
void GTopicCalcGravitation::ComputeCentroid(const GTopic* grp)
{
	// Clear the descriptions
	Description.Clear();
	Docs.Clear();
	Internal.Clear();

	// If no documents -> No relevant one
	if(!grp->GetNbObjs())
		return;

	// Go through the documents and sum the description
	RCursor<GDoc> Doc(grp->GetObjs());
	for(Doc.Start();!Doc.End();Doc.Next())
	{
		Tmp=(*Doc())();
		Tmp.Normalize();
		Internal+=Tmp;
		Docs.InsertDescription(&(*Doc())());
	}

	// Multiply by the if factors and divided by the number of documents
	Internal.MultiplyIF(Docs);
	Internal/=Docs.GetNb();

	// Compute the description
	CopyFilter(Internal,Description,LMax,LMin);
}


//-----------------------------------------------------------------------------
void GTopicCalcGravitation::ComputePrototype(const GTopic* grp)
{
	GDoc* Prototype(grp->RelevantObj());
	Description=((*Prototype)());
}


//-----------------------------------------------------------------------------
void GTopicCalcGravitation::Compute(const GTopic* grp)
{
	switch(Method)
	{
		case 1: ComputeCentroid(grp); break;
		case 2: ComputePrototype(grp); break;
		default: mThrowGException("No valid topic description computing method");
	}
}


//------------------------------------------------------------------------------
void GTopicCalcGravitation::CreateConfig(void)
{
	InsertParam(new RParamValue("LMax",60));
	InsertParam(new RParamValue("LMin",0));
	InsertParam(new RParamValue("Method","Centroid Method"));
}


//-----------------------------------------------------------------------------
GTopicCalcGravitation::~GTopicCalcGravitation(void)
{
}


//------------------------------------------------------------------------------
CREATE_TOPICCALC_FACTORY("Gravitation Method for Topics","Gravitation Method for Topics",GTopicCalcGravitation)
