/*

	GALILEI Research Project

	KViewUsers.cpp

	Window for manipulating the users - Implementation.

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
#include <rstd/rcontainercursor.h>
using namespace RStd;


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <sessions/gsession.h>
#include <profiles/guser.h>
#include <profiles/gprofile.h>
using namespace GALILEI;


//-----------------------------------------------------------------------------
// include files for Qt
#include <qpixmap.h>


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
	setIcon(QPixmap("/usr/share/icons/hicolor/16x16/apps/kdmconfig.png"));
	Users = new QListView(this);
	Users->addColumn(QString("User"));
	Users->setRootIsDecorated(true);
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
	GUserCursor CurUsr=Doc->GetSession()->GetUsersCursor();
	QListViewItemType* useritem=0;
	QListViewItemType* prof;

	for(CurUsr.Start();!CurUsr.End();CurUsr.Next())
	{
		useritem = new QListViewItemType(CurUsr(),Users,CurUsr()->GetFullName());
		useritem->setPixmap(0,QPixmap("/usr/share/icons/hicolor/16x16/apps/personal.png"));
		for (CurUsr()->Start(); !CurUsr()->End(); CurUsr()->Next())
		{
			prof=new QListViewItemType((*CurUsr())(), useritem, (*CurUsr())()->GetName());
			prof->setPixmap(0,QPixmap("/usr/share/icons/hicolor/16x16/apps/kdict.png"));
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
void KViewUsers::resizeEvent(QResizeEvent *)
{
	//resize of the listview
	Users->setGeometry(0,0,this->width(),this->height());
	Users->setColumnWidth(0,this->width()-2);
}


//-----------------------------------------------------------------------------
KViewUsers::~KViewUsers(void)
{
}
