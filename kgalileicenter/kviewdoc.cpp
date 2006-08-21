/*

	GALILEI Research Project

	KViewDoc.cpp

	Window for manipulating a specific document - Implementation.

	Copyright 2001-2002 by the Universit�Libre de Bruxelles.

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
using namespace GALILEI;


//-----------------------------------------------------------------------------
// includes files for Qt
#include <qpixmap.h>


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
// class KViewDoc
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
KViewDoc::KViewDoc(GDoc* document,KDoc* doc,QWidget* parent,const char* name,int wflags)
	: KView(doc,parent,name,wflags), Document(document), Fdbks(0), Struct(0),
	  bDelDoc(false), bDocXML(false)
{
	// Window proprieties
	setIcon(QPixmap(KGlobal::iconLoader()->loadIcon("document.png",KIcon::Small)));
	setCaption("Description of \" "+QString(ToQString(Document->GetName()))+"\"");

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
	Fdbks = new QListView(Infos);
	Infos->insertTab(Fdbks,"Profiles");
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

	// Initialisation of the XML Widget
	XML = new QGDocXML(Infos);
	Infos->insertTab(XML,"XML Structure");

	// Initialisation of the AnalyseResults Widget
	Results = new QListView(Infos);
	Infos->insertTab(Results,"Description");
	Results->addColumn("Information Entity");
	Results->addColumn("Weight");
	ConstructResults();
}


//-----------------------------------------------------------------------------
KViewDoc::KViewDoc(const char* file,const char* mime,KDoc* doc,QWidget* parent,const char* name,int wflags)
	: KView(doc,parent,name,wflags), Document(0), Fdbks(0), Struct(0),
	  bDelDoc(true), bDocXML(false)
{
	// Construct the document
	Document=new GDoc(file,file,cNoRef,0,GALILEIApp->GetManager<GFilterManager>("Filter")->GetMIMEType(mime),RDate(),RDate::null);

	// Window proprieties
	setIcon(QPixmap(KGlobal::iconLoader()->loadIcon("document.png",KIcon::Small)));
	setCaption("Description of \" "+QString(ToQString(Document->GetName()))+"\"");

	// initialisation of the tab widget
	Infos=new QTabWidget(this);
	Infos->resize(size());

	// Initialisation of the General Information Widget
	General = new QListView(Infos);
	Infos->insertTab(General,"General Information");
	General->addColumn("Variable");
	General->addColumn("Value");
	ConstructGeneral();

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
	GProfile* prof;
	RDate d;

	if(!Fdbks) return;
	if(!FdbksLinks) return;


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

	RVectorInt<true>* Profiles=Document->GetFdbks();
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
void KViewDoc::ConstructResults(void)
{
	class LocalItem : QListViewItem
	{
	public:
		double Val;

		LocalItem(QListView* v,QString str,double d) : QListViewItem(v,str, QString::number(d)), Val(d) {}
		virtual int compare( QListViewItem *i, int col, bool ascending ) const
    	{
			if(col==1)
			{
				double d=Val-static_cast<LocalItem*>(i)->Val;
				if(d==0.0) return(key(0, ascending ).compare( i->key(0, ascending)));
				if(d>0)
					return(1);
				return(-1);
			}
			return(key( col, ascending ).compare( i->key( col, ascending)));
    	}
	};
	Results->clear();
	if(!Document->GetLang()) return;
	RCursor<GWeightInfo> Words(Document->GetInfos());
	for (Words.Start();!Words.End();Words.Next())
	{
		new LocalItem(Results,ToQString(Doc->GetSession()->GetStorage()->LoadWord(Words()->GetId(),Document->GetLang()->GetCode())), Words()->GetWeight());
	}
}


//-----------------------------------------------------------------------------
void KViewDoc::ConstructGeneral(void)
{
	GLang* l;
	RDate d;
	char sDate[20];

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
		case osNewMem:
			strcpy(sDate,"New in memory");
			break;
		case osUnknow:
		default:
			strcpy(sDate,"Unknow");
			break;
	}
	new QListViewItem(General,"State",sDate);

	new QListViewItem(General,"Status",ToQString(GetDocStatus(Document->GetStatus())));
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
	if(!Dlg.Run(new QAnalyzeXML(Struct,Document))) return;
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
	if(bDelDoc)
		delete Document;
}
