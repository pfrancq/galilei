/*

	GALILEI Research Project

	StatData.cpp

	Data Statistics - Implementation.

	Copyright 2010 by Pascal Francq (pascal@francq.info).

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
#include <stdlib.h>


//------------------------------------------------------------------------------
// include files for R Library
#include <rxmlstruct.h>


//------------------------------------------------------------------------------
// include files for GALILEI
#include <glang.h>
#include <gsession.h>
#include <gsubject.h>
#include <gdoc.h>
#include <gmeasure.h>
#include <ggalileiapp.h>
#include <gsimulator.h>
using namespace GALILEI;
using namespace R;
using namespace std;


//------------------------------------------------------------------------------
// include statistics files
#include <statdata.h>


//------------------------------------------------------------------------------
//
//  class ConceptData
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
ConceptData::ConceptData(GConcept* concept)
	: Concept(concept), AvgWeight(0.0), AvgOccurs(0.0), MaxWeight(0.0), MaxOccurs(0.0)
{
}


//------------------------------------------------------------------------------
void ConceptData::Treat(GWeightInfo* info,const GWeightInfos& vector)
{
	double maxabsweight(vector.GetMaxAbsWeight(info->GetType()));
	double weight(info->GetWeight()/maxabsweight*Concept->GetIF(otDoc));
	if(weight>MaxWeight)
		MaxWeight=weight;
	AvgWeight+=weight;
	AvgOccurs+=info->GetWeight();
	if(info->GetWeight()>MaxOccurs)
		MaxOccurs=info->GetWeight();
}


//------------------------------------------------------------------------------
void ConceptData::Add(RWorksheet& stats,GMeasure* measure)
{
	double NbDocs(Concept->GetRef(otDoc));

	// Add a line for the word
	stats.AddLine(Concept->GetId(),Concept->GetName());
	stats.AddValue(0,Concept->GetId(),Concept->GetId());
	stats.AddValue(1,Concept->GetId(),Concept->GetIF(otDoc));
	stats.AddValue(2,Concept->GetId(),AvgWeight/NbDocs);
	stats.AddValue(3,Concept->GetId(),MaxWeight);
	stats.AddValue(4,Concept->GetId(),AvgOccurs/NbDocs);
	stats.AddValue(5,Concept->GetId(),MaxOccurs);
	if(measure)
	{
		double tmp;
		measure->Measure(0,Concept->GetId(),&tmp);
		stats.AddValue(6,Concept->GetId(),tmp);
	}
}



//------------------------------------------------------------------------------
//
//  class StatData
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
StatData::StatData(GSession* session,GPlugInFactory* fac)
	: GTool(session,fac), Data(100000)
{
}


//------------------------------------------------------------------------------
void StatData::ApplyConfig(void)
{
	RURI OldName(Results);
	Results=FindParam<RParamValue>("Results")->Get();
}


//------------------------------------------------------------------------------
void StatData::Run(GSlot*)
{
	// Clear the Data
	Data.Clear();

	// Go through the documents.
	R::RCursor<GDoc> Docs(Session->GetDocs());
	for(Docs.Start();!Docs.End();Docs.Next())
	{
		// Go through each concepts
		const GWeightInfos& Infos(Docs()->GetVector());
		RCursor<GWeightInfo> Words(Infos.GetInfos());
		for(Words.Start();!Words.End();Words.Next())
		{
			ConceptData* Concept(Data.GetInsertPtrAt(Words()->GetConcept(),Words()->GetId()));
			Concept->Treat(Words(),Infos);
		}
	}

	// Get the measure over the features evaluation
	GMeasure* Measure(GALILEIApp->GetCurrentPlugIn<GMeasure>("Measures","Features Evaluation",0));

	// Go through the concepts to build the statistics
	RWorksheet Stats("Global","Concept");
	Stats.AddCol(0,"Id");
	Stats.AddCol(1,"idf");
	Stats.AddCol(2,"Avg Weight");
	Stats.AddCol(3,"Max Weight");
	Stats.AddCol(4,"Avg Occurs");
	Stats.AddCol(5,"Max Occurs");
	if(Measure)
		Stats.AddCol(6,"MutualInfo");
	RCursor<ConceptData> Concepts(Data);
	for(Concepts.Start();!Concepts.End();Concepts.Next())
		Concepts()->Add(Stats,Measure);

	// Save the statistics
	File.Open(Results,RIO::Create,"utf8");
	Stats.Save(File);
	File.Close();
}


//------------------------------------------------------------------------------
void StatData::CreateConfig(void)
{
	InsertParam(new RParamValue("Results",""));
}


//------------------------------------------------------------------------------
CREATE_TOOL_FACTORY("Data Statistics","Multiple","Multiple Data Statistics",StatData)
