/*

	GALILEI Research Project

	GSubjectTree.cpp

	GSubjectTree - Implementation.

	Copyright 2002 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).
		David Wartel (dwartel@ulb.ac.be).
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
// include files for ANSI C/C++
#include <stdlib.h>
#include <iostream.h>
#include <math.h>


//-----------------------------------------------------------------------------
// include files for R-Project
#include <rio/rtextfile.h>
using namespace RIO;


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <groups/gsubjecttree.h>
#include <sessions/gsession.h>
#include <docs/gdoc.h>
#include <langs/glang.h>
#include <groups/gsubject.h>
#include <groups/ggroupvector.h>
#include <groups/ggroups.h>
#include <profiles/gprofile.h>
#include <profiles/guser.h>
#include <profiles/gprofdoc.h>
using namespace GALILEI;
using namespace RMath;
using namespace RStd;



//-----------------------------------------------------------------------------
//
// class GSubjectTree
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GALILEI::GSubjectTree::GSubjectTree(unsigned int NbOk,unsigned int NbKo,unsigned int nbusers)
	: RTree<GSubject,true,false>(100,50)
{

	NbDocsOk=NbOk;
	NbDocsKo=NbKo;
	NbUsers=nbusers;
	NbProfiles=NbUsers;
	profiles=new RContainer<GProfile,unsigned,false,true>(10,5);
	Random = new RRandomGood(12345);
}


//-----------------------------------------------------------------------------
void GALILEI::GSubjectTree::InsertProfiles(void)
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
				this->profiles->InsertPtr(new GProfile(usr,profid,(RString(subject->GetName())+"/"+sub->GetName()),true,0,0,1,1));
				subid++;
				subid++;
				profid++;
			}
		}
	}
	
}


//-----------------------------------------------------------------------------
void GALILEI::GSubjectTree::Judgments(GSession* ses,int ran,int percok,int percko,int nbmin,int nbmax,unsigned int percsocial)
{
	int NbProfilesWhoJudgesDocuments;
	unsigned int psocial;

	NbDocsOk=percok;
	NbDocsKo=percko;
	if(ran!=0) Random->Reset(ran);
	InitProfiles();
	InitSubSubjects();                                    
	GProfileCursor ProfCursor;

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
	
	
	ProfCursor=ses->GetProfilesCursor();	
	for(ProfCursor.Start();!ProfCursor.End();ProfCursor.Next())
	{
		psocial=Random->Value(100);
		if(psocial<=percsocial)	ProfCursor()->SetSocial(true);
		else  ProfCursor()->SetSocial(false);
		ses->SaveProfile(ProfCursor());
	}
	for (this->Start(); !this->End(); this->Next())
	{
		GSubject* subject=(*this)();
		for (subject->Start();!subject->End();subject->Next())
		{
			GSubject* sub1=(*subject)();
			if (nbmax>NbProfiles) NbProfilesWhoJudgesDocuments =(Random->Value(NbProfiles-nbmin))+nbmin;
			else  NbProfilesWhoJudgesDocuments =(Random->Value(nbmax-nbmin))+nbmin; 
			sub1->setIsJudged(true); 
			for (int i=0; i<NbProfilesWhoJudgesDocuments; i++)
			{
				// Create the judgement.
				// First, find the profile who can judge the docs
				int profid=(tab[sub1->GetId()-1])*nbrsububjects+sub1->GetId();
				GProfile* proftemp = profiles->GetPtr(profid);
				proftemp->SetState(osUpdated);
				tab[sub1->GetId()-1]=tab[sub1->GetId()-1]+1;
		
				// Documents OK.
				JudgeDocuments(profid,sub1,1,ses);

				// Documents KO.
				if (subject->NbPtr>1) //If the subsuject enables the KO judgement of documents.
				{
					int subthema = (Random->Value(subject->NbPtr))+subject->SubSubjectMinId();
					GSubject* sub2=subject->GetPtr(subthema);
					while(sub2==sub1)
					{
						int subthema = (Random->Value(subject->NbPtr))+subject->SubSubjectMinId();
						sub2=subject->GetPtr(subthema);
					}
					JudgeDocuments(profid,sub2,0,ses);
				}
			}
		}
	}   
	delete [] tab;
}
  

//-----------------------------------------------------------------------------
void GALILEI::GSubjectTree::JudgeDocuments(int profileid,GSubject* sub,bool i,GSession* ses )
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
		int url=(Random->Value(sub->urls->NbPtr))+(*(sub->urls))()->GetId();
//		int url=(rand()%(sub->urls->NbPtr))+(*(sub->urls))()->GetId();
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
void GALILEI::GSubjectTree::IdealGroupmentFile(const char* url)
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
void GALILEI::GSubjectTree::IdealGroupment(RStd::RContainer<GGroups,unsigned int,true,true>* Groups,GSession* ses,RStd::RContainer<GGroupIdParentId,unsigned int,true,true>* parent) 	// research by profiles' name to find ideal groups
{
	unsigned int nb;
	unsigned int id,ii;
	GGroups* groups;
	GGroup* group;
	unsigned int nbprof;
	GLang* lang;
	GProfile* profile;
	GSubProfile* subprofile;
	GLangCursor CurLang;

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
	CurLang=ses->GetLangsCursor();
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
				group=new GGroupVector(ii,lang);
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
void GALILEI::GSubjectTree::InitProfiles()
{
	//Profiles doenst have juged document.
	for (profiles->Start(); !profiles->End();profiles->Next())
	{
		(*profiles)()->SetState(osModified);
	}
}


//-----------------------------------------------------------------------------
void GALILEI::GSubjectTree::InitSubSubjects()
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


//-----------------------------------------------------------------------------
GALILEI::GSubjectTree::~GSubjectTree(void)
{
	if(Random)
		delete Random;

}
