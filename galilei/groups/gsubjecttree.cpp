/*

	GALILEI Research Project

	GSubjectTree.cpp

	GSubjectTree - Implementation.

	(C) 1999-2001 by J. Lamoral & D.Wartel

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
// include files for ANSI C/C++
#include <stdlib.h>
#include <iostream.h>
#include <math.h>


//-----------------------------------------------------------------------------
// include files for R-Project
#include <rtimedate/rdate.h>
#include<rstd/rcontainercursor.h>
#include<rstd/rtree.h>


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <groups/ggroup.h>
#include <groups/ggroups.h>
#include <groups/gsubject.h>
#include <profiles/gprofile.h>
#include <profiles/guser.h>
#include <profiles/gprofdoc.h>
#include <groups/gsubjecttree.h>
#include <groups/gsubject.h>



//-----------------------------------------------------------------------------
//
//class GSubjectTree
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GSubjectTree::GSubjectTree(unsigned int NbOk,unsigned int NbKo,unsigned int nbusers)
	:RTree<GSubject,true,false>(100,50)
{

	NbDocsOk=NbOk;
	NbDocsKo=NbKo;
	NbUsers=nbusers;
	NbProfiles=NbUsers;
	profiles=new RContainer<GProfile,unsigned,false,true>(10,5);
}


//---------------------------------------------------
void GSubjectTree::InsertProfiles(void)
{
	int profid=1, subid=1;
	for (int i=0; i<NbUsers; i++)
	{
		for (this->Start();!this->End(); this->Next())
		{
			GSubject* subject = (*this)();
			for (subject->Start();!subject->End();subject->Next())
			{
				GSubject* sub= (*subject)();
				GUser* usr = new GUser(0,"test","test",1);
				this->profiles->InsertPtr(new GProfile(usr,profid,(RString(subject->GetName())+"/"+sub->GetName()),0,0,1,1));
				subid++;
				subid++;
				profid++;
			}
		}
	}
	
}


//---------------------------------------------------
void GSubjectTree::Judgments(GSession* ses)
{
	InitProfiles();
	InitSubSubjects();

	//Calculation of the total number of subsubjects.
	int nbrsububjects=0;
	for (this->Start();!this->End(); this->Next())
	{
		nbrsububjects=nbrsububjects+(*this)()->NbPtr;
	}
    int* tab;
	tab=new int [nbrsububjects];
	for (int i=0; i<nbrsububjects;i++)
		tab[i]=0;

	for (this->Start(); !this->End(); this->Next())
	{
	 	GSubject* subject=(*this)();
		for (int i=0; i<NbProfiles; i++)
		{

			//Chose a subsubject.
			int soustheme = (rand()%(subject->NbPtr))+subject->SubSubjectMinId();
			GSubject* sub1=subject->GetPtr(soustheme);
			sub1->setIsJudged(true);

			//Create the judgement.
			//First, find the profile who can judge the docs
			int profid=(tab[sub1->GetId()-1])*nbrsububjects+sub1->GetId();
			GProfile* proftemp = profiles->GetPtr(profid);
			proftemp->SetState(osUpdated);
			tab[sub1->GetId()-1]=tab[sub1->GetId()-1]+1;
		
			//Documents OK.
			JudgeDocuments(profid,sub1,1,ses);

			//Documents KO.
			if (subject->NbPtr>1) //If the subsuject enables the KO judgement of documents.
			{
				int soustheme = (rand()%(subject->NbPtr))+subject->SubSubjectMinId();
				GSubject* sub2=subject->GetPtr(soustheme);
				while(sub2==sub1)
				{
					int soustheme = (rand()%(subject->NbPtr))+subject->SubSubjectMinId();
					sub2=subject->GetPtr(soustheme);
				}
				JudgeDocuments(profid,sub2,0,ses);
			}
		}
	}
	delete [] tab;
}
  

//-----------------------------------------------------------------------------
void GSubjectTree::JudgeDocuments(int profileid,GSubject* sub,bool i,GSession* ses )
{
	char today[12];
	RTimeDate::RDate date;
	int NbDocs;
	char judgement;

	//If i juge ok.
	if (i)
	{
		NbDocs=int(((double(NbDocsOk*sub->urls->NbPtr)/100))+0.5);
		judgement='O';
	}

	//If i=0 juge KO.
	else
	{
		NbDocs=int(((double(NbDocsKo*sub->urls->NbPtr)/100))+0.5);
		judgement='K';
	}

	RContainer<GDoc,unsigned,false,false>* judgedDocs=new RContainer<GDoc,unsigned,false,false>(10,5);
	int j=0;
	sprintf(today,"'%u-%u-%u'",date.GetYear(),date.GetMonth(),date.GetDay());
	while (j<NbDocs)
	{
		sub->urls->Start();
		int url=(rand()%(sub->urls->NbPtr))+(*(sub->urls))()->GetId();
		GDoc* doc=sub->urls->GetPtr(url);
		if (!judgedDocs->GetPtr(doc->GetId()))
		{
			if(judgement=='O') ses->InsertFdbk(ses->GetProfile(profileid),ses->GetDoc(doc->GetId()),djOK,today);
			else ses->InsertFdbk(ses->GetProfile(profileid),ses->GetDoc(doc->GetId()),djKO,today);
			judgedDocs->InsertPtr(doc);
			j++;
		}
	}	
	delete judgedDocs;
}     


//-----------------------------------------------------------------------------
void GSubjectTree::IdealGroupmentFile(char* url)
{
    // The file where the ideal groupment is stored.
	RTextFile* textfile = new RTextFile (url, RTextFile::Create);

	//calculation of the number of goups (ie the number of judged soubsubjects)
	int nbrsubsubjects=0;
    for (Start(); !End(); Next())
	{
		GSubject* sub= (*this)();
		for (sub->Start(); !sub->End(); sub->Next())
		{
			GSubject* subsub=(*sub)();
			if (subsub->isJudged())
			nbrsubsubjects++;
		}
	}
	textfile->WriteStr(itoa(nbrsubsubjects));
	textfile->WriteLine();

	RContainer<GProfile,unsigned,false,false>* prof=new RContainer<GProfile,unsigned,false,false>(10,5);
	//for each subject.
	for (Start(); !End(); Next())
	{
		GSubject * subject= (*this)();
		//for each subsubject.
		for (subject->Start(); !subject->End(); subject->Next())
		{
			GSubject* sub= (*subject)();
			if (sub->isJudged())
			{
				textfile->WriteStr(sub->GetLang());
				textfile->WriteStr("\t");
				int* tab;
				tab= new int[profiles->NbPtr];
				unsigned int k;
				for (k=0; k<(profiles->NbPtr); k++)
				{
				 	tab[k]=0;
				}
				int c=0;
				for (profiles->Start(); !profiles->End(); profiles->Next())
				{
					GProfile* profile= (*profiles)();
					GProfile* tmp=prof->GetPtr(profile);
					if((!tmp))
					{
						//if the profile as juged some document.
						if (profile->GetState()==osUpdated)
						{
							if(!strcmp(profile->GetName(),(RString(subject->GetName())+"/"+sub->GetName())))
							{
								c=c+1;
								prof->InsertPtr(profile);
								tab[c-1]=profile->GetId();
							}
						}
					}
				}
				textfile->WriteStr(itoa(c));
				int temp=0 ;
				while (tab[temp]!=0)
				{
					textfile->WriteStr(itoa(tab[temp]));
					temp++;
				}
				textfile->WriteLine();
				delete[] tab;
			}
		}
	}
	delete textfile;
	delete prof;
}


//-----------------------------------------------------------------------------
void GSubjectTree::IdealGroupment(RStd::RContainer<GGroups,unsigned int,true,true>* Groups,GSession* ses,RStd::RContainer<GGroupIdParentId,unsigned int,true,true>* parent) 	// research by profiles' name to find ideal groups
{
	unsigned int nb;
	unsigned int id,ii;
	GGroups* groups;
	GGroup* group;
	unsigned int nbprof;
	GLang* lang;
	GProfile* profile;
	GSubProfile* subprofile;
	RContainerCursor<GLang,unsigned int,true,true> CurLang(ses->GetLangs());

	//Calculation of the number of goups (ie the number of judged soubsubjects).
	int nbrsubsubjects=0;
	for (Start(); !End(); Next())
	{
		GSubject* sub= (*this)();
		for (sub->Start(); !sub->End(); sub->Next())
		{
			GSubject* subsub=(*sub)();
			if (subsub->isJudged())
			nbrsubsubjects++;
		}
	}
	ii=0;
	nb=nbrsubsubjects;
	for(CurLang.Start();!CurLang.End();CurLang.Next())
		Groups->InsertPtr(new GGroups(CurLang()));


	RContainer<GProfile,unsigned,false,false>* prof=new RContainer<GProfile,unsigned,false,false>(10,5);
	//For each subject.
	for (Start(); !End(); Next())
	{
		GSubject * subject= (*this)();
		//For each subsubject of the precedent subject.
		for (subject->Start(); !subject->End(); subject->Next())
		{
			GSubject* sub= (*subject)();
			if (sub->isJudged())
			{
				lang=ses->GetLang(sub->GetLang());
				int* tab;
				tab=new int[profiles->NbPtr];
				unsigned int k;
				for (k=0; k<(profiles->NbPtr); k++)
				{
					tab[k]=0;
				}
				int c=0;
				// find the profiles who are in the subsubject.
				for (profiles->Start(); !profiles->End(); profiles->Next())
				{
					GProfile* profile= (*profiles)();
					if((!prof->GetPtr(profile))&&(profile->GetState()==osUpdated)&&(!strcmp(profile->GetName(),(RString(subject->GetName())+"/"+sub->GetName()))))
					{
						c=c+1;
						prof->InsertPtr(profile);
						tab[c-1]=profile->GetId();
					}
				}
				nbprof=c;
				groups=Groups->GetPtr<const GLang*>(lang);
				ii++;
				group=new GGroup(ii,lang);
				parent->InsertPtr(new GGroupIdParentId(ii,subject->GetId()));
				groups->InsertPtr(group);
				int temp=0 ;
				while (tab[temp]!=0)
				{
					id=tab[temp];
					profile=ses->GetProfile(id);
					if(!profile) continue;
					subprofile=profile->GetSubProfile(lang);
					if(!subprofile) continue;
					group->InsertPtr(subprofile);
					temp++;
				}
				delete[] tab;
			}
		}
	}
	delete prof;
}


//-----------------------------------------------------------------------------
void GSubjectTree::InitProfiles()
{
	//Profiles doenst have juged document.
	for (profiles->Start(); !profiles->End();profiles->Next())
	{
		(*profiles)()->SetState(osModified);
	}
}


//-----------------------------------------------------------------------------
void GSubjectTree::InitSubSubjects()
{
	//No subject are judged.
	for (Start(); !End();Next())
	{
		GSubject * subject=(*this)();
		for(subject->Start();!subject->End();subject->Next())
		{
			(*subject)()->setIsJudged(false);
		}
	}
}
