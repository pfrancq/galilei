/*

	GALILEI Research Project

	GIdealGroup.cpp

	Get the jugments for the profiles

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
#include <rstd/rcontainer.h>
using namespace RStd;


//-----------------------------------------------------------------------------
//include specific files
#include <iostream.h>
#include <stdlib.h>


//-----------------------------------------------------------------------------
// includes files for GALILEI
#include <langs/glang.h>
#include <infos/giwordlist.h>
#include <profiles/guser.h>
#include <profiles/gprofile.h>
#include <profiles/gsubprofile.h>
#include <profiles/gsubprofilevector.h>
#include <profiles/gprofdoc.h>
#include <docs/gdoc.h>
#include <docs/gdocs.h>
#include <groups/gsubject.h>
#include <groups/gidealgroup.h>
#include <groups/ggroup.h>
#include <groups/ggroups.h>
#include <groups/gsubjecttree.h>
#include <sessions/gsession.h>
#include <profiles/gusers.h>
#include <groups/gsubject.h>
using namespace GALILEI;


//-----------------------------------------------------------------------------
//
//  GIdealGroup
//
//-----------------------------------------------------------------------------

GALILEI::GIdealGroup::GIdealGroup(GSession* session)
	: Session(session)
{
	PercOK=10;
	PercKO=10;
	PercHS=0;
	PercErr=0;
	Subjects=new GSubjectTree(PercOK,PercKO,Session->GetNbUsers());
	Session->LoadSubjectTree(Subjects);
	Subjects->InsertProfiles();
	NbProfMax=10;
	NbProfMin=2;
	PercSocial=100;
}


//-----------------------------------------------------------------------------
void GALILEI::GIdealGroup::CreateJudgement(RStd::RContainer<GGroupIdParentId,unsigned int,true,true>* &parent,RStd::RContainer<GGroups,unsigned int,true,true>* &groups,bool Save)
{
	if(!groups)
		groups=new RContainer<GGroups,unsigned int,true,true>(2,2);
	else
	{
		GGroupsCursor Cur;
		GGroupCursor Cur2;

		if(Save)
		{
			Cur.Set(groups);
			for(Cur.Start();!Cur.End();Cur.Next())
			{
				Cur2.Set(Cur());
				for(Cur2.Start();!Cur2.End();Cur2.Next())
					Session->DeleteGroup(Cur2());
			}
		}
		groups->Clear();
	};

 	if(!parent)
		parent=new RContainer<GGroupIdParentId,unsigned int,true,true>(10,10);
	else
		parent->Clear();

	// Clear the old feedback.
	Session->ClearFdbks();

	// Clear subprofiles assignement
	Session->ClearSubProfilesGroups();

	// Create the different judgments.
	Subjects->Judgments(Session,PercOK,PercKO,PercHS,NbProfMin,NbProfMax,PercSocial,PercErr);

	// Create the ideal groupment corresponding to the precedent judgment.
	Subjects->IdealGroupment(groups,Session,parent);
	if(Save)
	{ 
		Session->SaveFdbks();  
		Session->SaveIdealGroupment(groups);
	}

}


//-----------------------------------------------------------------------------
void GALILEI::GIdealGroup::CreateIdealGroupmentFile(const char* url)
{
	Subjects->IdealGroupmentFile(url);
}


//-----------------------------------------------------------------------------
const char* GALILEI::GIdealGroup::GetSettings(void)
{
	static char tmp[100];

	sprintf(tmp,"%u %u %u %i %u %u %u %u",PercOK,PercKO,PercHS,Rand,NbProfMin,NbProfMax,PercSocial,PercErr);

	return(tmp);
}


//-----------------------------------------------------------------------------
void GALILEI::GIdealGroup::SetSettings(const char* s)
{
	unsigned int a,b;
	if(!(*s)) return;
	sscanf(s,"%u %u %u %i %u %u %u %u",&PercOK,&PercKO,&PercHS,&Rand,&a,&b,&PercSocial,&PercErr);
	if (a<=b) NbProfMin=a;
	if (b<=Session->GetNbUsers()) NbProfMax=b;
	if (b==0) NbProfMax=Session->GetNbUsers();

	if(Rand!=0)
		Session->SetCurrentRandom(Rand);
}
