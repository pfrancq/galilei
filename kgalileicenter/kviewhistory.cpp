/*

	GALILEI Research Project

	KViewHistory.cpp

	Window to display history of groups - Implementation.

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
// include files for ANSI C/C++
#include <stdlib.h>


//-----------------------------------------------------------------------------
// include files for R Project
#include <rstd/rcontainer.h>


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <langs/glang.h>
#include <groups/ggroups.h>
#include <groups/ggroup.h>
#include <groups/gsubject.h>
#include <profiles/gsubprofile.h>
#include <profiles/gsubprofilevector.h>
#include <profiles/gprofile.h>
#include <profiles/gprofilessim.h>
#include <sessions/gsession.h>
#include <infos/giwordsweights.h>
#include <infos/giwordweight.h>
using namespace GALILEI;


//-----------------------------------------------------------------------------
// include files for qtwidgetsI
#include <galilei/qlistviewitemtype.h>


//-----------------------------------------------------------------------------
// include files for KDE
#include <kapp.h>
#include <kmessagebox.h>


//-----------------------------------------------------------------------------
// application specific includes
#include "kviewhistory.h"
#include "kdoc.h"



//-----------------------------------------------------------------------------
//
// class KViewHistory
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
KViewHistory::KViewHistory(KDoc* doc,const char* l,bool global,QWidget* parent,const char* name,int wflags, unsigned int minid, unsigned int maxid)
	: KView(doc,parent,name,wflags),
	  Global(global), Lang(doc->GetSession()->GetLang(l)), MinGen(minid), MaxGen(maxid),CurId(0),  SubProfiles(0), Groups(0),  Sims(0)
{
	static char tmp[100];
	GLang* lang;
	GSubProfileCursor Cur;
	GSubProfile* sub;

	//init the container of selected subprofiles.
	SelectedSubProfiles=new R::RContainer<GIWordsWeightsHistory, unsigned int, false, true>(5,2);

	//init ToolBar components.
	ToolBar=  new QMenuBar( this, "file operations" );
	ToolBar->setGeometry(QRect(0, 0,(this->width()),30));
	QPopupMenu* groupmenu;
	groupmenu= new QPopupMenu();
	groupmenu->insertItem(QString("Calc Modified Groups"),this, SLOT(slotCheckModifiedGroups()),0,1,-1) ;
	ToolBar->insertItem("Group",groupmenu);
	QPopupMenu* subprofmenu;
	subprofmenu= new QPopupMenu();
	subprofmenu->insertItem(QString("Calc bad-grouped Profiles"),this, SLOT(slotCheckWellGroupedSubProfs()),0,1,-1) ;
	subprofmenu->insertItem(QString("Calc NewProfiles"),this, SLOT(slotCheckNewProfiles()),0,2,-1) ;
	ToolBar->insertItem("SubProfiles",subprofmenu);
	QPopupMenu* viewmenu;
	viewmenu= new QPopupMenu();
	viewmenu->insertItem(QString("Show/Hide Similarities"),this, SLOT(slotViewSimilarities()),0,1,-1) ;
	viewmenu->insertItem(QString("Show/Hide RelationShip"),this, SLOT(slotViewRelationShip()),0,2,-1);
	viewmenu->insertSeparator();
	viewmenu->insertItem(QString("Clear Similarities"),this, SLOT(slotClearSimilarities()),0,3,-1) ;
	viewmenu->insertItem(QString("Clear RelationShip"),this, SLOT(slotClearRelationShip()),0,4,-1) ;
	ToolBar->insertItem("Views",viewmenu);

	// Window
	lang=Doc->GetSession()->GetLang(l);
	setCaption(QString("Show Store Chromosomes - ")+lang->GetName());

	// Tab
	setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)1, (QSizePolicy::SizeType)1, sizePolicy().hasHeightForWidth() ) );
	TabWidget = new QTabWidget( this, "TabWidget" );
	TabWidget->setGeometry(QRect(0,30,(this->width())/2,this->height()));
	TabWidget->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)7, (QSizePolicy::SizeType)7, TabWidget->sizePolicy().hasHeightForWidth() ) );
	TabWidget->setBackgroundOrigin( QTabWidget::ParentOrigin );

	// Sims view
	SimsView=new QListView(this);
	SimsView->setGeometry(QRect((this->width())/2, 30,(this->width())/2,this->height()));
	SimsView->addColumn("FirstSubProfile");
	SimsView->addColumn("Second SubProfile");
	SimsView->addColumn("Similarity");
	SimsView->setResizeMode(QListView::AllColumns);
	SimsView->hide();

	// RelationShip ListView
	RelationShip=new QListView(this);
	RelationShip->setGeometry(QRect((this->width())/2, 30,(this->width())/2,this->height()));
	RelationShip->addColumn("Group");
	RelationShip->addColumn("RelationShip");
	RelationShip->addColumn("Historic Id");
	RelationShip->setResizeMode(QListView::AllColumns);
	RelationShip->setRootIsDecorated(true);
	RelationShip->hide();

	// Construct the SubProfiles Container
	Cur=Doc->GetSession()->GetSubProfilesCursor(lang);
	GProfileCursor cur=Doc->GetSession()->GetProfilesCursor();
	SubProfiles=new R::RContainer<GSubProfile,unsigned int,false,true>(cur.GetNb());
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		sub=Cur();
		if(sub->IsDefined())
			SubProfiles->InsertPtr(sub);
	}

	// Load the chromosomes from the db
	Groups=Doc->GetSession()->LoadHistoricGroups(SubProfiles, lang, MinGen,MaxGen);
	CurId=MinGen;

	if(!Groups) return;

	// Solutions Part
	Solution = new QGGroupsHistory(TabWidget,Groups->GetPtrAt(0));
	sprintf(tmp,"Solution (%u/%u) ",CurId,MaxGen);
	TabWidget->insertTab(Solution,tmp);
	connect(Solution,SIGNAL(doubleClicked(QListViewItem*)),parent->parent()->parent(),SLOT(slotHandleItem(QListViewItem*)));
	Solution->setGroups(Groups->GetPtrAt(0));
	Solution->setChanged();

	//tmp display
	CreateGroupsRelationship();

	//connections.
	connect(Solution,SIGNAL(doubleClicked(QListViewItem*)),this,SLOT(slotSelectedSetChanged(QListViewItem*)));
}


//-----------------------------------------------------------------------------
void KViewHistory::update(unsigned int /*cmd*/)
{
}


//---------------------------------------------------------------------------
void KViewHistory::keyReleaseEvent(QKeyEvent* e)
{
	static char tmp[100];
//	QGoToPopDlg *dlg;

	GGroupsHistory* grps;

	if(TabWidget->currentPage()!=Solution)
	{
		KView::keyReleaseEvent(e);
		return;
	}
	switch(e->key())
	{
		case Key_PageUp:
			if(CurId<MaxGen) CurId++; else CurId=MinGen;
			sprintf(tmp,"Solution (%u/%u) ",CurId,MaxGen);
			TabWidget->changeTab(Solution,tmp);
			grps=Groups->GetPtr(CurId);
			Solution->setGroups(grps);
			Solution->setChanged();
			SelectedSubProfiles->Clear();
			SimsView->clear();
			break;

			
		case Key_PageDown:
			if(CurId>MinGen) CurId--; else CurId=Groups->NbPtr-1;
			sprintf(tmp,"Solution (%u/%u)",CurId,MaxGen);
			TabWidget->changeTab(Solution,tmp);
			grps=Groups->GetPtr(CurId);

			Solution->setGroups(grps);
			Solution->setChanged();
			break;
		default:
			e->ignore();
	}
}


//-----------------------------------------------------------------------------
void KViewHistory::resizeEvent(QResizeEvent*)
{
	TabWidget->setGeometry(QRect(0,30,(this->width())/2,this->height()-30));
	// Sims view
	SimsView->setGeometry(QRect((this->width())/2, 30,(this->width())/2,this->height()-30));
	RelationShip->setGeometry(QRect((this->width())/2, 30,(this->width())/2,this->height()-30));

	ToolBar->setGeometry(QRect(0, 0,(this->width()),50));
}


//-----------------------------------------------------------------------------
void KViewHistory::slotCheckModifiedGroups(void)
{
	for (Groups->Start(); !Groups->End(); Groups->Next())
		CheckModifiedGroups((*Groups)());
	ToolBar->setItemEnabled(1,false);
}


//-----------------------------------------------------------------------------
void KViewHistory::slotCheckWellGroupedSubProfs(void)
{
	for (Groups->Start(); !Groups->End(); Groups->Next())
	{
			SetGroupsSubject((*Groups)());
		CheckWellGroupedSubProfiles((*Groups)());
	}
	ToolBar->setItemEnabled(2,false);
}


//-----------------------------------------------------------------------------
void KViewHistory::slotCheckNewProfiles(void)
{
	for (Groups->Start(); !Groups->End(); Groups->Next())
		CheckNewProfiles((*Groups)());
	ToolBar->setItemEnabled(3,false);
}


//-----------------------------------------------------------------------------
void KViewHistory::slotViewSimilarities(void)
{
	RelationShip->hide() ;
	if (SimsView->isHidden())  SimsView->show();
	else SimsView->hide();
}


//-----------------------------------------------------------------------------
void KViewHistory::slotViewRelationShip(void)
{
	SimsView->hide();
	if (RelationShip->isHidden())  RelationShip->show();
	else RelationShip->hide();
}


//-----------------------------------------------------------------------------
void KViewHistory::slotSelectedSetChanged(QListViewItem* item)
{

	GIWordsWeightsHistory* tmp;
	QListViewItemType* itemtype=((QListViewItemType*)item);

	switch(itemtype->Type)
	{
		case (QListViewItemType::tGiwwh) :
			// check if item is selected or deselected
			tmp=SelectedSubProfiles->GetPtr(itemtype->Obj.Giwwh);
			if(tmp)
				SelectedSubProfiles->DeletePtr(itemtype->Obj.Giwwh);
			else
				SelectedSubProfiles->InsertPtr(itemtype->Obj.Giwwh);
			// update the SimsView listview.
			DisplaySimilarities();
			break;

		case (QListViewItemType::tGroupHistory) :
			DisplayRelationShip(itemtype->Obj.GroupHistory);
			break;

		default:
			return;
	}
}


//-----------------------------------------------------------------------------
void KViewHistory::DisplaySimilarities(void)
{
	unsigned int i,j;
	double similarity;
	GIWordsWeightsHistory** giwwh1, **giwwh2;
	QListViewItem* sim;
	char num1[50], num2[50], num3[50];

	//clear the list view
	SimsView->clear();

	//display selected profiles.
	for (i=SelectedSubProfiles->NbPtr, giwwh1=SelectedSubProfiles->Tab; i--;giwwh1++)
	{
		for (j=i, giwwh2=giwwh1+1; j--; giwwh2++)
		{
			if (Global) similarity=(*giwwh1)->SimilarityIdf(*giwwh2, otSubProfile,Lang) ;
			else similarity=(*giwwh1)->Similarity(*giwwh2);
			sprintf(num1,"%u",(*giwwh1)->GetId());
			sprintf(num2,"%u",(*giwwh2)->GetId());
			sprintf(num3,"%f",similarity);
			sim=new QListViewItem(SimsView, num1, num2, num3);
		}
	}
}


//-----------------------------------------------------------------------------
void KViewHistory::DisplayRelationShip(GGroupHistory* grp)
{
	char tmp[100];
	GGroupHistoryCursor cur;
	QListViewItemType* grpitem;
	char num1[50];

	//display this group
	sprintf(tmp,"Group %u" ,grp->GetId());
	sprintf(num1,"%u",grp->GetParent()->GetId());
	grpitem=new QListViewItemType(grp, RelationShip, tmp, 0, num1);
	grpitem->setOpen(true);
	grpitem->setSelected(true);

	//display children
	cur= grp->GetChildrenCursor();
	for (cur.Start(); !cur.End(); cur.Next())
		DisplayChildrenRelationShip(cur(), grpitem);
}


//-----------------------------------------------------------------------------
void KViewHistory::DisplayChildrenRelationShip(GGroupHistory* grp, QListViewItemType* attach )
{
	char tmp[100];
	GGroupHistoryCursor cur;
	QListViewItemType* grpitem;
	char num1[50];

	//recursive method to display all children
	sprintf(tmp,"Group %u" ,grp->GetId());
	sprintf(num1,"%u",grp->GetParent()->GetId());
	grpitem=new QListViewItemType(grp, attach, tmp, "children", num1);
	cur= grp->GetChildrenCursor();
	for (cur.Start(); !cur.End(); cur.Next())
		DisplayChildrenRelationShip(cur(), grpitem);
	grpitem->setOpen(true);
}


//-----------------------------------------------------------------------------
void KViewHistory::CheckModifiedGroups(GGroupsHistory* grps)
{
	R::RContainer<GIWordsWeightsHistory, unsigned int, false, true>* lastsubs;
	GGroupsHistory* lastgroups;
	GGroupHistory* grp, *lastgroup;
	GIWordsWeightsHistory* sub, *lastsub;
	unsigned int  lastcurid;
	
	// if the grps is the frst historic one, return
	if (grps->GetId()==MinGen) return;

	//get the last groups and put its profiles in a container
	lastsubs=new R::RContainer<GIWordsWeightsHistory, unsigned int, false, true>(10,5);
	lastgroups=Groups->GetPtr(grps->GetId()-1);
	if (!lastgroups) return;
	for (lastgroups->Start(); !lastgroups->End(); lastgroups->Next())
		for ((*lastgroups)()->Start(); !(*lastgroups)()->End();(*lastgroups)()->Next())
			lastsubs->InsertPtr((*(*lastgroups)())());

	// check the actual groups
	for (grps->Start(); !grps->End(); grps->Next())
	{
		grp=(*grps)();
		grp->Start();
		sub=(*grp)();

		//get the equivalent last subprofile
		lastsub=lastsubs->GetPtr(sub->GetId());
		//get the equivalent last group.
		lastgroup=lastgroups->GetPtr(lastsub->GetParent());
		//if the number of pointers is different, the group is modified
		if (lastgroup->NbPtr!=grp->NbPtr)
		{
			grp->SetModified(true);
			continue;
		}
		
		//if a couple of subprofiles doesn't belong to a same group in the current and historic groups
		// then the group is modified.
		lastcurid=lastsub->GetParent()->GetId();
		while(grp->GetModified()==false&&!grp->End())
		{
			lastsub=lastsubs->GetPtr((*grp)()->GetId());
			// if the subprofiles is a new one, group is modified
			if (!lastsub) grp->SetModified(true);
			
			//if the lastcurid has changed , group is modified
			if(lastcurid!=lastsub->GetParent()->GetId())
				grp->SetModified(true);
			grp->Next();
		}
	}

	delete lastsubs;
}


//-----------------------------------------------------------------------------
void KViewHistory::SetGroupsSubject(GGroupsHistory* grps)
{
	GGroupHistory* grp;
	GSubject* mainsubject;
	unsigned int occur, maxoccur,knownsubject;
	R::RContainer<GSubject, unsigned int, false, true>* subjects;


	//find the dominant subject
	for (grps->Start(); !grps->End(); grps->Next())
	{
		grp=(*grps)();
		maxoccur=knownsubject=0;

		// find all the subjects contained in the group.
		subjects=new R::RContainer<GSubject, unsigned int, false, true>(5,2);
		for (grp->Start(); !grp->End(); grp->Next())
		{
			subjects->InsertPtr((*grp)()->GetSubProfile()->GetSubject());
		}

		// find the most dominant one
		subjects->Start();
		while((maxoccur<(grp->NbPtr-knownsubject))&&!subjects->End())
		{
			occur=0;
			for (grp->Start(); !grp->End(); grp->Next())
			{
				if ((*grp)()->GetSubProfile()->GetSubject()->GetId()==(*subjects)()->GetId())
					occur++;
			}
			knownsubject+=occur;
			if(occur>maxoccur)
			{
				maxoccur=occur;
				mainsubject=(*subjects)();
			}
			subjects->Next();
		}
		grp->SetSubject(mainsubject);
		delete subjects;
	}	
}


//-----------------------------------------------------------------------------
void KViewHistory::CheckWellGroupedSubProfiles(GGroupsHistory* grps)
{
	GGroupHistory* grp;
	for (grps->Start(); !grps->End(); grps->Next())
	{
		grp=(*grps)();
		for (grp->Start(); !grp->End(); grp->Next())
			if((*grp)()->GetSubProfile()->GetSubject()->GetId()==grp->GetSubject()->GetId())
				(*grp)()->SetWellGrouped(true);
			else
				(*grp)()->SetWellGrouped(false);
	}
}


//-----------------------------------------------------------------------------
void KViewHistory::CheckNewProfiles(GGroupsHistory* grps)
{
	R::RContainer<GIWordsWeightsHistory, unsigned int, false, true>* lastsubs;
	GGroupsHistory* lastgroups;
	GGroupHistory* grp;

	//get the last groups and put its profiles in a container
	lastsubs=new R::RContainer<GIWordsWeightsHistory, unsigned int, false, true>(10,5);
	lastgroups=Groups->GetPtr(grps->GetId()-1);
	if (!lastgroups) return;
	for (lastgroups->Start(); !lastgroups->End(); lastgroups->Next())
		for ((*lastgroups)()->Start(); !(*lastgroups)()->End();(*lastgroups)()->Next())
			lastsubs->InsertPtr((*(*lastgroups)())());

	//check wether each subprofile was in the last groups
	//if not, the profiles is considered as new.
	for  (grps->Start(); !grps->End(); grps->Next())
	{
		grp=(*grps)();
		for (grp->Start(); !grp->End(); grp->Next())
			if (!lastsubs->GetPtr((*grp)()))
				(*grp)()->SetNewSubProfile(true);
	}

	delete lastsubs;
}


//-----------------------------------------------------------------------------
void KViewHistory::CreateGroupsRelationship(void)
{
	unsigned int  i, maxoccurs, nbchildren;
	GGroupsHistory* curgrps, *nextgrps;
	GGroupHistory* grp;
	GIWordsWeightsHistory* subprof;
	bool treated;


	for (Groups->Start(); !Groups->End(); Groups->Next())
	{
		//if the groups is the last one, no child
		if ((*Groups)()->GetId()==MaxGen) continue;

		curgrps=(*Groups)();
		nextgrps=Groups->GetPtr(curgrps->GetId()+1);

		for (curgrps->Start(); !curgrps->End(); curgrps->Next())
		{
			grp=(*curgrps)();
			nbchildren=0;
			//initialize the table od groupid / nboccurs
			unsigned int tab[grp->NbPtr][2];
			for (i=0; i<grp->NbPtr; i++)
				tab[i][0]=tab[i][1]=0;

			//fill the table
			for (grp->Start(); !grp->End(); grp->Next())
			{
				subprof=nextgrps->GetSubProfile((*grp)()->GetId());
				treated=false;
				i=0;
				while(!treated&&i<nbchildren)
				{
					if (tab[i][0]==subprof->GetParent()->GetId())
					{
						tab[i][1]++;
						treated=true;
					}
					i++;
				}
				if(!treated)
				{
					tab[i][0]=subprof->GetParent()->GetId();
					tab[i][1]++;
					nbchildren++;
				}
			}

			//analyse table;
			maxoccurs=0;
			for (i=0; i<grp->NbPtr;i++)
				if (tab[i][1]>maxoccurs)
					maxoccurs=tab[i][1];
			//and find the childrens
			for (i=0; i<grp->NbPtr;i++)
				if (tab[i][1]==maxoccurs)
					grp->InsertChildren(nextgrps->GetPtr(tab[i][0]));
		}
	}
}


//-----------------------------------------------------------------------------
KViewHistory::~KViewHistory(void)
{
	if(Groups)
		delete Groups;
	if(SubProfiles)
		delete SubProfiles;
	if(Sims)
		delete Sims;
	if(TabWidget)
		delete (TabWidget);
	if(SimsView)
		delete (SimsView);
	if(SelectedSubProfiles)
		delete SelectedSubProfiles;
}
