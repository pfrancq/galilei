/*

	GALILEI Research Project

	KViewGroup.cpp

	Window to manipulate a specific group - Implementation.

	(C) 2001 by Pascal Francq

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
#include <rstd/rstring.h>
using namespace RStd;


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <langs/glang.h>
#include <sessions/gsession.h>
#include <groups/ggroupvector.h>
#include <profiles/guser.h>
#include <profiles/gprofile.h>
#include <profiles/gsubprofile.h>
#include <galilei/qlistviewitemtype.h>
#include <infos/giwordweight.h>
using namespace GALILEI;


//-----------------------------------------------------------------------------
// includes files for Qt
#include <qpixmap.h>


//-----------------------------------------------------------------------------
// include files for current application
#include "kdoc.h"
#include "kviewgroup.h"
#include "qsessionprogress.h"
using namespace RTimeDate;



//-----------------------------------------------------------------------------
//
// class KViewGroup
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
KViewGroup::KViewGroup(GGroup* grp,KDoc* doc,QWidget* parent,const char* name,int wflags)
	: KView(doc,parent,name,wflags), Group(grp)
{
	char title[50];

	sprintf(title,"Group (Id=%u)",grp->GetId());
	setCaption("Group");
	setIcon(QPixmap("/usr/share/icons/hicolor/16x16/actions/window_new.png"));

	// initialisation of the tab widget
	Infos=new QTabWidget(this);
	Infos->resize(size());

	// Initialisation of the General Information Widget
	General = new QListView(Infos);
	Infos->insertTab(General,"General Information");
	General->addColumn("Variable");
	General->addColumn("Value");
	ConstructGeneral();

	// Initialisation of the Profiles Widget
	Profiles = new QListView(Infos);
	Infos->insertTab(Profiles,"Profiles");
	Profiles->addColumn(QString("Profiles"));
	Profiles->addColumn(QString("Users"));
	Profiles->addColumn(QString("Attached"));
	Profiles->setRootIsDecorated(true);
	connect(Profiles,SIGNAL(doubleClicked(QListViewItem*)),parent->parent()->parent(),SLOT(slotHandleItem(QListViewItem*)));
	ConstructProfiles();

	// Initialisation of Description
	// Create the 'Vector' ListView
	Vector = new QListView(this, "Vector" );
	Infos->insertTab(Vector,"Description");
	sprintf(title,"Words (%u)",static_cast<GGroupVector*>(Group)->GetNbNoNull());
	Vector->addColumn(title);
	Vector->addColumn(QString("Weights"));
	Vector->setSorting(2);
	ConstructDescription();
}


//-----------------------------------------------------------------------------
void KViewGroup::ConstructProfiles(void)
{
	char sDate[20];
	const RDate* d;

	Profiles->clear();
	for(Group->Start();!Group->End();Group->Next())
	{
		GSubProfile* sub=(*Group)();
		d=sub->GetAttached();
		sprintf(sDate,"%i/%i/%i",d->GetDay(),d->GetMonth(),d->GetYear());
		QListViewItemType* subitem=new QListViewItemType(sub->GetProfile(),Profiles,sub->GetProfile()->GetName(),sub->GetProfile()->GetUser()->GetFullName(),sDate);
		subitem->setPixmap(0,QPixmap("/usr/share/icons/hicolor/16x16/actions/find.png"));
	}
}


//-----------------------------------------------------------------------------
void KViewGroup::ConstructGeneral(void)
{
	GLang* l;
	char sDate[20];

	General->clear();
	new QListViewItem(General,"ID",QString(itoa(Group->GetId())));
	l=Group->GetLang();
	if(l)
		new QListViewItem(General,"Language",l->GetName());
	else
		new QListViewItem(General,"Language","Unknow");
	switch(Group->GetState())
	{
		case osUpToDate:
			strcpy(sDate,"Up to date");
			break;
		case osUpdated:
			strcpy(sDate,"Updated");
			break;
		case osModified:
			strcpy(sDate,"Modified");
			break;
		case osCreated:
			strcpy(sDate,"Created");
			break;
		case osUnknow:
		default:
			strcpy(sDate,"Unknow");
			break;
	}
	new QListViewItem(General,"Status",sDate);
}

//-----------------------------------------------------------------------------
void KViewGroup::ConstructDescription(void)
{
	GIWordWeightCursor Cur;
	QString W;
	char tmp[20];

	// Change the label of the first column
	sprintf(tmp,"Words (%u)",static_cast<GGroupVector*>(Group)->GetNbNoNull());
	Vector->setColumnText(0,tmp);

	// Read 'Ok'
	Vector->clear();
	Cur=static_cast<GGroupVector*>(Group)->GetVectorCursor();
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		W.setNum(Cur()->GetWeight());
		new QListViewItem(Vector,Doc->GetSession()->GetWord(Cur()->GetId(),Group->GetLang()),W);
	}
}


//-----------------------------------------------------------------------------
void KViewGroup::update(unsigned int cmd)
{
	if(cmd!=2) return;
	ConstructProfiles();
	ConstructGeneral();
	ConstructDescription();
}


//-----------------------------------------------------------------------------
void KViewGroup::resizeEvent(QResizeEvent *)
{
	Infos->resize(size());
}
