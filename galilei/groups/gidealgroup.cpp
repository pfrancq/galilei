/*

	GALILEI Research Project

	GIdealGroup.cpp

	Get the jugments for the profiles

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
using namespace GALILEI;


//-----------------------------------------------------------------------------
//
//  GIdealGroup
//
//-----------------------------------------------------------------------------

GALILEI::GIdealGroup::GIdealGroup(const char* txturl,GUsers* user,GSession* ses)
	:users(user)
{
	PercOK=10;
	PercKO=10;
	subjects=new GSubjectTree(PercOK,PercKO,users->NbPtr);
	//The file where is stoked the differents information about docs,subject,...
	RTextFile* textfile = new RTextFile (txturl, RTextFile::Read);
	char *ptr= textfile->GetLine(),*sub,*url;
	int c1=1,c2=1;
	int samesub=0;
	//Parse the file to find Subject, subSubject, and the url of the different documents
	while ((*ptr)=='T')
	{
		samesub++;
		GSubject* subject=new GSubject(RString(++ptr),c1);
		sub=textfile->GetWord();
		while ((*sub)=='S')
		{
			GSubject* subsubject=new GSubject(RString(++sub),c2);
			subsubject->SetLang(textfile->GetLine());
			url=textfile->GetLine();
			while ((*url)=='U')
			{
				char* temp=strchr(url,' ');
				(*temp)=0;
				char* temp2=strchr(++temp,' ');
				(*temp2)=0;
				(++url);
				subsubject->urls->InsertPtr(ses->GetDoc(atoi(temp)));
				url=textfile->GetLine();
			}
			sub=url;
			subject->InsertNode(subsubject);
			c2++ ;
		}
		subjects->InsertPtr(subject);
		ptr=sub;
		c1++;
	}
	subjects->InsertProfiles();
	delete textfile;
}


//-----------------------------------------------------------------------------
void GALILEI::GIdealGroup::CreateJudgement(GSession* ses,RStd::RContainer<GGroupIdParentId,unsigned int,true,true>* &parent,RStd::RContainer<GGroups,unsigned int,true,true>* &groups)
{
	groups=new RContainer<GGroups,unsigned int,true,true>(2,2);
	parent=new RContainer<GGroupIdParentId,unsigned int,true,true>(10,10);

	//Clear the old feedback.
	ses->ClearFdbks();
	//Create the different judgments.
	subjects->Judgments(ses);
	//Create the ideal groupment corresponding to the precedent judgment.
	subjects->IdealGroupment(groups,ses,parent);
}


//-----------------------------------------------------------------------------
const char* GALILEI::GIdealGroup::GetSettings(void)
{
	static char tmp[100];
	
	sprintf(tmp,"%u %u ",PercOK,PercKO);
	return(tmp);
}


//-----------------------------------------------------------------------------
void GALILEI::GIdealGroup::SetSettings(const char* s)
{
	if(!(*s)) return;
	sscanf(s,"%u %u ",&PercOK,&PercKO);
}



//-----------------------------------------------------------------------------
void GALILEI::GIdealGroup::CreateIdealGroupmentFile(char * url)
{
	subjects->IdealGroupmentFile(url);
}

