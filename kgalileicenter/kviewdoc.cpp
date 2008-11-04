/*

	GALILEI Research Project

	KViewDoc.cpp

	Window for manipulating a specific document - Implementation.

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
// include files ANSI C/C++
#include <stdlib.h>
using namespace std;


//-----------------------------------------------------------------------------
// include files for R Project
#include <rxmlfile.h>
#include <rqt.h>
#include <rvectorint.h>
using namespace R;


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <gweightinfo.h>
#include <gweightinfos.h>
#include <qlistviewitemtype.h>
#include <qgdocxml.h>
#include <gdoc.h>
#include <gdocxml.h>
#include <guser.h>
#include <gprofile.h>
#include <glang.h>
#include <gsession.h>
#include <gstorage.h>
#include <ggalileiapp.h>
#include <gconcepttype.h>
#include <qgweightinfos.h>
#include <qgdocstruct.h>
#include <gdocstruct.h>
using namespace GALILEI;


//-----------------------------------------------------------------------------
// includes files for Qt
#include <qpixmap.h>
#include <qlayout.h>
#include <qpushbutton.h>
#include <qgroupbox.h>


//-----------------------------------------------------------------------------
// include files for KDE
#include <kiconloader.h>


//-----------------------------------------------------------------------------
// application specific includes
#include "kviewdoc.h"
#include "qsessionprogress.h"
#include "kdoc.h"


//-----------------------------------------------------------------------------
//
// class MyAddFdbkDlg
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
MyAddFdbkDlg::MyAddFdbkDlg(QWidget* parent,GSession* session)
	: AddFdbkDlg(parent), Session(session), User(0)
{
	connect(cbUsers,SIGNAL(activated(const QString&)),this,SLOT(slotChangeUser(const QString&)));
	RCursor<GUser> Users(Session->GetUsers());
	for(Users.Start();!Users.End();Users.Next())
	{
		if(!Users()->GetNbProfiles())
			continue;
		if(!User)
			User=Users();
		cbUsers->insertItem(ToQString(Users()->GetFullName()));
	}
	if(User)
	{
		cbUsers->setCurrentItem(0);
		FillProfiles();
	}
}


//-----------------------------------------------------------------------------
void MyAddFdbkDlg::FillProfiles(void)
{
	cbProfiles->clear();
	Prof=0;
	RCursor<GProfile> Profiles(User->GetProfiles());
	for(Profiles.Start();!Profiles.End();Profiles.Next())
	{
		if(!Prof)
			Prof=Profiles();
		cbProfiles->insertItem(ToQString(Profiles()->GetName()));
	}
	if(Prof)
		cbProfiles->setCurrentItem(0);
}


//-----------------------------------------------------------------------------
void MyAddFdbkDlg::slotChangeUser(const QString& string)
{
	User=Session->GetUser(FromQString(string));
	FillProfiles();
}


//-----------------------------------------------------------------------------
void MyAddFdbkDlg::slotChangeProfile(const QString& string)
{
	Prof=User->GetProfile(FromQString(string));
}



//-----------------------------------------------------------------------------
//
// class KViewDoc
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
KViewDoc::KViewDoc(GDoc* document,KDoc* doc,QWidget* parent,const char* name,int wflags)
	: KView(doc,parent,name,wflags), Document(document), Fdbks(0), Struct(0),
	  bDelDoc(false), bDocXML(false)
{
	// Window properties
	setIcon(QPixmap(KGlobal::iconLoader()->loadIcon("document.png",KIcon::Small)));
	setCaption("Description of \" "+QString(ToQString(Document->GetName()))+"\"");

	// Main Layout
	QVBoxLayout* MainLayout = new QVBoxLayout(this,0,0,"MainLayout");
	Infos=new QTabWidget(this);
	MainLayout->addWidget(Infos);

	// Initialization of the General Information Widget
	General = new QListView(Infos);
	Infos->insertTab(General,"General Information");
	General->addColumn("Variable");
	General->addColumn("Value");
	ConstructGeneral();

	// Initialization of the Feedbacks Widget
	QGroupBox* boxFdbks=new QGroupBox(Infos);
	Infos->insertTab(boxFdbks,"Profiles");
	QVBoxLayout* FdbkLayout = new QVBoxLayout(boxFdbks,0,0,"FdbkLayout");
	QHBoxLayout* SearchLayout = new QHBoxLayout(0,0,0,"SearchLayout");
	SearchLayout->setAlignment(Qt::AlignTop);
	NewFdbk=new QPushButton(boxFdbks);
	NewFdbk->setAutoDefault(TRUE);
	NewFdbk->setText("New Fdbk");
	connect(NewFdbk,SIGNAL(clicked()),this,SLOT(slotNewFdbk()));
	SearchLayout->addWidget(NewFdbk);
	FdbkLayout->addLayout(SearchLayout);
	Fdbks = new QListView(boxFdbks);
	FdbkLayout->addWidget(Fdbks);
	Fdbks->addColumn(QString("Profile"));
	Fdbks->addColumn(QString("User"));
	Fdbks->addColumn(QString("Date"));
	Fdbks->setRootIsDecorated(true);
	connect(Fdbks,SIGNAL(doubleClicked(QListViewItem*)),parent->parent()->parent(),SLOT(slotHandleItem(QListViewItem*)));
	FdbksLinks = new QListView(Infos);
	Infos->insertTab(FdbksLinks,"Links");
	FdbksLinks->addColumn(QString("Profile"));
	FdbksLinks->addColumn(QString("User"));
	FdbksLinks->addColumn(QString("Date"));
	FdbksLinks->setRootIsDecorated(true);
	connect(FdbksLinks,SIGNAL(doubleClicked(QListViewItem*)),parent->parent()->parent(),SLOT(slotHandleItem(QListViewItem*)));
	ConstructFdbks();

	// Initialization of the XML Widget
	XML = new QGDocXML(Infos);
	Infos->insertTab(XML,"XML Structure");

	// Initialization of the AnalyseResults Widget
	Results = new QGWeightInfos(Infos,Document,Doc->GetSession());
	Infos->insertTab(Results,"Description");

	// Initialization of the AnalyseResults Widget
	DocStruct = new QGDocStruct(Infos,Document,Doc->GetSession());
	Infos->insertTab(DocStruct,"Structure");
}


//-----------------------------------------------------------------------------
KViewDoc::KViewDoc(const char* file,const char* mime,KDoc* doc,QWidget* parent,const char* name,int wflags)
	: KView(doc,parent,name,wflags), Document(0), Fdbks(0), Struct(0),
	  bDelDoc(true), bDocXML(false)
{
	// Construct the document
	Document=new GDoc(file,file,cNoRef,0,mime,cNoRef,RDate::Null,RDate(),RDate::Null,0,0);

	// Window proprieties
	setIcon(QPixmap(KGlobal::iconLoader()->loadIcon("document.png",KIcon::Small)));
	setCaption("Description of \" "+QString(ToQString(Document->GetName()))+"\"");

	// Main Layout
	QVBoxLayout* MainLayout = new QVBoxLayout(this,0,0,"MainLayout");
	Infos=new QTabWidget(this);
	MainLayout->addWidget(Infos);

	// Initialization of the General Information Widget
	General = new QListView(Infos);
	Infos->insertTab(General,"General Information");
	General->addColumn("Variable");
	General->addColumn("Value");
	ConstructGeneral();

	// Initialization of the XML Widget
	XML = new QGDocXML(Infos);
	Infos->insertTab(XML,"XML Structure");

	// Initialization of the AnalyseResults Widget
	Results = new QGWeightInfos(Infos,Document,Doc->GetSession());
	Infos->insertTab(Results,"Analyse Results");

	// Initialization of the AnalyseResults Widget
	DocStruct = new QGDocStruct(Infos,0,Doc->GetSession());
	Infos->insertTab(DocStruct,"Structure");
}


//-----------------------------------------------------------------------------
void KViewDoc::ConstructFdbks(void)
{
	QListViewItem *p;
	GProfile* prof;
	RDate d;

	if(!Fdbks) return;
	if(!FdbksLinks) return;
	Fdbks->clear();
	FdbksLinks->clear();

	// Init different judgements
	QListViewItemType* ok= new QListViewItemType(Fdbks,"Relevant Documents");
	ok->setPixmap(0,QPixmap(KGlobal::iconLoader()->loadIcon("ok.png",KIcon::Small)));
	QListViewItemType* ko= new QListViewItemType(Fdbks, "Fuzzy Relevant Documents");
	ko->setPixmap(0,QPixmap(KGlobal::iconLoader()->loadIcon("remove.png",KIcon::Small)));
	QListViewItemType* hs= new QListViewItemType(Fdbks, "Irrelevant Documents");
	hs->setPixmap(0,QPixmap(KGlobal::iconLoader()->loadIcon("stop.png",KIcon::Small)));

	// Init different judgements for documents from link analysis.
	QListViewItemType* lh= new QListViewItemType(FdbksLinks , "Hub Documents");
	lh->setPixmap(0,QPixmap(KGlobal::iconLoader()->loadIcon("ok.png",KIcon::Small)));
	QListViewItemType* la= new QListViewItemType(FdbksLinks, "Authority Documents");
	la->setPixmap(0,QPixmap(KGlobal::iconLoader()->loadIcon("ok.png",KIcon::Small)));


	// Add Judgements for profile.

	RVectorInt<size_t,true>* Profiles=Document->GetFdbks();
	if(!Profiles)
		return;
	for(Profiles->Start();!Profiles->End();Profiles->Next())
	{
		prof=getDocument()->GetSession()->GetProfile((*Profiles)());
		if(!prof)
			continue;
		GFdbk* fdbk=prof->GetFdbk(Document->GetId());
		if(!fdbk)
			continue;
		switch(fdbk->GetFdbk())
		{
			case djOK:
				p=ok;
				break;
			case (djOK | djAutority):
				p=la;
				break;
			case djKO:
				p=ko;
				break;
			case (djOK | djHub):
				p=lh;
				break;
			case djOutScope:
				p=hs;
				break;
			default:
				p=0;
				break;
		}
		if(!p) continue;
		d=fdbk->GetWhen();
		QListViewItemType* prof2 = new QListViewItemType(prof,p,ToQString(prof->GetName()),ToQString(prof->GetUser()->GetFullName()),ToQString(d));
		prof2->setPixmap(0,QPixmap(KGlobal::iconLoader()->loadIcon("personal.png",KIcon::Small)));
	}
}


//-----------------------------------------------------------------------------
void KViewDoc::ConstructGeneral(void)
{
	GLang* l;
	RDate d;

	new QListViewItem(General,"ID",QString::number(Document->GetId()));
	new QListViewItem(General,"URL",ToQString(Document->GetURL()));
	new QListViewItem(General,"Name",ToQString(Document->GetName()));
	InfoMIME=new QListViewItem(General,"MIME",ToQString(Document->GetMIMEType()));
	l=Document->GetLang();
	if(l)
		new QListViewItem(General,"Language",ToQString(l->GetName()));
	else
		new QListViewItem(General,"Language","Unknow");
	d=Document->GetUpdated();
	new QListViewItem(General,"Last Updated",ToQString(d));
	d=Document->GetComputed();
	new QListViewItem(General,"Last Analysed",ToQString(d));
	new QListViewItem(General,"State",ToQString(GetState(Document->GetState())));
}


//-----------------------------------------------------------------------------
void KViewDoc::update(tObjType type)
{
	if(type!=otDoc) return;
	General->clear();
	ConstructGeneral();
	Results->SetObject(Document);
	DocStruct->SetObject(Document);
}


//-----------------------------------------------------------------------------
void KViewDoc::CreateDocXML(void)
{
	QSessionProgressDlg Dlg(this,Doc->GetSession(),"Create Doc XML");
	if(!Dlg.Run(new QCreateDocXML(Struct,Document)))
		return;
	if(Struct)
	{
		XML->SetDocXML(Struct);
		RXMLTag* tag=Struct->GetTag("dc:format");
		if(tag)
			InfoMIME->setText(1,ToQString(tag->GetContent()));
		bDocXML=true;
	}
}


//-----------------------------------------------------------------------------
void KViewDoc::SaveDocXML(const char* name)
{
	if(!Struct) return;
	RXMLFile f(name,Struct);
	f.Open(R::RIO::Create);
}


//-----------------------------------------------------------------------------
void KViewDoc::AnalyseDocXML(void)
{
	bool b;

	b=Struct;
	QSessionProgressDlg Dlg(this,Doc->GetSession(),"Analyse Document");
	if(!Dlg.Run(new QAnalyzeXML(Document))) return;
	General->clear();
	ConstructGeneral();
	Results->SetObject(Document);
	DocStruct->SetObject(Document);
}


//-----------------------------------------------------------------------------
void KViewDoc::slotNewFdbk(void)
{
	MyAddFdbkDlg dlg(this,getDocument()->GetSession());
	if(dlg.exec())
	{
		tDocAssessment assess;
		if(dlg.cbFdbk->currentText()=="Relevant")
			assess=djOK;
		else if (dlg.cbFdbk->currentText()=="Fuzzy Relevant")
			assess=djKO;
		else if(dlg.cbFdbk->currentText()=="Irrelevant")
			assess=djOutScope;
		Doc->GetSession()->InsertFdbk(dlg.Prof->GetId(),Document->GetId(),assess,RDate::GetToday(),Document->GetComputed(),true);
		ConstructFdbks();
	}
}


//-----------------------------------------------------------------------------
KViewDoc::~KViewDoc(void)
{
	if(Struct) delete Struct;
	if(bDelDoc)
		delete Document;
}
