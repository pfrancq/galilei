/*

	GALILEI Research Project

	GProfileCalcFeedback.h

	Feedback-based Profile Computing Method - Implementation.

	Copyright 2001-2011 by Pascal Francq (pascal@francq.info).
	Copyright 2001-2008 Université Libre de Bruxelles (ULB).

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
#include <math.h>
using namespace std;


//-----------------------------------------------------------------------------
// include files for R Project
#include <rdir.h>
using namespace R;


//-----------------------------------------------------------------------------
//include files for GALILEI
#include <feedback.h>
#include <gdoc.h>
#include <gprofile.h>
#include <gconcept.h>
#include <gconcepttype.h>
#include <gsession.h>
#include <gfdbk.h>
using namespace GALILEI;



//-----------------------------------------------------------------------------
//
//  GProfileCalcFeedback
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GProfileCalcFeedback::GProfileCalcFeedback(GSession* session,GPlugInFactory* fac)
	: GProfileCalc(session,fac), GDescriptionFilter(),
	  LMax(60), LMin(0),
	  IncrementalMode(false), Alpha(1.0)
{
	Factors[ftRelevant]=1.0;
	Factors[ftFuzzyRelevant]=0.25;
	Factors[ftIrrelevant]=-0.75;
	Factors[ftUnknown]=0.0;
	Sets[ftRelevant]=new GDescriptionSet(session);
	Sets[ftFuzzyRelevant]=new GDescriptionSet(session);
	Sets[ftIrrelevant]=new GDescriptionSet(session);
}


//-----------------------------------------------------------------------------
void GProfileCalcFeedback::ApplyConfig(void)
{
	LMax=FindParam<RParamValue>("LMax")->GetUInt();
	LMin=FindParam<RParamValue>("LMin")->GetUInt();
	Factors[ftRelevant]=FindParam<RParamValue>("Relevant Factor")->GetDouble();
	Factors[ftFuzzyRelevant]=FindParam<RParamValue>("Fuzzy Factor")->GetDouble();
	Factors[ftUnknown]=FindParam<RParamValue>("Irrelevant Factor")->GetDouble();
	IncrementalMode=FindParam<RParamValue>("Incremental")->GetBool();
	Alpha=FindParam<RParamValue>("Alpha")->GetDouble();
}


//-----------------------------------------------------------------------------
void GProfileCalcFeedback::Compute(const GProfile* profile)
{
	// Clear all descriptions before computing
	Description.Clear();
	Internal.Clear();
	Desc[ftRelevant].Clear();
	Desc[ftFuzzyRelevant].Clear();
	Desc[ftIrrelevant].Clear();
	Sets[ftRelevant]->Clear();
	Sets[ftFuzzyRelevant]->Clear();
	Sets[ftIrrelevant]->Clear();

	// If incremental mode -> copy information of the profile in Internal.
	if(IncrementalMode&&profile->IsDefined())
		Internal+=Alpha*(*profile);

	// Go through all documents, add the frequencies of the words of "OK"
	// documents and subtract the frequencies of the words of "KO" documents.
	RCursor<GFdbk> Fdbk(profile->GetFdbks());
	for(Fdbk.Start();!Fdbk.End();Fdbk.Next())
	{
		// Find if the document must be treated
		if(Factors[Fdbk()->GetFdbk()]==0.0)
			continue;
		GDoc* Doc(Session->GetObj(pDoc,Fdbk()->GetDocId()));
		if((!Doc)||(!Doc->IsDefined()))
			continue;

		// Normalize the description and add it to corresponding set
		Tmp=(*Doc);
		Tmp.Normalize();
		Desc[Fdbk()->GetFdbk()]+=Tmp;
		Sets[Fdbk()->GetFdbk()]->InsertDescription(Doc);
	}

	// Multiply by the if factors
	Desc[ftRelevant].MultiplyIF(*Sets[ftRelevant]);
	Desc[ftFuzzyRelevant].MultiplyIF(*Sets[ftFuzzyRelevant]);
	Desc[ftIrrelevant].MultiplyIF(*Sets[ftIrrelevant]);

	// Add the description to Internal
	if(Sets[ftRelevant]->GetNb())
		Internal+=((Factors[ftRelevant]/Sets[ftRelevant]->GetNb())*Desc[ftRelevant]);
	if(Sets[ftFuzzyRelevant]->GetNb())
		Internal+=((Factors[ftFuzzyRelevant]/Sets[ftFuzzyRelevant]->GetNb())*Desc[ftFuzzyRelevant]);
	if(Sets[ftIrrelevant]->GetNb())
		Internal+=((Factors[ftIrrelevant]/Sets[ftIrrelevant]->GetNb())*Desc[ftIrrelevant]);

	// Compute the description
	CopyFilter(Internal,Description,LMax,LMin);
}


//------------------------------------------------------------------------------
void GProfileCalcFeedback::CreateConfig(void)
{
	InsertParam(new RParamValue("LMax",60));
	InsertParam(new RParamValue("LMin",0));
	InsertParam(new RParamValue("Relevant Factor",1.0));
	InsertParam(new RParamValue("Fuzzy Factor",0.25));
	InsertParam(new RParamValue("Irrelevant Factor",-0.75));
	InsertParam(new RParamValue("Incremental",false));
	InsertParam(new RParamValue("Alpha",1.0));
}


//-----------------------------------------------------------------------------
GProfileCalcFeedback::~GProfileCalcFeedback(void)
{
	delete Sets[ftRelevant];
	delete Sets[ftFuzzyRelevant];
	delete Sets[ftIrrelevant];
}


//------------------------------------------------------------------------------
CREATE_PROFILECALC_FACTORY("Feedback Computing Method","Feedback Computing Method",GProfileCalcFeedback)
