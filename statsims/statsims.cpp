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
#include <gsubjects.h>
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
GStatsSims::GStatsSims(GPlugInFactory* fac)
	: GStatsCalc(fac), ResultsFile(0)
{
}


//------------------------------------------------------------------------------
void GStatsSims::ApplyConfig(void)
{
	Docs=Factory->GetBool("Docs");
	ProfDoc=Factory->GetBool("ProfDoc");
	GroupDoc=Factory->GetBool("GroupDoc");
	Profiles=Factory->GetBool("Profiles");
	SameDocProf=Factory->GetBool("SameDocProf");
	GroupProf=Factory->GetBool("GroupProf");
	SaveResults=Factory->GetBool("SaveResults");
	RURI OldName(Results);
	Results=Factory->Get("Results");
	if((OldName!=Results)&&ResultsFile)
	{
		delete ResultsFile;
		ResultsFile=0;
	}
	ExportDocsSims=Factory->GetBool("ExportDocsSims");
	DocsSims=Factory->Get("DocsSims");
	ExportDocsIncs=Factory->GetBool("ExportDocsIncs");
	DocsIncs=Factory->Get("DocsIncs");
}


//------------------------------------------------------------------------------
void GStatsSims::Connect(GSession* session)
{
	GStatsCalc::Connect(session);
}


//------------------------------------------------------------------------------
void GStatsSims::Disconnect(GSession* session)
{
	GStatsCalc::Disconnect(session);
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
				Export<<Objs1()->GetVector().Inclusion(Objs2()->GetVector(),otDoc);
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
void GStatsSims::CreateParams(RConfig* params)
{
	params->InsertParam(new RParamValue("Docs",false));
	params->InsertParam(new RParamValue("ProfDoc",false));
	params->InsertParam(new RParamValue("GroupDoc",false));
	params->InsertParam(new RParamValue("Profiles",false));
	params->InsertParam(new RParamValue("SameDocProf",false));
	params->InsertParam(new RParamValue("GroupProf",false));
	params->InsertParam(new RParamValue("SaveResults",false));
	params->InsertParam(new RParamValue("Results",""));
	params->InsertParam(new RParamValue("ExportDocsSims",false));
	params->InsertParam(new RParamValue("DocsSims",""));
	params->InsertParam(new RParamValue("ExportDocsIncs",false));
	params->InsertParam(new RParamValue("DocsIncs",""));
}


//------------------------------------------------------------------------------
GStatsSims::~GStatsSims(void)
{
}


//------------------------------------------------------------------------------
CREATE_STATSCALC_FACTORY("Similarity Statistics","Similarity Statistics",GStatsSims)
