/*

	GALILEI Research Project

	KViewGroup.cpp

	Window to manipulate a specific group - Implementation.

	(C) 2001 by Pascal Francq

	Version $Revision$

	Last Modify: $Date$

*/



//-----------------------------------------------------------------------------
// include files for GALILEI
#include <groups/ggroup.h>
#include <profiles/guser.h>
#include <profiles/gprofile.h>
#include <profiles/gsubprofile.h>
#include <galilei/qlistviewitemtype.h>
using namespace GALILEI;


//-----------------------------------------------------------------------------
// includes files for Qt
#include <qpixmap.h>


//-----------------------------------------------------------------------------
// include files for current application
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

	sprintf(title,"Group (Id=%u)",Group->GetId());
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

	// Initialisation of the Profile's Widget
	Profiles = new QListView(Infos);
	Infos->insertTab(Profiles,"Profiles");
	Profiles->addColumn(QString("Profiles"));
	Profiles->addColumn(QString("Users"));
	Profiles->addColumn(QString("Attached"));
	Profiles->setRootIsDecorated(true);
	connect(Profiles,SIGNAL(doubleClicked(QListViewItem*)),parent->parent()->parent(),SLOT(slotHandleItem(QListViewItem*)));
	ConstructProfiles();
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
void KViewGroup::update(unsigned int cmd)
{
	if(cmd!=2) return;
	ConstructProfiles();
	ConstructGeneral();
}


//-----------------------------------------------------------------------------
void KViewGroup::resizeEvent(QResizeEvent *)
{
	Infos->resize(size());
}
