/*

	GALILEI Research Project

	KViewThGroups.cpp

	Window to manipulate theoritical groups - Implementation.

	Copyright 2001 by the Université Libre de Bruxelles.

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
// include files for R Project
#include <rstd/rcontainercursor.h>
#include <rstd/rtextfile.h>
using namespace R;


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <sessions/gsession.h>
#include <infos/glang.h>
#include <infos/glangmanager.h>
#include <profiles/guser.h>
#include <profiles/gprofile.h>
#include <profiles/gsubprofile.h>
#include <groups/ggroupvector.h>
#include <groups/ggroups.h>
#include <groups/gsubjecttree.h>
#include <galilei/qlistviewitemtype.h>
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
KViewThGroups::KViewThGroups(KDoc* doc,RContainer<GGroups,unsigned int,true,true>* idealgroup,QWidget* parent,const char* name,int wflags)
	: KView(doc,parent,name,wflags), Groups(0)
{
	Groups=idealgroup;
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
KViewThGroups::KViewThGroups(KDoc* doc,const char* filename,QWidget* parent,const char* name,int wflags)
	: KView(doc,parent,name,wflags), Groups(0)
{
	LoadGroups(filename);
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
void KViewThGroups::LoadGroups(const char* filename)
{
	unsigned int nb;
	unsigned int i,j,id;
	GGroups* groups;
	GGroup* group;
	unsigned int nbprof;
	GLang* lang;
	GProfile* prof;
	GSubProfile* sub;
	GFactoryLangCursor CurLang;

	RTextFile f(filename);
	f>>nb;
	Groups=new RContainer<GGroups,unsigned int,true,true>(nb,nb/2);
	CurLang=Doc->GetSession()->GetLangs()->GetLangsCursor();
	for(CurLang.Start();!CurLang.End();CurLang.Next())
	{
		lang=CurLang()->GetPlugin();
		if(!lang) continue;
		Groups->InsertPtr(new GGroups(lang));
	}
	for(i=0;i<nb;i++)
	{
		lang=Doc->GetSession()->GetLangs()->GetLang(f.GetWord());
		f>>nbprof;
		groups=Groups->GetPtr<const GLang*>(lang);
		groups->InsertPtr(group=new GGroupVector(i,lang));
		for(j=nbprof+1;--j;)
		{
			f>>id;
			prof=Doc->GetSession()->GetProfile(id);
			if(!prof) continue;
			sub=prof->GetSubProfile(lang);
			if(!sub) continue;
			group->InsertPtr(sub);
		}
	}
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
	GFactoryLangCursor CurLang;
	GLang* lang;


	thGroups->clear();
	CurLang=Doc->GetSession()->GetLangs()->GetLangsCursor();
	for(CurLang.Start();!CurLang.End();CurLang.Next())
	{
		lang=CurLang()->GetPlugin();
		if(!lang) continue;
		GGroups* grs=Groups->GetPtr<const GLang*>(lang);
		if(!grs) continue;
		QListViewItemType* grsitem = new QListViewItemType(thGroups,lang->GetName());
		grsitem->setPixmap(0,QPixmap(KGlobal::iconLoader()->loadIcon("locale.png",KIcon::Small)));
		for(grs->Start(); !grs->End(); grs->Next())
		{
			GGroup* gr=(*grs)();
			QListViewItemType* gritem= new QListViewItemType(gr,grsitem,"Group");
			gritem->setPixmap(0,QPixmap(KGlobal::iconLoader()->loadIcon("window_new.png",KIcon::Small)));
			for(gr->Start(); !gr->End(); gr->Next())
			{
				GSubProfile* sub=(*gr)();
				QListViewItemType* subitem=new QListViewItemType(sub->GetProfile(),gritem,sub->GetProfile()->GetName().Latin1(),sub->GetProfile()->GetUser()->GetFullName().Latin1());
				subitem->setPixmap(0,QPixmap(KGlobal::iconLoader()->loadIcon("find.png",KIcon::Small)));
			}
		}
	}
}


//-----------------------------------------------------------------------------
void KViewThGroups::ConstructGroups(void)
{
	GFactoryLangCursor CurLang;
	GLang* lang;
	char tmp1[70];
	char tmp2[30];

	Doc->GetSession()->GetSubjects()->Compare(0);
	sprintf(tmp1,"Groupement Comparaison: Precision=%1.3f - Recall=%1.3f - Total=%1.3f",Doc->GetSession()->GetSubjects()->GetPrecision(),Doc->GetSession()->GetSubjects()->GetRecall(),Doc->GetSession()->GetSubjects()->GetTotal());
	setCaption(tmp1);
	prGroups->clear();
	CurLang=Doc->GetSession()->GetLangs()->GetLangsCursor();
	for(CurLang.Start();!CurLang.End();CurLang.Next())
	{
		lang=CurLang()->GetPlugin();
		if(!lang) continue;
		GGroups* grs=Doc->GetSession()->GetGroups(lang);
		QListViewItemType* grsitem = new QListViewItemType(prGroups,lang->GetName());
		grsitem->setPixmap(0,QPixmap(KGlobal::iconLoader()->loadIcon("locale.png",KIcon::Small)));
		for (grs->Start(); !grs->End(); grs->Next())
		{
			GGroup* gr=(*grs)();
			sprintf(tmp1,"Precision: %1.3f",Doc->GetSession()->GetSubjects()->GetPrecision(gr));
			sprintf(tmp2,"Recall: %1.3f",Doc->GetSession()->GetSubjects()->GetRecall(gr));
			QListViewItemType* gritem= new QListViewItemType(gr,grsitem,"Group",tmp1,tmp2);
			gritem->setPixmap(0,QPixmap(KGlobal::iconLoader()->loadIcon("window_new.png",KIcon::Small)));
			for(gr->Start(); !gr->End(); gr->Next())
			{
				GSubProfile* sub=(*gr)();
				sprintf(tmp1,"%s (%s)",sub->GetProfile()->GetName().Latin1(),sub->GetProfile()->GetUser()->GetFullName().Latin1());
				QListViewItemType* subitem=new QListViewItemType(sub->GetProfile(),gritem,tmp1);
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
	if(Groups);
}
