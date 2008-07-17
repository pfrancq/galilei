 /*

	GALILEI Research Project

	GSugs.cpp

	Create suggestions  - Implementation

	Copyright 2003-2005 by the Université Libre de Bruxelles.

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
// include specific files
#include <stdlib.h>
#include <time.h>
#include <math.h>
using namespace std;


//------------------------------------------------------------------------------
//include files for GALILEI PLUGINS
#include <sugs.h>


//------------------------------------------------------------------------------
//include files for R
#include <rdb.h>


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
using namespace std;



//------------------------------------------------------------------------------
//
//  GComputeSubProfileSugsCmd
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
class GComputeSubProfileSugsCmd : public GALILEI::GStorageCmd
{
public:
	GComputeSubProfileSugsCmd(void) : GALILEI::GStorageCmd("ComputeSubProfileSugsCmd", "MySQL") {}
	virtual void Run(GALILEI::GStorage* storage,const GALILEI::GStorageTag& inst,void* caller);
	~GComputeSubProfileSugsCmd() {}
};


//------------------------------------------------------------------------------
void GComputeSubProfileSugsCmd::Run(GALILEI::GStorage* storage,const GALILEI::GStorageTag& inst,void* caller)
{
	// Get all the parameters
	GALILEI::GProfile* prof=static_cast<GALILEI::GProfile*>(caller);
	RDb* storeMySQL=static_cast<RDb*>(storage->GetInfos());;
	RString Sql;

	try
	{
		Sql="DELETE FROM sugsbyprofiles WHERE profileid="+RString::Number(prof->GetId());
		auto_ptr<RQuery> initsugs(storeMySQL->Query(Sql));
		Sql="INSERT INTO sugsbyprofiles(profileid,test,htmlid,rank) ";
		Sql+="SELECT "+RString::Number(prof->GetId())+",'From other members',htmlid,htmlsbygroups.score FROM htmlsbygroups,subprofiles,profiles ";
		Sql+="WHERE htmlsbygroups.groupid=profiles.groupid AND profiles.profileid="+RString::Number(prof->GetId())+" AND htmlid NOT IN ";
		Sql+="(SELECT htmlid FROM htmlsbyprofiles WHERE profileid="+RString::Number(prof->GetId())+") ";
		Sql+="ORDER BY htmlsbygroups.score LIMIT "+inst.GetAttrValue("NbSugs");
		auto_ptr<RQuery> insertsugs(storeMySQL->Query(Sql));
	}
	catch(RException& e)
	{
		throw GALILEI::GException(e.GetMsg());
	}
	catch(...)
	{
		throw GALILEI::GException("Unknown problem");
	}
}



//------------------------------------------------------------------------------
//
//  GSugs
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GSugs::GSugs(GALILEI::GFactoryPostCommunity* fac)
		: GALILEI::GPostCommunity(fac), Docs(0)
{
}


//------------------------------------------------------------------------------
void GSugs::CreateParams(RConfig* params)
{
	params->InsertParam(new RParamValue("NbSugs",50));
	params->InsertParam(new RParamValue("UseLevels",false));
}


//------------------------------------------------------------------------------
void GSugs::ApplyConfig(void)
{
	GPostCommunity::ApplyConfig();
	NbSugs=Factory->GetUInt("NbSugs");
	UseLevels=Factory->GetBool("UseLevels");
}


//------------------------------------------------------------------------------
void GSugs::Connect(GALILEI::GSession* session)
{
	GALILEI::GPostCommunity::Connect(session);
	GComputeSubProfileSugsCmd* command=new GComputeSubProfileSugsCmd();
	if(!GALILEI::GALILEIApp->GetManager<GALILEI::GStorageManager>("Storage")->InsertCmd(command))
		delete command;
}


//------------------------------------------------------------------------------
void GSugs::Disconnect(GALILEI::GSession* session)
{
	GALILEI::GPostCommunity::Disconnect(session);
}


//------------------------------------------------------------------------------
void GSugs::Run(void)
{
	if(!UseLevels)
	{
		if(!Docs)
			Docs=new R::RContainer<GALILEI::GFdbk,false,false>(2000,500);

		R::RCursor<GALILEI::GCommunity> Grps;
		RCursor<GALILEI::GProfile> Sub;
		RCursor<GALILEI::GFdbk> Doc;
		size_t i;

		// Clear the table
		RDate Now(RDate::GetToday());
		Session->GetStorage()->CreateSugs(Now);

		// Similarities
		GALILEI::GMeasure* ProfilesDocsSims=GALILEI::GALILEIApp->GetManager<GALILEI::GMeasureManager>("Measures")->GetCurrentMethod("Documents/Profiles Similarities");

		// -1- Store sugestion with description= S+order
		// Go through the groups
		Grps=Session->GetCommunities();
		for(Grps.Start();!Grps.End();Grps.Next())
		{
			// Go through the subprofiles
			Sub=Grps()->GetObjs();
			for(Sub.Start();!Sub.End();Sub.Next())
			{
				// Get all relevant documents ordered
				Grps()->NotJudgedDocsRelList(ProfilesDocsSims,*Docs,Sub(),Session);

				// Store them in the database
				Doc.Set(*Docs);
				for(Doc.Start(),i=0;!Doc.End();Doc.Next(),i++)
					Session->GetStorage()->AddSugsProfile(Now,Sub()->GetId(),Doc()->GetDocId(),i);
			}
		}
	}
	else
	{
		GALILEI::GStorageTag tag("ComputeSubProfileSugsCmd");
		tag.InsertAttr("NbSugs",RString::Number(NbSugs));
		RCursor<GALILEI::GProfile> Profiles(Session->GetProfiles());
		for(Profiles.Start();!Profiles.End();Profiles.Next())
		{
			void* caller=static_cast<void*>(Profiles());
			Session->GetStorage()->ExecuteCmd(tag,caller);
		}
	}
}


//------------------------------------------------------------------------------
GSugs::~GSugs(void)
{
	delete Docs;
}



//------------------------------------------------------------------------------
CREATE_POSTCOMMUNITY_FACTORY("Suggestions",GSugs)
