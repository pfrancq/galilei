/*

	GALILEI Research Project

	StatsSims.cpp

	Similarity Statistics - Implementation.

	Copyright 2003-2014 by Pascal Francq (pascal@francq.info).
	Copyright 2003-2008 by the Universit√© Libre de Bruxelles (ULB).

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
#include <math.h>


//------------------------------------------------------------------------------
// include files for R Library
#include <rxmlstruct.h>


//------------------------------------------------------------------------------
// include files for GALILEI
#include <glang.h>
#include <gsession.h>
#include <gsubject.h>
#include <gdoc.h>
#include <gprofile.h>
#include <gmeasure.h>
#include <ggalileiapp.h>
#include <gsimulator.h>
using namespace GALILEI;
using namespace R;
using namespace std;


//------------------------------------------------------------------------------
// include statistics files
#include <gstatsimelements.h>
#include <statsims.h>
#include <gstatprofdoc.h>



//------------------------------------------------------------------------------
//
// Instantiation of the templates
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Statistics between documents
class GStatSimDocs : public GStatSimElements<GDoc,GDoc>
{
public:
	GStatSimDocs(GSession* ses,GMeasure* weighting,RWorksheet& stats,size_t idx) : GStatSimElements<GDoc,GDoc>(ses,weighting,otDoc,otDoc,stats,idx)
	{
		Measure=GALILEIApp->GetCurrentPlugIn<GMeasure>("Measures","Document Similarities");
	}

	virtual R::RCursor<GDoc> GetE1Cursor(void)
	{
		return(Session->GetObjs(pDoc));
	}

	virtual R::RCursor<GDoc> GetE1Cursor(GSubject* sub)
	{
		return(R::RCursor<GDoc>(sub->GetUsedObjs(pDoc)));
	}

	virtual R::RCursor<GDoc> GetE2Cursor(GSubject* sub)
	{
		return(R::RCursor<GDoc>(sub->GetUsedObjs(pDoc)));
	}
};


//------------------------------------------------------------------------------
// Statistics between profiles
class GStatSimProfiles : public GStatSimElements<GProfile,GProfile>
{
public:

	GStatSimProfiles(GSession* ses,GMeasure* weighting,RWorksheet& stats,size_t idx) : GStatSimElements<GProfile,GProfile>(ses,weighting,otProfile,otProfile,stats,idx)
	{
		Measure=GALILEIApp->GetCurrentPlugIn<GMeasure>("Measures","Profile Similarities");
	}

	virtual R::RCursor<GProfile> GetE1Cursor(void)
	{
		return(Session->GetObjs(pProfile));
	}

	virtual R::RCursor<GProfile> GetE1Cursor(GSubject* sub)
	{
		return(R::RCursor<GProfile>(sub->GetObjs(pProfile)));
	}

	virtual R::RCursor<GProfile> GetE2Cursor(GSubject* sub)
	{
		return(R::RCursor<GProfile>(sub->GetObjs(pProfile)));
	}
};


//------------------------------------------------------------------------------
// Statistics between groups and documents
class GStatSimDocGrp : public GStatSimElements<GDoc,GCommunity>
{
	R::RContainer<GCommunity,false,false> Com;

public:

	GStatSimDocGrp(GSession* ses,GMeasure* weighting,RWorksheet& stats,size_t idx) : GStatSimElements<GDoc,GCommunity>(ses,weighting,otDoc,otCommunity,stats,idx), Com(1)
	{
		Measure=GALILEIApp->GetCurrentPlugIn<GMeasure>("Measures","Document/Community Similarities");
	}

	virtual R::RCursor<GDoc> GetE1Cursor(void)
	{
		return(Session->GetObjs(pDoc));
	}

	virtual R::RCursor<GDoc> GetE1Cursor(GSubject* sub)
	{
		return(sub->GetUsedObjs(pDoc));
	}

	virtual R::RCursor<GCommunity> GetE2Cursor(GSubject* sub)
	{
		Com.InsertPtrAt(sub->GetIdealCommunity(),0,true);
		return(R::RCursor<GCommunity>(Com));
	}
};


//------------------------------------------------------------------------------
// Statistics between documents/profiles
class GStatSimDocProf : public GStatSimElements<GDoc,GProfile>
{
public:

	GStatSimDocProf(GSession* ses,GMeasure* weighting,RWorksheet& stats,size_t idx) : GStatSimElements<GDoc,GProfile>(ses,weighting,otDoc,otProfile,stats,idx)
	{
		Measure=GALILEIApp->GetCurrentPlugIn<GMeasure>("Measures","Document/Profile Similarities");
	}

	virtual R::RCursor<GDoc> GetE1Cursor(void)
	{
		return(Session->GetObjs(pDoc));
	}

	virtual R::RCursor<GDoc> GetE1Cursor(GSubject* sub)
	{
		return(R::RCursor<GDoc>(sub->GetUsedObjs(pDoc)));
	}

	virtual R::RCursor<GProfile> GetE2Cursor(GSubject* sub)
	{
		return(R::RCursor<GProfile>(sub->GetObjs(pProfile)));
	}
};


//------------------------------------------------------------------------------
// Statistics between groups and profiles
class GStatSimProfGrp : public GStatSimElements<GProfile,GCommunity>
{
	R::RContainer<GCommunity,false,false> Com;

public:

	GStatSimProfGrp(GSession* ses,GMeasure* weighting,RWorksheet& stats,size_t idx) : GStatSimElements<GProfile,GCommunity>(ses,weighting,otProfile,otCommunity,stats,idx), Com(1)
	{
		Measure=GALILEIApp->GetCurrentPlugIn<GMeasure>("Measures","Profile/Community Similarities");
	}

	virtual R::RCursor<GProfile> GetE1Cursor(void)
	{
		return(Session->GetObjs(pProfile));
	}

	virtual R::RCursor<GProfile> GetE1Cursor(GSubject* sub)
	{
		return(sub->GetObjs(pProfile));
	}

	virtual R::RCursor<GCommunity> GetE2Cursor(GSubject* sub)
	{
		Com.InsertPtrAt(sub->GetIdealCommunity(),0,true);
		return(R::RCursor<GCommunity>(Com));
	}
};



//------------------------------------------------------------------------------
//
//  class GStatsSims
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GStatsSims::GStatsSims(GSession* session,GPlugInFactory* fac)
	: RObject(fac->GetMng()->GetName()+"|"+fac->GetList()+"|"+fac->GetName()),
	  GTool(session,fac)
{
}


//------------------------------------------------------------------------------
void GStatsSims::Init(void)
{
	GTool::Init();
	Weighting=GALILEIApp->GetCurrentPlugIn<GMeasure>("Measures","Features Evaluation",0);
	InsertObserver(HANDLER(GStatsSims::HandleCurrentPlugIn),hCurrentPlugIn,GALILEIApp->GetManager("Measures")->GetPlugInList("Features Evaluation"));
}


//------------------------------------------------------------------------------
void GStatsSims::HandleCurrentPlugIn(const R::RNotification& notification)
{
	Weighting=dynamic_cast<GMeasure*>(GetData<GPlugIn*>(notification));
}


//------------------------------------------------------------------------------
void GStatsSims::ApplyConfig(void)
{
	Docs=FindParam<RParamValue>("Docs")->GetBool();
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
		mThrowGException("'"+Tmp+"' is invalid : Only 'Complete' or 'Nearest Neighbors' are allowed for the type of measure");
	MeasureType=Tmp;
}


//------------------------------------------------------------------------------
void GStatsSims::DoExportDocsSims(void)
{
	if(!ExportDocsSims)
		return;

	// Get the measure
	GMeasure* Measure(GALILEIApp->GetCurrentPlugIn<GMeasure>("Measures","Document Similarities"));
	double tmp;

	// Create the file
	RTextFile Export(DocsSims);
	Export.Open(RIO::Create);

	// Write the identifiers of the documents
	Export<<"Documents";
	R::RCursor<GDoc> Objs1(Session->GetObjs(pDoc));
	for(Objs1.Start();!Objs1.End();Objs1.Next())
		Export<<","<<Objs1()->GetId();

	// Save the sims
	R::RCursor<GDoc> Objs2(Session->GetObjs(pDoc));
	for(Objs1.Start();!Objs1.End();Objs1.Next())
	{
		// New Line and start with the identifier of the document
		Export<<endl<<Objs1()->GetId();
		for(Objs2.Start();!Objs2.End();Objs2.Next())
		{
			Export<<',';
			if(Objs1()==Objs2())
				Export<<"1.000000E+00";
			else
			{
				Measure->Measure(0,Objs1()->GetId(),Objs2()->GetId(),&tmp);
				Export<<tmp;
			}
		}
	}
}


//------------------------------------------------------------------------------
template<class cObj>
	double GStatsSims::ComputeInclusion(cObj* obj1,cObj* obj2)
{
	// if one description is empty -> the inclusion is null
	if((!obj1->IsDefined())||(!obj2->IsDefined()))
		return(0.0);

	// Parse the vector to found those associated wit the same concept
	double Den(0.0), Num(0.0);
	RConstCursor<GVector> Vec1(obj1->GetVectors());
	RConstCursor<GVector> Vec2(obj2->GetVectors());
	for(Vec1.Start(),Vec2.Start();!Vec1.End();Vec1.Next())
	{
		// Parse all the elements of Vec2 with an identifier lower than the current element of Vec1
		while((!Vec2.End())&&(Vec2()->GetMetaConcept()->GetId()<Vec1()->GetMetaConcept()->GetId()))
			Vec2.Next();

		// Verify if both elements are identical
		if((!Vec2.End())&&(Vec2()->GetMetaConcept()->GetId()==Vec1()->GetMetaConcept()->GetId()))
		{
			double LocalDen(0.0),LocalNum(0.0),Max(-numeric_limits<double>().max());
			// Parse the concepts
			RConstCursor<GConceptRef> Concept1(Vec1()->GetRefs());
			RConstCursor<GConceptRef> Concept2(Vec2()->GetRefs());
			for(Concept1.Start(),Concept2.Start();!Concept1.End();Concept1.Next())
			{
				// Maximum value and add denominator
				if(fabs(Concept1()->GetWeight())>Max)
					Max=fabs(Concept1()->GetWeight());
				double iffactor;
				Weighting->Measure(0,Concept1()->GetConcept(),obj1->GetObjType(),&iffactor);
				double tmp(Concept1()->GetWeight()*iffactor);
				LocalDen+=tmp;

				// Parse all the elements of Concept2 with an identifier lower than the current element of Concept1
				while((!Concept2.End())&&(Concept2()->GetId()<Concept1()->GetId()))
					Concept2.Next();

				// Verify if both concepts are identical
				if((!Concept2.End())&&(Concept2()->GetId()==Concept1()->GetId()))
					LocalNum+=tmp;
			}

			if(Max==0.0)
				continue;
			Den+=LocalDen/Max;
			Num+=LocalNum/Max;
		}
	}

	if(Den==0.0)
		return(0.0);
	return(Num/Den);
}


//------------------------------------------------------------------------------
void GStatsSims::DoExportDocsIncs(void)
{
	if(!ExportDocsIncs)
		return;

	bool NewLine;
	bool NewComma;

	// Create the file
	RTextFile Export(DocsIncs,"utf-8");
	Export.Open(RIO::Create);

	// Save the sims
	RCursor<GDoc> Objs1(Session->GetObjs(pDoc));
	RCursor<GDoc> Objs2(Session->GetObjs(pDoc));
	for(Objs1.Start(),NewLine=false;!Objs1.End();Objs1.Next())
	{
		if(NewLine)
			Export<<endl;
		for(Objs2.Start(),NewComma=false;!Objs2.End();Objs2.Next())
		{
			if(NewComma)
				Export<<',';
			if(Objs1()==Objs2())
				Export<<"1.000000E+00";
			else
				Export<<ComputeInclusion(Objs1(),Objs2());
			NewComma=true;
		}
		NewLine=true;
	}
}


//------------------------------------------------------------------------------
void GStatsSims::AddColumns(RWorksheet& stats,size_t& idx,const RString& name)
{
	RString Type(" ("+name+")");

	if(MeasureType=="Complete")
	{
		stats.AddCol(idx++,"Min ‚ąą"+Type);       // 0
		stats.AddCol(idx++,"Avg ‚ąą"+Type);       // 1
		stats.AddCol(idx++,"Max ‚ąČ"+Type);       // 2
		stats.AddCol(idx++,"Avg ‚ąČ"+Type);       // 3
		stats.AddCol(idx++,"Rie"+Type);         // 4
		stats.AddCol(idx++,"In (%)"+Type);      // 5
		stats.AddCol(idx++,"Ov (%)"+Type);      // 6
		stats.AddCol(idx++,"J"+Type);           // 7
	}
	else if(MeasureType=="Nearest Neighbors")
	{
		stats.AddCol(idx++,"In (%)"+Type);        // 0
		stats.AddCol(idx++,"Ov (%)"+Type);        // 1
		stats.AddCol(idx++,"Nb NN"+Type);         // 2
	}
}


//------------------------------------------------------------------------------
void GStatsSims::Run(GSlot*)
{
	if(!Weighting)
		mThrowGException("No plug-in selected for \"Features Evaluation\"");
	
	// Verify if the ideal communities should be created
	if(GroupDoc||GroupProf)
		Session->GetSimulator()->BuildIdealCommunities();

	// Export matrices
	DoExportDocsSims();
	DoExportDocsIncs();

	// Create the statistics
	RWorksheet Stats("Global","Name");
	size_t Idx(0);

	// Compute Statistics
	if(Docs)
	{
		size_t First(Idx);
		AddColumns(Stats,Idx,"Documents");
		GStatSimDocs Stat(Session,Weighting,Stats,First);
		Stat.Run(GetMeasureType());
	}
	if(ProfDoc)
	{
		size_t First(Idx);
		AddColumns(Stats,Idx,"Documents/Profiles");
		GStatSimDocProf Stat(Session,Weighting,Stats,First);
		Stat.Run(GetMeasureType());
	}
	if(GroupDoc)
	{
		size_t First(Idx);
		AddColumns(Stats,Idx,"Documents/Communities");
		GStatSimDocGrp Stat(Session,Weighting,Stats,First);
		Stat.Run(GetMeasureType());
	}
	if(Profiles)
	{
		size_t First(Idx);
		AddColumns(Stats,Idx,"Profiles");
		GStatSimProfiles Stat(Session,Weighting,Stats,First);
		Stat.Run(GetMeasureType());
	}
	if(SameDocProf)
	{
		//size_t First(Idx);
		AddColumns(Stats,Idx,"Profiles/Common Documents");
		GStatProfDoc Stat(Session,File);
		Stat.Run();
	}
	if(GroupProf)
	{
		size_t First(Idx);
		AddColumns(Stats,Idx,"Profiles/Communities");
		GStatSimProfGrp Stat(Session,Weighting,Stats,First);
		Stat.Run(GetMeasureType());
	}

	// Save the statistics
	File.Open(Results,RIO::Create,"utf-8");
	Stats.Save(File);
	File.Close();
}


//------------------------------------------------------------------------------
void GStatsSims::CreateConfig(void)
{
	InsertParam(new RParamValue("Docs",false));
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
	InsertParam(new RParamValue("MeasureType","Complete"));
}


//------------------------------------------------------------------------------
CREATE_TOOL_FACTORY("Similarity Statistics","Multiple","Multiple Similarity Statistics",GStatsSims)
