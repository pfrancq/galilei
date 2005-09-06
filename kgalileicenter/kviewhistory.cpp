/*

	GALILEI Research Project

	KViewHistory.cpp

	Window to display history of groups - Implementation.

	Copyright 2002 by the Universit�Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).

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
#include <rcontainer.h>
#include <rqt.h>


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <glang.h>
#include <ggroup.h>
#include <gsubject.h>
#include <gsubprofile.h>
#include <gprofile.h>
#include <gsession.h>
#include <gweightinfos.h>
#include <gweightinfo.h>
using namespace GALILEI;


//-----------------------------------------------------------------------------
// include files for qtwidgetsI
#include <qlistviewitemtype.h>


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
KViewHistory::KViewHistory(KDoc* doc,bool global,QWidget* parent,const char* name,int wflags, unsigned int minid, unsigned int maxid,
	 const char* mindate, const char* maxdate, bool bdate)
	: KView(doc,parent,name,wflags),
	  Global(global), MinGen(minid), MaxGen(maxid),MinDate(mindate), MaxDate(maxdate), CurId(0),  SubProfiles(0), Groups(0),  bDate(bdate)
{
	static char tmp[100];

	//init the container of selected subprofiles.
	SelectedSubProfiles=new R::RContainer<GWeightInfosHistory,false,true>(5,2);

	//init ToolBar components.
	ToolBar=  new QMenuBar( this, "file operations" );
	ToolBar->setGeometry(QRect(0, 0,(this->width()),30));
	QPopupMenu* groupmenu;
	groupmenu= new QPopupMenu();
	groupmenu->insertItem(QString("Calc Modified Groups"),this, SLOT(slotCheckModifiedGroups()),0,1,-1) ;
	ToolBar->insertItem("Group",groupmenu,2);
	QPopupMenu* subprofmenu;
	subprofmenu= new QPopupMenu();
	subprofmenu->insertItem(QString("Calc bad-grouped Profiles"),this, SLOT(slotCheckWellGroupedSubProfs()),0,1,-1) ;
	subprofmenu->insertItem(QString("Calc NewProfiles"),this, SLOT(slotCheckNewProfiles()),0,2,-1) ;
	ToolBar->insertItem("SubProfiles",subprofmenu,2);
	QPopupMenu* viewmenu;
	viewmenu= new QPopupMenu();
	viewmenu->insertItem(QString("Show/Hide Similarities"),this, SLOT(slotViewSimilarities()),0,1,-1) ;
	viewmenu->insertItem(QString("Show/Hide RelationShip"),this, SLOT(slotViewRelationShip()),0,2,-1);
	viewmenu->insertSeparator();
	viewmenu->insertItem(QString("Clear Similarities"),this, SLOT(slotClearSimilarities()),0,3,-1) ;
	viewmenu->insertItem(QString("Clear RelationShip"),this, SLOT(slotClearRelationShip()),0,4,-1) ;
	ToolBar->insertItem("Views",viewmenu);

	// Window
	setCaption(QString("Show Historical Groups"));

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
	SimsView->addColumn("Historic Id");
	SimsView->addColumn("Historic Date");
	SimsView->setResizeMode(QListView::AllColumns);
	SimsView->hide();

	// RelationShip ListView
	RelationShip=new QListView(this);
	RelationShip->setGeometry(QRect((this->width())/2, 30,(this->width())/2,this->height()));
	RelationShip->addColumn("Group");
	RelationShip->addColumn("RelationShip");
	RelationShip->addColumn("Historic Id");
	RelationShip->addColumn("Historic Date");
	RelationShip->setResizeMode(QListView::AllColumns);
	RelationShip->setRootIsDecorated(true);
	RelationShip->hide();

	// Load the chromosomes from the db
	if (bDate)
		Doc->GetSession()->LoadHistoricGroupsByDate(MinDate,MaxDate);
	else
		Doc->GetSession()->LoadHistoricGroupsById(MinGen,MaxGen);
	Groups=Doc->GetSession()->GetGroupsHistoryManager();

	//if no chromosomes, return.
	if((!Groups)||(!Groups->GetNb()))
		return;

	// if sorted by date, set MinGen and MaxGen.
	if (bDate)
	{
		MinGen=MaxGen=0;
		RCursor<GGroupsHistory> cGroup(*Groups);
		cGroup.Start();
		MinGen=MaxGen=cGroup()->GetId();
		for(cGroup.Next();!cGroup.End();cGroup.Next())
		{
			if(cGroup()->GetId()<MinGen)
				MinGen=cGroup()->GetId();;
			if(cGroup()->GetId()>MaxGen)
				MaxGen=cGroup()->GetId();
		}
	}

	//set the current id to the minimum id.
	CurId=MinGen;

	// Solutions Part
	Solution = new QGGroupsHistory(TabWidget,(*Groups)[0]);
	sprintf(tmp,"Solution (%u) %s",CurId,((*Groups)[0])->GetDate().ToString().Latin1());
	TabWidget->insertTab(Solution,tmp);
	Solution->setGroups((*Groups)[0]);
	Solution->setChanged();

	//tmp display
	Groups->CreateGroupsRelationship(MaxGen);

	//connections.
	connect(Solution,SIGNAL(doubleClicked(QListViewItem*)),parent->parent()->parent(),SLOT(slotHandleItem(QListViewItem*)));
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

	GGroupsHistory* grps;

	if(TabWidget->currentPage()!=Solution)
	{
		KView::keyReleaseEvent(e);
		return;
	}
	switch(e->key())
	{
		case Key_PageUp:
			if(CurId<MaxGen) CurId++;
			else CurId=MinGen;
			grps=Groups->GetPtr(CurId);
			Solution->setGroups(grps);
			Solution->setChanged();
			SelectedSubProfiles->Clear();
			SimsView->clear();
			sprintf(tmp,"Solution (%u) [%s]",CurId, grps->GetDate().ToString().Latin1());
			TabWidget->changeTab(Solution,tmp);
			break;
		case Key_PageDown:
			if(CurId>MinGen) CurId--;
			else CurId=MaxGen;
			grps=Groups->GetPtr(CurId);
			Solution->setGroups(grps);
			Solution->setChanged();
			sprintf(tmp,"Solution (%u) [%s]",CurId,grps->GetDate().ToString().Latin1());
			TabWidget->changeTab(Solution,tmp);
			break;
		default:
			e->ignore();
	}
}


//-----------------------------------------------------------------------------
void KViewHistory::resizeEvent(QResizeEvent*)
{
	TabWidget->setGeometry(QRect(0,30,(this->width())/2,this->height()-30));
	SimsView->setGeometry(QRect((this->width())/2, 30,(this->width())/2,this->height()-30));
	RelationShip->setGeometry(QRect((this->width())/2, 30,(this->width())/2,this->height()-30));
	ToolBar->setGeometry(QRect(0, 0,(this->width()),50));
}


//-----------------------------------------------------------------------------
void KViewHistory::slotCheckModifiedGroups(void)
{
	Doc->GetSession()->GetGroupsHistoryManager()->CheckModifiedGroups(MinGen);
}


//-----------------------------------------------------------------------------
void KViewHistory::slotCheckWellGroupedSubProfs(void)
{
	RCursor<GGroupsHistory> cGroup(*Groups);
	for(cGroup.Start();!cGroup.End();cGroup.Next())
	{
		cGroup()->SetGroupsSubject();
		cGroup()->CheckWellGroupedSubProfs();
	}
}


//-----------------------------------------------------------------------------
void KViewHistory::slotCheckNewProfiles(void)
{
	RCursor<GGroupsHistory> cGroup(*Groups);
	for(cGroup.Start();!cGroup.End();cGroup.Next())
		cGroup()->CheckNewProfiles();
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

	GWeightInfosHistory* tmp;
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
	unsigned int i;
	double similarity;
	QListViewItem* sim;
	char num1[50], num2[50], num3[50], id[10];

	//clear the list view
	SimsView->clear();

	//display selected profiles.
	RCursor<GWeightInfosHistory> cInfos1(*SelectedSubProfiles);
	RCursor<GWeightInfosHistory> cInfos2(*SelectedSubProfiles);
	for(i=SelectedSubProfiles->GetNb(),cInfos1.Start();i--;cInfos1.Next())
	{
		for(cInfos2.GoTo(i+1);!cInfos2.End();cInfos2.Next())
		{
			if(Global)
				similarity=cInfos1()->SimilarityIFF(*cInfos2(), otSubProfile,cInfos1()->GetParent()->GetLang());
			else
				similarity=cInfos1()->Similarity(*cInfos2());
			sprintf(num1,"%u",cInfos1()->GetId());
			sprintf(num2,"%u",cInfos2()->GetId());
			sprintf(num3,"%f",similarity);
			sprintf(id, "%u",cInfos1()->GetParent()->GetParent()->GetId());
			sim=new QListViewItem(SimsView, num1, num2, num3,id, ToQString(cInfos1()->GetParent()->GetParent()->GetDate()));
		}
	}

}


//-----------------------------------------------------------------------------
void KViewHistory::DisplayRelationShip(GGroupHistory* grp)
{
	char tmp[100];
	R::RCursor<GGroupHistory> cur;
	QListViewItemType* grpitem;
	char num1[50];

	//display this group
	sprintf(tmp,"Group %u" ,grp->GetId());
	sprintf(num1,"%u",grp->GetParent()->GetId());
	grpitem=new QListViewItemType(grp, RelationShip, tmp, 0, num1,ToQString(grp->GetParent()->GetDate()));
	grpitem->setOpen(true);
	grpitem->setSelected(true);

	//display children
	cur= grp->GetChildrens();
	for (cur.Start(); !cur.End(); cur.Next())
		DisplayChildrenRelationShip(cur(), grpitem);
}


//-----------------------------------------------------------------------------
void KViewHistory::DisplayChildrenRelationShip(GGroupHistory* grp, QListViewItemType* attach )
{
	char tmp[100];
	R::RCursor<GGroupHistory> cur;
	QListViewItemType* grpitem;
	char num1[50];

	//recursive method to display all children
	sprintf(tmp,"Group %u" ,grp->GetId());
	sprintf(num1,"%u",grp->GetParent()->GetId());
	grpitem=new QListViewItemType(grp, attach, tmp, "children", num1, ToQString(grp->GetParent()->GetDate()));
	cur= grp->GetChildrens();
	for (cur.Start(); !cur.End(); cur.Next())
		DisplayChildrenRelationShip(cur(), grpitem);
	grpitem->setOpen(true);
}


//-----------------------------------------------------------------------------
KViewHistory::~KViewHistory(void)
{
	if(Groups)
		Groups->Clear();
	if(SubProfiles)
		delete SubProfiles;
	if(TabWidget)
		delete (TabWidget);
	if(SimsView)
		delete (SimsView);
	if(SelectedSubProfiles)
		delete SelectedSubProfiles;
}

