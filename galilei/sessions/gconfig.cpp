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
#include <rio.h>
#include <rxmlfile.h>
//#include <rxmltag.h>


//------------------------------------------------------------------------------
// include files for GALILEI
#include <gconfig.h>
#include <gplugin.h>
#include <gfilter.h>
#include <gengine.h>
#include <gmetaengine.h>
#include <gprofilecalc.h>
#include <gpostprofile.h>
#include <ggrouping.h>
#include <ggroupcalc.h>
#include <gstatscalc.h>
#include <gsession.h>
#include <glinkcalc.h>
#include <gpostdoc.h>
#include <glang.h>
#include <gdocanalyse.h>
#include <gpostgroup.h>
#include <gplugin.h>
#include <gpluginmanagers.h>
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
	AddTag(0,Root=new RXMLTag(this,"rdf:RDF"));
	AddTag(Root,SessionParams=new RXMLTag(this,"galileiconfig:session"));

	//Insert a tag for every manager of plugins
	RCursor<GGenericPluginManager> cur(GPluginManagers::PluginManagers.GetManagers());
	for(cur.Start();!cur.End();cur.Next())
	{
		RString n;
		n+=RChar::ToLower(cur()->GetName()[static_cast<size_t>(0)]);
		n+=cur()->GetName().Mid(1);
		AddTag(Root,new RXMLTag(this,"galileiconfig:"+n));
	}
}


//------------------------------------------------------------------------------
void GConfig::Load(void) throw(GException)
{
	Clear();
	try
	{
		RXMLFile File(FileName,this);
		File.Open(R::RIO::Read);
		SessionParams=GetTop()->GetTag("galileiconfig:session");

		RCursor<GGenericPluginManager> cur(GPluginManagers::PluginManagers.GetManagers());
		RXMLTag* t;

		for(cur.Start();!cur.End();cur.Next())
		{
			RString n;
			n+=RChar::ToLower(cur()->GetName()[static_cast<size_t>(0)]);
			n+=cur()->GetName().Mid(1);
			t=GetTop()->GetTag("galileiconfig:"+n);
			if(t)
				cur()->ReadConfig(t);
		}
	}
	catch(...)
	{
		throw GException("Problem loading config file.");
	}
}


//------------------------------------------------------------------------------
void GConfig::Save(void) throw(GException)
{
	RCursor<GGenericPluginManager> cur(GPluginManagers::PluginManagers.GetManagers());
	RXMLTag* t;

	for(cur.Start();!cur.End();cur.Next())
	{
		RString n;
		n+=RChar::ToLower(cur()->GetName()[static_cast<size_t>(0)]);
		n+=cur()->GetName().Mid(1);
		t=GetTop()->GetTag("galileiconfig:"+n);
		if(t)
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


//-----------------------------------------------------------------------------
GConfig::~GConfig(void)
{
}
