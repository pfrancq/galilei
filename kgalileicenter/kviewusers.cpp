/*

	GALILEI Research Project

	KViewUsers.cpp

	Window for manipulating the users - Implementation.

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
// include files for R
#include <rqt.h>
#include <rcursor.h>


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <gsession.h>
#include <guser.h>
#include <gprofile.h>
#include <gstorage.h>
using namespace GALILEI;
using namespace R;
using namespace std;


//-----------------------------------------------------------------------------
// include files for Qt
#include <qpixmap.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qinputdialog.h>


//-----------------------------------------------------------------------------
// include files for KDE
#include <kiconloader.h>


//-----------------------------------------------------------------------------
// application specific includes
#include "kviewusers.h"
#include "kdoc.h"



//-----------------------------------------------------------------------------
//
// class KViewUsers
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
KViewUsers::KViewUsers(KDoc* doc,QWidget* parent,const char* name,int wflags)
	: KView(doc,parent,name,wflags)
{
	setCaption("List of users");
	setIcon(QPixmap(KGlobal::iconLoader()->loadIcon("kdmconfig.png",KIcon::Small)));

	// Main Layout
	QVBoxLayout* MainLayout = new QVBoxLayout(this,0,0,"MainLayout");

	// Search Bar
	QHBoxLayout* SearchLayout = new QHBoxLayout(0,0,0,"SearchLayout");
	SearchLayout->setAlignment(Qt::AlignTop);
	NewUser=new QPushButton(this);
	NewUser->setAutoDefault(TRUE);
	NewUser->setText("New User");
	connect(NewUser,SIGNAL(clicked()),this,SLOT(slotAddUser()));
	SearchLayout->addWidget(NewUser);
	SearchLayout->addItem(new QSpacerItem(10, 10, QSizePolicy::Fixed, QSizePolicy::Minimum ));
	ModifyUser=new QPushButton(this);
	ModifyUser->setAutoDefault(TRUE);
	ModifyUser->setText("Modify User");
	connect(ModifyUser,SIGNAL(clicked()),this,SLOT(slotModifyUser()));
	SearchLayout->addWidget(ModifyUser);
	SearchLayout->addItem(new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum ));
	NewProfile=new QPushButton(this);
	NewProfile->setAutoDefault(TRUE);
	NewProfile->setText("New Profile");
	connect(NewProfile,SIGNAL(clicked()),this,SLOT(slotAddProfile()));
	SearchLayout->addWidget(NewProfile);
	SearchLayout->addItem(new QSpacerItem(10, 10, QSizePolicy::Fixed, QSizePolicy::Minimum ));
	ModifyProfile=new QPushButton(this);
	ModifyProfile->setAutoDefault(TRUE);
	ModifyProfile->setText("Modify Profile");
	connect(ModifyProfile,SIGNAL(clicked()),this,SLOT(slotModifyProfile()));
	SearchLayout->addWidget(ModifyProfile);
//	SearchLayout->addItem(new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum ));
    MainLayout->addLayout(SearchLayout);

	// Users
	Users = new QListView(this);
	Users->resize(size());
	Users->addColumn(QString("User"));
	Users->setRootIsDecorated(true);
	MainLayout->addWidget(Users);
	connect(Users,SIGNAL(doubleClicked(QListViewItem*)),parent->parent()->parent(),SLOT(slotHandleItem(QListViewItem*)));
	CreateUsersListView();
}


//-----------------------------------------------------------------------------
GUser* KViewUsers::GetCurrentUser(void)
{
	QListViewItemType* t;

	t=(QListViewItemType*)Users->selectedItem();
	if(!t)
		return(0);
	if(t->Type!=QListViewItemType::tUser)
		return(0);
	return(t->Obj.User);
}


//-----------------------------------------------------------------------------
void KViewUsers::CreateUsersListView(void)
{
	R::RCursor<GUser> CurUsr=Doc->GetSession()->GetUsers();
	QListViewItemType* useritem=0;
	QListViewItemType* prof;

	for(CurUsr.Start();!CurUsr.End();CurUsr.Next())
	{
		useritem = new QListViewItemType(CurUsr(),Users,ToQString(CurUsr()->GetFullName()));
		useritem->setPixmap(0,QPixmap(KGlobal::iconLoader()->loadIcon("personal.png",KIcon::Small)));
		RCursor<GProfile> cProfile(*CurUsr());
		for(cProfile.Start();!cProfile.End();cProfile.Next())
		{
			prof=new QListViewItemType(cProfile(), useritem,ToQString(cProfile()->GetName()));
			prof->setPixmap(0,QPixmap(KGlobal::iconLoader()->loadIcon("kdict.png",KIcon::Small)));
		}
	}
}


//-----------------------------------------------------------------------------
void KViewUsers::update(unsigned int cmd)
{
	if(cmd!=1) return;
	Users->clear();
	CreateUsersListView();
}


//-----------------------------------------------------------------------------
void KViewUsers::slotAddUser(void)
{
	bool Ok;
	QString Name=QInputDialog::getText("New User", "Enter the name:",QLineEdit::Normal,QString::null,&Ok,this);
	if(Ok&&!Name.isEmpty())
	{
		GSession* session=getDocument()->GetSession();
		session->InsertUser(new GUser(cNoRef,FromQString(Name),FromQString(Name)));
		update(1);
	}
}


//-----------------------------------------------------------------------------
void KViewUsers::slotModifyUser(void)
{
	QListViewItemType* t((QListViewItemType*)Users->selectedItem());
	if((!t)||(t->Type!=QListViewItemType::tUser))
		return;
	GUser* usr=t->Obj.User;
	bool Ok;
	QString Name=QInputDialog::getText("Modify User", "Enter the new name:",QLineEdit::Normal,ToQString(usr->GetName()),&Ok,this);
	if(Ok&&!Name.isEmpty())
	{
		GSession* session=getDocument()->GetSession();
		GStorage* save=session->GetStorage();
		usr->SetName(FromQString(Name));
		save->SaveUser(usr);
		t->setText(0,Name);
	}
}


//-----------------------------------------------------------------------------
void KViewUsers::slotAddProfile(void)
{
	QListViewItemType* t((QListViewItemType*)Users->selectedItem());
	if((!t)||(t->Type!=QListViewItemType::tUser))
		return;
	GUser* usr=t->Obj.User;
	bool Ok;
	QString Name=QInputDialog::getText("Add Profile to "+ToQString(usr->GetName()), "Enter the profile name:",QLineEdit::Normal,QString::null,&Ok,this);
	if(Ok&&!Name.isEmpty())
	{
		GSession* session=getDocument()->GetSession();
		GStorage* save=session->GetStorage();
		session->InsertProfile(new GProfile(usr,cNoRef,FromQString(Name),cNoRef,RDate::Null,RDate::GetToday(),RDate::Null,true,20));
		save->SaveUser(usr);
		update(1);
	}
}


//-----------------------------------------------------------------------------
void KViewUsers::slotModifyProfile(void)
{
	QListViewItemType* t((QListViewItemType*)Users->selectedItem());
	if((!t)||(t->Type!=QListViewItemType::tProfile))
		return;
	GProfile* prof=t->Obj.Profile;
	bool Ok;
	QString Name=QInputDialog::getText("Modify Profile", "Enter the new name:",QLineEdit::Normal,ToQString(prof->GetName()),&Ok,this);
	if(Ok&&!Name.isEmpty())
	{
		GSession* session=getDocument()->GetSession();
		GStorage* save=session->GetStorage();
		prof->SetName(FromQString(Name));
		save->SaveProfile(prof);
		t->setText(0,Name);
	}
}


//-----------------------------------------------------------------------------
KViewUsers::~KViewUsers(void)
{
}
