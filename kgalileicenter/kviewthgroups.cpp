/*

	GALILEI Research Project

	KViewThGroups.cpp

	Window to manipulate theoritical groups - Implementation.

	Copyright 2001 by the Universit�Libre de Bruxelles.

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
#include <gsubprofile.h>
#include <ggroup.h>
#include <gsubjects.h>
#include <gsubject.h>
#include <qlistviewitemtype.h>
#include <gpluginmanagers.h>
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
#include "kviewthgroups.h"
#include "qsessionprogress.h"
#include "kdoc.h"



//-----------------------------------------------------------------------------
//
// class KViewThGroups
//
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
KViewThGroups::KViewThGroups(KDoc* doc,GSubjects* subjects,QWidget* parent,const char* name,int wflags)
	: KView(doc,parent,name,wflags), Subjects(subjects)
{
	setIcon(QPixmap(KGlobal::iconLoader()->loadIcon("kmultiple.png",KIcon::Small)));

	// initialisation of the tab widget
	Infos=new QTabWidget(this);
	Infos->resize(size());

	// Theoritic groupement
	thGroups = new QListView(this);
	Infos->insertTab(thGroups,"Ideal Groupement");
	thGroups->resize(size());
	thGroups->addColumn(QString("Profiles"));
	thGroups->addColumn(QString("Users"));
	thGroups->setRootIsDecorated(true);
	connect(thGroups,SIGNAL(doubleClicked(QListViewItem*)),parent->parent()->parent(),SLOT(slotHandleItem(QListViewItem*)));
	ConstructThGroups();

	// Theoritic groupement
	prGroups = new QListView(this);
	Infos->insertTab(prGroups,"Computed Groupement");
	prGroups->resize(size());
	prGroups->addColumn(QString("Profiles"));
	prGroups->addColumn(QString("Precision"));
	prGroups->addColumn(QString("Recall"));
	prGroups->setRootIsDecorated(true);
	connect(prGroups,SIGNAL(doubleClicked(QListViewItem*)),parent->parent()->parent(),SLOT(slotHandleItem(QListViewItem*)));
	ConstructGroups();

}


//-----------------------------------------------------------------------------
GGroup* KViewThGroups::GetCurrentGroup(void)
{
	QListViewItemType* t;

	t=(QListViewItemType*)thGroups->selectedItem();
	if(!t)
		return(0);
	if(t->Type!=QListViewItemType::tGroup)
		return(0);
	return(t->Obj.Group);
}


//-----------------------------------------------------------------------------
void KViewThGroups::ConstructThGroups(void)
{
	RCursor<GSubProfile> Sub;
	QListViewItemType* gritem=0;

	thGroups->clear();

	// Get the active languages
	RCursor<GLang> Langs=GPluginManagers::GetManager<GLangManager>("Lang")->GetPlugIns();

	// Go through each subjects
	R::RCursor<GSubject> Grps(getDocument()->GetSession()->GetSubjects()->GetNodes());
	for(Grps.Start();!Grps.End();Grps.Next())
	{
		if(!Grps()->GetNbProfiles())
			continue;

		gritem= new QListViewItemType(Grps(),thGroups,ToQString(Grps()->GetName()));
		gritem->setPixmap(0,QPixmap(KGlobal::iconLoader()->loadIcon("window_new.png",KIcon::Small)));

		// Go trough each lang
		for(Langs.Start();!Langs.End();Langs.Next())
		{
			// If the subject has no subprofiles -> next one.
			if(!Grps()->GetNbSubProfiles(Langs()))
				continue;

			QListViewItemType* grsitem = new QListViewItemType(gritem,ToQString(Langs()->GetName()));
			grsitem->setPixmap(0,QPixmap(KGlobal::iconLoader()->loadIcon("locale.png",KIcon::Small)));
			Sub=Grps()->GetSubProfiles(Langs());
			for(Sub.Start(); !Sub.End(); Sub.Next())
			{
				GSubProfile* sub=Sub();
				QListViewItemType* subitem=new QListViewItemType(sub->GetProfile(),grsitem,ToQString(sub->GetProfile()->GetName()),ToQString(sub->GetProfile()->GetUser()->GetFullName()));
				subitem->setPixmap(0,QPixmap(KGlobal::iconLoader()->loadIcon("find.png",KIcon::Small)));
			}
			if(!grsitem->childCount())
				gritem->takeItem(grsitem);
		}
	}
}


//-----------------------------------------------------------------------------
void KViewThGroups::ConstructGroups(void)
{
	R::RCursor<GFactoryLang> CurLang;
	GLang* lang;
	char tmp1[70];
	char tmp2[30];
	RCursor<GSubProfile> Sub;

	Doc->GetSession()->GetSubjects()->Compare();
	sprintf(tmp1,"Groupement Comparaison: Precision=%1.3f - Recall=%1.3f - Total=%1.3f",Doc->GetSession()->GetSubjects()->GetPrecision(),Doc->GetSession()->GetSubjects()->GetRecall(),Doc->GetSession()->GetSubjects()->GetTotal());
	setCaption(tmp1);
	prGroups->clear();
	CurLang=GPluginManagers::GetManager<GLangManager>("Lang")->GetFactories();
	for(CurLang.Start();!CurLang.End();CurLang.Next())
	{
		lang=CurLang()->GetPlugin();
		if(!lang) continue;
		R::RCursor<GGroup> grs=Doc->GetSession()->GetGroups(lang);
		QListViewItemType* grsitem = new QListViewItemType(prGroups,ToQString(lang->GetName()));
		grsitem->setPixmap(0,QPixmap(KGlobal::iconLoader()->loadIcon("locale.png",KIcon::Small)));
		for (grs.Start(); !grs.End(); grs.Next())
		{
			GGroup* gr=grs();
			sprintf(tmp1,"Precision: %1.3f",Doc->GetSession()->GetSubjects()->GetPrecision(gr));
			sprintf(tmp2,"Recall: %1.3f",Doc->GetSession()->GetSubjects()->GetRecall(gr));
			QListViewItemType* gritem= new QListViewItemType(gr,grsitem,"Group",tmp1,tmp2);
			gritem->setPixmap(0,QPixmap(KGlobal::iconLoader()->loadIcon("window_new.png",KIcon::Small)));
			Sub=grs()->GetSubProfiles();
			for(Sub.Start(); !Sub.End(); Sub.Next())
			{
				GSubProfile* sub=Sub();
				QListViewItemType* subitem=new QListViewItemType(sub->GetProfile(),gritem,ToQString(sub->GetProfile()->GetName())+" ("+ToQString(sub->GetProfile()->GetUser()->GetFullName())+")");
				subitem->setPixmap(0,QPixmap(KGlobal::iconLoader()->loadIcon("find.png",KIcon::Small)));
			}
		}
	}
}


//-----------------------------------------------------------------------------
void KViewThGroups::update(unsigned int cmd)
{
	if(cmd!=2) return;
	ConstructGroups();
}


//-----------------------------------------------------------------------------
void KViewThGroups::resizeEvent(QResizeEvent *)
{
	Infos->resize(size());
}


//-----------------------------------------------------------------------------
KViewThGroups::~KViewThGroups(void)
{
}
