/*

	GALILEI Research Project

	GConfig.cpp

	XML Structure representing a configuration - Implementation.

	Copyright 2003 by the Université libre de Bruxelles.

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
// include files for R Project
#include <rstd/rio.h>
#include <rstd/rxmlfile.h>
//#include <rstd/rxmltag.h>


//------------------------------------------------------------------------------
// include files for GALILEI
#include <sessions/gconfig.h>
#include <sessions/gplugin.h>
#include <docs/gfiltermanager.h>
#include <docs/gfilter.h>
#include <engines/gengine.h>
#include <engines/gmetaengine.h>
#include <engines/genginemanager.h>
#include <profiles/gprofilecalcmanager.h>
#include <profiles/gprofilecalc.h>
#include <groups/ggroupingmanager.h>
#include <groups/ggrouping.h>
#include <groups/ggroupcalcmanager.h>
#include <groups/ggroupcalc.h>
#include <sessions/gstatscalcmanager.h>
#include <sessions/gstatscalc.h>
#include <sessions/gsession.h>
#include <docs/glinkcalcmanager.h>
#include <docs/glinkcalc.h>
#include <docs/gpostdocmanager.h>
#include <docs/gpostdoc.h>
#include <infos/glang.h>
#include <infos/glangmanager.h>
#include <docs/gdocanalyse.h>
#include <docs/gdocanalysemanager.h>
#include <groups/gpostgroup.h>
#include <groups/gpostgroupmanager.h>

using namespace R;
using namespace GALILEI;



//------------------------------------------------------------------------------
//
// class GConfig
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GConfig::GConfig(const char* f) throw(std::bad_alloc)
	: RXMLStruct(), FileName(f)
{
	RXMLTag* t;

	AddTag(0,t=new RXMLTag("rdf:RDF"));
	AddTag(t,Filters=new RXMLTag("galileiconfig:filters"));
	AddTag(t,ProfileCalcs=new RXMLTag("galileiconfig:profileCalcs"));
	AddTag(t,Groupings=new RXMLTag("galileiconfig:groupings"));
	AddTag(t,GroupCalcs=new RXMLTag("galileiconfig:groupCalcs"));
	AddTag(t,StatsCalcs=new RXMLTag("galileiconfig:statsCalcs"));
	AddTag(t,LinkCalcs=new RXMLTag("galileiconfig:linkCalcs"));
	AddTag(t,PostDocs=new RXMLTag("galileiconfig:postDocs"));
	AddTag(t,Langs=new RXMLTag("galileiconfig:langs"));
	AddTag(t,DocAnalyses=new RXMLTag("galileiconfig:docanalyses"));
	AddTag(t,PostGroups=new RXMLTag("galileiconfig:postgroups"));
	AddTag(t,Engines=new RXMLTag("galileiconfig:engines"));
	AddTag(t,MetaEngines=new RXMLTag("galileiconfig:metaengines"));
	AddTag(t,SessionParams=new RXMLTag("galileiconfig:session"));
}


//------------------------------------------------------------------------------
void GConfig::Load(void) throw(GException)
{
	Clear();
	try
	{
		RXMLFile File(FileName,this,R::Read);
		File.Process();
		Filters=GetTop()->GetTag("galileiconfig:filters");
		ProfileCalcs=GetTop()->GetTag("galileiconfig:profileCalcs");
		Groupings=GetTop()->GetTag("galileiconfig:groupings");
		GroupCalcs=GetTop()->GetTag("galileiconfig:groupCalcs");
		StatsCalcs=GetTop()->GetTag("galileiconfig:statsCalcs");
		LinkCalcs=GetTop()->GetTag("galileiconfig:linkCalcs");
		PostDocs=GetTop()->GetTag("galileiconfig:postDocs");
		Langs=GetTop()->GetTag("galileiconfig:langs");
		DocAnalyses=GetTop()->GetTag("galileiconfig:docanalyses");
		PostGroups=GetTop()->GetTag("galileiconfig:postgroups");
		Engines=GetTop()->GetTag("galileiconfig:engines");
		MetaEngines=GetTop()->GetTag("galileiconfig:metaengines");
		SessionParams=GetTop()->GetTag("galileiconfig:session");
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
		File.Process();
	}
	catch(...)
	{
		throw GException("Problem loading config file.");
	}
}


//------------------------------------------------------------------------------
void GConfig::Read(GFilterManager* mng)
{
	GFactoryFilterCursor Cur;

	if(!Filters) return;
	Cur=mng->GetFiltersCursor();
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		Cur()->ReadConfig(Filters);
	}
}


//------------------------------------------------------------------------------
void GConfig::Store(GFilterManager* mng)
{
	GFactoryFilterCursor Cur;

	Cur=mng->GetFiltersCursor();
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		Cur()->SaveConfig(this,Filters);
	}
}


//------------------------------------------------------------------------------
void GConfig::Read(GProfileCalcManager* mng)
{
	GFactoryProfileCalcCursor Cur;

	if(!ProfileCalcs) return;
	Cur=mng->GetProfileCalcsCursor();
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		Cur()->ReadConfig(ProfileCalcs);
	}
	try
	{
		mng->SetCurrentMethod(ProfileCalcs->GetAttrValue("Current"));
	}
	catch(GException)
	{
	}
}


//------------------------------------------------------------------------------
void GConfig::Store(GProfileCalcManager* mng)
{
	GFactoryProfileCalcCursor Cur;
	GProfileCalc* calc;

	Cur=mng->GetProfileCalcsCursor();
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		Cur()->SaveConfig(this,ProfileCalcs);
	}
	calc=mng->GetCurrentMethod();
	if(calc)
		ProfileCalcs->InsertAttr("Current",calc->GetFactory()->GetName());
	else
		ProfileCalcs->InsertAttr("Current","None");
}


//------------------------------------------------------------------------------
void GConfig::Read(GGroupingManager* mng)
{
	GFactoryGroupingCursor Cur;

	if(!Groupings) return;
	Cur=mng->GetGroupingsCursor();
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		Cur()->ReadConfig(Groupings);
	}
	try
	{
		mng->SetCurrentMethod(Groupings->GetAttrValue("Current"));
	}
	catch(GException)
	{
	}
}


//------------------------------------------------------------------------------
void GConfig::Store(GGroupingManager* mng)
{
	GFactoryGroupingCursor Cur;
	GGrouping* calc;

	Cur=mng->GetGroupingsCursor();
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		Cur()->SaveConfig(this,Groupings);
	}
	calc=mng->GetCurrentMethod();
	if(calc)
		Groupings->InsertAttr("Current",calc->GetFactory()->GetName());
	else
		Groupings->InsertAttr("Current","None");
}


//------------------------------------------------------------------------------
void GConfig::Read(GGroupCalcManager* mng)
{
	GFactoryGroupCalcCursor Cur;

	if(!GroupCalcs) return;
	Cur=mng->GetGroupCalcsCursor();
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		Cur()->ReadConfig(GroupCalcs);
	}
	try
	{
		mng->SetCurrentMethod(GroupCalcs->GetAttrValue("Current"));
	}
	catch(GException)
	{
	}
}


//------------------------------------------------------------------------------
void GConfig::Store(GGroupCalcManager* mng)
{
	GFactoryGroupCalcCursor Cur;
	GGroupCalc* calc;

	Cur=mng->GetGroupCalcsCursor();
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		Cur()->SaveConfig(this,GroupCalcs);
	}
	calc=mng->GetCurrentMethod();
	if(calc)
		GroupCalcs->InsertAttr("Current",calc->GetFactory()->GetName());
	else
		GroupCalcs->InsertAttr("Current","None");
}


//------------------------------------------------------------------------------
void GConfig::Read(GStatsCalcManager* mng)
{
	GFactoryStatsCalcCursor Cur;

	if(!StatsCalcs) return;
	Cur=mng->GetStatsCalcsCursor();
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		Cur()->ReadConfig(StatsCalcs);
	}
}


//------------------------------------------------------------------------------
void GConfig::Store(GStatsCalcManager* mng)
{
	GFactoryStatsCalcCursor Cur;

	Cur=mng->GetStatsCalcsCursor();
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		Cur()->SaveConfig(this,StatsCalcs);
	}
}


//------------------------------------------------------------------------------
void GConfig::Read(GLinkCalcManager* mng)
{
	GFactoryLinkCalcCursor Cur;

	if(!LinkCalcs) return;
	Cur=mng->GetLinkCalcsCursor();
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		Cur()->ReadConfig(LinkCalcs);
	}
	try
	{
		mng->SetCurrentMethod(LinkCalcs->GetAttrValue("Current"));
	}
	catch(GException)
	{
	}
}


//------------------------------------------------------------------------------
void GConfig::Store(GLinkCalcManager* mng)
{
	GFactoryLinkCalcCursor Cur;
	GLinkCalc* lcalc;

	Cur=mng->GetLinkCalcsCursor();
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		Cur()->SaveConfig(this,LinkCalcs);
	}
	lcalc=mng->GetCurrentMethod();
	if(lcalc)
		LinkCalcs->InsertAttr("Current",lcalc->GetFactory()->GetName());
	else
		LinkCalcs->InsertAttr("Current","None");
}


//------------------------------------------------------------------------------
void GConfig::Read(GPostGroupManager* mng)
{
	GFactoryPostGroupCursor Cur;

	if(!PostGroups) return;
	Cur=mng->GetPostGroupsCursor();
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		Cur()->ReadConfig(PostGroups);
	}
}


//------------------------------------------------------------------------------
void GConfig::Store(GPostGroupManager* mng)
{
	GFactoryPostGroupCursor Cur;

	Cur=mng->GetPostGroupsCursor();
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		Cur()->SaveConfig(this,PostGroups);
	}
}


//------------------------------------------------------------------------------
void GConfig::Read(GPostDocManager* mng)
{
	GFactoryPostDocCursor Cur;

	if(!PostDocs) return;
	Cur=mng->GetPostDocsCursor();
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		Cur()->ReadConfig(PostDocs);
	}
}


//------------------------------------------------------------------------------
void GConfig::Store(GPostDocManager* mng)
{
	GFactoryPostDocCursor Cur;

	Cur=mng->GetPostDocsCursor();
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		Cur()->SaveConfig(this,PostDocs);
	}
}


//------------------------------------------------------------------------------
void GConfig::Read(GLangManager* mng)
{
	GFactoryLangCursor Cur;

	if(!Langs) return;
	Cur=mng->GetLangsCursor();
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		Cur()->ReadConfig(Langs);
	}
}


//------------------------------------------------------------------------------
void GConfig::Store(GLangManager* mng)
{
	GFactoryLangCursor Cur;

	Cur=mng->GetLangsCursor();
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		Cur()->SaveConfig(this,Langs);
	}
}


//------------------------------------------------------------------------------
void GConfig::Read(GDocAnalyseManager* mng)
{
	GFactoryDocAnalyseCursor Cur;

	if(!DocAnalyses) return;
	Cur=mng->GetDocAnalysesCursor();
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		Cur()->ReadConfig(DocAnalyses);
	}
	try
	{
		mng->SetCurrentMethod(DocAnalyses->GetAttrValue("Current"));
	}
	catch(GException)
	{
	}
}


//------------------------------------------------------------------------------
void GConfig::Store(GDocAnalyseManager* mng)
{
	GFactoryDocAnalyseCursor Cur;
	GDocAnalyse* lcalc;

	Cur=mng->GetDocAnalysesCursor();
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		Cur()->SaveConfig(this,DocAnalyses);
	}
	lcalc=mng->GetCurrentMethod();
	if(lcalc)
		DocAnalyses->InsertAttr("Current",lcalc->GetFactory()->GetName());
	else
		DocAnalyses->InsertAttr("Current","None");
}


//------------------------------------------------------------------------------
void GConfig::Read(GEngineManager* mng)
{
	GFactoryEngineCursor Cur;
	GFactoryMetaEngineCursor CurM;
	
	if(Engines) 
	{
		Cur=mng->GetEnginesCursor();
		for(Cur.Start();!Cur.End();Cur.Next())
		{
			Cur()->ReadConfig(Engines);
		}
	}
	
	if(!MetaEngines) return;
	CurM=mng->GetMetaEnginesCursor();
	for(CurM.Start();!CurM.End();CurM.Next())
	{
		CurM()->ReadConfig(MetaEngines);
	}
	try
	{
		mng->SetCurrentMethod(MetaEngines->GetAttrValue("Current"));
	}
	catch(GException)
	{
	}
}


//------------------------------------------------------------------------------
void GConfig::Store(GEngineManager* mng)
{
	GFactoryEngineCursor Cur;
	GFactoryMetaEngineCursor CurM;
	GMetaEngine* meta;
	
	Cur=mng->GetEnginesCursor();
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		Cur()->SaveConfig(this,Engines);
	}

	CurM=mng->GetMetaEnginesCursor();
	for(CurM.Start();!CurM.End();CurM.Next())
	{
		CurM()->SaveConfig(this,MetaEngines);
	}
	meta=mng->GetCurrentMethod();
	if(meta)
		MetaEngines->InsertAttr("Current",meta->GetFactory()->GetName());
	else
		MetaEngines->InsertAttr("Current","None");
}


//------------------------------------------------------------------------------
void GConfig::Read(GSessionParams& p)
{
	if (!SessionParams) return;
	try
	{
		p.ReadConfig(SessionParams);
	}
	catch(GException)
	{
	}
}


//------------------------------------------------------------------------------
void GConfig::Store(GSessionParams& p)
{
	if (!SessionParams) return;
	p.SaveConfig(this,SessionParams);
}


//-----------------------------------------------------------------------------
GConfig::~GConfig(void)
{
}
