/*

	GALILEI Research Project

	KViewDoc.cpp

	Window for manipulating a specific document - Implementation.

	Copyright 2001-2009 by Pascal Francq (pascal@francq.info).
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
// include files for R/GALILEI Projects
#include <rqt.h>
#include <rxmlfile.h>
#include <guser.h>
#include <gprofile.h>


//-----------------------------------------------------------------------------
// application specific includes
#include <kviewdoc.h>
#include <kgalileicenter.h>
#include <qsessionprogress.h>



//-----------------------------------------------------------------------------
//
// class AddFdbkDlg
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
AddFdbkDlg::AddFdbkDlg(QWidget* parent)
	: KDialog(parent), Ui_AddFdbkDlg(), User(0)
{
	GSession* Session(GALILEIApp->GetSession());
	QWidget* widget=new QWidget(this);
	setupUi(widget);
	setMainWidget(widget);

	connect(cbUsers,SIGNAL(activated(const QString&)),this,SLOT(slotChangeUser(const QString&)));
	RCursor<GUser> Users(Session->GetUsers());
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
	User=GALILEIApp->GetSession()->GetUser(FromQString(string));
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
KViewDoc::KViewDoc(GDoc* doc)
	: QMdiSubWindow(), Ui_KViewDoc(), Document(doc), XML(0), bDelDoc(false)
{
	setUp();
}


//-----------------------------------------------------------------------------
KViewDoc::KViewDoc(const RURI& file,const RString& mime)
	: QMdiSubWindow(), Ui_KViewDoc(), Document(new GDoc(file,file(),cNoRef,0,mime,cNoRef,RDate::Null,RDate(),RDate::Null,0,0)), XML(0), bDelDoc(true)
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
	Desc->Set(Document);
	Struct->Set(Document);
	Assessments->Set(QGObjectsList::Assessments,Document);
	connect(AddFeedback,SIGNAL(pressed()),this,SLOT(newFdbk()));
	connect(dynamic_cast<KGALILEICenter*>(GALILEIApp),SIGNAL(docsChanged()),this,SLOT(update()));
}


//-----------------------------------------------------------------------------
void KViewDoc::newFdbk(void)
{
	AddFdbkDlg dlg(this);
	if(dlg.exec())
	{
		tDocAssessment assess;
		if(dlg.cbFdbk->currentText()=="Relevant")
			assess=djOK;
		else if (dlg.cbFdbk->currentText()=="Fuzzy Relevant")
			assess=djKO;
		else
			assess=djOutScope;
		GALILEIApp->GetSession()->InsertFdbk(dlg.Prof->GetId(),Document->GetId(),assess,RDate::GetToday(),Document->GetComputed(),true);
		Assessments->Set(QGObjectsList::Assessments,Document);
	}
}


//-----------------------------------------------------------------------------
void KViewDoc::update(void)
{
	Vars->Set(Document);
	Desc->Set(Document);
	Struct->Set(Document);
}


//-----------------------------------------------------------------------------
void KViewDoc::updateXML(void)
{
	XMLStruct->Set(XML);
}


//-----------------------------------------------------------------------------
void KViewDoc::CreateDocXML(void)
{
	QSessionProgressDlg Dlg(this,"Create Doc XML");
	if(!XML)
		XML=new RXMLStruct();
	QCreateDocXML* Task(new QCreateDocXML(XML,Document));
	connect(Task,SIGNAL(finish()),this,SLOT(updateXML()));
	if(!Dlg.Run(Task))
		return;
}


//-----------------------------------------------------------------------------
void KViewDoc::SaveDocXML(const RURI& name)
{
	if(!XML) return;
	RXMLFile f(name,XML);
	f.Open(R::RIO::Create);
}


//-----------------------------------------------------------------------------
void KViewDoc::AnalyseDocXML(void)
{
	QSessionProgressDlg Dlg(QMdiSubWindow::widget(),ToQString("Analyze Document "+Document->GetName()));
	QAnalyzeXML* Task(new QAnalyzeXML(Document));
	connect(Task,SIGNAL(finish()),this,SLOT(update()));
	Dlg.Run(Task);
}


//-----------------------------------------------------------------------------
KViewDoc::~KViewDoc(void)
{
	if(XML) delete XML;
	if(bDelDoc)	delete Document;
}
