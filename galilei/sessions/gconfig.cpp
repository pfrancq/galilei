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
#include <engines/gmetaenginemanager.h>
#include <profiles/gprofilecalcmanager.h>
#include <profiles/gprofilecalc.h>
#include <profiles/gpostprofilemanager.h>
#include <profiles/gpostprofile.h>
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

#include <sessions/gplugin.h>
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
	RString n;

	AddTag(0,Root=new RXMLTag("rdf:RDF"));
	AddTag(Root,SessionParams=new RXMLTag("galileiconfig:session"));

	//Insert a tag for every manager of plugins
	RCursor<GPluginManager> cur(GPluginManager::GetCursor());
	for(cur.Start();!cur.End();cur.Next())
	{
		n="";
		n+=RChar::ToLower(cur()->GetName()[static_cast<size_t>(0)]);
		n+=cur()->GetName().Mid(1);
		AddTag(Root,new RXMLTag("galileiconfig:"+n));
	}
}


//------------------------------------------------------------------------------
void GConfig::Load(void) throw(GException)
{
	RString n;

	Clear();
	try
	{
		RXMLFile File(FileName,this);
		File.Open(R::RIO::Read);
		SessionParams=GetTop()->GetTag("galileiconfig:session");

		RCursor<GPluginManager> cur(GPluginManager::GetCursor());
		RXMLTag* t;

		for(cur.Start();!cur.End();cur.Next())
		{
			n="";
			n+=RChar::ToLower(cur()->GetName()[static_cast<size_t>(0)]);
			n+=cur()->GetName().Mid(1);
			t=GetTop()->GetTag("galileiconfig:"+n);
			cur()->ReadConfig(t);
		}
	}
	catch(...)
	{
/*		Filters=0;
		ProfileCalcs=0;*/
		throw GException("Problem loading config file.");
	}
}


//------------------------------------------------------------------------------
void GConfig::Save(void) throw(GException)
{
	RString n;

	//GPluginManager::GetManager("Filter")->SaveConfig(this,GetTop()->GetTag("galileiconfig:filters"));
	RCursor<GPluginManager> cur(GPluginManager::GetCursor());
	RXMLTag* t;

	for(cur.Start();!cur.End();cur.Next())
	{
		n="";
		n+=RChar::ToLower(cur()->GetName()[static_cast<size_t>(0)]);
		n+=cur()->GetName().Mid(1);
		t=GetTop()->GetTag("galileiconfig:"+n);
		cur()->SaveConfig(this,t);
	}
	try
	{
		RXMLFile File(FileName,this);
		File.Open(R::RIO::Create);
	}
	catch(...)
	{
		throw GException("Problem loading config file.");
	}
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
