 /*

	GALILEI Research Project

	Sugs.cpp

	Create suggestions - Implementation

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
// include specific files
#include <memory>
#include <stdlib.h>
#include <time.h>
#include <math.h>
using namespace std;


//------------------------------------------------------------------------------
//include files for GALILEI PLUGINS
#include <sugs.h>


//------------------------------------------------------------------------------
//include files for R
#include <rdbmysql.h>


//------------------------------------------------------------------------------
//include files for GALILEI
#include <gsession.h>
#include <gprofile.h>
#include <gdoc.h>
#include <gcommunity.h>
#include <glang.h>
#include <gstorage.h>
#include <ggalileiapp.h>
#include <gmeasure.h>
#include <gstorage.h>
#include <gsuggestion.h>
using namespace GALILEI;
using namespace R;
using namespace std;



//------------------------------------------------------------------------------
//
//  Sugs
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Sugs::Sugs(GSession* session,GPlugInFactory* fac)
	: GComputeSugs(session,fac), TheSugs(otProfile,0,30)
{
}


//------------------------------------------------------------------------------
void Sugs::CreateParams(GPlugInFactory* fac)
{
	fac->InsertParam(new RParamValue("NbSugs",50));
	fac->InsertParam(new RParamValue("UseLevels",false));
}


//------------------------------------------------------------------------------
void Sugs::ApplyConfig(void)
{
	GComputeSugs::ApplyConfig();
	NbSugs=Factory->FindParam<RParamValue>("NbSugs")->GetUInt();
	UseLevels=Factory->FindParam<RParamValue>("UseLevels")->GetBool();
}


//------------------------------------------------------------------------------
void Sugs::Run(void)
{
	RString Info("GSugs plug-in");
	GMeasure* Sim(GALILEIApp->GetCurrentPlugIn<GMeasure>("Measures","Documents/Profiles Similarities"));
	GCommunityDocs Docs;

	// Go trough the communities
	RCursor<GALILEI::GCommunity> Grps(Session->GetCommunities());
	for(Grps.Start();!Grps.End();Grps.Next())
	{
		// Prepare the relevant documents
		if(UseLevels)
		{
			// Load the relevant documents for the current community and order it
			Docs.SetCommunityId(Grps()->GetId());
			Session->GetStorage()->LoadDocs(Docs);
			Docs.ReOrder(GDocRanking::SortOrderRanking);
		}
		else
		{
			// Get the list of relevant documents of the community
			Grps()->GetRelevantDocs(Docs);
		}

		// Go through each profile
		RCursor<GALILEI::GProfile> Profiles(Grps()->GetObjs());
		for(Profiles.Start();!Profiles.End();Profiles.Next())
		{
			// Prepare the suggestions
			TheSugs.Clear();
			TheSugs.SetAddresseeId(otProfile,Profiles()->GetId());

			// If the levels are not used -> the similarity is used to rank the documents
			if(!UseLevels)
			{
				// Go through each document to compute the similarity
				RCursor<GDocRanking> Doc(Docs);
				for(Doc.Start();!Doc.End();Doc.Next())
				{
					double res;
					Sim->Measure(0,Doc()->GetDocId(),Profiles()->GetId(),&res);
					Doc()->SetRanking(res);
				}

				// Order the ranking
				Docs.ReOrder(GDocRanking::SortOrderRanking);
			}

			// Take the first NbSugs documents not assessed
			size_t Nb(0);
			RCursor<GDocRanking> Doc(Docs);
			for(Doc.Start();(!Doc.End())&&(Nb<NbSugs);Doc.Next())
			{
				if(Profiles()->GetFdbk(Doc()->GetDocId())) continue;
				TheSugs.InsertPtr(new GSuggestion(Doc()->GetDocId(),Doc()->GetRanking(),RDate::GetToday(),Info));
				Nb++;
			}

			if(Session->MustSaveResults())
				Session->GetStorage()->SaveSugs(TheSugs,Nb);
		}
	}

}


//------------------------------------------------------------------------------
CREATE_COMPUTESUGS_FACTORY("Compute Suggestions","Compute Suggestions",Sugs)
