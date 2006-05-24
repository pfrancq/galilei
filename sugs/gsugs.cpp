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


//------------------------------------------------------------------------------
//include files for GALILEI PLUGINS
#include <gsugs.h>


//------------------------------------------------------------------------------
//include files for R
#include <rmysql.h>


//------------------------------------------------------------------------------
//include files for GALILEI
#include <gsession.h>
#include <gprofile.h>
#include <gsubprofile.h>
#include <gdoc.h>
#include <ggroup.h>
#include <glang.h>
#include <gstorage.h>
#include <ggalileiapp.h>
#include <gmeasure.h>
#include <gstorage.h>
using namespace GALILEI;
using namespace std;




//------------------------------------------------------------------------------
//
//  GComputeSubProfileSugsCmd
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
class GComputeSubProfileSugsCmd : public GStorageCmd
{
public:
	GComputeSubProfileSugsCmd(void) : GStorageCmd("ComputeSubProfileSugsCmd", "MySQL") {}
	virtual void Run(GStorage* storage,const GStorageTag& inst,void* caller);
	~GComputeSubProfileSugsCmd() {}
};


//------------------------------------------------------------------------------
void GComputeSubProfileSugsCmd::Run(GStorage* storage,const GStorageTag& inst,void* caller)
{
	// Get all the parameters
	GProfile* prof=static_cast<GProfile*>(caller);
	RDb* storeMySQL=static_cast<RDb*>(storage->GetInfos());;
	RString Sql;

	try
	{
		Sql="DELETE FROM sugsbyprofiles WHERE profileid="+RString::Number(prof->GetId());
		RQuery initsugs(storeMySQL,Sql);
		Sql="INSERT INTO sugsbyprofiles(profileid,test,htmlid,rank) ";
		Sql+="SELECT "+RString::Number(prof->GetId())+",'From other members',htmlid,htmlsbygroups.score FROM htmlsbygroups,subprofiles,profiles ";
		Sql+="WHERE htmlsbygroups.groupid=subprofiles.groupid and subprofiles.groupid AND subprofiles.profileid=profiles.profileid AND profiles.profileid="+RString::Number(prof->GetId())+" AND htmlid NOT IN ";
		Sql+="(SELECT htmlid FROM htmlsbyprofiles WHERE profileid="+RString::Number(prof->GetId())+") ";
		Sql+="ORDER BY htmlsbygroups.score LIMIT "+inst.GetAttrValue("NbSugs");
		RQuery insertsugs(storeMySQL,Sql);
	}
	catch(RException& e)
	{
		throw GException(e.GetMsg());
	}
	catch(...)
	{
		throw GException("Unknown problem");
	}
}



//------------------------------------------------------------------------------
//
//  GSugs
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GSugs::GSugs(GFactoryPostGroup* fac)
		: GPostGroup(fac), Docs(0)
{
}


//------------------------------------------------------------------------------
void GSugs::CreateParams(RConfig* params)
{
	params->InsertParam(new RParamValue("NbSugs",50));
	params->InsertParam(new RParamValue("UseLevels",false));
	params->InsertParam(new RParamValue("UseGroupsSugs",false));
}


//------------------------------------------------------------------------------
void GSugs::ApplyConfig(void)
{
	GPostGroup::ApplyConfig();
	NbSugs=Factory->GetUInt("NbSugs");
	UseLevels=Factory->GetBool("UseLevels");
	UseGroupsSugs=Factory->GetBool("UseGroupsSugs");
}


//------------------------------------------------------------------------------
void GSugs::Connect(GSession* session)
{
	GPostGroup::Connect(session);
	GComputeSubProfileSugsCmd* command=new GComputeSubProfileSugsCmd();
	if(!GALILEIApp->GetManager<GStorageManager>("Storage")->InsertCmd(command))
		delete command;
}


//------------------------------------------------------------------------------
void GSugs::Disconnect(GSession* session)
{
	GPostGroup::Disconnect(session);
}


//------------------------------------------------------------------------------
void GSugs::Run(void)
{
	if(!UseLevels)
	{
		if(!Docs)
			Docs=new R::RContainer<GFdbk,false,false>(2000,500);

		R::RCursor<GGroup> Grps;
		RCursor<GSubProfile> Sub;
		RCursor<GFdbk> Doc;
		unsigned int i;

		// Clear the table
		RDate Now(RDate::GetToday());
		Session->GetStorage()->CreateSugs(Now);

		// Similarities
		GMeasure* ProfilesDocsSims=GALILEIApp->GetManager<GMeasureManager>("Measures")->GetCurrentMethod("Profiles/Documents Similarities");

		// -1- Store sugestion with description= S+order
		// Go through the groups
		Grps=Session->GetGroups();
		for(Grps.Start();!Grps.End();Grps.Next())
		{
			// Go through the subprofiles
			Sub=Grps()->GetSubProfiles();
			for(Sub.Start();!Sub.End();Sub.Next())
			{
				// Get all relevant documents ordered
				Grps()->NotJudgedDocsRelList(ProfilesDocsSims,Docs,Sub(),Session);

				// Store them in the database
				Doc.Set(*Docs);
				for(Doc.Start(),i=0;!Doc.End();Doc.Next(),i++)
					Session->GetStorage()->AddSugsProfile(Now,Sub()->GetProfile()->GetId(),Doc()->GetDocId(),i);
			}
		}
	}
	else
	{
		GStorageTag tag("ComputeSubProfileSugsCmd");
		tag.InsertAttr("NbSugs",RString::Number(NbSugs));
		RCursor<GProfile> Profiles(Session->GetProfiles());
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
CREATE_POSTGROUP_FACTORY("Suggestions",GSugs)
