/*

	GALILEI Research Project

	KViewTopic.cpp

	Window to manipulate a specific topic - Implementation.

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
#include <qlistviewitemtype.h>
#include <rqt.h>
#include <gweightinfo.h>
#include <gtopic.h>
#include <qgweightinfos.h>
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
#include "kviewtopic.h"
#include "qsessionprogress.h"



//-----------------------------------------------------------------------------
//
// class KViewTopic
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
KViewTopic::KViewTopic(GTopic* grp,KDoc* doc,QWidget* parent,const char* name,int wflags)
	: KView(doc,parent,name,wflags), Topic(grp)
{
	setCaption("Topic "+QString::number(grp->GetId()));
	setIcon(QPixmap(KGlobal::iconLoader()->loadIcon("window_new",KIcon::Small)));

	// initialization of the tab widget
	Infos=new QTabWidget(this);
	Infos->resize(size());

	// Initialization of the General Information Widget
	General = new QListView(Infos);
	Infos->insertTab(General,"General Information");
	General->addColumn("Variable");
	General->addColumn("Value");
	ConstructGeneral();

	// Initialization of the Docs Widget
	Docs = new QListView(Infos);
	Infos->insertTab(Docs,"Docs");
	Docs->addColumn(QString("Title"));
	Docs->addColumn(QString("URL"));
	Docs->addColumn(QString("Attached"));
	Docs->setRootIsDecorated(true);
	connect(Docs,SIGNAL(doubleClicked(QListViewItem*)),parent->parent()->parent(),SLOT(slotHandleItem(QListViewItem*)));
	ConstructDocs();

	// Initialization of Description
	Vector = new QGWeightInfos(Infos,Topic,Doc->GetSession());
	Infos->insertTab(Vector,"Description");
}


//-----------------------------------------------------------------------------
void KViewTopic::ConstructDocs(void)
{
	RDate d;
	RCursor<GDoc> Sub;

	Docs->clear();
	Sub=Topic->GetObjs();
	for(Sub.Start(); !Sub.End(); Sub.Next())
	{
		GDoc* sub=Sub();
		d=sub->GetAttached();
		QListViewItemType* subitem=new QListViewItemType(sub,Docs,ToQString(sub->GetName()),ToQString(sub->GetURL()),ToQString(d));
		subitem->setPixmap(0,QPixmap(KGlobal::iconLoader()->loadIcon("find",KIcon::Small)));
	}
}


//-----------------------------------------------------------------------------
void KViewTopic::ConstructGeneral(void)
{
	General->clear();
	new QListViewItem(General,"ID",ToQString(RString::Number(Topic->GetId())));
	new QListViewItem(General,"Status",ToQString(GetState(Topic->GetState())));
}


//-----------------------------------------------------------------------------
void KViewTopic::update(tObjType type)
{
	if(type!=otTopic) return;
	ConstructDocs();
	ConstructGeneral();
	Vector->SetObject(Topic);
}


//-----------------------------------------------------------------------------
void KViewTopic::slotMenu(int)
{
	int dlg;
	KURL url;
	GDoc* doc;
	size_t i,size,maxsize,newsize,j;
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
	RCursor<GDoc> theDocs(Topic->GetObjs());
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
				newsize=size+static_cast<size_t>(0.5*size);
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
void KViewTopic::resizeEvent(QResizeEvent *)
{
	Infos->resize(size());
}
