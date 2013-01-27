/*

	GALILEI Research Project

	MutualInfo.cpp

	Mutual Information - Implementation.

	Copyright 2003-2013 by Pascal Francq (pascal@francq.info).

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
#include <mutualinfo.h>



//------------------------------------------------------------------------------
//
//  MutualInfo
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
MutualInfo::MutualInfo(GSession* session,GPlugInFactory* fac)
	: RObject(fac->GetName()), GMeasure(session,fac), Dirty(false), MutualInfos(20000), CurWeights(0)
{
	InsertObserver(HANDLER(MutualInfo::Handle),"ObjectChanged");
}


//------------------------------------------------------------------------------
void MutualInfo::Measure(size_t measure,...)
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
void MutualInfo::Info(size_t info,...)
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
void MutualInfo::BuildMutualInformation(void)
{
	// Initialize the vector
	MutualInfos.Init(Session->GetMaxObjId(otConcept),0.0);
	double InvPdj(static_cast<double>(Session->GetNbObjs(pDoc)));
//	double SumWij(0.0);
	RVector SumjWij(1);
	SumjWij.Init(Session->GetMaxObjId(otConcept),0.0);

	// Go through the documents to compute SumjWij and SumWij
	R::RCursor<GDoc> Docs(Session->GetObjs(pDoc));
	for(Docs.Start();!Docs.End();Docs.Next())
	{
		// Go through each concepts
		RCursor<GVector> Vector(Docs()->GetVectors());
		for(Vector.Start();!Vector.End();Vector.Next())
		{
			RCursor<GConceptRef> Words(Vector()->GetRefs());
			for(Words.Start();!Words.End();Words.Next())
			{
	//			SumWij+=Words()->GetWeight();
				SumjWij[Words()->GetConcept()->GetId()]+=Words()->GetWeight();
			}
		}
	}

	// Go through the documents to compute I(y)
	for(Docs.Start();!Docs.End();Docs.Next())
	{
		// Go through each concepts
		RCursor<GVector> Vector(Docs()->GetVectors());
		for(Vector.Start();!Vector.End();Vector.Next())
		{
			RCursor<GConceptRef> Words(Vector()->GetRefs());
			for(Words.Start();!Words.End();Words.Next())
			{
				MutualInfos[Words()->GetConcept()->GetId()]+=log10(InvPdj*Words()->GetWeight()/(SumjWij[Words()->GetConcept()->GetId()]));
			}
		}
	}

	Dirty=false;
}


//------------------------------------------------------------------------------
void MutualInfo::Handle(const RNotification& notification)
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
void MutualInfo::ApplyConfig(void)
{
/*	Docs=FindParam<RParamValue>("Docs")->GetBool();
	ProfDoc=FindParam<RParamValue>("ProfDoc")->GetBool();
	GroupDoc=FindParam<RParamValue>("GroupDoc")->GetBool();
	Profiles=FindParam<RParamValue>("Profiles")->GetBool();
	SameDocProf=FindParam<RParamValue>("SameDocProf")->GetBool();
	GroupProf=FindParam<RParamValue>("GroupProf")->GetBool();
	RURI OldName(Results);
	Results=FindParam<RParamValue>("Results")->Get();
	ExportDocsSims=FindParam<RParamValue>("ExportDocsSims")->GetBool();
	DocsSims=FindParam<RParamValue>("DocsSims")->Get();
	ExportDocsIncs=FindParam<RParamValue>("ExportDocsIncs")->GetBool();
	DocsIncs=FindParam<RParamValue>("DocsIncs")->Get();
	RString Tmp(FindParam<RParamValue>("MeasureType")->Get());
	if((Tmp!="Complete")&&(Tmp!="Nearest Neighbors"))
		ThrowGException("'"+Tmp+"' is invalid : Only 'Complete' or 'Nearest Neighbors' are allowed for the type of measure");
	MeasureType=Tmp;*/
	CurWeights=FindParam<RParamValue>("CurWeights")->GetUInt();
}


//------------------------------------------------------------------------------
void MutualInfo::CreateConfig(void)
{
/*	InsertParam(new RParamValue("Docs",false));
	InsertParam(new RParamValue("ProfDoc",false));
	InsertParam(new RParamValue("GroupDoc",false));
	InsertParam(new RParamValue("Profiles",false));
	InsertParam(new RParamValue("SameDocProf",false));
	InsertParam(new RParamValue("GroupProf",false));
	InsertParam(new RParamValue("Results",""));
	InsertParam(new RParamValue("ExportDocsSims",false));
	InsertParam(new RParamValue("DocsSims",""));
	InsertParam(new RParamValue("ExportDocsIncs",false));
	InsertParam(new RParamValue("DocsIncs",""));
	InsertParam(new RParamValue("MeasureType","Complete"));*/
	InsertParam(new RParamValue("CurWeights",0));
	InsertParam(new RParamValue("MaxWeights",2));
}


//------------------------------------------------------------------------------
CREATE_MEASURE_FACTORY("Features Evaluation","Mutual Information","Mutual Information",MutualInfo)
