 /*

	GALILEI Research Project

	GSubProfilesLevel.cpp

	Compute Profiles and Documents Levels - Implementation

	Copyright 2005 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).
		David Wartel (dwartel@e-parkos.com)

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
//include files for R
#include <rmysql.h>


//------------------------------------------------------------------------------
//include files for GALILEI
#include <gsession.h>
#include <gprofile.h>
#include <gdoc.h>
#include <ggroup.h>
#include <glang.h>
#include <gstorage.h>
#include <ggalileiapp.h>
#include <gstorage.h>
using namespace GALILEI;
using namespace R;
using namespace std;


//------------------------------------------------------------------------------
//include files for GALILEI PLUGINS
#include <gsubprofileslevel.h>


//------------------------------------------------------------------------------
//
//  Scoring
//
//------------------------------------------------------------------------------

struct Scoring
{
	size_t SubProfileId;
	double Score;
	size_t NbDocs;

	Scoring(size_t id) : SubProfileId(id), Score(0.0), NbDocs(0) {}
	int Compare(const Scoring& scoring) const {return(SubProfileId-scoring.SubProfileId);}
	int Compare(const size_t scoring) const {return(SubProfileId-scoring);}
};



//------------------------------------------------------------------------------
//
//  GSubProfilesLevelCmd
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GSubProfilesLevelCmd::GSubProfilesLevelCmd(void) : GStorageCmd("SubProfilesLevelCMD", "MySQL")
{
}


//------------------------------------------------------------------------------
void GSubProfilesLevelCmd::Run(GStorage* storage,const GStorageTag& inst,void* caller)
{
	RString sql;
	GGroup* grp;
	RDb* storeMySQL;
	unsigned int sublevel; //level of the subprofile
	unsigned int  levelswidth; // width (between 0 and 100) of a level
	double subscore; // score of the subprofile

	grp=static_cast<GGroup*>(caller); //subprofile was given by caller
	storeMySQL=static_cast<RDb*>(storage->GetInfos());
	RContainer<Scoring,true,true> Scores(grp->GetNbProfiles());

	levelswidth=100/atoi(inst.GetAttrValue("NbLevels").Latin1());

	try
	{
		// Init
		RQuery initsubprofiles(storeMySQL,"UPDATE profiles SET level='0',score='0' WHERE groupid="+RString::Number(grp->GetId()));

		// Find all documents assessed as relevant by someone of the group
		sql="SELECT DISTINCT(htmlid) FROM htmlsbyprofiles,profiles WHERE htmlsbyprofiles.profileid=profiles.profileid AND judgement='O' AND profiles.groupid="+RString::Number(grp->GetId());
		RQuery docs(storeMySQL, sql);
		for(docs.Start();!docs.End();docs.Next())
		{
			// Class all profiles assessing the current document by date
			sql="SELECT profiles.profileid FROM profiles,htmlsbyprofiles WHERE profiles.profileid=htmlsbyprofiles.profileid AND profiles.groupid="+RString::Number(grp->GetId())+" AND judgement='O' AND htmlid="+docs[0]+" ORDER BY when2";
			RQuery subprofiles(storeMySQL,sql);
			subprofiles.Start();
			size_t nb=subprofiles.GetNb()-1;
			if(!nb) continue;
			subscore=static_cast<double>(nb)/static_cast<double>(grp->GetNbProfiles()-1);
			Scoring* ptr=Scores.GetInsertPtr(atoi(subprofiles[0]));
			ptr->Score+=subscore;
			ptr->NbDocs++;
		}

		// Goes through each Profile
		RCursor<Scoring> Cur(Scores);
		for(Cur.Start();!Cur.End();Cur.Next())
		{
			// Compute the score,level and store it in the database
			Cur()->Score/=static_cast<double>(Cur()->NbDocs);
			sublevel=(int(100*Cur()->Score)+(levelswidth-1))/levelswidth;
			sql="UPDATE profiles SET level='"+RString::Number(sublevel)+"',score='"+RString::Number(Cur()->Score)+"' WHERE profileid="+RString::Number(Cur()->SubProfileId);
			RQuery update(storeMySQL, sql);
		}
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
GSubProfilesLevelCmd::~GSubProfilesLevelCmd(void)
{
}



//------------------------------------------------------------------------------
//
//  GDocsLevelCmd
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GDocsLevelCmd::GDocsLevelCmd(void) : GStorageCmd("DocsLevelCMD", "MySQL")
{
}


//------------------------------------------------------------------------------
void GDocsLevelCmd::Run(GStorage* storage,const GStorageTag&,void*)
{
	try
	{
		// Compute score for documents
		RDb* storeMySQL=static_cast<RDb*>(storage->GetInfos());;
		RQuery intidocs(storeMySQL,"DELETE FROM htmlsbygroups");
		RQuery insertdocs(storeMySQL,"INSERT INTO htmlsbygroups(groupid,htmlid,score) SELECT groupid,htmlid,SUM(score) FROM subprofiles,htmlsbyprofiles WHERE subprofiles.profileid=htmlsbyprofiles.profileid AND subprofiles.langid=htmlsbyprofiles.langid AND judgement='O' GROUP BY groupid,htmlid");
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
GDocsLevelCmd::~GDocsLevelCmd(void)
{
}



//------------------------------------------------------------------------------
//
//  GSubProfilesLevel
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GSubProfilesLevel::GSubProfilesLevel(GFactoryPostGroup* fac)
		: GPostGroup(fac), Docs(2000,500)
{
}


//------------------------------------------------------------------------------
void GSubProfilesLevel::CreateParams(RConfig* params)
{
	params->InsertParam(new RParamValue("NbLevels",5));
}


//------------------------------------------------------------------------------
void GSubProfilesLevel::ApplyConfig(void)
{
	GPostGroup::ApplyConfig();
	NbLevels=Factory->GetUInt("NbLevels");
}


//------------------------------------------------------------------------------
void GSubProfilesLevel::Connect(GSession* session)
{
	// Try to insert the command, eventually, delete it
	GPostGroup::Connect(session);
	GSubProfilesLevelCmd* command=new GSubProfilesLevelCmd();
	if(!GALILEIApp->GetManager<GStorageManager>("Storage")->InsertCmd(command))
		delete command;
	GDocsLevelCmd* command2=new GDocsLevelCmd();
	if(!GALILEIApp->GetManager<GStorageManager>("Storage")->InsertCmd(command2))
		delete command2;
}


//------------------------------------------------------------------------------
void GSubProfilesLevel::Disconnect(GSession* session)
{
	GPostGroup::Disconnect(session);
}


//------------------------------------------------------------------------------
void GSubProfilesLevel::Run(void)
{
	if (!NbLevels)
		throw GException ("[Compute Subprofiles Level]: Error: number of levels is null !");
	GStorageTag tag("SubProfilesLevelCMD");
	tag.InsertAttr("NbLevels",RString::Number(NbLevels));
	
	RCursor<GGroup> Groups(Session->GetGroups());
	for(Groups.Start();!Groups.End();Groups.Next())
	{
		void* caller=static_cast<void*>(Groups());
		Session->GetStorage()->ExecuteCmd(tag,caller);
	}
	GStorageTag tag2("DocsLevelCMD");
	Session->GetStorage()->ExecuteCmd(tag2,0);
}


//------------------------------------------------------------------------------
GSubProfilesLevel::~GSubProfilesLevel(void)
{
}


//------------------------------------------------------------------------------
CREATE_POSTGROUP_FACTORY("Subprofiles Level Computation",GSubProfilesLevel)
