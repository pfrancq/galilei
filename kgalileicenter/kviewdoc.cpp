/*

	GALILEI Research Project

	KViewDoc.cpp

	Window for manipulating a specific document - Implementation.

	Copyright 2001-2002 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).

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
using namespace std;


//-----------------------------------------------------------------------------
// include files for R Project
#include <rstd/rcontainercursor.h>
#include <rstd/rxmlfile.h>
#include <rga/qxmlcontainer.h>
using namespace R;


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <infos/gweightinfo.h>
#include <infos/gweightinfos.h>
#include <galilei/qlistviewitemtype.h>
#include <galilei/qgdocxml.h>
#include <docs/gdocvector.h>
#include <docs/gdocxml.h>
#include <profiles/guser.h>
#include <profiles/gprofile.h>
#include <profiles/gprofdoc.h>
#include <infos/glang.h>
#include <sessions/gsession.h>
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
	: KView(doc,parent,name,wflags), Document(dynamic_cast<GDocVector*>(document)), Fdbks(0), Struct(0),
	  bDelDoc(false), bDocXML(false)
{
	// Window proprieties
	setIcon(QPixmap(KGlobal::iconLoader()->loadIcon("document.png",KIcon::Small)));
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

		FdbksLinks = new QListView(Infos);
		Infos->insertTab(FdbksLinks,"Links");
		FdbksLinks->addColumn(QString("Profile"));
		FdbksLinks->addColumn(QString("User"));
		FdbksLinks->addColumn(QString("Date"));
		FdbksLinks->setRootIsDecorated(true);
		connect(FdbksLinks,SIGNAL(doubleClicked(QListViewItem*)),parent->parent()->parent(),SLOT(slotHandleItem(QListViewItem*)));

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
KViewDoc::KViewDoc(const char* file,const char* mime,KDoc* doc,QWidget* parent,const char* name,int wflags)
	: KView(doc,parent,name,wflags), Document(0), Fdbks(0), Struct(0),
	  bDelDoc(true), bDocXML(false)
{
	// Construct the document
	Document=new GDocVector(file,file,cNoRef,0,Doc->GetSession()->GetMIMEType(mime),0,0,0,0,0,0,0);

	// Window proprieties
	setIcon(QPixmap(KGlobal::iconLoader()->loadIcon("document.png",KIcon::Small)));
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
	if(!FdbksLinks) return;


	// Init different judgements
	QListViewItemType* ok= new QListViewItemType(Fdbks,"Relevant Documents");
	ok->setPixmap(0,QPixmap(KGlobal::iconLoader()->loadIcon("ok.png",KIcon::Small)));
	QListViewItemType* ko= new QListViewItemType(Fdbks, "Fuzzy Relevant Documents");
	ko->setPixmap(0,QPixmap(KGlobal::iconLoader()->loadIcon("remove.png",KIcon::Small)));
	QListViewItemType* hs= new QListViewItemType(Fdbks, "Irrelevant Documents");
	hs->setPixmap(0,QPixmap(KGlobal::iconLoader()->loadIcon("stop.png",KIcon::Small)));

	//init different judgements for documents from link analysis.
	QListViewItemType* lh= new QListViewItemType(FdbksLinks , "Hub Documents");
	//lh->setPixmap(0,QPixmap(KGlobal::iconLoader()->loadIcon("stop.png",KIcon::Small)));
	QListViewItemType* la= new QListViewItemType(FdbksLinks, "Authority Documents");
	//la->setPixmap(0,QPixmap(KGlobal::iconLoader()->loadIcon("stop.png",KIcon::Small)));


	// Add Judgements for profile.
	Profiles=Document->GetProfDocCursor();
	for(Profiles.Start();!Profiles.End();Profiles.Next())
	{
		switch(Profiles()->GetFdbk()/* & djMaskJudg*/)
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
		d=Profiles()->GetUpdated();
		sprintf(sDate,"%i/%i/%i",d->GetDay(),d->GetMonth(),d->GetYear());
		QListViewItemType* prof = new QListViewItemType(Profiles()->GetProfile(),p,Profiles()->GetProfile()->GetName(),Profiles()->GetProfile()->GetUser()->GetFullName(),sDate);
		prof->setPixmap(0,QPixmap(KGlobal::iconLoader()->loadIcon("personal.png",KIcon::Small)));
	}
}


//-----------------------------------------------------------------------------
void KViewDoc::ConstructResults(void)
{
	class LocalItem : QListViewItem
	{
	public:
		double Val;

		LocalItem(QListView* v,QString str,double d) : QListViewItem(v,str, dtou(d).Latin1()), Val(d) {}
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

	GWeightInfoCursor Words=Document->GetWeightInfoCursor();

	for (Words.Start();!Words.End();Words.Next())
	{
		new LocalItem(Results,Doc->GetSession()->LoadWord(Words()->GetId(),Document->GetLang()->GetCode()), Words()->GetWeight());
	}
}


//-----------------------------------------------------------------------------
void KViewDoc::ConstructGeneral(void)
{
	GLang* l;
	const RDate* d;
	char sDate[20];

	new QListViewItem(General,"ID",itou(Document->GetId()).Latin1());
	new QListViewItem(General,"URL",Document->GetURL());
	new QListViewItem(General,"Name",Document->GetName());
	new QListViewItem(General,"MIME",Document->GetMIMEType());
	l=Document->GetLang();
	if(l)
		new QListViewItem(General,"Language",l->GetName());
	else
		new QListViewItem(General,"Language","Unknow");
	new QListViewItem(General,"Total number of words",itou(Document->GetN()).Latin1());
	new QListViewItem(General,"Number of different words",itou(Document->GetNdiff()).Latin1());
	new QListViewItem(General,"Total number of valid words",itou(Document->GetV()).Latin1());
	new QListViewItem(General,"Number of different valid words",itou(Document->GetVdiff()).Latin1());
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
		new QListViewItem(General,"Number of failed",itou(Document->GetFailed()).Latin1());
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
	{
		XML->SetDocXML(Struct);
		bDocXML=true;
	}
}


//-----------------------------------------------------------------------------
void KViewDoc::SaveDocXML(const char* name)
{
	if(!Struct) return;
	RXMLFile f(name,Struct,R::Create);
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
	if(bDelDoc)
		delete Document;
}
