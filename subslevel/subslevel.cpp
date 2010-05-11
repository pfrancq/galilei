 /*

	GALILEI Research Project

	SubsLevel.cpp

	Compute Profiles and Documents Confidence - Implementation

	Copyright 2005-2010 by Pascal Francq (pascal@francq.info).
	Copyright 2003-2005 by David Wartel.
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
// include specific files
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <memory>
using namespace std;


//------------------------------------------------------------------------------
// include files for R
#include <rdbmysql.h>


//------------------------------------------------------------------------------
// include files for GALILEI
#include <gsession.h>
#include <gprofile.h>
#include <gdoc.h>
#include <gcommunity.h>
#include <glang.h>
#include <gstorage.h>
#include <ggalileiapp.h>
#include <gstorage.h>
#include <gfdbk.h>



//------------------------------------------------------------------------------
// include file for current plug-in
#include <subslevel.h>



//------------------------------------------------------------------------------
//
//  SubsLevel
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
SubsLevel::SubsLevel(GSession* session,GPlugInFactory* fac)
		: GComputeTrust(session,fac), Fdbks(2000,500), Docs(2000,500), Scores(500,200)
{
}


//------------------------------------------------------------------------------
void SubsLevel::CreateConfig(void)
{
	InsertParam(new RParamValue("NbLevels",5));
}


//------------------------------------------------------------------------------
void SubsLevel::ApplyConfig(void)
{
	GComputeTrust::ApplyConfig();
	NbLevels=FindParam<RParamValue>("NbLevels")->GetUInt();
}


//------------------------------------------------------------------------------
int SubsLevel::SortOrderAssessed(const void* a,const void* b)
{
	RDate ad((*((ProfileDoc**)(a)))->When);
	RDate bd((*((ProfileDoc**)(b)))->When);

	if(ad==bd) return(0);
	if(ad>bd)
		return(-1);
	else
		return(1);
}


//------------------------------------------------------------------------------
void SubsLevel::ComputeFdbks(size_t docid,GCommunity* group)
{
	// Fill Fdbks with all the relevant of the profiles of the group
	Fdbks.Clear();
	RCursor<GProfile> Profiles(group->GetObjs());
	for(Profiles.Start();!Profiles.End();Profiles.Next())
	{
		GFdbk* fdbk(Profiles()->GetFdbk(docid));
		if(!fdbk) continue;
		switch(fdbk->GetFdbk())
		{
			case ftRelevant:
				Fdbks.InsertPtr(new ProfileDoc(Profiles()->GetId(),fdbk->GetDone()));
			default:
				break;
		}
	}
}


//------------------------------------------------------------------------------
void SubsLevel::ComputeOrderedFdbks(size_t docid,GCommunity* group)
{
	// Fill Fdbks with all the relevant of the profiles of the group
	ComputeFdbks(docid,group);

	// Ordered the feedbacks by date
	Fdbks.ReOrder(SortOrderAssessed);
}


//------------------------------------------------------------------------------
void SubsLevel::ComputeProfilesConfidence(void)
{
	double LevelWidth(100/NbLevels);

	// Compute the confidence on a community basis
	RCursor<GCommunity> Groups(Session->GetCommunities());
	for(Groups.Start();!Groups.End();Groups.Next())
	{
		// At least two profiles in the community is needed
		if(Groups()->GetNbObjs()<2) continue;

		// Get the list of relevant documents of the community
		Groups()->GetRelevantDocs(Docs);

		// Go through each document
		RCursor<GDocRanking> Doc(Docs);
		for(Doc.Start();!Doc.End();Doc.Next())
		{
			// Get the feedbacks on the document ordered by date
			ComputeOrderedFdbks(Doc()->GetDocId(),Groups());
			if(Fdbks.GetNb()<2) continue;

			ProfileScoring* ptr(Scores.GetInsertPtr(Fdbks[0]->ProfileId));
			ptr->Score+=static_cast<double>(Fdbks.GetNb()-1)/static_cast<double>(Groups()->GetNbObjs());
			ptr->NbDocs++;
		}
	}

	// Compute the global score and level for the profiles (and save them eventually).
	RCursor<GProfile> Profiles(Session->GetProfiles());
	for(Profiles.Start();!Profiles.End();Profiles.Next())
	{
		ProfileScoring* ptr(Scores.GetPtr(Profiles()->GetId()));
		if(ptr)
		{
			double Score(ptr->Score/static_cast<double>(ptr->NbDocs));
			Profiles()->SetConfidence(Score,(Score*100+LevelWidth-1)/LevelWidth);
		}
		else
			Profiles()->SetConfidence(0,0);
		if(Session->MustSaveResults())
			Session->GetStorage()->SaveProfile(Profiles());
	}
}


//------------------------------------------------------------------------------
void SubsLevel::ComputeDocsConfidence(void)
{
	// Compute the confidence on a community basis
	RCursor<GCommunity> Groups(Session->GetCommunities());
	for(Groups.Start();!Groups.End();Groups.Next())
	{
		// Get the list of relevant documents of the community
		Groups()->GetRelevantDocs(Docs);

		// Go through each document
		RCursor<GDocRanking> Doc(Docs);
		for(Doc.Start();!Doc.End();Doc.Next())
		{
			// Sum the score of each profiles
			ComputeFdbks(Doc()->GetDocId(),Groups());
			RCursor<ProfileDoc> Cur(Fdbks);
			for(Cur.Start();!Cur.End();Cur.Next())
			{
				ProfileScoring* ptr(Scores.GetPtr(Cur()->ProfileId));
				if(ptr)
					Doc()->SetRanking(Doc()->GetRanking()+ptr->Score);
			}
		}

		// Save it
		Session->GetStorage()->SaveDocs(Docs);
	}
}


//------------------------------------------------------------------------------
void SubsLevel::Run(void)
{
	if(!NbLevels)
		ThrowGException("Number of levels cannot be null");

	ComputeProfilesConfidence();
	if(Session->MustSaveResults())
		ComputeDocsConfidence();
}


//------------------------------------------------------------------------------
CREATE_COMPUTETRUST_FACTORY("Profiles/Documents Trust Computation","Profiles/Documents Trust Computation",SubsLevel)
