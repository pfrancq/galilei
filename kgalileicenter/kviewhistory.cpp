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
// include files for GALILEI
//include files for R-Project
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

	SelectedItems=new RStd::RContainer<GIWordsWeightsHistory, unsigned int, false, true>(5,2);

	// Window
	lang=Doc->GetSession()->GetLang(l);
	setCaption(QString("Show Store Chromosomes - ")+lang->GetName());

	// Tab
	setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)1, (QSizePolicy::SizeType)1, sizePolicy().hasHeightForWidth() ) );
	TabWidget = new QTabWidget( this, "TabWidget" );
	TabWidget->setGeometry(QRect(0, 0,(this->width())/2,this->height()));
	TabWidget->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)7, (QSizePolicy::SizeType)7, TabWidget->sizePolicy().hasHeightForWidth() ) );
	TabWidget->setBackgroundOrigin( QTabWidget::ParentOrigin );

	// Sims view
	SimsView=new QListView(this);
	SimsView->setGeometry(QRect((this->width())/2, 0,(this->width())/2,this->height()));
	SimsView->addColumn("FirstSubProfile");
	SimsView->addColumn("Second SubProfile");
	SimsView->addColumn("Similarity");
	

	// Construct the SubProfiles Container
	Cur=Doc->GetSession()->GetSubProfilesCursor(lang);
	GProfileCursor cur=Doc->GetSession()->GetProfilesCursor();
	SubProfiles=new RStd::RContainer<GSubProfile,unsigned int,false,true>(cur.GetNb());
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

	//  check the modified groups, the groups subject and the well grouped profiles.
	for (Groups->Start(); !Groups->End(); Groups->Next())
	{
		CheckModifiedGroups((*Groups)());
		SetGroupsSubject((*Groups)());
		CheckWellGroupedSubProfiles((*Groups)());
	}

	// Solutions Part
	Solution = new QGGroupsHistory(TabWidget,Groups->GetPtrAt(0));
	sprintf(tmp,"Solution (%u/%u) ",CurId,MaxGen);
	TabWidget->insertTab(Solution,tmp);
	connect(Solution,SIGNAL(doubleClicked(QListViewItem*)),parent->parent()->parent(),SLOT(slotHandleItem(QListViewItem*)));
	Solution->setGroups(Groups->GetPtrAt(0));
	Solution->setChanged();

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
//	TabWidget->resize(size());
//	TabWidget->resize(width()/2,height()/2);
//	SimsView->resize(width()/2,height()/2);

	TabWidget->setGeometry(QRect(0, 0,(this->width())/2,this->height()));
	// Sims view
	SimsView->setGeometry(QRect((this->width())/2, 0,(this->width())/2,this->height()));
	
}



//-----------------------------------------------------------------------------
void KViewHistory::slotSelectedSetChanged(QListViewItem* item)
{

	GIWordsWeightsHistory* tmp;
	QListViewItemType* itemtype=((QListViewItemType*)item);

	//check if the item is a subprofile
	if (!itemtype->Type==QListViewItemType::tGiwwh)  return;

	// check if item is selected or deselected
	tmp=SelectedItems->GetPtr(itemtype->Obj.Giwwh);
	if(tmp)
		SelectedItems->DeletePtr(itemtype->Obj.Giwwh);
	else
		SelectedItems->InsertPtr(itemtype->Obj.Giwwh);

	// update the SimsView listview.
	DisplaySimilarities();

}


//-----------------------------------------------------------------------------
void KViewHistory::DisplaySimilarities(void)
{
	unsigned int i,j;
	double similarity;
	GIWordsWeightsHistory** giwwh1, **giwwh2;
	QListViewItem* sim;

	SimsView->clear();

	for (i=SelectedItems->NbPtr, giwwh1=SelectedItems->Tab; i--;giwwh1++)
	{
		for (j=i, giwwh2=giwwh1+1; j--; giwwh2++)
		{
			if (Global) similarity=(*giwwh1)->SimilarityIdf(*giwwh2, otSubProfile,Lang) ;
			else similarity=(*giwwh1)->Similarity(*giwwh2);
			sim=new QListViewItem(SimsView, QString(itoa((*giwwh1)->GetId())), QString(itoa((*giwwh2)->GetId())), QString(dtoa(similarity)));
		}
	}
}


//-----------------------------------------------------------------------------
void KViewHistory::CheckModifiedGroups(GGroupsHistory* grps)
{
	RStd::RContainer<GIWordsWeightsHistory, unsigned int, false, true>* lastsubs;
	GGroupsHistory* lastgroups;
	GGroupHistory* grp, *lastgroup;
	GIWordsWeightsHistory* sub, *lastsub;
	unsigned int  lastcurid;
	
	// if the grps is the frst historic one, return
	if (grps->GetId()==MinGen) return;

	//get the last groups and put its profiles in a container
	lastsubs=new RStd::RContainer<GIWordsWeightsHistory, unsigned int, false, true>(10,5);
	lastgroups=Groups->GetPtr(grps->GetId()-1);
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
		
		//if a couple od subprofiles doesn't belong to a same group in the current and hsitoric groups
		// then the group is modified.
		lastcurid=lastsub->GetParent()->GetId();
		while(grp->GetModified()==false&&!grp->End())
		{
			lastsub=lastsubs->GetPtr((*grp)()->GetId());
			// if the subprofiles is a new one, group is modified
			if (!lastsub) grp->SetModified(true);
			
			//if the lastcurid has chnaged , group is modified
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
	RStd::RContainer<GSubject, unsigned int, false, true>* subjects;


	//find the dominant subject
	for (grps->Start(); !grps->End(); grps->Next())
	{
		grp=(*grps)();
		maxoccur=knownsubject=0;

		// find all the subjects contained in the group.
		subjects=new RStd::RContainer<GSubject, unsigned int, false, true>(5,2);
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
}
