/*

	GALILEI Research Project

	KViewUsers.cpp

	Window for manipulating the users - Implementation.

	Copyright 2001-2010 by Pascal Francq (pascal@francq.info).
	Copyright 2001-2008 by the Université Libre de Bruxelles (ULB).

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
#include <ggalileiapp.h>
#include <guser.h>
#include <gprofile.h>
#include <gstorage.h>
using namespace GALILEI;
using namespace R;
using namespace std;


//-----------------------------------------------------------------------------
// include files for Qt/KDE
#include <QtGui/QInputDialog>


//-----------------------------------------------------------------------------
// application specific includes
#include <kviewusers.h>
#include <kgalileicenter.h>



//-----------------------------------------------------------------------------
//
// class KViewUsers
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
KViewUsers::KViewUsers(void)
	: QMdiSubWindow(), Ui_KViewUsers()
{
	QWidget* ptr=new QWidget();
	setupUi(ptr);
	setWidget(ptr);
	setAttribute(Qt::WA_DeleteOnClose);
	setWindowTitle("Users");
	connect(NewUser,SIGNAL(clicked()),this,SLOT(slotAddUser()));
	connect(ModifyUser,SIGNAL(clicked()),this,SLOT(slotModifyUser()));
	connect(NewProfile,SIGNAL(clicked()),this,SLOT(slotAddProfile()));
	connect(ModifyProfile,SIGNAL(clicked()),this,SLOT(slotModifyProfile()));
	connect(List,SIGNAL(Show(GProfile*)),dynamic_cast<KGALILEICenter*>(GALILEIApp),SLOT(showProfile(GProfile*)));
	List->Set(QGObjectsList::Users);
}


//-----------------------------------------------------------------------------
void KViewUsers::update(void)
{
	List->Set(QGObjectsList::Users);
}


//-----------------------------------------------------------------------------
void KViewUsers::slotAddUser(void)
{
	bool Ok;
	QString Name(QInputDialog::getText(this,"New User", "Enter the name:",QLineEdit::Normal,QString(),&Ok));
	if(Ok&&!Name.isEmpty())
	{
		GALILEIApp->GetSession()->InsertUser(new GUser(cNoRef,FromQString(Name),FromQString(Name)));
		update();
	}
}


//-----------------------------------------------------------------------------
void KViewUsers::slotModifyUser(void)
{
	GUser* usr(List->GetCurrentUser());
	if(!usr) return;
	bool Ok;
	QString Name(QInputDialog::getText(this,"Modify User", "Enter the new name:",QLineEdit::Normal,ToQString(usr->GetName()),&Ok));
	if(Ok&&!Name.isEmpty())
	{
		usr->SetName(FromQString(Name));
		GALILEIApp->GetSession()->GetStorage()->SaveUser(usr);
		List->currentItem()->setText(0,Name);
	}
}


//-----------------------------------------------------------------------------
void KViewUsers::slotAddProfile(void)
{
	GUser* usr(List->GetCurrentUser());
	if(!usr) return;
	bool Ok;
	QString Name(QInputDialog::getText(this,"Add Profile to "+ToQString(usr->GetName()), "Enter the profile name:",QLineEdit::Normal,QString(),&Ok));
	if(Ok&&!Name.isEmpty())
	{
		GALILEIApp->GetSession()->InsertProfile(new GProfile(usr,ptInterest,FromQString(Name),true));
		GALILEIApp->GetSession()->GetStorage()->SaveUser(usr);
		update();
	}
}


//-----------------------------------------------------------------------------
void KViewUsers::slotModifyProfile(void)
{
	GProfile* prof(List->GetCurrentProfile());
	if(!prof) return;
	bool Ok;
	QString Name(QInputDialog::getText(this,"Modify Profile", "Enter the new name:",QLineEdit::Normal,ToQString(prof->GetName()),&Ok));
	if(Ok&&!Name.isEmpty())
	{
		prof->SetName(FromQString(Name));
		GALILEIApp->GetSession()->GetStorage()->SaveProfile(prof);
		List->currentItem()->setText(0,Name);
	}
}


//-----------------------------------------------------------------------------
KViewUsers::~KViewUsers(void)
{
}
