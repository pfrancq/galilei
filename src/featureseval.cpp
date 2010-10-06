/*

	GALILEI Research Project

	FeaturesEval.cpp

	Features Evaluation - Implementation.

	Copyright 2003-2010 by Pascal Francq (pascal@francq.info).

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
#include <math.h>



//------------------------------------------------------------------------------
// include files for GALILEI
#include <gdoc.h>
#include <gsession.h>
#include <featureseval.h>



//------------------------------------------------------------------------------
//
//  Features
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
FeaturesEval::FeaturesEval(GSession* session,GPlugInFactory* fac)
	: RObject(fac->GetName()), GMeasure(session,fac), Dirty(false), MutualInfos(20000), CurWeights(0)
{
	InsertObserver(HANDLER(FeaturesEval::Handle),"ObjectChanged");
}


//------------------------------------------------------------------------------
void FeaturesEval::Measure(size_t measure,...)
{
	va_list ap;
	va_start(ap,measure);
	size_t id(va_arg(ap,size_t));
	double* res(va_arg(ap,double*));
	va_end(ap);

	switch(measure)
	{
		case 0:
		{
			switch(CurWeights)
			{
				case 0:
					(*res)=Session->GetConcept(id)->GetIF(otDoc);
					break;
				case 1:
					if(Dirty)
						BuildMutualInformation();
					(*res)=MutualInfos[id];
					break;
				default:
					ThrowGException(RString::Number(CurWeights)+" is not allowed as features weight.");
			}
		}
		break;
		default:
			ThrowGException(RString::Number(measure)+" is not allowed as measure.");
	}
}


//------------------------------------------------------------------------------
void FeaturesEval::Info(size_t info,...)
{
	va_list ap;
	va_start(ap,info);
	if(info==cNoRef)
	{
		size_t* res(va_arg(ap,size_t*));
		(*res)=2;
	}
	else
	{
		RString* res(va_arg(ap,RString*));
		switch(info)
		{
			case 0:
				(*res)="idf";
				break;
			case 1:
				(*res)="MutualInfos";
				break;
			default:
				ThrowGException(RString::Number(CurWeights)+" is not allowed as features weight.");
		}
	}
	va_end(ap);
}


//------------------------------------------------------------------------------
void FeaturesEval::BuildMutualInformation(void)
{
	// Initialize the vector
	MutualInfos.Init(Session->GetMaxObjectId(otConcept),0.0);
	double InvPdj(static_cast<double>(Session->GetNbObjects(otDoc)));
	RVector SumjWij(1);
	SumjWij.Init(Session->GetMaxObjectId(otConcept),0.0);

	// Go through the documents to compute SumjWij and SumWij
	R::RCursor<GDoc> Docs(Session->GetDocs());
	for(Docs.Start();!Docs.End();Docs.Next())
	{
		// Go through each concepts
		RCursor<GWeightInfo> Words(Docs()->GetVector().GetInfos());
		for(Words.Start();!Words.End();Words.Next())
		{
			SumjWij[Words()->GetConcept()->GetId()]+=Words()->GetWeight();
		}
	}

	// Go through the documents to compute I(y)
	for(Docs.Start();!Docs.End();Docs.Next())
	{
		// Go through each concepts
		RCursor<GWeightInfo> Words(Docs()->GetVector().GetInfos());
		for(Words.Start();!Words.End();Words.Next())
		{
			MutualInfos[Words()->GetConcept()->GetId()]+=log10(InvPdj*Words()->GetWeight()/(SumjWij[Words()->GetConcept()->GetId()]));
		}
	}

	Dirty=false;
}


//------------------------------------------------------------------------------
void FeaturesEval::Handle(const RNotification& notification)
{
	// Only document changes
	GEvent& Event(GetData<GEvent&>(notification));
	if(Event.Object->GetObjType()!=otDoc)
		return;

	switch(Event.Event)
	{
		case GEvent::eObjNew:
		case GEvent::eObjModified:
		case GEvent::eObjDelete:
		case GEvent::eObjDestroyed:
			Dirty=true;
		default:
			break;
	}
}


//------------------------------------------------------------------------------
void FeaturesEval::ApplyConfig(void)
{
	CurWeights=FindParam<RParamValue>("CurWeights")->GetUInt();
}


//------------------------------------------------------------------------------
void FeaturesEval::CreateConfig(void)
{
	InsertParam(new RParamValue("CurWeights",0));
	InsertParam(new RParamValue("MaxWeights",2));
}


//------------------------------------------------------------------------------
CREATE_MEASURE_FACTORY("Features Evaluation","Basic Measures","Basic Measures",FeaturesEval)
