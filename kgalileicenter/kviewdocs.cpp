/*

	GALILEI Research Project

	KViewDocs.cpp

	Window to show all the documents - Implementation.

	Copyright 2001 by the Universit�Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).
		David Wartel (dwartel@ulb.ac.be).

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
// include files for GALILEI
#include <qlistviewitemtype.h>
#include <rqt.h>
#include <gdoc.h>
#include <glang.h>
#include <gsession.h>
#include <ggalileiapp.h>
using namespace GALILEI;
using namespace R;


//-----------------------------------------------------------------------------
// include files for Qt
#include <qpixmap.h>
#include <qlistview.h>


//-----------------------------------------------------------------------------
// include files for KDE
#include <kiconloader.h>


//-----------------------------------------------------------------------------
// includes files for current application
#include "kviewdocs.h"
#include "kdoc.h"



//-----------------------------------------------------------------------------
//
// class LangItem
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
struct LangItem
{
	GLang* Lang;
	QListViewItem* Item;

	LangItem(GLang* l,QListViewItem* i) : Lang(l), Item(i)
	{Item->setPixmap(0,QPixmap(KGlobal::iconLoader()->loadIcon("locale.png",KIcon::Small)));}
	int Compare(const LangItem* l) const;
	int Compare(const LangItem& l) const;
	int Compare(const GLang* l) const;
};


//-----------------------------------------------------------------------------
int LangItem::Compare(const LangItem* l) const
{
	if((!Lang)&&(!l->Lang)) return(0);
	if(!Lang) return(1);
	if(!l->Lang) return(-1);
	return(Lang->Compare(l->Lang));
}


//-----------------------------------------------------------------------------
int LangItem::Compare(const LangItem& l) const
{
	if((!Lang)&&(!l.Lang)) return(0);
	if(!Lang) return(1);
	if(!l.Lang) return(-1);
	return(Lang->Compare(l.Lang));
}


//-----------------------------------------------------------------------------
int LangItem::Compare(const GLang* l) const
{
	if((!Lang)&&(!l)) return(0);
	if(!Lang) return(1);
	if(!l) return(-1);
	return(Lang->Compare(l));
}



//-----------------------------------------------------------------------------
//
// class KViewDocs
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
KViewDocs::KViewDocs(KDoc* doc,QWidget* parent,const char* name,int wflags)
	: KView(doc,parent,name,wflags)
{
	setCaption("List of documents");
	setIcon(QPixmap(KGlobal::iconLoader()->loadIcon("window_list.png",KIcon::Small)));
	Docs = new QListView(this,"QListView of KViewDocs");
	Docs->addColumn(QString("Title"));
	Docs->addColumn(QString("URL"));
	Docs->addColumn(QString("MIME Type"));
	Docs->setRootIsDecorated(true);
	connect(Docs,SIGNAL(doubleClicked(QListViewItem*)),parent->parent()->parent(),SLOT(slotHandleItem(QListViewItem*)));
	CreateDocsListView();
}


//-----------------------------------------------------------------------------
GDoc* KViewDocs::GetCurrentDoc(void)
{
	QListViewItemType* t;

	t=(QListViewItemType*)Docs->selectedItem();
	if(!t)
		return(0);
	if(t->Type!=otDoc)
		return(0);
	return(t->Obj.Doc);
}


//-----------------------------------------------------------------------------
void KViewDocs::CreateDocsListView(void)
{
	R::RCursor<GDoc> CurDocs=Doc->GetSession()->GetDocs();
	R::RCursor<GFactoryLang> CurLang=GALILEIApp->GetManager<GLangManager>("Lang")->GetFactories();
	GLang* lang;
	RContainer<LangItem,true,true> LangItems(GALILEIApp->GetManager<GLangManager>("Lang")->GetNbFactories());
	const char* t;
	const char det[]="Unknown";
	const char* ptr;

	// Go trough each language and create a Item.
	CurLang=GALILEIApp->GetManager<GLangManager>("Lang")->GetFactories();
	for(CurLang.Start(); !CurLang.End(); CurLang.Next())
	{
		lang=CurLang()->GetPlugin();
		if(!lang) continue;
		LangItems.InsertPtr(new LangItem(lang,new QListViewItemType(Docs,ToQString(lang->GetName()))));
	}
	LangItems.InsertPtr(new LangItem(0,new QListViewItemType(Docs, "????")));

	// Go through the documents and attach to the item of the corresponding language.
	for(CurDocs.Start();!CurDocs.End();CurDocs.Next())
	{
		t=CurDocs()->GetMIMEType();
		if(t)
			ptr=t;
		else
			ptr=det;
		QListViewItemType* docitem= new QListViewItemType(CurDocs(),LangItems.GetPtr<const GLang*>(CurDocs()->GetLang())->Item,ToQString(CurDocs()->GetName()),ToQString(CurDocs()->GetURL()),ptr);
		docitem->setPixmap(0,QPixmap(KGlobal::iconLoader()->loadIcon("document.png",KIcon::Small)));
		docitem->setPixmap(1,QPixmap(KGlobal::iconLoader()->loadIcon("konqueror.png",KIcon::Small)));
	}
}


//-----------------------------------------------------------------------------
void KViewDocs::update(tObjType type)
{
	if(type!=otTopic) return;
	Docs->clear();
	CreateDocsListView();
}


//-----------------------------------------------------------------------------
void KViewDocs::resizeEvent(QResizeEvent *)
{
	//resize of the listview
	Docs->setGeometry(0,0,this->width(),this->height());
	Docs->setColumnWidth(0,(this->width()/3)-2);
	Docs->setColumnWidth(1,(this->width()/3)-2);
	Docs->setColumnWidth(2,(this->width()/3)-2);
}


//-----------------------------------------------------------------------------
KViewDocs::~KViewDocs(void)
{
}
