/*

	GALILEI Research Project

	KViewThCommunities.cpp

	Window to manipulate theoretical communities - Implementation.

	Copyright 2001-2008 by the Université Libre de Bruxelles.

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
#include <gprofile.h>
#include <gsubjects.h>
#include <gsubject.h>
#include <gcommunity.h>
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
#include "kviewthcommunities.h"
#include "qsessionprogress.h"
#include "kdoc.h"



//-----------------------------------------------------------------------------
//
// class KViewThCommunities
//
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
KViewThCommunities::KViewThCommunities(KDoc* doc,GSubjects* subjects,QWidget* parent,const char* name,int wflags)
	: KView(doc,parent,name,wflags), Subjects(subjects)
{
	setIcon(QPixmap(KGlobal::iconLoader()->loadIcon("kmultiple.png",KIcon::Small)));

	// initialization of the tab widget
	Infos=new QTabWidget(this);
	Infos->resize(size());

	// Theoretical communities
	thCommunities = new QListView(this);
	Infos->insertTab(thCommunities,"Ideal Communities");
	thCommunities->resize(size());
	thCommunities->addColumn(QString("Profiles"));
	thCommunities->addColumn(QString("Users"));
	thCommunities->setRootIsDecorated(true);
	connect(thCommunities,SIGNAL(doubleClicked(QListViewItem*)),parent->parent()->parent(),SLOT(slotHandleItem(QListViewItem*)));
	ConstructThCommunities();

	// Computed communities
	prCommunities = new QListView(this);
	Infos->insertTab(prCommunities,"Computed Communities");
	prCommunities->resize(size());
	prCommunities->addColumn(QString("Profiles"));
	prCommunities->addColumn(QString("Precision"));
	prCommunities->addColumn(QString("Recall"));
	prCommunities->setRootIsDecorated(true);
	connect(prCommunities,SIGNAL(doubleClicked(QListViewItem*)),parent->parent()->parent(),SLOT(slotHandleItem(QListViewItem*)));
	ConstructCommunities();

}


//-----------------------------------------------------------------------------
GCommunity* KViewThCommunities::GetCurrentGroup(void)
{
	QListViewItemType* t;

	t=(QListViewItemType*)thCommunities->selectedItem();
	if(!t)
		return(0);
	if(t->Type!=otCommunity)
		return(0);
	return(t->Obj.Community);
}


//-----------------------------------------------------------------------------
void KViewThCommunities::ConstructThCommunities(void)
{
	RCursor<GProfile> Sub;
	QListViewItemType* gritem=0;

	thCommunities->clear();

	// Go through each subjects
	R::RCursor<GSubject> Grps(getDocument()->GetSession()->GetSubjects()->GetNodes());
	for(Grps.Start();!Grps.End();Grps.Next())
	{
		if(!Grps()->GetNbObjs(otProfile))
			continue;

		gritem= new QListViewItemType(Grps(),thCommunities,ToQString(Grps()->GetName())+" - "+QString::number(Grps()->GetNbObjs(otProfile)));
		gritem->setPixmap(0,QPixmap(KGlobal::iconLoader()->loadIcon("window_new.png",KIcon::Small)));

		// If the subject has no profiles -> next one.
		if(!Grps()->GetNbObjs(otProfile))
			continue;
		Sub=Grps()->GetObjs(static_cast<GProfile*>(0));
		for(Sub.Start(); !Sub.End(); Sub.Next())
		{
			GProfile* sub=Sub();
			QListViewItemType* subitem=new QListViewItemType(sub,gritem,ToQString(sub->GetName()),ToQString(sub->GetUser()->GetFullName()));
			subitem->setPixmap(0,QPixmap(KGlobal::iconLoader()->loadIcon("find.png",KIcon::Small)));
		}
		if(!gritem->childCount())
			thCommunities->takeItem(gritem);
	}
}


//-----------------------------------------------------------------------------
void KViewThCommunities::ConstructCommunities(void)
{
	char tmp1[70];
	char tmp2[30];
	RCursor<GProfile> Sub;

	Doc->GetSession()->GetSubjects()->Compare(otCommunity);
	sprintf(tmp1,"Clustring Comparison: Precision=%1.3f - Recall=%1.3f - Total=%1.3f",Doc->GetSession()->GetSubjects()->GetPrecision(otCommunity),Doc->GetSession()->GetSubjects()->GetRecall(otCommunity),Doc->GetSession()->GetSubjects()->GetTotal(otCommunity));
	setCaption(tmp1);
	prCommunities->clear();

	R::RCursor<GCommunity> grs=Doc->GetSession()->GetCommunities();
	for (grs.Start(); !grs.End(); grs.Next())
	{
		GCommunity* gr=grs();
		sprintf(tmp1,"Precision: %1.3f",Doc->GetSession()->GetSubjects()->GetPrecision(gr));
		sprintf(tmp2,"Recall: %1.3f",Doc->GetSession()->GetSubjects()->GetRecall(gr));
		QListViewItemType* gritem= new QListViewItemType(gr,prCommunities,"Group ("+QString::number(gr->GetId())+") - "+QString::number(gr->GetNbObjs()),tmp1,tmp2);
		gritem->setPixmap(0,QPixmap(KGlobal::iconLoader()->loadIcon("window_new.png",KIcon::Small)));
		Sub=grs()->GetObjs();
		for(Sub.Start(); !Sub.End(); Sub.Next())
		{
			GProfile* sub=Sub();
			QListViewItemType* subitem=new QListViewItemType(sub,gritem,ToQString(sub->GetName())+" ("+ToQString(sub->GetUser()->GetFullName())+")");
			subitem->setPixmap(0,QPixmap(KGlobal::iconLoader()->loadIcon("find.png",KIcon::Small)));
		}
	}
}


//-----------------------------------------------------------------------------
void KViewThCommunities::update(tObjType type)
{
	if(type!=otCommunity) return;
	ConstructCommunities();
}


//-----------------------------------------------------------------------------
void KViewThCommunities::resizeEvent(QResizeEvent *)
{
	Infos->resize(size());
}


//-----------------------------------------------------------------------------
KViewThCommunities::~KViewThCommunities(void)
{
}
