/*

	GALILEI Research Project

	KViewDoc.cpp

	Window for manipulating a specific document - Implementation.

	(C) 2001-2002 by Pascal Francq

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
// include files ANSI C/C++
#include <stdlib.h>


//-----------------------------------------------------------------------------
// include files for R Project
#include <rstd/rcontainercursor.h>
using namespace RStd;
#include <rga/qxmlcontainer.h>
using namespace RGA;


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <galilei/qlistviewitemtype.h>
#include <galilei/qgdocxml.h>
#include <infos/giwordoccur.h>
#include <docs/gdoc.h>
#include <docs/gdocxml.h>
#include <profiles/guser.h>
#include <profiles/gprofile.h>
#include <profiles/gprofdoc.h>
#include <langs/glang.h>
#include <sessions/gsession.h>
using namespace GALILEI;


//-----------------------------------------------------------------------------
// includes files for Qt
#include <qpixmap.h>


//-----------------------------------------------------------------------------
// application specific includes
#include "kviewdoc.h"
#include "qsessionprogress.h"
#include "kdoc.h"
using namespace RTimeDate;



//-----------------------------------------------------------------------------
//
// class KViewDoc
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
KViewDoc::KViewDoc(GDoc* document,KDoc* doc,QWidget* parent,const char* name,int wflags)
	: KView(doc,parent,name,wflags), Document(document), Fdbks(0), Struct(0)
{
	// Window proprieties
	setIcon(QPixmap("/usr/share/icons/hicolor/16x16/mimetypes/document.png"));
	setCaption("Description of \" "+QString(Document->GetName())+"\"");

	// initialisation of the tab widget
	Infos=new QTabWidget(this);
	Infos->resize(size());

	// Initialisation of the General Information Widget
	General = new QListView(Infos);
	Infos->insertTab(General,"General Information");
	General->addColumn("Variable");
	General->addColumn("Value");
	ConstructGeneral();

	// Initialisation of the Feedbacks Widget
	if(Doc->GetSession()->IsFdbksLoad())
	{
		Fdbks = new QListView(Infos);
		Infos->insertTab(Fdbks,"Profiles");
		Fdbks->addColumn(QString("Profile"));
		Fdbks->addColumn(QString("User"));
		Fdbks->addColumn(QString("Date"));
		Fdbks->setRootIsDecorated(true);
		connect(Fdbks,SIGNAL(doubleClicked(QListViewItem*)),parent->parent()->parent(),SLOT(slotHandleItem(QListViewItem*)));
		ConstructFdbks();
	}

	// Initialisation of the XML Widget
	XML = new QGDocXML(Infos);
	Infos->insertTab(XML,"XML Structure");

	// Initialisation of the AnalyseResults Widget
	Results = new QListView(Infos);
	Infos->insertTab(Results,"Analyse Results");
	Results->addColumn("Word");
	Results->addColumn("Occurence");
	ConstructResults();
}


//-----------------------------------------------------------------------------
void KViewDoc::ConstructFdbks(void)
{
	QListViewItem *p;
	const RDate* d;
	char sDate[20];
	GProfDocCursor Profiles;

	if(!Fdbks) return;

	// Init different judgements
	QListViewItemType* ok= new QListViewItemType(Fdbks, "OK Judgements");
	ok->setPixmap(0,QPixmap("/usr/share/icons/hicolor/16x16/actions/ok.png"));
	QListViewItemType* ko= new QListViewItemType(Fdbks, "KO Judgements");
	ko->setPixmap(0,QPixmap("/usr/share/icons/hicolor/16x16/actions/remove.png"));
	QListViewItemType* n= new QListViewItemType(Fdbks, "N Judgements");
	n->setPixmap(0,QPixmap("/usr/share/icons/hicolor/16x16/actions/view_tree.png"));
	QListViewItemType* hs= new QListViewItemType(Fdbks, "HS Judgements");
	hs->setPixmap(0,QPixmap("/usr/share/icons/hicolor/16x16/actions/stop.png"));

	// Add Judgements
	Profiles=Document->GetProfDocCursor();
	for(Profiles.Start();!Profiles.End();Profiles.Next())
	{
		switch(Profiles()->GetFdbk())
		{
			case 'O':
				p=ok;
				break;
			case 'K':
				p=ko;
				break;
			case 'N':
				p=n;
				break;
			case 'H':
				p=hs;
				break;
			default:
				p=0;
				break;
		}
		if(!p) continue;
		d=Profiles()->GetUpdated();
		sprintf(sDate,"%i/%i/%i",d->GetDay(),d->GetMonth(),d->GetYear());
		QListViewItemType* prof = new QListViewItemType(Profiles()->GetProfile(),p,Profiles()->GetProfile()->GetName(),Profiles()->GetProfile()->GetUser()->GetFullName(),sDate);
		prof->setPixmap(0,QPixmap("/usr/share/icons/hicolor/16x16/apps/personal.png"));
	}
}


//-----------------------------------------------------------------------------
void KViewDoc::ConstructResults(void)
{
	GIWordOccurCursor Words=Document->GetWordOccurCursor();

	for (Words.Start();!Words.End();Words.Next())
	{
		new QListViewItem(Results,Doc->GetSession()->GetWord(Words()->GetId(),Document->GetLang()), QString(itoa(Words()->GetNbOccurs())));
	}
}


//-----------------------------------------------------------------------------
void KViewDoc::ConstructGeneral(void)
{
	GLang* l;
	const RDate* d;
	char sDate[20];

	new QListViewItem(General,"ID",QString(itoa(Document->GetId())));
	new QListViewItem(General,"URL",Document->GetURL());
	new QListViewItem(General,"Name",Document->GetName());
	l=Document->GetLang();
	if(l)
		new QListViewItem(General,"Language",l->GetName());
	else
		new QListViewItem(General,"Language","Unknow");
	new QListViewItem(General,"Total number of words",QString(itoa(Document->GetN())));
	new QListViewItem(General,"Number of different words",QString(itoa(Document->GetNdiff())));
	new QListViewItem(General,"Total number of valid words",QString(itoa(Document->GetV())));
	new QListViewItem(General,"Number of different valid words",QString(itoa(Document->GetVdiff())));
	d=Document->GetUpdated();
	sprintf(sDate,"%i/%i/%i",d->GetDay(),d->GetMonth(),d->GetYear());
	new QListViewItem(General,"Last Updated",sDate);
	d=Document->GetComputed();
	sprintf(sDate,"%i/%i/%i",d->GetDay(),d->GetMonth(),d->GetYear());
	new QListViewItem(General,"Last Analysed",sDate);
	switch(Document->GetState())
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
	if(Document->GetFailed())
		new QListViewItem(General,"Number of failed",QString(itoa(Document->GetFailed())));
}


//-----------------------------------------------------------------------------
void KViewDoc::update(unsigned int cmd)
{
	if(cmd!=0) return;
	General->clear();
	ConstructGeneral();
	Results->clear();
	ConstructResults();
}


//-----------------------------------------------------------------------------
void KViewDoc::resizeEvent(QResizeEvent *)
{
	Infos->resize(size());
}


//-----------------------------------------------------------------------------
void KViewDoc::CreateDocXML(void)
{
	QSessionProgressDlg* d=new QSessionProgressDlg(this,Doc->GetSession(),"Create Doc XML");
	d->CreateDocXML(Struct,Document);
	if(Struct)
		XML->SetDocXML(Struct);
}


//-----------------------------------------------------------------------------
void KViewDoc::AnalyseDocXML(void)
{
	bool b;

	b=Struct;
	QSessionProgressDlg* d=new QSessionProgressDlg(this,Doc->GetSession(),"Analyse Document");
	d->AnalyseXML(Struct,Document);
	if((!b)&&Struct)
		XML->SetDocXML(Struct);
	General->clear();
	ConstructGeneral();
	Results->clear();
	ConstructResults();
}


//-----------------------------------------------------------------------------
KViewDoc::~KViewDoc(void)
{
	if(Struct) delete Struct;
}
