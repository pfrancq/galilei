/*

	GALILEI Research Project

	GStatsSims.cpp

	Groups Evaluation - Implementation.

	Copyright 2002-2008 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).

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
	GStatSimDocs(GSession* ses,R::RTextFile* f) : GStatSimElements<GDoc,GDoc>(ses,true,f)
	{
		Measure=GALILEIApp->GetManager<GMeasureManager>("Measures")->GetCurrentMethod("Documents Similarities");
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

	GStatSimProfiles(GSession* ses,R::RTextFile* f) : GStatSimElements<GProfile,GProfile>(ses,true,f)
	{
		Measure=GALILEIApp->GetManager<GMeasureManager>("Measures")->GetCurrentMethod("Profiles Similarities");
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

	GStatSimDocGrp(GSession* ses,R::RTextFile* f) : GStatSimElements<GDoc,GCommunity>(ses,false,f), Com(1)
	{
		Measure=GALILEIApp->GetManager<GMeasureManager>("Measures")->GetCurrentMethod("Documents/Groups Similarities");
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

	GStatSimDocProf(GSession* ses,R::RTextFile* f) : GStatSimElements<GDoc,GProfile>(ses,false,f)
	{
		Measure=GALILEIApp->GetManager<GMeasureManager>("Measures")->GetCurrentMethod("Documents/Profiles Similarities");
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

	GStatSimProfGrp(GSession* ses,R::RTextFile* f) : GStatSimElements<GProfile,GCommunity>(ses,false,f), Com(1)
	{
		Measure=GALILEIApp->GetManager<GMeasureManager>("Measures")->GetCurrentMethod("Profiles/Groups Similarities");
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
GStatsSims::GStatsSims(GFactoryStatsCalc* fac)
	: GStatsCalc(fac), Details(0)
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
	File=Factory->GetBool("File");
	RString OldName(Name);
	Name=Factory->Get("Name");
	if((OldName!=Name)&&Details)
	{
		delete Details;
		Details=0;
	}
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
void GStatsSims::Compute(R::RXMLStruct* xml,R::RXMLTag& res)
{
	RXMLTag* tag;
	RXMLTag* tag2;
	RString str;

	// Init Main XML Tag
	tag=new RXMLTag(Factory->GetName());
	xml->AddTag(&res,tag);

	// Create Details File if necessary
	if(File)
	{
		if(Details)
			(*Details)<<endl<<"-----------------------------------------------------"<<endl<<endl;
		else
		{
			try
			{
				Details=new RTextFile(Name);
				Details->Open(RIO::Create);
			}
			catch(...)
			{
				Details=0;
			}
		}
	}

	// Verify if the ideal group should be created
	if(GroupDoc||GroupProf)
		Session->BuildGroupsFromIdeal(otCommunity);

	// Compute Statistics
	if(Docs)
	{
		tag2=new RXMLTag("Documents");
		xml->AddTag(tag,tag2);
		GStatSimDocs Stat(Session,Details);
		Stat.Run(this,xml,tag2);
	}
	if(ProfDoc)
	{
		tag2=new RXMLTag("Documents-Profiles");
		xml->AddTag(tag,tag2);
		GStatSimDocProf Stat(Session,Details);
		Stat.Run(this,xml,tag2);
	}
	if(GroupDoc)
	{
		tag2=new RXMLTag("Documents-Groups");
		xml->AddTag(tag,tag2);
		GStatSimDocGrp Stat(Session,Details);
		Stat.Run(this,xml,tag2);
	}
	if(Profiles)
	{
		tag2=new RXMLTag("Profiles");
		xml->AddTag(tag,tag2);
		GStatSimProfiles Stat(Session,Details);
		Stat.Run(this,xml,tag2);
	}
	if(SameDocProf)
	{
		tag2=new RXMLTag("Profiles-Common-Documents");
		xml->AddTag(tag,tag2);
		GStatProfDoc Stat(Session,Details);
		Stat.Run(this,xml,tag2);
	}
	if(GroupProf)
	{
		tag2=new RXMLTag("Profiles-Groups");
		xml->AddTag(tag,tag2);
		GStatSimProfGrp Stat(Session,Details);
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
	params->InsertParam(new RParamValue("File",false));
	params->InsertParam(new RParamValue("Name",""));
}


//------------------------------------------------------------------------------
GStatsSims::~GStatsSims(void)
{
}


//------------------------------------------------------------------------------
CREATE_STATSCALC_FACTORY("Similarity Statistics",GStatsSims)
