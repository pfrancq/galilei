/*

	GALILEI Research Project

	KViewGroups.cpp

	Window to manipulate the groups - Implementation.

	Copyright 2001 by the Universit�Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).
		David Wartel (dwartel@ulb.ac.be).

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
#include <profiles/guser.h>
#include <profiles/gprofile.h>
#include <profiles/gsubprofile.h>
#include <groups/ggroup.h>
#include <infos/glang.h>
#include <groups/ggroups.h>
#include <sessions/gsession.h>
#include <frontend/kde/qlistviewitemtype.h>
#include <frontend/kde/rqt.h>
using namespace GALILEI;
using namespace R;


//-----------------------------------------------------------------------------
// includes files for Qt
#include <qpixmap.h>


//-----------------------------------------------------------------------------
// include files for KDE
#include <kiconloader.h>


//-----------------------------------------------------------------------------
// include files for current application
#include "kviewgroups.h"
#include "qsessionprogress.h"
#include "kdoc.h"



//-----------------------------------------------------------------------------
//
// class KViewGroups
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
KViewGroups::KViewGroups(KDoc* doc,QWidget* parent,const char* name,int wflags)
	: KView(doc,parent,name,wflags)
{
	setCaption("List of Groups");
	setIcon(QPixmap(KGlobal::iconLoader()->loadIcon("kmultiple.png",KIcon::Small)));
	Groups = new QListView(this);
	Groups->resize(size());
	Groups->addColumn(QString("Profiles"));
	Groups->addColumn(QString("Users"));
	Groups->addColumn(QString("Attached"));
	Groups->setRootIsDecorated(true);
	connect(Groups,SIGNAL(doubleClicked(QListViewItem*)),parent->parent()->parent(),SLOT(slotHandleItem(QListViewItem*)));
	ConstructGroups();
}


//-----------------------------------------------------------------------------
GGroup* KViewGroups::GetCurrentGroup(void)
{
	QListViewItemType* t;

	t=(QListViewItemType*)Groups->selectedItem();
	if(!t)
		return(0);
	if(t->Type!=QListViewItemType::tGroup)
		return(0);
	return(t->Obj.Group);
}


//-----------------------------------------------------------------------------
void KViewGroups::ConstructGroups(void)
{
	GFactoryLangCursor CurLang;
	GLang* lang;
	char sDate[20];
	RDate d;
	GSubProfileCursor Sub;

	Groups->clear();
	CurLang=Doc->GetSession()->GetLangs()->GetLangsCursor();
	for(CurLang.Start();!CurLang.End();CurLang.Next())
	{
		lang=CurLang()->GetPlugin();
		if(!lang) continue;
		GGroupCursor grs=Doc->GetSession()->GetGroupsCursor(lang);
		QListViewItemType* grsitem = new QListViewItemType(Groups,ToQString(lang->GetName()));
		grsitem->setPixmap(0,QPixmap(KGlobal::iconLoader()->loadIcon("locale.png",KIcon::Small)));
		for (grs.Start(); !grs.End(); grs.Next())
		{
			GGroup* gr=grs();
			QListViewItemType* gritem= new QListViewItemType(gr,grsitem,"Group");
			gritem->setPixmap(0,QPixmap(KGlobal::iconLoader()->loadIcon("window_new.png",KIcon::Small)));
			Sub=grs()->GetSubProfilesCursor();
			for(Sub.Start(); !Sub.End(); Sub.Next())
			{
				GSubProfile* sub=Sub();
				d=sub->GetAttached();
				sprintf(sDate,"%i/%i/%i",d.GetDay(),d.GetMonth(),d.GetYear());
				QListViewItemType* subitem=new QListViewItemType(sub->GetProfile(),gritem,ToQString(sub->GetProfile()->GetName()),ToQString(sub->GetProfile()->GetUser()->GetFullName()),sDate);
				subitem->setPixmap(0,QPixmap(KGlobal::iconLoader()->loadIcon("find.png",KIcon::Small)));
			}
		}
	}
}


//-----------------------------------------------------------------------------
void KViewGroups::update(unsigned int cmd)
{
	if(cmd!=2) return;
	ConstructGroups();
}


//-----------------------------------------------------------------------------
void KViewGroups::resizeEvent(QResizeEvent *)
{
	if(Groups->height())
	{
		int l=width()/3;
		Groups->setGeometry(0,0,this->width(),this->height());
		Groups->setColumnWidth(0,l-2);
		Groups->setColumnWidth(1,this->width()-2*l-1);
		Groups->setColumnWidth(2,l-2);
	}
}


//-----------------------------------------------------------------------------
KViewGroups::~KViewGroups(void)
{
}
