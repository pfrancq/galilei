/*

	GALILEI Project.

	QGALILEIWin.cpp

	Main windows - Implementation.

	Copyright 2001-2014 by Pascal Francq (pascal@francq.info).

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
// include files for R/GALILEI
#include <rqt.h>


//-----------------------------------------------------------------------------
// include files for Qt
#include <QMessageBox>
#include <QInputDialog>


//-----------------------------------------------------------------------------
// include files for current project
#include <qgalileiwin.h>
#include <qgalilei.h>
#include <qsessionprogress.h>
#include <kviewdicts.h>
#include <preferences.h>
using namespace std;
using namespace R;
using namespace GALILEI;


//-----------------------------------------------------------------------------
const double QGALILEIVersion=0.1;



//-----------------------------------------------------------------------------
//
// class QGALILEIWin
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
QGALILEIWin::QGALILEIWin(QGALILEI* app)
	: QMainWindow(0), App(app), Session(0)
{
	setupUi(this);
	connectMenus();
	sessionConnected();
	setWindowTitle("QGALILEI "+QString::number(QGALILEIVersion));
}


//-----------------------------------------------------------------------------
void QGALILEIWin::connectMenus(void)
{
	connect(aConnect,SIGNAL(triggered()),this,SLOT(sessionConnect()));
	connect(aDisconnect,SIGNAL(triggered()),this,SLOT(sessionDisconnect()));
	connect(aExit,SIGNAL(triggered()),this,SLOT(close()));

	connect(aDicts,SIGNAL(triggered()),this,SLOT(showDicts()));

	connect(aCloseAll,SIGNAL(triggered()),Desktop,SLOT(closeAllSubWindows()));
	connect(aTile,SIGNAL(triggered()),Desktop,SLOT(tileSubWindows()));
	connect(aCascade,SIGNAL(triggered()),Desktop,SLOT(cascadeSubWindows()));

	connect(aConfigSession,SIGNAL(triggered()),this,SLOT(configSession()));
	connect(aConfigQGALILEI,SIGNAL(triggered()),this,SLOT(configQGALILEI()));
}


//-----------------------------------------------------------------------------
void QGALILEIWin::sessionConnected(void)
{
	aConnect->setEnabled(Session==0);
	aDisconnect->setEnabled(Session);
	aDicts->setEnabled(Session);
	aConfigSession->setEnabled(Session);
	aCloseAll->setEnabled(Session);
	aTile->setEnabled(Session);
	aCascade->setEnabled(Session);
}


//-----------------------------------------------------------------------------
void QGALILEIWin::sessionConnect(void)
{
	statusBar()->showMessage(tr("Connecting..."));
   bool ok;
   SessionName=FromQString(QInputDialog::getText(this,tr("Connect to a session"),tr("Session name:"),QLineEdit::Normal,ToQString(SessionName),&ok));
   if((!ok)||(SessionName.IsEmpty()))
		return;
	if(QCreateSession(this,Session,SessionName).run())
	{
		sessionConnected();
		statusBar()->showMessage(tr("Connected"));
	}
	else
	{
		statusBar()->showMessage(tr("Ready"));
		Session=0;
	}
}


//-----------------------------------------------------------------------------
void QGALILEIWin::sessionDisconnect(void)
{
	if(Session)
	{
		Desktop->closeAllSubWindows();
		App->DeleteSession(Session);
		Session=0;
		sessionConnected();
		statusBar()->showMessage(tr("Not Connected!"));
	}
}


//-----------------------------------------------------------------------------
void QGALILEIWin::showDicts(void)
{
	KViewDicts* ptr(new KViewDicts(this));
	Desktop->addSubWindow(ptr);
	ptr->adjustSize();
	ptr->show();
	ptr->create();
}


//-----------------------------------------------------------------------------
void QGALILEIWin::configQGALILEI(void)
{
	Preferences Dlg(this);
	Dlg.exec();
}


//-----------------------------------------------------------------------------
void QGALILEIWin::configSession(void)
{
//	Configure Dlg(this);
//	Dlg.exec();
}


//-----------------------------------------------------------------------------
QGALILEIWin::~QGALILEIWin(void)
{
	App=0;
}
