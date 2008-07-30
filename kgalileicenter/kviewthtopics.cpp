/*

	GALILEI Research Project

	KViewThTopics.cpp

	Window to manipulate theoretical topics - Implementation.

	Copyright 2008 by the Université Libre de Bruxelles.

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
// include files for R Project
#include <rtextfile.h>
#include <rqt.h>
using namespace R;


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <gsession.h>
#include <glang.h>
#include <guser.h>
#include <gdoc.h>
#include <gsubjects.h>
#include <gsubject.h>
#include <gtopic.h>
#include <qlistviewitemtype.h>
#include <ggalileiapp.h>
using namespace GALILEI;


//-----------------------------------------------------------------------------
// includes files for Qt
#include <qpixmap.h>
#include <qlistview.h>
#include <qtabwidget.h>


//-----------------------------------------------------------------------------
// include files for KDE
#include <kiconloader.h>


//-----------------------------------------------------------------------------
// include files for current application
#include "kviewthtopics.h"
#include "qsessionprogress.h"
#include "kdoc.h"



//-----------------------------------------------------------------------------
//
// class KViewThTopics
//
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
KViewThTopics::KViewThTopics(KDoc* doc,GSubjects* subjects,QWidget* parent,const char* name,int wflags)
	: KView(doc,parent,name,wflags), Subjects(subjects)
{
	setIcon(QPixmap(KGlobal::iconLoader()->loadIcon("kmultiple.png",KIcon::Small)));

	// initialization of the tab widget
	Infos=new QTabWidget(this);
	Infos->resize(size());

	// Theoretical communities
	thTopics = new QListView(this);
	Infos->insertTab(thTopics,"Ideal Topics");
	thTopics->resize(size());
	thTopics->addColumn(QString("Docs"));
	thTopics->addColumn(QString("Users"));
	thTopics->setRootIsDecorated(true);
	connect(thTopics,SIGNAL(doubleClicked(QListViewItem*)),parent->parent()->parent(),SLOT(slotHandleItem(QListViewItem*)));
	ConstructThTopics();

	// Computed communities
	prTopics = new QListView(this);
	Infos->insertTab(prTopics,"Computed Topics");
	prTopics->resize(size());
	prTopics->addColumn(QString("Docs"));
	prTopics->addColumn(QString("Precision"));
	prTopics->addColumn(QString("Recall"));
	prTopics->setRootIsDecorated(true);
	connect(prTopics,SIGNAL(doubleClicked(QListViewItem*)),parent->parent()->parent(),SLOT(slotHandleItem(QListViewItem*)));
	ConstructTopics();

}


//-----------------------------------------------------------------------------
GTopic* KViewThTopics::GetCurrentGroup(void)
{
	QListViewItemType* t;

	t=(QListViewItemType*)thTopics->selectedItem();
	if(!t)
		return(0);
	if(t->Type!=otTopic)
		return(0);
	return(t->Obj.Topic);
}


//-----------------------------------------------------------------------------
void KViewThTopics::ConstructThTopics(void)
{
	RCursor<GDoc> Sub;
	QListViewItemType* gritem=0;

	thTopics->clear();

	// Go through each subjects
	R::RCursor<GSubject> Grps(getDocument()->GetSession()->GetSubjects()->GetNodes());
	for(Grps.Start();!Grps.End();Grps.Next())
	{
		if(!Grps()->GetNbObjs(otDoc))
			continue;

		gritem= new QListViewItemType(Grps(),thTopics,ToQString(Grps()->GetName())+" - "+QString::number(Grps()->GetNbObjs(otDoc)));
		gritem->setPixmap(0,QPixmap(KGlobal::iconLoader()->loadIcon("window_new.png",KIcon::Small)));

		// If the subject has no profiles -> next one.
		if(!Grps()->GetNbObjs(otDoc))
			continue;
		Sub=Grps()->GetObjs(static_cast<GDoc*>(0));
		for(Sub.Start(); !Sub.End(); Sub.Next())
		{
			GDoc* sub=Sub();
			QListViewItemType* subitem=new QListViewItemType(sub,gritem,ToQString(sub->GetName()),ToQString(sub->GetURL()));
			subitem->setPixmap(0,QPixmap(KGlobal::iconLoader()->loadIcon("find.png",KIcon::Small)));
		}
		if(!gritem->childCount())
			thTopics->takeItem(gritem);
	}
}


//-----------------------------------------------------------------------------
void KViewThTopics::ConstructTopics(void)
{
	char tmp1[70];
	char tmp2[30];
	RCursor<GDoc> Sub;

	Doc->GetSession()->GetSubjects()->Compare(otTopic);
	sprintf(tmp1,"Clustring Comparison: Precision=%1.3f - Recall=%1.3f - Total=%1.3f",Doc->GetSession()->GetSubjects()->GetPrecision(otTopic),Doc->GetSession()->GetSubjects()->GetRecall(otTopic),Doc->GetSession()->GetSubjects()->GetTotal(otTopic));
	setCaption(tmp1);
	prTopics->clear();

	R::RCursor<GTopic> grs=Doc->GetSession()->GetTopics();
	for (grs.Start(); !grs.End(); grs.Next())
	{
		GTopic* gr=grs();
		sprintf(tmp1,"Precision: %1.3f",Doc->GetSession()->GetSubjects()->GetPrecision(gr));
		sprintf(tmp2,"Recall: %1.3f",Doc->GetSession()->GetSubjects()->GetRecall(gr));
		QListViewItemType* gritem= new QListViewItemType(gr,prTopics,"Group ("+QString::number(gr->GetId())+") - "+QString::number(gr->GetNbObjs()),tmp1,tmp2);
		gritem->setPixmap(0,QPixmap(KGlobal::iconLoader()->loadIcon("window_new.png",KIcon::Small)));
		Sub=grs()->GetObjs();
		for(Sub.Start(); !Sub.End(); Sub.Next())
		{
			GDoc* sub=Sub();
			QListViewItemType* subitem=new QListViewItemType(sub,gritem,ToQString(sub->GetName())+" ("+ToQString(sub->GetURL())+")");
			subitem->setPixmap(0,QPixmap(KGlobal::iconLoader()->loadIcon("find.png",KIcon::Small)));
		}
	}
}


//-----------------------------------------------------------------------------
void KViewThTopics::update(tObjType type)
{
	if(type!=otTopic) return;
	ConstructTopics();
}


//-----------------------------------------------------------------------------
void KViewThTopics::resizeEvent(QResizeEvent *)
{
	Infos->resize(size());
}


//-----------------------------------------------------------------------------
KViewThTopics::~KViewThTopics(void)
{
}
