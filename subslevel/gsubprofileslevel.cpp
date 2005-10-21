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
#include <gsubprofile.h>
#include <gdoc.h>
#include <ggroup.h>
#include <glang.h>
#include <gstorage.h>
#include <gpluginmanagers.h>
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
	RContainer<Scoring,true,true> Scores(grp->GetNbSubProfiles());

	levelswidth=100/atoi(inst.GetAttrValue("NbLevels").Latin1());

	try
	{
		// Init
		RQuery initsubprofiles(storeMySQL,"UPDATE subprofiles SET level=0,score=0 WHERE groupid="+RString::Number(grp->GetId()));

		// Find all documents assessed as relevant by someone of the group
		sql="SELECT DISTINCT(htmlid) FROM htmlsbyprofiles,subprofiles WHERE htmlsbyprofiles.profileid=subprofiles.profileid AND subprofiles.langid=htmlsbyprofiles.langid AND judgement='O' AND subprofiles.groupid="+RString::Number(grp->GetId());
		RQuery docs(storeMySQL, sql);
		for(docs.Start();!docs.End();docs.Next())
		{
			// Class all subprofiles assessing the current document by date
			sql="SELECT subprofiles.subprofileid FROM subprofiles,htmlsbyprofiles WHERE subprofiles.langid=htmlsbyprofiles.langid AND subprofiles.profileid=htmlsbyprofiles.profileid AND subprofiles.groupid="+RString::Number(grp->GetId())+" AND judgement='O' AND htmlid="+docs[0]+" ORDER BY when2";
			RQuery subprofiles(storeMySQL,sql);
			subprofiles.Start();
			size_t nb=subprofiles.GetNb()-1;
			if(!nb) continue;
			subscore=static_cast<double>(nb)/static_cast<double>(grp->GetNbSubProfiles()-1);
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
			sql="UPDATE subprofiles SET level="+RString::Number(sublevel)+",score="+RString::Number(Cur()->Score)+" WHERE subprofileid="+RString::Number(Cur()->SubProfileId);
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
GSubProfilesLevel::GSubProfilesLevel(GFactoryPostGroup* fac) throw(bad_alloc,GException)
		: GPostGroup(fac), Docs(2000,500)
{
}


//------------------------------------------------------------------------------
void GSubProfilesLevel::CreateParams(GParams* params)
{
	params->InsertPtr(new GParamUInt("NbLevels",5));
}


//------------------------------------------------------------------------------
void GSubProfilesLevel::ApplyConfig(void)
{
	GPostGroup::ApplyConfig();
	NbLevels=Factory->GetUInt("NbLevels");
}


//------------------------------------------------------------------------------
void GSubProfilesLevel::Connect(GSession* session) throw(GException)
{
	// Try to insert the command, eventually, delete it
	GPostGroup::Connect(session);
	GSubProfilesLevelCmd* command=new GSubProfilesLevelCmd();
	if(!GPluginManagers::GetManager<GStorageManager>("Storage")->InsertCmd(command))
		delete command;
	GDocsLevelCmd* command2=new GDocsLevelCmd();
	if(!GPluginManagers::GetManager<GStorageManager>("Storage")->InsertCmd(command2))
		delete command2;
}


//------------------------------------------------------------------------------
void GSubProfilesLevel::Disconnect(GSession* session) throw(GException)
{
	GPostGroup::Disconnect(session);
}


//------------------------------------------------------------------------------
void GSubProfilesLevel::Run(void) throw(GException)
{
	if (!NbLevels)
		throw GException ("[Compute Subprofiles Level]: Error: number of levels is null !");
	GStorageTag tag(0,"SubProfilesLevel CMD");
	tag.InsertAttr("NbLevels",RString::Number(NbLevels));
	RCursor<GGroup> Groups(Session->GetGroups());
	for(Groups.Start();!Groups.End();Groups.Next())
	{
		void* caller=static_cast<void*>(Groups());
		Session->GetStorage()->ExecuteCmd(tag,caller);
	}
	GStorageTag tag2(0,"DocsLevel CMD");
	Session->GetStorage()->ExecuteCmd(tag2,0);
}


//------------------------------------------------------------------------------
GSubProfilesLevel::~GSubProfilesLevel(void)
{
}


//------------------------------------------------------------------------------
CREATE_POSTGROUP_FACTORY("Subprofiles Level Computation",GSubProfilesLevel)
