/*

	GALILEI Research Project

	KViewGroup.cpp

	Window to manipulate a specific group - Implementation.

	Copyright 2001 by the Université Libre de Bruxelles.

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
// include files for R Project
#include <rstd/rstring.h>
#include <rstd/rtextfile.h>
using namespace R;


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <langs/glang.h>
#include <sessions/gsession.h>
#include <profiles/gprofdoc.h>
#include <docs/gdocvector.h>
#include <groups/ggroupvector.h>
#include <profiles/guser.h>
#include <profiles/gprofile.h>
#include <profiles/gsubprofile.h>
#include <galilei/qlistviewitemtype.h>
#include <infos/giwordweight.h>
using namespace GALILEI;


//-----------------------------------------------------------------------------
// includes files for Qt/KDE
#include <qpixmap.h>
#include <qpopmenu.h>
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
KViewGroup::KViewGroup(GGroup* grp,KDoc* doc,QWidget* parent,const char* name,int wflags)
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
	// Create the 'Vector' ListView
	Vector = new QListView(this, "Vector" );
	Infos->insertTab(Vector,"Description");
	sprintf(title,"Words (%u)",static_cast<GGroupVector*>(Group)->GetNbNoNull());
	Vector->addColumn(title);
	Vector->addColumn(QString("Weights"));
	Vector->setSorting(2);
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
	char sDate[20];
	const RDate* d;

	Profiles->clear();
	for(Group->Start();!Group->End();Group->Next())
	{
		GSubProfile* sub=(*Group)();
		d=sub->GetAttached();
		sprintf(sDate,"%i/%i/%i",d->GetDay(),d->GetMonth(),d->GetYear());
		QListViewItemType* subitem=new QListViewItemType(sub->GetProfile(),Profiles,sub->GetProfile()->GetName(),sub->GetProfile()->GetUser()->GetFullName(),sDate);
		subitem->setPixmap(0,QPixmap(KGlobal::iconLoader()->loadIcon("find",KIcon::Small)));
	}
}


//-----------------------------------------------------------------------------
void KViewGroup::ConstructGeneral(void)
{
	GLang* l;
	char sDate[20];

	General->clear();
	new QListViewItem(General,"ID",itou(Group->GetId()).Latin1());
	l=Group->GetLang();
	if(l)
		new QListViewItem(General,"Language",l->GetName());
	else
		new QListViewItem(General,"Language","Unknow");
	switch(Group->GetState())
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
}


//-----------------------------------------------------------------------------
void KViewGroup::ConstructDocs(void)
{
 	const RDate* d;
 	char sDate[20];
 	GProfDocCursor docs;
	GDocCursor docs2;

	// Clear the Widget
	Docs->clear();
	OkDocs.Clear();

	// Goes trough the subprofiles of the group
	// And put in OkDocs all the relevant documents
	for(Group->Start();!Group->End();Group->Next())
	{
		GSubProfile* sub=(*Group)();
		docs=sub->GetProfDocCursor();
		for(docs.Start();!docs.End();docs.Next())
		{
			// If not a relevant document -> goes next
			if((docs()->GetFdbk()!=djOK)&&(docs()->GetFdbk()!=djNav))
				continue;

			// If doc already in OkDocs -> goes next
			if(OkDocs.IsIn(docs()->GetDoc()))
				continue;

			OkDocs.InsertPtr(docs()->GetDoc());
		}
	}

	// Insert documents.
	docs2.Set(OkDocs);
	for(docs2.Start();!docs2.End();docs2.Next())
	{
		d=docs2()->GetUpdated();
		sprintf(sDate,"%i/%i/%i",d->GetDay(),d->GetMonth(),d->GetYear());
		QListViewItemType* prof = new QListViewItemType(docs2(),Docs,docs2()->GetName(),docs2()->GetURL(),sDate);
		prof->setPixmap(0,QPixmap(KGlobal::iconLoader()->loadIcon("konqueror",KIcon::Small)));
	}
}


//-----------------------------------------------------------------------------
void KViewGroup::ConstructDescription(void)
{
	GIWordWeightCursor Cur;
	char tmp[20];
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

	// Change the label of the first column
	sprintf(tmp,"Words (%u)",static_cast<GGroupVector*>(Group)->GetNbNoNull());
	Vector->setColumnText(0,tmp);

	// Read 'Ok'
	Vector->clear();
	Cur=static_cast<GGroupVector*>(Group)->GetVectorCursor();
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		new LocalItem(Vector,Doc->GetSession()->LoadWord(Cur()->GetId(),Group->GetLang()->GetCode()),Cur()->GetWeight());
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
	GDocCursor theDocs;
	GDocVector* doc;
	unsigned int i,size,maxsize,newsize,j;
	GIWordWeight** tab;
	GIWordWeight** tmp;

	dlg=KMessageBox::No;
	while(dlg!=KMessageBox::Yes)
	{
		url=KFileDialog::getSaveURL(QString::null,i18n("*.hd"), this, i18n("Save List..."));
		if(url.isEmpty()) return;
		QFile Test(url.path().latin1());
		if(Test.exists())
		{
			dlg=KMessageBox::warningYesNoCancel(this,"A Document with this Name exists.\nDo you want to overwrite it?","Warning");
			if(dlg==KMessageBox::No) return;
		}
		else
			dlg=KMessageBox::Yes;
	}

	// Open Document and write header
	RTextFile Res(url.path().latin1(),R::Create);
	Res.SetSeparator("");
	Res<<"<?xml version=\"1.0\" ?>\n<!DOCTYPE Hierarchical-Data>\n<Hierarchical-Data>\n\t<Objects>\n";

	// Write Docs
	theDocs.Set(OkDocs);
	for(theDocs.Start(),i=0,maxsize=0,tab=0;!theDocs.End();theDocs.Next(),i++)
	{
		doc=dynamic_cast<GDocVector*>(theDocs());
		Res<<"\t\t<Object Id=\""<<theDocs()->GetName()<<"\">\n";
		if(doc)
		{
			// Write attributes
			size=doc->NbPtr;
			if(!size) continue;
			if(size>maxsize)
			{
				newsize=size+static_cast<unsigned int>(0.5*size);
				tmp=new GIWordWeight*[newsize];
				if(tab)
				{
					memcpy(tmp,tab,maxsize*sizeof(GIWordWeight*));
					delete[] tab;
				}
				maxsize=newsize;
				tab=tmp;
			}
			memcpy(tab,doc->Tab,size*sizeof(GIWordWeight*));
			qsort(static_cast<void*>(tab),size,sizeof(GIWordWeight*),GIWordsWeights::sortOrder);
			for(j=21,size++,tmp=tab;(--j)&&(--size);tmp++)
				Res<<"\t\t\t<Include Attribute=\""<<Doc->GetSession()->LoadWord((*tmp)->GetId(),theDocs()->GetLang()->GetCode())<<"\"/>\n";
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
