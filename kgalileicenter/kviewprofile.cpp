/*

	GALILEI Research Project

	KViewProfile.cpp

	Window to manipulate a given profile - Implementation.

	Copyright 2001 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).
		David Wartel (dwartel@ulb.ac.be).

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
// includes files for Qt
#include <qpixmap.h>
#include <qtabwidget.h>


//-----------------------------------------------------------------------------
// include files for R Project
#include <rstd/rcontainercursor.h>
using namespace R;


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <langs/glang.h>
#include <galilei/qgprofile.h>
#include <profiles/guser.h>
#include <profiles/gprofile.h>
#include <profiles/gsubprofile.h>
#include <docs/gdoc.h>
#include <profiles/gprofdoc.h>
#include <groups/ggroup.h>
#include <groups/ggroups.h>
#include <sessions/gsession.h>
#include <galilei/qlistviewitemtype.h>
#include <galilei/qgsubprofiledescs.h>
using namespace GALILEI;


//-----------------------------------------------------------------------------
// include files for current application
#include "kviewprofile.h"
#include "qsessionprogress.h"
#include "kdoc.h"



//-----------------------------------------------------------------------------
//
// class KViewProfile
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
KViewProfile::KViewProfile(GProfile* profile,KDoc* doc,QWidget* parent,const char* name,int wflags)
	: KView(doc,parent,name,wflags), Profile(profile), Fdbks(0)
{
	// Window initialisation
	setIcon(QPixmap("/usr/share/icons/hicolor/16x16/actions/find.png"));
	setCaption("Profile \""+QString(Profile->GetName())+"\"");

	// initialisation of the tab widget
	Infos=new QTabWidget(this);
	Infos->resize(size());

	// Initialisation of the General Information Widget
	General = new QGProfile(Infos,Profile);
	Infos->insertTab(General,"General Information");

	// Initialisation of the User Widget
	User = new QListView(Infos);
	Infos->insertTab(User,"User");
	User->addColumn("Variable");
	User->addColumn("Value");
	ConstructUser();

	// Initialisation of the Descriptions Widget
	Desc=new QGSubProfileDescs(Infos,Doc->GetSession(),Profile,sdVector);

	// Initialisation of the Groups Widget
	Groups=new QListView(Infos);
	Infos->insertTab(Groups,"Groups");
	Groups->addColumn(QString("Profiles"));
	Groups->addColumn(QString("Users"));
	Groups->addColumn(QString("Attached"));
	Groups->setRootIsDecorated(true);
	connect(Groups,SIGNAL(doubleClicked(QListViewItem*)),parent->parent()->parent(),SLOT(slotHandleItem(QListViewItem*)));
	ConstructGroups();

	// Initialisation of the Feedbacks Widget
	if(Doc->GetSession()->IsFdbksLoad())
	{
		Fdbks = new QListView(Infos);
		Infos->insertTab(Fdbks,"Documents");
		Fdbks->addColumn(QString("Document"));
		Fdbks->addColumn(QString("URL"));
		Fdbks->addColumn(QString("Date"));
		Fdbks->setRootIsDecorated(true);
		connect(Fdbks,SIGNAL(doubleClicked(QListViewItem*)),parent->parent()->parent(),SLOT(slotHandleItem(QListViewItem*)));
		ConstructFdbks();
	}
}


//-----------------------------------------------------------------------------
void KViewProfile::ConstructFdbks(void)
{
	QListViewItem *p;
	const RDate* d;
	char sDate[20];
	GProfDocCursor Docs;
	GSubProfileCursor SubCur;

	if(!Fdbks) return;

	// Init different judgements
	Fdbks->clear();
	QListViewItemType* ok= new QListViewItemType(Fdbks, "OK Judgements");
	ok->setPixmap(0,QPixmap("/usr/share/icons/hicolor/16x16/actions/ok.png"));
	QListViewItemType* ko= new QListViewItemType(Fdbks, "KO Judgements");
	ko->setPixmap(0,QPixmap("/usr/share/icons/hicolor/16x16/actions/remove.png"));
	QListViewItemType* n= new QListViewItemType(Fdbks, "N Judgements");
	n->setPixmap(0,QPixmap("/usr/share/icons/hicolor/16x16/actions/view_tree.png"));
	QListViewItemType* hs= new QListViewItemType(Fdbks, "HS Judgements");
	hs->setPixmap(0,QPixmap("/usr/share/icons/hicolor/16x16/actions/stop.png"));

	// Add Judgements
	SubCur=Profile->GetSubProfilesCursor();
	for (SubCur.Start(); !SubCur.End(); SubCur.Next())
	{
		Docs=SubCur()->GetProfDocCursor();
		for(Docs.Start();!Docs.End();Docs.Next())
		{
			switch(Docs()->GetFdbk())
			{
				case djOK:
					p=ok;
					break;
				case djKO:
					p=ko;
					break;
				case djNav:
					p=n;
					break;
				case djOutScope:
					p=hs;
					break;
				default:
					p=0;
					break;
			}
			if(!p) continue;
			d=Docs()->GetUpdated();
			sprintf(sDate,"%i/%i/%i",d->GetDay(),d->GetMonth(),d->GetYear());
			QListViewItemType* prof = new QListViewItemType(Docs()->GetDoc(),p,Docs()->GetDoc()->GetName(),Docs()->GetDoc()->GetURL(),sDate);
			prof->setPixmap(0,QPixmap("/usr/share/icons/hicolor/16x16/apps/konqueror.png"));
		}
	}      
}

//-----------------------------------------------------------------------------
void KViewProfile::ConstructGroups(void)
{
	GSubProfile* sub;
	RContainerCursor<GLang,unsigned int,true,true> CurLang(Doc->GetSession()->GetLangs());
	char sDate[20];
	const RDate* d;

	Groups->clear();
	for(CurLang.Start();!CurLang.End();CurLang.Next())
	{
		GGroups* grs=Doc->GetSession()->GetGroups(CurLang());
		QListViewItemType* grsitem = new QListViewItemType(Groups,CurLang()->GetName());
		grsitem->setPixmap(0,QPixmap("/usr/share/icons/hicolor/16x16/apps/locale.png"));
		sub=Profile->GetSubProfile(CurLang());
		if(!sub) continue;
		for (grs->Start(); !grs->End(); grs->Next())
		{
			GGroup* gr=(*grs)();
			if(!gr->IsIn(sub)) continue;
			for(gr->Start(); !gr->End(); gr->Next())
			{
				GSubProfile* sub=(*gr)();
				d=sub->GetAttached();
				sprintf(sDate,"%i/%i/%i",d->GetDay(),d->GetMonth(),d->GetYear());
				QListViewItemType* subitem=new QListViewItemType(sub->GetProfile(),grsitem,sub->GetProfile()->GetName(),sub->GetProfile()->GetUser()->GetFullName(),sDate);
				subitem->setPixmap(0,QPixmap("/usr/share/icons/hicolor/16x16/actions/find.png"));
			}
		}
	}
}


//-----------------------------------------------------------------------------
void KViewProfile::ConstructUser(void)
{
	const GUser* usr=Profile->GetUser();

	User->clear();
	new QListViewItem(User,"ID",QString(itoa(usr->GetId())));
	new QListViewItem(User,"Full Name",usr->GetFullName());
	new QListViewItem(User,"Name",usr->GetName());
}


//-----------------------------------------------------------------------------
void KViewProfile::update(unsigned int cmd)
{
	if(cmd==1)
	{
		General->slotProfileChanged();
		Desc->slotProfileChanged();
	}
	if(cmd==2)
	{
		ConstructGroups();
	}
}


//-----------------------------------------------------------------------------
void KViewProfile::resizeEvent(QResizeEvent *)
{
	Infos->resize(size());
}


//-----------------------------------------------------------------------------
void KViewProfile::ComputeProfile(void)
{
	QSessionProgressDlg* d=new QSessionProgressDlg(this,Doc->GetSession(),"Compute Profile");
	d->ComputeProfile(Profile);
	Desc->slotProfileChanged();
	General->slotProfileChanged();
}


//-----------------------------------------------------------------------------
KViewProfile::~KViewProfile(void)
{
}
