/*

	GALILEI Research Project

	GStatsSims.cpp

	Groups Evaluation - Implementation.

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
// include files for ANSI C/C++
#include <stdlib.h>


//-----------------------------------------------------------------------------
//include files for GALILEI
#include <gstatssims.h>
#include <groups/ggroups.h>
#include <groups/ggroup.h>
#include <profiles/gsubprofile.h>
#include <sessions/gsession.h>
#include <gstatsimdoc.h>
#include <gstatsimdocprof.h>
#include <gstatsimdocgrp.h>
#include <gstatprofdoc.h>
#include <gstatsimsubprof.h>
#include <gstatsimprofgrp.h>
using namespace GALILEI;
using namespace R;



//-----------------------------------------------------------------------------
//
//  class GStatsSims
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GStatsSims::GStatsSims(GFactoryStatsCalc* fac) throw(bad_alloc)
	: GStatsCalc(fac)
{
}


//-----------------------------------------------------------------------------
void GStatsSims::ApplyConfig(void)
{
	Docs=Factory->GetBool("Docs");
	ProfDoc=Factory->GetBool("ProfDoc");
	GroupDoc=Factory->GetBool("GroupDoc");
	Profiles=Factory->GetBool("Profiles");
	SameDocProf=Factory->GetBool("SameDocProf");
	GroupProf=Factory->GetBool("GroupProf");
}


//-----------------------------------------------------------------------------
void GStatsSims::Connect(GSession* session)
{
	GStatsCalc::Connect(session);
}


//-----------------------------------------------------------------------------
void GStatsSims::Disconnect(GSession* session)
{
	GStatsCalc::Disconnect(session);
}


//---------------------------------------------------------------------------
void GStatsSims::Compute(R::RXMLTag& res)
{
	RXMLTag* tag;
	RXMLTag* tag2;
	RXMLTag* tag3;
	RString str;

	tag=new RXMLTag(Factory->GetName());
	res.AddTag(tag);
	if(Docs)
	{
		tag2=new RXMLTag("Documents");
		tag->AddTag(tag2);
		GStatSimDoc Stat(Session,0,true,true);
		Stat.Run();
		tag3=new RXMLTag("Without idf");
		tag2->AddTag(tag3);
		str=dtoa(Stat.GetAvgIntraL());
		tag3->AddTag(new RXMLTag("Avg Intra="+str));
		str=dtoa(Stat.GetAvgInterL());
		tag3->AddTag(new RXMLTag("Avg Inter="+str));
		str=dtoa(Stat.GetAVGolL());
		tag3->AddTag(new RXMLTag("Avg Local Overlap Factor="+str));
		str=dtoa(Stat.GetAVGGolL());
		tag3->AddTag(new RXMLTag("Avg Global Overlap Factor="+str));
		str=dtoa(Stat.GetRieL());
		tag3->AddTag(new RXMLTag("Rie Factor="+str));
		tag3=new RXMLTag("With idf");
		tag2->AddTag(tag3);
		str=dtoa(Stat.GetAvgIntraG());
		tag3->AddTag(new RXMLTag("Avg Intra="+str));
		str=dtoa(Stat.GetAvgInterG());
		tag3->AddTag(new RXMLTag("Avg Inter="+str));
		str=dtoa(Stat.GetAVGolG());
		tag3->AddTag(new RXMLTag("Avg Local Overlap Factor="+str));
		str=dtoa(Stat.GetAVGGolG());
		tag3->AddTag(new RXMLTag("Avg Global Overlap Factor="+str));
		str=dtoa(Stat.GetRieG());
		tag3->AddTag(new RXMLTag("Rie Factor="+str));
	}
	if(ProfDoc)
	{
		tag2=new RXMLTag("Documents/Profiles");
		tag->AddTag(tag2);
		GStatSimDocProf Stat(Session,0,true,true);
		Stat.Run();
		tag3=new RXMLTag("Without idf");
		tag2->AddTag(tag3);
		str=dtoa(Stat.GetAvgIntraL());
		tag3->AddTag(new RXMLTag("Avg Intra="+str));
		str=dtoa(Stat.GetAvgInterL());
		tag3->AddTag(new RXMLTag("Avg Inter="+str));
		str=dtoa(Stat.GetAVGolL());
		tag3->AddTag(new RXMLTag("Avg Local Overlap Factor="+str));
		str=dtoa(Stat.GetAVGGolL());
		tag3->AddTag(new RXMLTag("Avg Global Overlap Factor="+str));
		str=dtoa(Stat.GetRieL());
		tag3->AddTag(new RXMLTag("Rie Factor="+str));
		tag3=new RXMLTag("With idf");
		tag2->AddTag(tag3);
		str=dtoa(Stat.GetAvgIntraG());
		tag3->AddTag(new RXMLTag("Avg Intra="+str));
		str=dtoa(Stat.GetAvgInterG());
		tag3->AddTag(new RXMLTag("Avg Inter="+str));
		str=dtoa(Stat.GetAVGolG());
		tag3->AddTag(new RXMLTag("Avg Local Overlap Factor="+str));
		str=dtoa(Stat.GetAVGGolG());
		tag3->AddTag(new RXMLTag("Avg Global Overlap Factor="+str));
		str=dtoa(Stat.GetRieG());
		tag3->AddTag(new RXMLTag("Rie Factor="+str));
	}
	if(GroupDoc)
	{
		tag2=new RXMLTag("Documents/Groups");
		tag->AddTag(tag2);
		GStatSimDocGrp Stat(Session,0,true,true);
		Stat.Run();
		tag3=new RXMLTag("Without idf");
		tag2->AddTag(tag3);
		str=dtoa(Stat.GetAvgIntraL());
		tag3->AddTag(new RXMLTag("Avg Intra="+str));
		str=dtoa(Stat.GetAvgInterL());
		tag3->AddTag(new RXMLTag("Avg Inter="+str));
		str=dtoa(Stat.GetAVGolL());
		tag3->AddTag(new RXMLTag("Avg Local Overlap Factor="+str));
		str=dtoa(Stat.GetAVGGolL());
		tag3->AddTag(new RXMLTag("Avg Global Overlap Factor="+str));
		str=dtoa(Stat.GetRieL());
		tag3->AddTag(new RXMLTag("Rie Factor="+str));
		tag3=new RXMLTag("With idf");
		tag2->AddTag(tag3);
		str=dtoa(Stat.GetAvgIntraG());
		tag3->AddTag(new RXMLTag("Avg Intra="+str));
		str=dtoa(Stat.GetAvgInterG());
		tag3->AddTag(new RXMLTag("Avg Inter="+str));
		str=dtoa(Stat.GetAVGolG());
		tag3->AddTag(new RXMLTag("Avg Local Overlap Factor="+str));
		str=dtoa(Stat.GetAVGGolG());
		tag3->AddTag(new RXMLTag("Avg Global Overlap Factor="+str));
		str=dtoa(Stat.GetRieG());
		tag3->AddTag(new RXMLTag("Rie Factor="+str));
	}
	if(Profiles)
	{
		tag2=new RXMLTag("Profiles");
		tag->AddTag(tag2);
		GStatSimSubProf Stat(Session,0,true,true);
		Stat.Run();
		tag3=new RXMLTag("Without idf");
		tag2->AddTag(tag3);
		str=dtoa(Stat.GetAvgIntraL());
		tag3->AddTag(new RXMLTag("Avg Intra="+str));
		str=dtoa(Stat.GetAvgInterL());
		tag3->AddTag(new RXMLTag("Avg Inter="+str));
		str=dtoa(Stat.GetAVGolL());
		tag3->AddTag(new RXMLTag("Avg Local Overlap Factor="+str));
		str=dtoa(Stat.GetAVGGolL());
		tag3->AddTag(new RXMLTag("Avg Global Overlap Factor="+str));
		str=dtoa(Stat.GetRieL());
		tag3->AddTag(new RXMLTag("Rie Factor="+str));
		tag3=new RXMLTag("With idf");
		tag2->AddTag(tag3);
		str=dtoa(Stat.GetAvgIntraG());
		tag3->AddTag(new RXMLTag("Avg Intra="+str));
		str=dtoa(Stat.GetAvgInterG());
		tag3->AddTag(new RXMLTag("Avg Inter="+str));
		str=dtoa(Stat.GetAVGolG());
		tag3->AddTag(new RXMLTag("Avg Local Overlap Factor="+str));
		str=dtoa(Stat.GetAVGGolG());
		tag3->AddTag(new RXMLTag("Avg Global Overlap Factor="+str));
		str=dtoa(Stat.GetRieG());
		tag3->AddTag(new RXMLTag("Rie Factor="+str));
	}
	if(SameDocProf)
	{
		tag2=new RXMLTag("Profiles/Common Documents");
		tag->AddTag(tag2);
		GStatProfDoc Stat(Session,0);
		Stat.Run();
		str=dtoa(Stat.GetMeanNbProf());
		tag2->AddTag(new RXMLTag("Avg Number of profiles assessing same documents="+str));
		str=dtoa(Stat.GetMeanSame());
		tag2->AddTag(new RXMLTag("Avg Agreement Ratio="+str));
		str=dtoa(Stat.GetMeanDiff());
		tag2->AddTag(new RXMLTag("Avg Disagreement Ratio="+str));
	}
	if(GroupDoc)
	{
		tag2=new RXMLTag("Profiles/Groups");
		tag->AddTag(tag2);
		GStatSimProfGrp Stat(Session,Session->GetIdealGroups());
		Stat.Run();
		str=dtoa(Stat.GetAvgIntra());
		tag2->AddTag(new RXMLTag("Avg Intra="+str));
		str=dtoa(Stat.GetAvgInter());
		tag2->AddTag(new RXMLTag("Avg Inter="+str));
		str=dtoa(Stat.GetAVGol());
		tag2->AddTag(new RXMLTag("Avg Local Overlap Factor="+str));
		str=dtoa(Stat.GetAVGGrpol());
		tag2->AddTag(new RXMLTag("Avg Global Overlap Factor="+str));
		str=dtoa(Stat.GetRie());
		tag2->AddTag(new RXMLTag("Rie Factor="+str));
		str=dtoa(Stat.GetJ());
		tag2->AddTag(new RXMLTag("J="+str));
	}
}


//------------------------------------------------------------------------------
void GStatsSims::CreateParams(GParams* params)
{
	params->InsertPtr(new GParamBool("Docs",false));
	params->InsertPtr(new GParamBool("ProfDoc",false));
	params->InsertPtr(new GParamBool("GroupDoc",false));
	params->InsertPtr(new GParamBool("Profiles",false));
	params->InsertPtr(new GParamBool("SameDocProf",false));
	params->InsertPtr(new GParamBool("GroupProf",false));
}


//-----------------------------------------------------------------------------
GStatsSims::~GStatsSims(void)
{
}


//------------------------------------------------------------------------------
CREATE_STATSCALC_FACTORY("Similarity Statistics",GStatsSims)
