/*

	GALILEI Research Project

	StatsSims.cpp

	Similarity Statistics - Implementation.

	Copyright 2003-2010 by Pascal Francq (pascal@francq.info).
	Copyright 2003-2008 by the Université Libre de Bruxelles (ULB).

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
	GStatSimDocs(GSession* ses,R::RTextFile* f) : GStatSimElements<GDoc,GDoc>(ses,otDoc,otDoc,f)
	{
		Measure=GALILEIApp->GetCurrentPlugIn<GMeasure>("Measures","Documents Similarities");
	}

	virtual R::RCursor<GDoc> GetE1Cursor(void)
	{
		return(Session->GetDocs());
	}

	virtual R::RCursor<GDoc> GetE1Cursor(GSubject* sub)
	{
		return(R::RCursor<GDoc>(sub->GetObjs(static_cast<GDoc*>(0))));
	}

	virtual R::RCursor<GDoc> GetE2Cursor(GSubject* sub)
	{
		return(R::RCursor<GDoc>(sub->GetObjs(static_cast<GDoc*>(0))));
	}
};


//------------------------------------------------------------------------------
// Statistics between profiles
class GStatSimProfiles : public GStatSimElements<GProfile,GProfile>
{
public:

	GStatSimProfiles(GSession* ses,R::RTextFile* f) : GStatSimElements<GProfile,GProfile>(ses,otProfile,otProfile,f)
	{
		Measure=GALILEIApp->GetCurrentPlugIn<GMeasure>("Measures","Profiles Similarities");
	}

	virtual R::RCursor<GProfile> GetE1Cursor(void)
	{
		return(Session->GetProfiles());
	}

	virtual R::RCursor<GProfile> GetE1Cursor(GSubject* sub)
	{
		return(R::RCursor<GProfile>(sub->GetObjs(static_cast<GProfile*>(0))));
	}

	virtual R::RCursor<GProfile> GetE2Cursor(GSubject* sub)
	{
		return(R::RCursor<GProfile>(sub->GetObjs(static_cast<GProfile*>(0))));
	}
};


//------------------------------------------------------------------------------
// Statistics between groups and documents
class GStatSimDocGrp : public GStatSimElements<GDoc,GCommunity>
{
	R::RContainer<GCommunity,false,false> Com;

public:

	GStatSimDocGrp(GSession* ses,R::RTextFile* f) : GStatSimElements<GDoc,GCommunity>(ses,otDoc,otCommunity,f), Com(1)
	{
		Measure=GALILEIApp->GetCurrentPlugIn<GMeasure>("Measures","Documents/Groups Similarities");
	}

	virtual R::RCursor<GDoc> GetE1Cursor(void)
	{
		return(Session->GetDocs());
	}

	virtual R::RCursor<GDoc> GetE1Cursor(GSubject* sub)
	{
		return(sub->GetObjs(static_cast<GDoc*>(0)));
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

	GStatSimDocProf(GSession* ses,R::RTextFile* f) : GStatSimElements<GDoc,GProfile>(ses,otDoc,otProfile,f)
	{
		Measure=GALILEIApp->GetCurrentPlugIn<GMeasure>("Measures","Documents/Profiles Similarities");
	}

	virtual R::RCursor<GDoc> GetE1Cursor(void)
	{
		return(Session->GetDocs());
	}

	virtual R::RCursor<GDoc> GetE1Cursor(GSubject* sub)
	{
		return(R::RCursor<GDoc>(sub->GetObjs(static_cast<GDoc*>(0))));
	}

	virtual R::RCursor<GProfile> GetE2Cursor(GSubject* sub)
	{
		return(R::RCursor<GProfile>(sub->GetObjs(static_cast<GProfile*>(0))));
	}
};


//------------------------------------------------------------------------------
// Statistics between groups and profiles
class GStatSimProfGrp : public GStatSimElements<GProfile,GCommunity>
{
	R::RContainer<GCommunity,false,false> Com;

public:

	GStatSimProfGrp(GSession* ses,R::RTextFile* f) : GStatSimElements<GProfile,GCommunity>(ses,otProfile,otCommunity,f), Com(1)
	{
		Measure=GALILEIApp->GetCurrentPlugIn<GMeasure>("Measures","Profiles/Groups Similarities");
	}

	virtual R::RCursor<GProfile> GetE1Cursor(void)
	{
		return(Session->GetProfiles());
	}

	virtual R::RCursor<GProfile> GetE1Cursor(GSubject* sub)
	{
		return(sub->GetObjs(static_cast<GProfile*>(0)));
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
	: GStatsCalc(session,fac), ResultsFile(0)
{
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
	SaveResults=FindParam<RParamValue>("SaveResults")->GetBool();
	RURI OldName(Results);
	Results=FindParam<RParamValue>("Results")->Get();
	if((OldName!=Results)&&ResultsFile)
	{
		delete ResultsFile;
		ResultsFile=0;
	}
	ExportDocsSims=FindParam<RParamValue>("ExportDocsSims")->GetBool();
	DocsSims=FindParam<RParamValue>("DocsSims")->Get();
	ExportDocsIncs=FindParam<RParamValue>("ExportDocsIncs")->GetBool();
	DocsIncs=FindParam<RParamValue>("DocsIncs")->Get();
	RString Tmp(FindParam<RParamValue>("MeasureType")->Get());
	if((Tmp!="Complete")&&(Tmp!="Nearest Neighbors"))
		ThrowGException("'"+Tmp+"' is invalid : Only 'Complete' or 'Nearest Neighbors' are allowed for the type of measure");
	MeasureType=Tmp;
}


//------------------------------------------------------------------------------
void GStatsSims::DoExportDocsSims(void)
{
	if(!ExportDocsSims)
		return;

	// Get the measure
	GMeasure* Measure(GALILEIApp->GetCurrentPlugIn<GMeasure>("Measures","Documents Similarities"));
	double tmp;
	bool NewLine;
	bool NewComma;

	// Create the file
	RTextFile Export(DocsSims);
	Export.Open(RIO::Create);

	// Save the sims
	R::RCursor<GDoc> Objs1(Session->GetDocs());
	R::RCursor<GDoc> Objs2(Session->GetDocs());
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
			{
				Measure->Measure(0,Objs1()->GetId(),Objs2()->GetId(),&tmp);
				Export<<tmp;
			}
			NewComma=true;
		}
		NewLine=true;
	}
}


//------------------------------------------------------------------------------
void GStatsSims::DoExportDocsIncs(void)
{
	if(!ExportDocsIncs)
		return;

	bool NewLine;
	bool NewComma;

	// Create the file
	RTextFile Export(DocsIncs);
	Export.Open(RIO::Create);

	// Save the sims
	R::RCursor<GDoc> Objs1(Session->GetDocs());
	R::RCursor<GDoc> Objs2(Session->GetDocs());
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
				Export<<Objs1()->GetVector().Inclusion(Session,Objs2()->GetVector(),otDoc);
			NewComma=true;
		}
		NewLine=true;
	}
}


//------------------------------------------------------------------------------
void GStatsSims::Compute(R::RXMLStruct* xml,R::RXMLTag& res)
{
	RXMLTag* tag;
	RXMLTag* tag2;
	RString str;

	// Init Main XML Tag
	tag=new RXMLTag(Factory->GetName());
	xml->AddTag(&res,tag);

	// Create Details File if necessary
	if(SaveResults)
	{
		if(ResultsFile)
			(*ResultsFile)<<endl<<"-----------------------------------------------------"<<endl<<endl;
		else
		{
			try
			{
				ResultsFile=new RTextFile(Results);
				ResultsFile->Open(RIO::Create);
			}
			catch(...)
			{
				ResultsFile=0;
			}
		}
	}

	// Verify if the ideal communities should be created
	if(GroupDoc||GroupProf)
		Session->GetSimulator()->BuildIdealCommunities();

	// Export matrices
	DoExportDocsSims();
	DoExportDocsIncs();

	// Compute Statistics
	if(Docs)
	{
		tag2=new RXMLTag("Documents");
		xml->AddTag(tag,tag2);
		GStatSimDocs Stat(Session,ResultsFile);
		Stat.Run(this,xml,tag2);
	}
	if(ProfDoc)
	{
		tag2=new RXMLTag("Documents-Profiles");
		xml->AddTag(tag,tag2);
		GStatSimDocProf Stat(Session,ResultsFile);
		Stat.Run(this,xml,tag2);
	}
	if(GroupDoc)
	{
		tag2=new RXMLTag("Documents-Groups");
		xml->AddTag(tag,tag2);
		GStatSimDocGrp Stat(Session,ResultsFile);
		Stat.Run(this,xml,tag2);
	}
	if(Profiles)
	{
		tag2=new RXMLTag("Profiles");
		xml->AddTag(tag,tag2);
		GStatSimProfiles Stat(Session,ResultsFile);
		Stat.Run(this,xml,tag2);
	}
	if(SameDocProf)
	{
		tag2=new RXMLTag("Profiles-Common-Documents");
		xml->AddTag(tag,tag2);
		GStatProfDoc Stat(Session,ResultsFile);
		Stat.Run(this,xml,tag2);
	}
	if(GroupProf)
	{
		tag2=new RXMLTag("Profiles-Groups");
		xml->AddTag(tag,tag2);
		GStatSimProfGrp Stat(Session,ResultsFile);
		Stat.Run(this,xml,tag2);
	}
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
	InsertParam(new RParamValue("SaveResults",false));
	InsertParam(new RParamValue("Results",""));
	InsertParam(new RParamValue("ExportDocsSims",false));
	InsertParam(new RParamValue("DocsSims",""));
	InsertParam(new RParamValue("ExportDocsIncs",false));
	InsertParam(new RParamValue("DocsIncs",""));
	InsertParam(new RParamValue("MeasureType","Complete"));
}


//------------------------------------------------------------------------------
GStatsSims::~GStatsSims(void)
{
}


//------------------------------------------------------------------------------
CREATE_STATSCALC_FACTORY("Similarity Statistics","Similarity Statistics",GStatsSims)
