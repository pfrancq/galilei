/*

	GALILEI Research Project

	GConfig.cpp

	XML Structure representing a configuration - Implementation.

	Copyright 2003 by the Université Libre de Bruxelles.

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



//------------------------------------------------------------------------------
// include files for R Project
#include <rstd/rio.h>
#include <rstd/rxmlfile.h>
using namespace R;


//------------------------------------------------------------------------------
// include files for GALILEI
#include <sessions/gconfig.h>
#include <filters/gurlmanager.h>
#include <filters/gfilter.h>
#include <profiles/gprofilecalcmanager.h>
#include <profiles/gprofilecalc.h>
#include <groups/ggroupingmanager.h>
#include <groups/ggrouping.h>
#include <groups/ggroupcalcmanager.h>
#include <groups/ggroupcalc.h>
#include <sessions/gstatscalcmanager.h>
#include <sessions/gstatscalc.h>
#include <docs/glinkcalcmanager.h>
#include <docs/glinkcalc.h>
#include <langs/glang.h>
#include <langs/glangs.h>
#include <docs/gdocanalyse.h>
#include <docs/gdocanalysemanager.h>
using namespace GALILEI;



//------------------------------------------------------------------------------
//
// class GConfig
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GConfig::GConfig(const char* f) throw(bad_alloc)
	: RXMLStruct(), FileName(f)
{
	RXMLTag* t;

	AddNode(0,t=new RXMLTag("rdf:RDF"));
	AddNode(t,Filters=new RXMLTag("galileiconfig:filters"));
	AddNode(t,ProfileCalcs=new RXMLTag("galileiconfig:profileCalcs"));
	AddNode(t,Groupings=new RXMLTag("galileiconfig:groupings"));
	AddNode(t,GroupCalcs=new RXMLTag("galileiconfig:groupCalcs"));
	AddNode(t,StatsCalcs=new RXMLTag("galileiconfig:statsCalcs"));
	AddNode(t,LinkCalcs=new RXMLTag("galileiconfig:linkCalcs"));
	AddNode(t,Langs=new RXMLTag("galileiconfig:langs"));
	AddNode(t,DocAnalyses=new RXMLTag("galileiconfig:docanalyses"));
}


//------------------------------------------------------------------------------
void GConfig::Load(void) throw(GException)
{
	Clear();
	try
	{
		RXMLFile File(FileName,this,R::Read);
		Filters=GetTop()->GetTag("galileiconfig:filters");
		ProfileCalcs=GetTop()->GetTag("galileiconfig:profileCalcs");
		Groupings=GetTop()->GetTag("galileiconfig:groupings");
		GroupCalcs=GetTop()->GetTag("galileiconfig:groupCalcs");
		StatsCalcs=GetTop()->GetTag("galileiconfig:statsCalcs");
		LinkCalcs=GetTop()->GetTag("galileiconfig:linkCalcs");
		Langs=GetTop()->GetTag("galileiconfig:langs");
		DocAnalyses=GetTop()->GetTag("galileiconfig:docanalyses");
	}
	catch(...)
	{
		Filters=0;
		ProfileCalcs=0;
		throw GException("Problem loading config file.");
	}
}


//------------------------------------------------------------------------------
void GConfig::Save(void) throw(GException)
{
	try
	{
		RXMLFile File(FileName,this,R::Create);
	}
	catch(...)
	{
		throw GException("Problem loading config file.");
	}
}


//------------------------------------------------------------------------------
void GConfig::Read(GURLManager& mng)
{
	GFactoryFilterCursor Cur;

	if(!Filters) return;
	Cur=mng.GetFiltersCursor();
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		Cur()->ReadConfig(Filters);
	}
}


//------------------------------------------------------------------------------
void GConfig::Store(GURLManager& mng)
{
	GFactoryFilterCursor Cur;

	Cur=mng.GetFiltersCursor();
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		Cur()->SaveConfig(Filters);
	}
}


//------------------------------------------------------------------------------
void GConfig::Read(GProfileCalcManager& mng)
{
	GFactoryProfileCalcCursor Cur;

	if(!ProfileCalcs) return;
	Cur=mng.GetProfileCalcsCursor();
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		Cur()->ReadConfig(ProfileCalcs);
	}
	try
	{
		mng.SetCurrentMethod(ProfileCalcs->GetAttrValue("Current"));
	}
	catch(GException)
	{
	}
}


//------------------------------------------------------------------------------
void GConfig::Store(GProfileCalcManager& mng)
{
	GFactoryProfileCalcCursor Cur;
	GProfileCalc* calc;

	Cur=mng.GetProfileCalcsCursor();
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		Cur()->SaveConfig(ProfileCalcs);
	}
	calc=mng.GetCurrentMethod();
	if(calc)
		ProfileCalcs->InsertAttr("Current",calc->GetFactory()->GetName());
	else
		ProfileCalcs->InsertAttr("Current","None");
}


//------------------------------------------------------------------------------
void GConfig::Read(GGroupingManager& mng)
{
	GFactoryGroupingCursor Cur;

	if(!Groupings) return;
	Cur=mng.GetGroupingsCursor();
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		Cur()->ReadConfig(Groupings);
	}
	try
	{
		mng.SetCurrentMethod(Groupings->GetAttrValue("Current"));
	}
	catch(GException)
	{
	}
}


//------------------------------------------------------------------------------
void GConfig::Store(GGroupingManager& mng)
{
	GFactoryGroupingCursor Cur;
	GGrouping* calc;

	Cur=mng.GetGroupingsCursor();
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		Cur()->SaveConfig(Groupings);
	}
	calc=mng.GetCurrentMethod();
	if(calc)
		Groupings->InsertAttr("Current",calc->GetFactory()->GetName());
	else
		Groupings->InsertAttr("Current","None");
}


//------------------------------------------------------------------------------
void GConfig::Read(GGroupCalcManager& mng)
{
	GFactoryGroupCalcCursor Cur;

	if(!GroupCalcs) return;
	Cur=mng.GetGroupCalcsCursor();
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		Cur()->ReadConfig(GroupCalcs);
	}
	try
	{
		mng.SetCurrentMethod(GroupCalcs->GetAttrValue("Current"));
	}
	catch(GException)
	{
	}
}


//------------------------------------------------------------------------------
void GConfig::Store(GGroupCalcManager& mng)
{
	GFactoryGroupCalcCursor Cur;
	GGroupCalc* calc;

	Cur=mng.GetGroupCalcsCursor();
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		Cur()->SaveConfig(GroupCalcs);
	}
	calc=mng.GetCurrentMethod();
	if(calc)
		GroupCalcs->InsertAttr("Current",calc->GetFactory()->GetName());
	else
		GroupCalcs->InsertAttr("Current","None");
}


//------------------------------------------------------------------------------
void GConfig::Read(GStatsCalcManager& mng)
{
	GFactoryStatsCalcCursor Cur;

	if(!StatsCalcs) return;
	Cur=mng.GetStatsCalcsCursor();
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		Cur()->ReadConfig(StatsCalcs);
	}
}


//------------------------------------------------------------------------------
void GConfig::Store(GStatsCalcManager& mng)
{
	GFactoryStatsCalcCursor Cur;

	Cur=mng.GetStatsCalcsCursor();
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		Cur()->SaveConfig(StatsCalcs);
	}
}


//------------------------------------------------------------------------------
void GConfig::Read(GLinkCalcManager& mng)
{
	GFactoryLinkCalcCursor Cur;

	if(!LinkCalcs) return;
	Cur=mng.GetLinkCalcsCursor();
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		Cur()->ReadConfig(LinkCalcs);
	}
	try
	{
		mng.SetCurrentMethod(LinkCalcs->GetAttrValue("Current"));
	}
	catch(GException)
	{
	}
}


//------------------------------------------------------------------------------
void GConfig::Store(GLinkCalcManager& mng)
{
	GFactoryLinkCalcCursor Cur;
	GLinkCalc* lcalc;

	Cur=mng.GetLinkCalcsCursor();
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		Cur()->SaveConfig(LinkCalcs);
	}
	lcalc=mng.GetCurrentMethod();
	if(lcalc)
		LinkCalcs->InsertAttr("Current",lcalc->GetFactory()->GetName());
	else
		LinkCalcs->InsertAttr("Current","None");
}


//------------------------------------------------------------------------------
void GConfig::Read(GLangs& mng)
{
	GFactoryLangCursor Cur;

	if(!Langs) return;
	Cur=mng.GetLangsCursor();
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		Cur()->ReadConfig(Langs);
	}
}


//------------------------------------------------------------------------------
void GConfig::Store(GLangs& mng)
{
	GFactoryLangCursor Cur;

	Cur=mng.GetLangsCursor();
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		Cur()->SaveConfig(Langs);
	}
}


//------------------------------------------------------------------------------
void GConfig::Read(GDocAnalyseManager& mng)
{
	GFactoryDocAnalyseCursor Cur;

	if(!DocAnalyses) return;
	Cur=mng.GetDocAnalysesCursor();
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		Cur()->ReadConfig(DocAnalyses);
	}
	try
	{
		mng.SetCurrentMethod(DocAnalyses->GetAttrValue("Current"));
	}
	catch(GException)
	{
	}
}


//------------------------------------------------------------------------------
void GConfig::Store(GDocAnalyseManager& mng)
{
	GFactoryDocAnalyseCursor Cur;
	GDocAnalyse* lcalc;

	Cur=mng.GetDocAnalysesCursor();
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		Cur()->SaveConfig(DocAnalyses);
	}
	lcalc=mng.GetCurrentMethod();
	if(lcalc)
		DocAnalyses->InsertAttr("Current",lcalc->GetFactory()->GetName());
	else
		DocAnalyses->InsertAttr("Current","None");
}


//-----------------------------------------------------------------------------
GConfig::~GConfig(void)
{
}
