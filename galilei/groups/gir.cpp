/*

	GALILEI Research Project

	GIR.cpp

	GA for the Information Retrieval Problem - Implementation.

	Copyright 2002 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).

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
// includes files for GALILEI
#include <groups/gir.h>



//-----------------------------------------------------------------------------
//
// GSimMeasure
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GALILEI::GSimMeasure::GSimMeasure(const char* n,SimType t,bool u,double w)
  : Name(n), Type(t), Use(u), Weight(w)
{
}


//-----------------------------------------------------------------------------
int GALILEI::GSimMeasure::Compare(const GALILEI::GSimMeasure* s) const
{
	return(Name.Compare(s->Name));
}


//-----------------------------------------------------------------------------
int GALILEI::GSimMeasure::Compare(const char* s) const
{
	return(Name.Compare(s));
}


//-----------------------------------------------------------------------------
GALILEI::GSimMeasure& GALILEI::GSimMeasure::operator=(const GALILEI::GSimMeasure& src)
{
	Name=src.Name;
	Type=src.Type;
	Use=src.Use;
	Weight=src.Weight;
	return(*this);
}



//-----------------------------------------------------------------------------
//
// GIRParams
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GALILEI::GIRParams::GIRParams(void)
	: GGroupingParams("Grouping Genetic Algorithms"), Measures(20,10)
{
	Measures.InsertPtr(new GSimMeasure("Constant",stNone));
	Measures.InsertPtr(new GSimMeasure("AvgSim",stAvgSim));
	Measures.InsertPtr(new GSimMeasure("J",stJ));
	Measures.InsertPtr(new GSimMeasure("AvgRatio",stAvgRatio));
	Measures.InsertPtr(new GSimMeasure("MinRatio",stMinRatio));
	Measures.InsertPtr(new GSimMeasure("Ratio",stRatio));
	Measures.InsertPtr(new GSimMeasure("WOverB",stWOverB));
	Measures.InsertPtr(new GSimMeasure("SimWB",stSimWB));
}


//-----------------------------------------------------------------------------
const char* GALILEI::GIRParams::GetSettings(void)
{
	static char tmp[200];
	char c,c1;

	if(Step) c='1'; else c='0';
	if(GlobalSim) c1='1'; else c1='0';
	sprintf(tmp,"%u %c %u %u %c %u %f %f %f %u %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f",
	        SimMeasures,c1,PopSize,MaxGen,c,StepGen,MinSimLevel,MinCommonOK,MinCommonDiff,MaxKMeans,
	        ParamsSim.P,ParamsSim.Q,ParamsSim.Weight,
	        ParamsInfo.P,ParamsInfo.Q,ParamsInfo.Weight,
	        ParamsSameFeedbacks.P,ParamsSameFeedbacks.Q,ParamsSameFeedbacks.Weight,
	        ParamsDiffFeedbacks.P,ParamsDiffFeedbacks.Q,ParamsDiffFeedbacks.Weight,
	        ParamsSocial.P,ParamsSocial.Q,ParamsSocial.Weight);
	return(tmp);
}


//-----------------------------------------------------------------------------
void GALILEI::GIRParams::SetSettings(const char* s)
{
	char c,c1;
	unsigned int t;

	if(!(*s)) return;
	sscanf(s,"%u %c %u %u %c %u %lf %lf %lf %u %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf",
	       &t,&c1,&PopSize,&MaxGen,&c,&StepGen,&MinSimLevel,&MinCommonOK,&MinCommonDiff,&MaxKMeans,
	       &ParamsSim.P,&ParamsSim.Q,&ParamsSim.Weight,
	       &ParamsInfo.P,&ParamsInfo.Q,&ParamsInfo.Weight,
	       &ParamsSameFeedbacks.P,&ParamsSameFeedbacks.Q,&ParamsSameFeedbacks.Weight,
	       &ParamsDiffFeedbacks.P,&ParamsDiffFeedbacks.Q,&ParamsDiffFeedbacks.Weight,
	       &ParamsSocial.P,&ParamsSocial.Q,&ParamsSocial.Weight);
	SimMeasures=static_cast<SimCritType>(t);
	if(c=='1') Step=true; else Step=false;
	if(c1=='1') GlobalSim=true; else GlobalSim=false;
}


//-----------------------------------------------------------------------------
GALILEI::GIRParams& GALILEI::GIRParams::operator=(const GALILEI::GIRParams& src)
{
	GSimMeasure** tab;
	GSimMeasure** tab2;
	unsigned int i;

	GGroupingParams::operator=(src);
	PopSize=src.PopSize;
	MaxGen=src.MaxGen;
	Step=src.MaxGen;
	StepGen=src.StepGen;
	MinSimLevel=src.MinSimLevel;
	MinCommonOK=src.MinCommonOK;
	MinCommonDiff=src.MinCommonDiff;
	MaxKMeans=src.MaxKMeans;
	SimMeasures=src.SimMeasures;
	for(i=Measures.NbPtr+1,tab2=src.Measures.Tab,tab=Measures.Tab;--i;tab++,tab2++)
	{
		(**tab)=(**tab2);
	}
	ParamsSim=src.ParamsSim;
	ParamsInfo=src.ParamsInfo;
	ParamsEntropy=src.ParamsEntropy;
	ParamsSameFeedbacks=src.ParamsSameFeedbacks;
	ParamsDiffFeedbacks=src.ParamsDiffFeedbacks;
	ParamsSocial=src.ParamsSocial;
	GlobalSim=src.GlobalSim;
	return(*this);
}
