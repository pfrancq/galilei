/*

	GALILEI Research Project

	KViewGroup.cpp

	Window to manipulate a specific group - Implementation.

	Copyright 2001-2007 by the Université Libre de Bruxelles.

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
// include files for R Project
#include <rstring.h>
#include <rtextfile.h>


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <glang.h>
#include <gsession.h>
#include <gstorage.h>
#include <gdoc.h>
#include <ggroup.h>
#include <guser.h>
#include <gprofile.h>
#include <qlistviewitemtype.h>
#include <rqt.h>
#include <gweightinfo.h>
#include <gcommunity.h>
using namespace GALILEI;
using namespace R;

//-----------------------------------------------------------------------------
// includes files for Qt/KDE
#include <qpixmap.h>
#include <qpopupmenu.h>
#include <kiconloader.h>
#include <kglobal.h>
#include <kmessagebox.h>
#include <kfiledialog.h>
#include <klocale.h>


//-----------------------------------------------------------------------------
// include files for current application
#include "kdoc.h"
#include "kviewgroup.h"
#include "qsessionprogress.h"



//-----------------------------------------------------------------------------
//
// class KViewGroup
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
KViewGroup::KViewGroup(GCommunity* grp,KDoc* doc,QWidget* parent,const char* name,int wflags)
	: KView(doc,parent,name,wflags), Group(grp), OkDocs(100,50)
{
	char title[50];

	sprintf(title,"Group (Id=%u)",grp->GetId());
	setCaption("Group");
	setIcon(QPixmap(KGlobal::iconLoader()->loadIcon("window_new",KIcon::Small)));

	// initialisation of the tab widget
	Infos=new QTabWidget(this);
	Infos->resize(size());

	// Initialisation of the General Information Widget
	General = new QListView(Infos);
	Infos->insertTab(General,"General Information");
	General->addColumn("Variable");
	General->addColumn("Value");
	ConstructGeneral();

	// Initialisation of the Profiles Widget
	Profiles = new QListView(Infos);
	Infos->insertTab(Profiles,"Profiles");
	Profiles->addColumn(QString("Profiles"));
	Profiles->addColumn(QString("Users"));
	Profiles->addColumn(QString("Attached"));
	Profiles->setRootIsDecorated(true);
	connect(Profiles,SIGNAL(doubleClicked(QListViewItem*)),parent->parent()->parent(),SLOT(slotHandleItem(QListViewItem*)));
	ConstructProfiles();

	// Initialisation of Description
	Vector = new QListView(Infos,"Vector");
	Infos->insertTab(Vector,"Description");
	Vector->addColumn("Information Entity");
	Vector->addColumn(QString("Weight"));
	ConstructDescription();

	// Initialisation of the Documentss Widget
	Docs = new QListView(this,"QListView of KViewDocs");
	Infos->insertTab(Docs,"Documents");
	Docs->addColumn(QString("Title"));
	Docs->addColumn(QString("URL"));
	Docs->addColumn(QString("MIME Type"));
	Docs->setRootIsDecorated(true);
	connect(Docs,SIGNAL(doubleClicked(QListViewItem*)),parent->parent()->parent(),SLOT(slotHandleItem(QListViewItem*)));
	connect(Docs,SIGNAL(contextMenuRequested(QListViewItem*,const QPoint&,int)),this,SLOT(askDocsMenu(QListViewItem*,const QPoint&,int)));
	ConstructDocs();
}


//-----------------------------------------------------------------------------
void KViewGroup::ConstructProfiles(void)
{
	RDate d;
	RCursor<GProfile> Sub;

	Profiles->clear();
	Sub=Group->GetObjs();
	for(Sub.Start(); !Sub.End(); Sub.Next())
	{
		GProfile* sub=Sub();
		d=sub->GetAttached();
		QListViewItemType* subitem=new QListViewItemType(sub,Profiles,ToQString(sub->GetName()),ToQString(sub->GetUser()->GetFullName()),ToQString(d));
		subitem->setPixmap(0,QPixmap(KGlobal::iconLoader()->loadIcon("find",KIcon::Small)));
	}
}


//-----------------------------------------------------------------------------
void KViewGroup::ConstructGeneral(void)
{
	General->clear();
	new QListViewItem(General,"ID",ToQString(RString::Number(Group->GetId())));
	new QListViewItem(General,"Status",ToQString(GetState(Group->GetState())));
}


//-----------------------------------------------------------------------------
void KViewGroup::ConstructDocs(void)
{
	RDate d;
	RCursor<GFdbk> docs;
	RCursor<GProfile> Sub;
	GDoc* doc;

	// Clear the Widget
	Docs->clear();
	OkDocs.Clear();

	// Goes trough the profiles of the group
	// And put in OkDocs all the relevant documents
	Sub=Group->GetObjs();
	for(Sub.Start(); !Sub.End(); Sub.Next())
	{
		GProfile* sub=Sub();
		docs=sub->GetFdbks();
		for(docs.Start();!docs.End();docs.Next())
		{
			// If not a relevant document -> goes next
			if(docs()->GetFdbk()!=djOK)
				continue;

			GDoc* doc=getDocument()->GetSession()->GetDoc(docs()->GetDocId());
			if(!doc)
				continue;

			// If doc already in OkDocs -> goes next
			if(OkDocs.IsIn(doc))
				continue;

			OkDocs.InsertPtr(doc);
		}
	}

	// Insert documents.
	RCursor<GDoc> docs2(OkDocs);
	for(docs2.Start();!docs2.End();docs2.Next())
	{
		doc=GSession::Get()->GetDoc(docs2()->GetId());
		d=doc->GetUpdated();
		QListViewItemType* prof = new QListViewItemType(doc,Docs,ToQString(doc->GetName()),ToQString(doc->GetURL()),ToQString(d));
		prof->setPixmap(0,QPixmap(KGlobal::iconLoader()->loadIcon("konqueror",KIcon::Small)));
	}
}


//-----------------------------------------------------------------------------
void KViewGroup::ConstructDescription(void)
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
			return(key( col, ascending ).lower().compare( i->key( col, ascending).lower()));
    	}
	};

	// Read 'Ok'
	Vector->clear();
	RCursor<GWeightInfo> Words(Group->GetInfos());
	for (Words.Start();!Words.End();Words.Next())
	{
		new LocalItem(Vector,ToQString(Doc->GetSession()->GetStorage()->LoadConcept(Words()->GetId(),Words()->GetType())), Words()->GetWeight());
	}
}


//-----------------------------------------------------------------------------
void KViewGroup::update(unsigned int cmd)
{
	if(cmd!=2) return;
	ConstructProfiles();
	ConstructGeneral();
	ConstructDescription();
	ConstructDocs();
}


//-----------------------------------------------------------------------------
void KViewGroup::askDocsMenu(QListViewItem*,const QPoint& pt,int)
{
	QPopupMenu* InfoBox=new QPopupMenu(parentWidget());
	InfoBox->insertItem("Save List as XML for HGA",this,SLOT(slotMenu(int)));
	InfoBox->popup(pt);
}


//-----------------------------------------------------------------------------
void KViewGroup::slotMenu(int)
{
	int dlg;
	KURL url;
	GDoc* doc;
	unsigned int i,size,maxsize,newsize,j;
	GWeightInfo** tab;
	GWeightInfo** tmp;

	dlg=KMessageBox::No;
	while(dlg!=KMessageBox::Yes)
	{
		url=KFileDialog::getSaveURL(QString::null,i18n("*.hd"), this, i18n("Save List..."));
		if(url.isEmpty()) return;
		QFile Test(url.path());
		if(Test.exists())
		{
			dlg=KMessageBox::warningYesNoCancel(this,"A Document with this Name exists.\nDo you want to overwrite it?","Warning");
			if(dlg==KMessageBox::No) return;
		}
		else
			dlg=KMessageBox::Yes;
	}

	// Open Document and write header
	RTextFile Res(url.path().latin1());
	Res.Open(R::RIO::Create);
	Res.SetSeparator("");
	Res<<"<?xml version=\"1.0\" ?>\n<!DOCTYPE Hierarchical-Data>\n<Hierarchical-Data>\n\t<Objects>\n";

	// Write Docs
	RCursor<GDoc> theDocs(OkDocs);
	for(theDocs.Start(),i=0,maxsize=0,tab=0;!theDocs.End();theDocs.Next(),i++)
	{
		doc=GSession::Get()->GetDoc(theDocs()->GetId());
		Res<<"\t\t<Object Id=\""<<doc->GetName()<<"\">\n";
		if(doc)
		{
			// Write attributes
			size=doc->GetNb();
			if(!size) continue;
			if(size>maxsize)
			{
				newsize=size+static_cast<unsigned int>(0.5*size);
				tmp=new GWeightInfo*[newsize];
				if(tab)
				{
					memcpy(tmp,tab,maxsize*sizeof(GWeightInfo*));
					delete[] tab;
				}
				maxsize=newsize;
				tab=tmp;
			}
			doc->GetTab(tab);
			qsort(static_cast<void*>(tab),size,sizeof(GWeightInfo*),GWeightInfos::sortOrder);
			for(j=21,size++,tmp=tab;(--j)&&(--size);tmp++)
				Res<<"\t\t\t<Include Attribute=\""<<Doc->GetSession()->GetStorage()->LoadConcept((*tmp)->GetId(),(*tmp)->GetType())<<"\"/>\n";
		}
		Res<<"\t\t</Object>\n";
	}

	// Write footer
	Res<<"\t</Objects>\n</Hierarchical-Data>";
	if(tab)
		delete[] tab;
}


//-----------------------------------------------------------------------------
void KViewGroup::resizeEvent(QResizeEvent *)
{
	Infos->resize(size());
}
