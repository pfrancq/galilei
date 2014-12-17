/*

	GALILEI Research Project

	KViewDoc.cpp

	Window for manipulating a specific document - Implementation.

	Copyright 2001-2014 by Pascal Francq (pascal@francq.info).
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



//------------------------------------------------------------------------------
// include files for Qt/KDE
#include <kmessagebox.h>
#include <kapplication.h>


//-----------------------------------------------------------------------------
// include files for R/GALILEI Projects
#include <rqt.h>
#include <rxmlfile.h>
#include <guser.h>
#include <gprofile.h>
#include <gsession.h>


//-----------------------------------------------------------------------------
// application specific includes
#include <kviewdoc.h>
#include <qsessionprogress.h>
#include <qgalileiwin.h>
#include <rstringbuilder.h>



//-----------------------------------------------------------------------------
//
// class AddFdbkDlg
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
AddFdbkDlg::AddFdbkDlg(GSession* session,QWidget* parent)
	: QDialog(parent), Ui_AddFdbkDlg(), Session(session), User(0)
{
	QWidget* widget=new QWidget(this);
	setupUi(widget);

	connect(cbUsers,SIGNAL(activated(const QString&)),this,SLOT(slotChangeUser(const QString&)));
	RCursor<GUser> Users(Session->GetObjs(static_cast<GUser*>(0)));
	for(Users.Start();!Users.End();Users.Next())
	{
		if(!Users()->GetNbProfiles())
			continue;
		if(!User)
			User=Users();
		cbUsers->insertItem(cbUsers->count(),ToQString(Users()->GetFullName()));
	}
	if(User)
	{
		cbUsers->setCurrentIndex(0);
		FillProfiles();
	}
}


//-----------------------------------------------------------------------------
void AddFdbkDlg::FillProfiles(void)
{
	cbProfiles->clear();
	Prof=0;
	RCursor<GProfile> Profiles(User->GetProfiles());
	for(Profiles.Start();!Profiles.End();Profiles.Next())
	{
		if(!Prof)
			Prof=Profiles();
		cbProfiles->insertItem(cbProfiles->count(),ToQString(Profiles()->GetName()));
	}
	if(Prof)
		cbProfiles->setCurrentIndex(0);
}


//-----------------------------------------------------------------------------
void AddFdbkDlg::slotChangeUser(const QString& string)
{
	User=Session->GetObj(static_cast<GUser*>(0),FromQString(string));
	FillProfiles();
}


//-----------------------------------------------------------------------------
void AddFdbkDlg::slotChangeProfile(const QString& string)
{
	Prof=User->GetProfile(FromQString(string));
}



//-----------------------------------------------------------------------------
//
// class KViewDoc
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
KViewDoc::KViewDoc(QGALILEIWin* win,GDoc* doc)
	: QMdiSubWindow(), Ui_KViewDoc(), Win(win), Document(doc), bDelDoc(false)
{
	setUp();
}


//-----------------------------------------------------------------------------
KViewDoc::KViewDoc(QGALILEIWin* win,const RURI& file)
	: QMdiSubWindow(), Ui_KViewDoc(), Win(win), Document(new GDoc(win->getSession(),file,file(),0,RString::Null)), bDelDoc(true)
{
	setUp();
}


//-----------------------------------------------------------------------------
void KViewDoc::setUp(void)
{
	QWidget* ptr=new QWidget();
	setupUi(ptr);
	setWidget(ptr);
	setAttribute(Qt::WA_DeleteOnClose);
	setWindowTitle(ToQString(Document->GetName()));
	Vars->Set(Document);
	try
	{
		Desc->Set(Win->getSession(),&(*Document)());
	}
	catch(GException& e)
	{
		QMessageBox::critical(0, QWidget::tr("GALILEI Exception"),QWidget::trUtf8(e.GetMsg()),QMessageBox::Ok);
	}
	Struct->Set(Document);
	Assessments->Set(QGObjectsList::Assessments,Document);
	connect(AddFeedback,SIGNAL(pressed()),this,SLOT(newFdbk()));
	connect(Win,SIGNAL(docsChanged()),this,SLOT(update()));
}


//-----------------------------------------------------------------------------
void KViewDoc::newFdbk(void)
{
	AddFdbkDlg dlg(Win->getSession(),this);
	if(dlg.exec())
	{
		tFdbkType assess;
		if(dlg.cbFdbk->currentText()=="Relevant")
			assess=ftRelevant;
		else if (dlg.cbFdbk->currentText()=="Fuzzy Relevant")
			assess=ftFuzzyRelevant;
		else
			assess=ftIrrelevant;
		Win->getSession()->InsertFdbk(dlg.Prof->GetId(),Document->GetId(),assess,RDate::GetToday());
		Assessments->Set(QGObjectsList::Assessments,Document);
	}
}


//-----------------------------------------------------------------------------
void KViewDoc::update(void)
{
	Vars->Set(Document);
	Desc->Set(Win->getSession(),&(*Document)());
	Struct->Set(Document);
}


//-----------------------------------------------------------------------------
void KViewDoc::AnalyzeDoc(void)
{
	if(QAnalyzeDoc(Win,Document).run())
		update();
}


//-----------------------------------------------------------------------------
KViewDoc::~KViewDoc(void)
{
	if(bDelDoc)
		delete Document;
}
