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
#include <tests/ggroupsevaluate.h>
using namespace GALILEI;
using namespace RMath;
using namespace RStd;



//-----------------------------------------------------------------------------
//
// class GSubjectTree
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GALILEI::GSubjectTree::GSubjectTree(double NbOk,double NbKo,unsigned int nbusers)
	: RTree<GSubject,true,false>(100,50)
{
	RTimeDate::RDate date;

	NbDocsOk=NbOk;
	NbDocsKo=NbKo;
	NbUsers=nbusers;
	NbProfiles=NbUsers;
	profiles=new RContainer<GProfile,unsigned,false,true>(10,5);
	sprintf(today,"'%u-%u-%u'",date.GetYear(),date.GetMonth(),date.GetDay());
	IdealDoc = new RContainer<GGroupsEvaluate,unsigned int,false,false> (2,2);
}


//-----------------------------------------------------------------------------
void GALILEI::GSubjectTree::InsertProfiles(void)
{
	unsigned int profid=1, subid=1;
	unsigned int i;

	for (i=0; i<NbUsers; i++)
	{
		for (Start();!End();Next())
		{
			GSubject* subject = (*this)();
			for (subject->Start();!subject->End();subject->Next())
			{
				GSubject* sub= (*subject)();
				GUser* usr = new GUser(0,"test","test",1);
				profiles->InsertPtr(new GProfile(usr,profid,(RString(subject->GetName())+"/"+sub->GetName()),true,0,0,1,1));
				subid++;
				subid++;
				profid++;
			}
		}
	}
}


//-----------------------------------------------------------------------------
void GALILEI::GSubjectTree::ChooseSubject(GSession* ses,double percgrp,unsigned int NbDocPerGrp)
{
	IdealDoc=(ses->GetIdealDoc());
	double PercGrp=percgrp;
	unsigned int nbrsububjects=0;
	unsigned int compt=0;
	for (this->Start();!this->End(); this->Next())
	{
		nbrsububjects=nbrsububjects+(*this)()->NbPtr;
	}
	unsigned int* tab;
	unsigned int* tab1;
	unsigned int* tab2;
	tab=new unsigned int [nbrsububjects];
	tab1=new unsigned int [nbrsububjects];
	tab2=new unsigned int [nbrsububjects];

	for(IdealDoc->Start();!IdealDoc->End();IdealDoc->Next())
	{
		GGroupEvaluateCursor Grp=(*IdealDoc)()->GetGroupEvaluateCursor();
		for(Grp.Start();!Grp.End();Grp.Next())
		{
			tab2[compt]=Grp()->NbPtr();
			compt++;
		}
	}
	for (unsigned int i=0; i<nbrsububjects;i++)
	{
		tab[i]=ses->GetCurrentRandomValue(100);
		tab1[i]=tab[i];
	}
	qsort(tab, nbrsububjects, sizeof(int),sortOrder);
	unsigned int nb= int(nbrsububjects*PercGrp/100);
	if(nb>nbrsububjects-1) nb=nbrsububjects-1;
	unsigned int minvalue=tab[nb];
	compt=0;
	unsigned int comptused=0;
	for (this->Start(); !this->End(); this->Next())
	{
		GSubject* subject=(*this)();
		for (subject->Start();!subject->End();subject->Next())
		{
			if((tab1[compt]<=minvalue)&&(tab2[compt]>NbDocPerGrp))
			{
				(*subject)()->setIsUsed(true);
				comptused++;
			}
			else
			{
				(*subject)()->setIsUsed(false);
			}
			compt++;
		}
	}
	delete [] tab;
	delete [] tab1;
	delete [] tab2;

}


//-----------------------------------------------------------------------------
int GALILEI::GSubjectTree::sortOrder(const void * a, const void * b)
{
	return ( *(int*)a - *(int*)b );
}


//-----------------------------------------------------------------------------
void GALILEI::GSubjectTree::Judgments(GSession* ses,double percok,double percko,double perchs,unsigned int nbmin,unsigned int nbmax,double percsocial,double percerr,bool Save)
{
	unsigned int NbProfilesWhoJudgesDocuments;
	double psocial;

	NbDocsOk=percok;
	NbDocsKo=percko;
	NbDocsHs=perchs;
	InitProfiles();
	InitSubSubjects();
	GProfileCursor ProfCursor;

	//Calculation of the total number of subsubjects.
	unsigned int nbrsububjects=0;
	for (this->Start();!this->End(); this->Next())
	{
		nbrsububjects=nbrsububjects+(*this)()->NbPtr;
	}

	unsigned int* tab;
	tab=new unsigned int [nbrsububjects];
	for (unsigned int i=0; i<nbrsububjects;i++)
		tab[i]=0;

	unsigned int* tabs;
	tabs=new unsigned int [nbrsububjects];
	for (unsigned int i=0; i<nbrsububjects;i++)
		tabs[i]=0;


	unsigned int compt=0;
	for (this->Start(); !this->End(); this->Next())
	{
		GSubject* subject=(*this)();
		for (subject->Start();!subject->End();subject->Next())
		{
			if((*subject)()->isUsed())	tabs[compt]=1;
			compt++;
		}
	}

	// Create the social parameters.
	ProfCursor=ses->GetProfilesCursor();
	for(ProfCursor.Start();!ProfCursor.End();ProfCursor.Next())
	{
		psocial=ses->GetCurrentRandomValue(100);
		if(psocial<=percsocial)	ProfCursor()->SetSocial(true);
		else  ProfCursor()->SetSocial(false);
		if(Save)
			ses->SaveProfile(ProfCursor());
	}

	// Create the judgments.
	for (this->Start(); !this->End(); this->Next())
	{
		GSubject* subject=(*this)();
		// For each subject chose documents to juge.
		for (subject->Start();!subject->End();subject->Next())
		{
			GSubject* sub1=(*subject)();
			if(sub1->isUsed())
			{
				// find the number of profile who can judge documents.
				if (nbmax>NbProfiles) NbProfilesWhoJudgesDocuments =(ses->GetCurrentRandomValue(NbProfiles-nbmin))+nbmin;
				else  NbProfilesWhoJudgesDocuments =(ses->GetCurrentRandomValue(nbmax-nbmin))+nbmin;

				// Set that the subject is judged.
				sub1->setIsJudged(true);
			
				// For each subprofiles who can judge documents/
				for (unsigned int i=0;i<NbProfilesWhoJudgesDocuments;i++)
				{
					// Create the judgement.
					// First, find the profile who can judge the docs
					unsigned int profid=(tab[sub1->GetId()-1])*nbrsububjects+sub1->GetId();
					GProfile* proftemp=profiles->GetPtr(profid);
					proftemp->SetState(osUpdated);
					tab[sub1->GetId()-1]=tab[sub1->GetId()-1]+1;
		
					// Documents OK.
					JudgeDocuments(profid,sub1,1,ses,percerr);

					// The Number of Ok documents judged by this profiles.
					unsigned int nbDocok =int(((double(NbDocsOk*sub1->urls->NbPtr)/100))+0.5);

					// Documents KO.
					//If the subsuject enables the KO judgement of documents.
					unsigned int* tabko;
					tabko=new unsigned int [subject->NbPtr];
					for (unsigned int i=0; i<subject->NbPtr;i++)
						tabko[i]=0;

					if (subject->NbPtr>1)
					{
						for(unsigned int i=0;i<subject->NbPtr-1;i++)
						{
							int subthema=i+subject->SubSubjectMinId();
							GSubject* sub2=subject->GetPtr(subthema);
							if(sub2!=sub1)
							{
	  							if(sub2->isUsed()) JudgeDocuments(profid,sub2,2,ses,percerr);
							}
						}
					}

					// Documents HS.
					// If the subsuject enables the KO judgement of documents.
					if (NbDocsHs>0)
					{
						// Judge NBDocsHs percent of NbDocok
						for (int i=1;i<int(NbDocsHs*nbDocok/100);i++)
						{
							unsigned int DocIdToJug=(ses->GetCurrentRandomValue(ses->GetNbDocs())+1);
							unsigned int GrpDocId=0;

							GDoc* doc=ses->GetDoc(DocIdToJug);

							// Verify that the Document is not in ok or ko subject
							for(IdealDoc->Start();!IdealDoc->End();IdealDoc->Next())
							{
								GGroupEvaluateCursor Grp=(*IdealDoc)()->GetGroupEvaluateCursor();
								bool find=false;
								for(Grp.Start();!Grp.End();Grp.Next())
								{
									if(Grp()->IsIn(DocIdToJug))
									{
										GrpDocId=Grp()->GetId();
										find=true;
									}
									if(find) continue;
								}
							}

							// If the doc is not in the ok subject or ko subject
							// and that the profiles doenst have judged and that the subject is judged
							if((GrpDocId<subject->SubSubjectMinId())||(GrpDocId>subject->SubSubjectMinId()+subject->NbPtr)&&(tabs[GrpDocId-1]==1))
							{
								if(ses->GetProfile(profid)->GetFeedback(doc)==0)
								{
									// the default value is HS
									unsigned int tempjug=3;

									//But if their is random error
									if(ses->GetCurrentRandomValue(100)<int(percerr))
									{
										tempjug=int(ses->GetCurrentRandomValue(3)+1);
									}
									if(tempjug==1)
									{
										ses->InsertFdbk(ses->GetProfile(profid),doc,djOK,today);
									}
									else if(tempjug==2)
									{
										ses->InsertFdbk(ses->GetProfile(profid),doc,djKO,today);
									}
									else if(tempjug==3)
									{
										ses->InsertFdbk(ses->GetProfile(profid),doc,djOutScope,today);
									}
								}
								else i--;
							}
						}
					}
					delete[] tabko;
				}
			}
		}
	}
	
	delete [] tab;
	delete [] tabs;
}
  

//-----------------------------------------------------------------------------
void GALILEI::GSubjectTree::JudgeDocuments(unsigned int profileid,GSubject* sub,unsigned int i,GSession* ses,double PercErr )
{

	unsigned int NbDocs;
	unsigned int judgement;

	//If i juge ok.
	if (i==1)
	{
		NbDocs=int(((double(NbDocsOk*sub->urls->NbPtr)/100))+0.5);
		judgement=1;
	}

	//If i=0 juge KO.
	else if (i==2)
	{
		NbDocs=int(((double(NbDocsKo*sub->urls->NbPtr)/100))+0.5);
		judgement=2;
	}


	RContainer<GDoc,unsigned,false,false>* judgedDocs=new RContainer<GDoc,unsigned,false,false>(10,5);
	unsigned int j=0;
	while (j<NbDocs)
	{
		sub->urls->Start();
		unsigned int url=(ses->GetCurrentRandomValue(sub->urls->NbPtr));
		GDoc* doc=sub->urls->GetPtrAt(url);
		if (!judgedDocs->GetPtr(doc->GetId()))
		{
			unsigned int tempjug=judgement;
			// If there is Random change the judgment.
			if(ses->GetCurrentRandomValue(100)<int(PercErr))
			{
				tempjug= int(ses->GetCurrentRandomValue(3)+1);
			}
			if(tempjug==1)
			{
				ses->InsertFdbk(ses->GetProfile(profileid),ses->GetDoc(doc->GetId()),djOK,today);
			}
			else if(tempjug==2)
			{
				ses->InsertFdbk(ses->GetProfile(profileid),ses->GetDoc(doc->GetId()),djKO,today);
			}
			else if(tempjug==3)
			{
				ses->InsertFdbk(ses->GetProfile(profileid),ses->GetDoc(doc->GetId()),djOutScope,today);
			}

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
	RTextFile textfile(url, RIO::Create);

	//calculation of the number of goups (ie the number of judged soubsubjects)
	unsigned int nbrsubsubjects=0;
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
	textfile.WriteStr(itoa(nbrsubsubjects));
	textfile.WriteLine();

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
				textfile.WriteStr(sub->GetLang());
				textfile.WriteStr("\t");
				unsigned int* tab;
				tab= new unsigned int[profiles->NbPtr];
				unsigned int k;
				for (k=0; k<(profiles->NbPtr); k++)
				{
				 	tab[k]=0;
				}
				unsigned int c=0;
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
				textfile.WriteStr(itoa(c));
				unsigned int temp=0 ;
				while (tab[temp]!=0)
				{
					textfile.WriteStr(itoa(tab[temp]));
					temp++;
				}
				textfile.WriteLine();
				delete[] tab;
			}
		}
	}
	delete prof;
}


//-----------------------------------------------------------------------------
void GALILEI::GSubjectTree::IdealGroupment(RStd::RContainer<GGroups,unsigned int,true,true>* Groups,GSession* ses,RStd::RContainer<GGroupIdParentId,unsigned int,true,true>* parent) 	// research by profiles' name to find ideal groups
{
	unsigned int nb;
	unsigned int id;
	GGroups* groups;
	GGroup* group;
	unsigned int nbprof;
	GLang* lang;
	GProfile* profile;
	GSubProfile* subprofile;
	GLangCursor CurLang;

	//Calculation of the number of goups (ie the number of judged soubsubjects).
	unsigned int nbrsubsubjects=0;
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

	nb=nbrsubsubjects;
	CurLang=ses->GetLangsCursor();
	for(CurLang.Start();!CurLang.End();CurLang.Next())
		Groups->InsertPtr(new GGroups(CurLang()));

	CreateParent(parent);
	RContainer<GProfile,unsigned,false,false>* prof=new RContainer<GProfile,unsigned,false,false>(10,5);
	//For each subject.
	for (Start(); !End(); Next())
	{
		GSubject * subject= (*this)();
		//For each subsubject of the precedent subject.
		for (subject->Start(); !subject->End(); subject->Next())
		{
			GSubject* sub= (*subject)();
			if (sub->isUsed())
			{
				if (sub->isJudged())
				{
					lang=ses->GetLang(sub->GetLang());
					unsigned int* tab;
					tab=new unsigned int[profiles->NbPtr];
					unsigned int k;
					for (k=0; k<(profiles->NbPtr); k++)
					{
						tab[k]=0;
					}
					unsigned int c=0;
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
					group=new GGroupVector(sub->GetId(),lang);
					//parent->InsertPtr(new GGroupIdParentId(ii,subject->GetId()));
					groups->InsertPtr(group);
					unsigned int temp=0 ;
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
	}
	delete prof;
}


//-----------------------------------------------------------------------------
void GALILEI::GSubjectTree::CreateParent(RStd::RContainer<GGroupIdParentId,unsigned int,true,true>* parent)
{
	parent->Clear();
	for (Start(); !End(); Next())
	{
		GSubject * subject= (*this)();
		//For each subsubject of the precedent subject.
		for (subject->Start(); !subject->End(); subject->Next())
		{
			GSubject* sub= (*subject)();
			parent->InsertPtr(new GGroupIdParentId(sub->GetId(), subject->GetId()));
		}
	}
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
	delete profiles;
	delete IdealDoc;
}
