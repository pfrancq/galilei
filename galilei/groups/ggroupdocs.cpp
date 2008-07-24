/*

	GALILEI Research Project

	GGroupDocs.cpp

	Generic Documents Grouping Method - Implementation

	Copyright 2008 by the Université Libre de Bruxelles.

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
// include files for GALILEI
#include <ggroupdocs.h>
#include <gtopiccalc.h>
#include <gtopic.h>
#include <gdoc.h>
#include <gsession.h>
#include <ggalileiapp.h>
#include <gstorage.h>
#include <gslot.h>
using namespace R;
using namespace GALILEI;


//-----------------------------------------------------------------------------
// defines
//#define GROUP_DOCS_NOT_DEFINED



//------------------------------------------------------------------------------
//
//  GGroupDocs
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GGroupDocs::GGroupDocs(GFactoryGroupDocs* fac)
	: GPlugin<GFactoryGroupDocs>(fac), Docs(100,50)
{
}


//-----------------------------------------------------------------------------
void GGroupDocs::Grouping(GSlot*,bool save)
{
	// How to compute the groups
	GTopicCalc* CalcDesc=GALILEIApp->GetManager<GTopicCalcManager>("TopicCalc")->GetCurrentMethod();
	Docs.Clear();
	RCursor<GDoc> cur(Session->GetDocs());
	for(cur.Start();!cur.End();cur.Next())
	{
		#ifdef GROUP_DOCS_NOT_DEFINED
		if(cur()->IsDefined()||cur()->GetNbJudgedDocs())
			Docs.InsertPtr(cur());
		#else
		if(cur()->IsDefined())
			Docs.InsertPtr(cur());
		#endif
	}

	// Make the grouping
	Run();

	// Compute the description of the topics and save the information.
	if(CalcDesc)
	{
		R::RCursor<GTopic> Groups(Session->GetTopics());
		for(Groups.Start();!Groups.End();Groups.Next())
			CalcDesc->Compute(Groups());
	}

	// Save if necessary
	if(save)
	{
		Session->GetStorage()->SaveTopics();
		R::RCursor<GTopic> Groups(Session->GetTopics());
		for(Groups.Start();!Groups.End();Groups.Next())
			Groups()->SetState(osSaved);
	}
}


//------------------------------------------------------------------------------
GGroupDocs::~GGroupDocs(void)
{
}



//------------------------------------------------------------------------------
//
// class GGroupDocsManager
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GGroupDocsManager::GGroupDocsManager(void)
	: GPluginManager<GGroupDocsManager,GFactoryGroupDocs,GGroupDocs>("GroupDocs",API_GROUPDOCS_VERSION,ptSelect)
{
}


//------------------------------------------------------------------------------
GGroupDocsManager::~GGroupDocsManager(void)
{
}
