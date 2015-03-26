/*

	GALILEI Research Project

	KViewUsers.cpp

	Window for manipulating the users - Implementation.

	Copyright 2001-2015 by Pascal Francq (pascal@francq.info).
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
// include files for Qt
#include <QtGui/QInputDialog>
#include <QResizeEvent>


//-----------------------------------------------------------------------------
// application specific includes
#include <kviewusers.h>
#include <qgalileiwin.h>


//-----------------------------------------------------------------------------
// static sizes
static int Width;
static int Height;



//-----------------------------------------------------------------------------
//
// class KViewUsers
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
KViewUsers::KViewUsers(QGALILEIWin* win)
	: QMdiSubWindow(), Ui_KViewUsers(), Win(win)
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
	connect(List,SIGNAL(Show(GProfile*)),Win,SLOT(showProfile(GProfile*)));
	List->Set(Win->getSession(),QGObjectsList::Users);
	resize(Width,Height);
}


//------------------------------------------------------------------------------
void KViewUsers::createOptions(RConfig& config)
{
	config.InsertParam(new RParamValue("KViewUsers::Width",300));
	config.InsertParam(new RParamValue("KViewUsers::Height",200));
}


//------------------------------------------------------------------------------
void KViewUsers::readOptions(RConfig& config)
{
	Width=config.GetInt("KViewUsers::Width");
	Height=config.GetInt("KViewUsers::Height");
}


//------------------------------------------------------------------------------
void KViewUsers::saveOptions(RConfig& config)
{
	config.SetInt("KViewUsers::Width",Width);
	config.SetInt("KViewUsers::Height",Height);
}


//------------------------------------------------------------------------------
void KViewUsers::resizeEvent(QResizeEvent* resizeEvent)
{
	QMdiSubWindow::resizeEvent(resizeEvent);
	Width=resizeEvent->size().width();
	Height=resizeEvent->size().height();
}


//-----------------------------------------------------------------------------
void KViewUsers::update(void)
{
	List->Set(Win->getSession(),QGObjectsList::Users);
}


//-----------------------------------------------------------------------------
void KViewUsers::slotAddUser(void)
{
	bool Ok;
	QString Name(QInputDialog::getText(this,"New User", "Enter the name:",QLineEdit::Normal,QString(),&Ok));
	if(Ok&&!Name.isEmpty())
	{
		Win->getSession()->InsertObj(new GUser(Win->getSession(),cNoRef,FromQString(Name),FromQString(Name)));
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
		Win->getSession()->SetName(usr,FromQString(Name));
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
		Win->getSession()->InsertObj(new GProfile(Win->getSession(),usr,ptInterest,FromQString(Name),true));
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
		Win->getSession()->SetName(prof,FromQString(Name));
		List->currentItem()->setText(0,Name);
	}
}


//-----------------------------------------------------------------------------
KViewUsers::~KViewUsers(void)
{
}
