 /*

	GALILEI Research Project

	GSugs.cpp

	Create suggestions  - Implementation

	(C) 2001 by Pascal Franq, Vandaele Valery.

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
//include files for GALILEI
#include <sessions/gsession.h>
#include <profiles/gprofileproxy.h>
#include <profiles/gsubprofile.h>
#include <docs/gdocproxy.h>
#include <groups/ggroups.h>
#include <groups/ggroup.h>
#include <infos/glangproxy.h>
#include <sessions/gstorage.h>
using namespace GALILEI;
using namespace std;



//------------------------------------------------------------------------------
//
//  GSugs
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GSugs::GSugs(GFactoryPostGroup* fac) throw(bad_alloc)
		: GPostGroup(fac), Docs(2000,500)
{
}


//------------------------------------------------------------------------------
void GSugs::CreateParams(GParams* params)
{
	//level param
	params->InsertPtr(new GParamUInt("Level",0));
}


//------------------------------------------------------------------------------
void GSugs::ApplyConfig(void)
{
}


//------------------------------------------------------------------------------
void GSugs::Connect(GSession* session) throw(GException)
{
	GPostGroup::Connect(session);
	Session->GetStorage()->CreateDummy("sugs");
}


//------------------------------------------------------------------------------
void GSugs::Disconnect(GSession* session) throw(GException)
{
	GPostGroup::Disconnect(session);
}


//------------------------------------------------------------------------------
void GSugs::Run(void) throw(GException)
{
	GGroupCursor Grps;
	RCursor<GSubProfile> Sub;
	RCursor<GFdbk> Doc;
	unsigned int i;
	char tmp[10];

	// Clear the table
	Session->GetStorage()->ClearDummy("sugs");

	// -1- Store sugestion with description= S+order
	// Go through the groups
	Grps=Session->GetGroupsCursor();
	for(Grps.Start();!Grps.End();Grps.Next())
	{
		// Go through the subprofiles
		Sub=Grps()->GetSubProfilesCursor();
		for(Sub.Start();!Sub.End();Sub.Next())
		{
			// Get all relevant documents ordered
			Grps()->NotJudgedDocsRelList(&Docs,Sub(),Session);

			// Store them in the database
			Doc.Set(Docs);
			for(Doc.Start(),i=0;!Doc.End();Doc.Next(),i++)
			{
				switch(Doc()->GetFdbk())
				{
					case (djOK | djHub):
						sprintf(tmp,"H%u",i);
						break;

					case (djOK | djAutority):
						sprintf(tmp,"A%u",i);
						break;

					default :
						sprintf(tmp,"S%u",i);
						break;
				}
				Session->GetStorage()->AddDummyEntry("sugs",Sub()->GetId(),tmp,Doc()->GetDoc()->GetId());
			}
		}
	}

}


//------------------------------------------------------------------------------
GSugs::~GSugs(void)
{
}


//------------------------------------------------------------------------------
CREATE_POSTGROUP_FACTORY("Suggestions",GSugs)
