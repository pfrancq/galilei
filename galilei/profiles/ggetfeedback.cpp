/*

	GALILEI Research Project

	GGetFeedback.cpp

	Auto create feedback for users.

	Copyright 2002 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).

	Version $Revision$

	Last Modify: $Date$

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



//-----------------------------------------------------------------------------
// inluce files for R-Project
#include <rio/rtextfile.h>
using namespace RIO;


//-----------------------------------------------------------------------------
//include specific files
#include <iostream.h>
#include <stdlib.h>


//-----------------------------------------------------------------------------
// includes files for GALILEI
#include <langs/glang.h>
#include <langs/gdict.h>
#include <langs/gdicts.h>
#include <infos/giwordlist.h>
#include <profiles/ggetfeedback.h>
#include <profiles/guser.h>
#include <profiles/gprofile.h>
#include <profiles/gsubprofilevector.h>
#include <profiles/gsubprofile.h>
#include <profiles/gprofdoc.h>
#include <docs/gdoc.h>
#include <docs/gdocs.h>
#include <groups/ggroup.h>
#include <groups/ggroups.h>
#include <sessions/gsession.h>
using namespace GALILEI;



//-----------------------------------------------------------------------------
//
// class GGetFeedback
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GALILEI::GGetFeedback::GGetFeedback(GSession* session)
	: Session(session), NbDocs(30), Global(true), PercErr(0), Docs(NbDocs)
{
	RTimeDate::RDate date;

	sprintf(today,"%u-%u-%u",date.GetYear(),date.GetMonth(),date.GetDay());
}


//-----------------------------------------------------------------------------
void GALILEI::GGetFeedback::Run(bool Save)
{
	GGroupsCursor Grps;
	GSubProfileCursor SubProfile;
	GGroupCursor Grp;
	unsigned int i;

	// Go through the languages
	Grps=Session->GetGroupsCursor();
	for(Grps.Start();!Grps.End();Grps.Next())
	{
		// Go through the groups of the current language.
		Grp=Grps()->GetGroupCursor();
		for(Grp.Start();!Grp.End();Grp.Next())
		{
			// Go through the subprofile contained in the group.
			SubProfile=Grp()->GetSubProfileCursor();
			for(SubProfile.Start();!SubProfile.End();SubProfile.Next())
			{
				Grp()->NotJudgedDocsRelList(&Docs,SubProfile(),Global);
				for(Docs.Start(),i=NbDocs+1;(!Docs.End())&&(--i);Docs.Next())
				{
					// Look if 'OK'
					if(Docs()->GetDoc()->IsFromSubject(SubProfile()->GetSubject()))
					{
						Session->InsertFdbk(SubProfile()->GetProfile(),Docs()->GetDoc(),GProfDoc::ErrorJudgment(djOK,PercErr,Session->GetRandom()),today);
					}
					else
					{
						// Look If 'KO'
						if(Docs()->GetDoc()->IsFromParentSubject(SubProfile()->GetSubject()))
						{
							Session->InsertFdbk(SubProfile()->GetProfile(),Docs()->GetDoc(),GProfDoc::ErrorJudgment(djKO,PercErr,Session->GetRandom()),today);
						}
						else
						{
							// Must be H
							Session->InsertFdbk(SubProfile()->GetProfile(),Docs()->GetDoc(),GProfDoc::ErrorJudgment(djOutScope,PercErr,Session->GetRandom()),today);
						}
					}
				}
				SubProfile()->GetProfile()->SetState(osModified);
			}
		}
	}

	if(Save)
		Session->SaveFdbks();
}


//-----------------------------------------------------------------------------
void GALILEI::GGetFeedback::SetSettings(const char* s)
{
	char c;
	
	if(!(*s)) return;
	sscanf(s,"%c %u %u",&c,&NbDocs,&PercErr);
	if(c=='1') Global=true; else Global=false;
}


//-----------------------------------------------------------------------------
const char* GALILEI::GGetFeedback::GetSettings(void)
{
	static char tmp[100];
	char c;
	
	if(Global) c='1'; else c='0';
	sprintf(tmp,"%c %u %u",c,NbDocs,PercErr);
	return(tmp);
}


//-----------------------------------------------------------------------------
GALILEI::GGetFeedback::~GGetFeedback()
{
}

