/*

	GALILEI Research Project

	GGetFeedback.cpp

	Auto create feedback for users.

	(C) 2002 by Lamoral Julien

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
GALILEI::GGetFeedback::GGetFeedback(RStd::RContainer<GGroups,unsigned int,true,true>* idealgroup,RStd::RContainer<GGroupIdParentId,unsigned int,true,true>* parent)
	:IdealGroup(idealgroup),Parent(parent)
{
	NbDoc=30;
	Global=true;
}


//-----------------------------------------------------------------------------
void GALILEI::GGetFeedback::Run(GSession* ses)
{
	//Compare each profiles whith all profiles in the same computed group an juge the Nbdocuments most relevant for the user
	RStd::RContainer<GProfDoc,unsigned,false,false>* docs=new RStd::RContainer<GProfDoc,unsigned,false,false>(100,50);
	unsigned int prof1,prof2;
	GGroupsCursor Cur;
	GSubProfileCursor Prof1;
	GGroupCursor Grp;

	Cur=ses->GetGroupsCursor();

	for(Cur.Start();!Cur.End();Cur.Next())
	{
		Grp=Cur()->GetGroupCursor();
		for(Grp.Start();!Grp.End();Grp.Next())
		{
			Prof1=Grp()->GetSubProfileCursor();
			
			for(Prof1.Start();!Prof1.End();Prof1.Next())
			{
				Grp()->NotJudgedDocsRelList(docs, Prof1(),Global);
				unsigned int i=0;
				prof1=Prof1()->GetProfile()->GetSubProfile(Grp()->GetLang())->GetId();
				for(docs->Start();!docs->End();docs->Next())
				{
					prof2=(*docs)()->GetProfile()->GetSubProfile((*docs)()->GetDoc()->GetLang())->GetId();
					if(i<NbDoc)
					{
						CreateNewFeedback(AreInSameGroup(prof1,prof2),Prof1(),ses,(*docs)()->GetDoc());
					}	
					i++;
				}
			}
		}
	}
	delete docs;
}


//-----------------------------------------------------------------------------
int GALILEI::GGetFeedback::AreInSameGroup(int prof11,int prof22)
{
	int prof1,prof2;
	GSubProfileCursor Prof1;
	GSubProfileCursor Prof2;
	int subgrpid1,subgrpid2;
	subgrpid1=-1;
	subgrpid2=-2;

	GGroupCursor Grp;

	//Verify if 2 profiles are in the same ideal group.
	for(IdealGroup->Start();!IdealGroup->End();IdealGroup->Next())
	{
		Grp=(*IdealGroup)()->GetGroupCursor();

		for(Grp.Start();!Grp.End();Grp.Next())
		{
			Prof1=Grp()->GetSubProfileCursor();
			Prof2=Grp()->GetSubProfileCursor();
			for(Prof1.Start();!Prof1.End();Prof1.Next())
			{
				prof1=Prof1()->GetId();
				if (prof11==prof1)
				{
					subgrpid1=Parent->GetPtr(Grp()->GetId())->ParentId;
					for(Prof2.Start();!Prof2.End();Prof2.Next())
					{
						prof2=Prof2()->GetId();
						if(prof22==prof2) return 1;
					}
				}
			}
		}
	}

	//If there arent in the same group.. verify if there are in the same general ideal group.
	for(IdealGroup->Start();!IdealGroup->End();IdealGroup->Next())
	{
		Grp=(*IdealGroup)()->GetGroupCursor();

		for(Grp.Start();!Grp.End();Grp.Next())
		{
			Prof1=Grp()->GetSubProfileCursor();
			for(Prof1.Start();!Prof1.End();Prof1.Next())
			{
				prof1=Prof1()->GetId();
				if (prof22==prof1)
				{
					subgrpid2=Parent->GetPtr(Grp()->GetId())->ParentId;
				}
			}
		}
	}

	if (subgrpid2==subgrpid1)
	{
		return 2;
	}
		
	// there are not in the same group and havent the same parent.
	return 3;
}


//-----------------------------------------------------------------------------
void GALILEI::GGetFeedback::CreateNewFeedback(int fdbk,GSubProfile* sub,GSession* ses,GDoc* doc)
{
	//Auto create Judgement for prof11 on a % of ok, ko or hs documents from prof22.
	char today[12];
	RTimeDate::RDate date;

	sprintf(today,"'%u-%u-%u'",date.GetYear(),date.GetMonth(),date.GetDay());
	if (fdbk==1)
	{
		ses->InsertFdbk(sub->GetProfile(),doc,djOK,today);
	}

	else if(fdbk==2)
	{
		ses->InsertFdbk(sub->GetProfile(),doc,djKO,today);
	}

	else if(fdbk==3)
	{
		ses->InsertFdbk(sub->GetProfile(),doc,djOutScope,today);
	}
}


//-----------------------------------------------------------------------------
void GALILEI::GGetFeedback::SetSettings(const char* s)
{
	char c;
	
	if(!(*s)) return;
	sscanf(s,"%c %u",&c,&NbDoc);
	if(c=='1') Global=true; else Global=false;
}


//-----------------------------------------------------------------------------
const char* GALILEI::GGetFeedback::GetSettings(void)
{
	static char tmp[100];
	char c;
	
	if(Global) c='1'; else c='0';
	sprintf(tmp,"%c %u",c,NbDoc);
	return(tmp);
}
