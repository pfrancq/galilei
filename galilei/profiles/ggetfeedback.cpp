/*

	GALILEI Research Project

	GGetFeedback.cpp

	Auto create feedback for users.

	Copyright 2002 by the Université Libre de Bruxelles.

	Authors:
		Julien Lamoral (jlamoral@ulb.ac.be).

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
	: Session(session), NbDoc(30), Global(true), PercErr(0),IdealGroup(0), Parent(0)
{
	IdealDoc = new RContainer<GGroupsEvaluate,unsigned int,false,false> (2,2);
	Session->LoadIdealDocument(IdealDoc);
}


//-----------------------------------------------------------------------------
void GALILEI::GGetFeedback::Run(RStd::RContainer<GGroupIdParentId,unsigned int,true,true>* parent,RStd::RContainer<GGroups,unsigned int,true,true>* idealgroup,bool Save)
{
	//Compare each profiles whith all profiles in the same computed group an juge the Nbdocuments most relevant for the user
	RStd::RContainer<GProfDoc,unsigned,false,false>* docs=new RStd::RContainer<GProfDoc,unsigned,false,false>(100,50);
	unsigned int prof1;
	GGroupsCursor Cur;
	GSubProfileCursor Prof1;
	GGroupCursor Grp;

	// Init Part
	IdealGroup=idealgroup;
	Parent=parent;

	// Go through the groups
	Cur=Session->GetGroupsCursor();
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
					if(i<NbDoc)
					{
						CreateNewFeedback(JudgType(Prof1(),(*docs)()->GetDoc()),Prof1(),(*docs)()->GetDoc());
					}
					else
						break;
					i++;
				}
				Prof1()->GetProfile()->SetState(osModified);
			}
		}
	}
	
	delete docs;
	if(Save)
		Session->SaveFdbks();
}


//-----------------------------------------------------------------------------
unsigned int GALILEI::GGetFeedback::JudgType(GSubProfile* SubProf,GDoc* Doc)
{
	unsigned int GrpDocId;
	unsigned int GrpProfId;
	unsigned int tempres=3;
	unsigned int nbgrp=0;
	unsigned int compt=0;

	for(IdealGroup->Start();!IdealGroup->End();IdealGroup->Next())
	{
		GGroupCursor Grp=(*IdealGroup)()->GetGroupCursor();
		for(Grp.Start();!Grp.End();Grp.Next())
		{
			nbgrp++;
		}
	}
	
	unsigned int* tab;
	tab=new unsigned int [nbgrp];
	for (unsigned int i=0; i<nbgrp;i++)
		tab[i]=0;

	// Find The id of the group where the document is.
	for(IdealDoc->Start();! IdealDoc->End();  IdealDoc->Next())
	{
		GGroupEvaluateCursor Grp=(*IdealDoc)()->GetGroupEvaluateCursor();
		for(Grp.Start();!Grp.End();Grp.Next())
		{
			if(Grp()->IsIn(Doc->GetId()))
			{
				GrpDocId=Grp()->GetId();
				tab[compt]=GrpDocId;
				compt++;
			}
		}
	}

	// Find the id of the group  where the subprofile is.
	for(IdealGroup->Start();!IdealGroup->End();IdealGroup->Next())
	{
		GGroupCursor Grp=(*IdealGroup)()->GetGroupCursor();
		for(Grp.Start();!Grp.End();Grp.Next())
		{
			GSubProfileCursor Prof=Grp()->GetSubProfileCursor();
			for(Prof.Start();!Prof.End();Prof.Next())
			{
				if (SubProf->GetId()==Prof()->GetId())
				{
					GrpProfId=Grp()->GetId();
				}
			}
		}
	}

	if(GrpProfId!=0)
	{
		// Compare his two id.
		// Same grp.  
		for(unsigned int i=0;i<nbgrp;i++)
		{
			if(tab[i]!=0)
			{
				if(tab[i]==GrpProfId) tempres=1;
			}
			else continue;
		}

		// Same parent.
		if(tempres==3)
		{
			unsigned int b=Parent->GetPtr(GrpProfId)->ParentId;
			for(unsigned  int i=0;i<nbgrp;i++)
			{
				if(tab[i]!=0)
				{
					unsigned int a=Parent->GetPtr(tab[i])->ParentId;
					if(a==b) tempres=2;
				}
				else continue;
			}
		
		}
	}

	// If there is Random change the judgment.
	if(Session->GetCurrentRandomValue(100)<int(PercErr))
	{
		tempres=int(Session->GetCurrentRandomValue(3)+1);
	}

	delete [] tab;
	return(tempres);
	
}


//-----------------------------------------------------------------------------
void GALILEI::GGetFeedback::CreateNewFeedback(unsigned int fdbk,GSubProfile* sub,GDoc* doc)
{
	//Auto create Judgement for prof11 on a % of ok, ko or hs documents from prof22.
	char today[12];
	RTimeDate::RDate date;

	sprintf(today,"'%u-%u-%u'",date.GetYear(),date.GetMonth(),date.GetDay());
	switch(fdbk)
	{
		case 1:
			Session->InsertFdbk(sub->GetProfile(),doc,djOK,today);
			break;

		case 2:
			Session->InsertFdbk(sub->GetProfile(),doc,djKO,today);
			break;

		case 3:
			Session->InsertFdbk(sub->GetProfile(),doc,djOutScope,today);
			break;
	}
}


//-----------------------------------------------------------------------------
void GALILEI::GGetFeedback::SetSettings(const char* s)
{
	char c;
	
	if(!(*s)) return;
	sscanf(s,"%c %u %u",&c,&NbDoc,&PercErr);
	if(c=='1') Global=true; else Global=false;
}


//-----------------------------------------------------------------------------
const char* GALILEI::GGetFeedback::GetSettings(void)
{
	static char tmp[100];
	char c;
	
	if(Global) c='1'; else c='0';
	sprintf(tmp,"%c %u %u",c,NbDoc,PercErr);
	return(tmp);
}

//-----------------------------------------------------------------------------
GALILEI::GGetFeedback::~GGetFeedback()
{
	delete IdealDoc;
}

