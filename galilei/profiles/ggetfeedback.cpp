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

GALILEI::GGetFeedback::GGetFeedback(GSession* ses,RStd::RContainer<GGroups,unsigned int,true,true>* idealgroup,RStd::RContainer<GGroupIdParentId,unsigned int,true,true>* parent,int nbok,int nbko,int nbhs)
      :NbOK(nbok),NbKO(nbko),NbHS(nbhs)
{
	//Compare each profiles whith all profiles in the same computed group an juge a % of ok, ko or hs document from
	//the orther profiles.
	unsigned int prof1,prof2;
	GGroupsCursor Cur;
	GSubProfileCursor Prof1;
	GSubProfileCursor Prof2;

	GGroupCursor Grp;
	
	Cur=ses->GetGroupsCursor();

	for(Cur.Start();!Cur.End();Cur.Next())
	{
		Grp=Cur()->GetGroupCursor();

		for(Grp.Start();!Grp.End();Grp.Next())
		{
			Prof1=Grp()->GetSubProfileCursor();
			Prof2=Grp()->GetSubProfileCursor();
			for(Prof1.Start();!Prof1.End();Prof1.Next())
			{
				prof1=Prof1()->GetId();
				for(Prof2.Start();!Prof2.End();Prof2.Next())
				{
					prof2=Prof2()->GetId();
					if(prof1!=prof2)
					{
						CreateNewFeedback(AreInSameGroup(prof1,prof2,idealgroup,parent),prof1,prof2,ses);
					}
				}
			}
		}
	}
}


//-----------------------------------------------------------------------------
int GALILEI::GGetFeedback::AreInSameGroup(int prof11,int prof22,RStd::RContainer<GGroups,unsigned int,true,true>* idealgroup,RStd::RContainer<GGroupIdParentId,unsigned int,true,true>* parent)
{
	int prof1,prof2;
	GSubProfileCursor Prof1;
	GSubProfileCursor Prof2;
	int subgrpid1,subgrpid2;

	GGroupCursor Grp;
	
    //Verify if 2 profiles are in the same ideal group.
	for(idealgroup->Start();!idealgroup->End();idealgroup->Next())
	{
		Grp=(*idealgroup)()->GetGroupCursor();

		for(Grp.Start();!Grp.End();Grp.Next())
		{
			Prof1=Grp()->GetSubProfileCursor();
			Prof2=Grp()->GetSubProfileCursor();
			for(Prof1.Start();!Prof1.End();Prof1.Next())
			{
				prof1=Prof1()->GetId();
				if (prof11==prof1)
				{
					subgrpid1=parent->GetPtr(Grp()->GetId())->ParentId;
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
	for(idealgroup->Start();!idealgroup->End();idealgroup->Next())
	{
		Grp=(*idealgroup)()->GetGroupCursor();

		for(Grp.Start();!Grp.End();Grp.Next())
		{
			Prof1=Grp()->GetSubProfileCursor();
			for(Prof1.Start();!Prof1.End();Prof1.Next())
			{
				prof1=Prof1()->GetId();
				if (prof22==prof1)
				{
					subgrpid2=parent->GetPtr(Grp()->GetId())->ParentId;
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

void GALILEI::GGetFeedback::CreateNewFeedback(int fdbk,int prof11,int prof22,GSession* ses)
{
	//Auto create Judgement for prof11 on a % of ok, ko or hs documents from prof22.
	GProfile* prof;
	char today[12];
	RTimeDate::RDate date;
	int NbDocs=0;
	prof=ses->GetSubProfile(prof22)->GetProfile();
	GProfDocCursor Cur;
	Cur=prof->GetProfDocCursor();

	NbDocs=Cur.GetNb();
	int nbdocok,nbdocko,nbdochs;
	nbdocok=int((NbDocs*NbOK)/100);
	nbdocko=int((NbDocs*NbKO)/100);
	nbdochs=int((NbDocs*NbHS)/100);
	sprintf(today,"'%u-%u-%u'",date.GetYear(),date.GetMonth(),date.GetDay());

	if (fdbk==1)
	{
		for(int i=1;i<=nbdocok;i++)
		{
			//create feadback ok
			int stop=0;
			int nb=int((rand()%(NbDocs-1))+1);
			for(Cur.Start();!Cur.End();Cur.Next())
			{
				stop++;
				if (stop==nb)
				{
					if(ses->GetSubProfile(prof11)->GetProfile()->GetFeedback(Cur()->GetDoc())==0&&ses->GetSubProfile(prof22)->GetProfile()->GetFeedback(Cur()->GetDoc())->GetFdbk()==djOK)
					{
						ses->InsertFdbk(ses->GetSubProfile(prof11)->GetProfile(),Cur()->GetDoc(),djOK,today);
					}
				}
			}
		}
	}

	else if(fdbk==2)
	{
		for(int i=1;i<=nbdocko;i++)
		{
			//create feadback ko
			int stop=0;
			int nb=int((rand()%(NbDocs-1))+1);
			for(Cur.Start();!Cur.End();Cur.Next())
			{
				stop++;
				if (stop==nb)
				{
					if(ses->GetSubProfile(prof11)->GetProfile()->GetFeedback(Cur()->GetDoc())==0&&ses->GetSubProfile(prof22)->GetProfile()->GetFeedback(Cur()->GetDoc())->GetFdbk()==djOK)
						{
							ses->InsertFdbk(ses->GetSubProfile(prof11)->GetProfile(),Cur()->GetDoc(),djKO,today);
						}
				}
			}
		}
	}

	else if(fdbk==3)
	{
		for(int i=1;i<=nbdochs;i++)
		{
			//create feadback hs
			int stop=0;
			int nb=int((rand()%(NbDocs-1))+1);
			GProfDocCursor Cur2;
			Cur2=prof->GetProfDocCursor();
			for(Cur.Start();!Cur.End();Cur.Next())
			{
				stop++;
				if (stop==nb)
				{
					if(ses->GetSubProfile(prof11)->GetProfile()->GetFeedback(Cur()->GetDoc())==0&&ses->GetSubProfile(prof22)->GetProfile()->GetFeedback(Cur()->GetDoc())->GetFdbk()==djOK)
						ses->InsertFdbk(ses->GetSubProfile(prof11)->GetProfile(),Cur()->GetDoc(),djOutScope,today);
				}
			}
		}
	}
}
