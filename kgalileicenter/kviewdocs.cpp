/*

	GALILEI Research Project

	KViewDocs.cpp

	Window to show all the documents - Implementation.

	(C) 2001 by P. Francq & D. Wartel.

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
#include <rstd/rcontainercursor.h>
using namespace RStd;


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <filters/gmimefilter.h>
#include <galilei/qlistviewitemtype.h>
#include <docs/gdoc.h>
#include <langs/glang.h>
#include <sessions/gsession.h>
using namespace GALILEI;


//-----------------------------------------------------------------------------
// include files for Qt
#include <qpixmap.h>
#include <qlistview.h>


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
	{Item->setPixmap(0,QPixmap("/usr/share/icons/hicolor/16x16/apps/locale.png"));}
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
	setIcon(QPixmap("/usr/share/icons/hicolor/16x16/apps/window_list.png"));
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
	if(t->Type!=QListViewItemType::tDocument)
		return(0);
	return(t->Obj.Doc);
}


//-----------------------------------------------------------------------------
void KViewDocs::CreateDocsListView(void)
{
	RContainerCursor<GLang,unsigned int,true,true> CurLang(Doc->GetSession()->GetLangs());
	RContainerCursor<GDoc,unsigned int,true,true> CurDocs(Doc->GetSession()->GetDocs());
	RContainer<LangItem,unsigned int,true,true> LangItems(Doc->GetSession()->GetNbLangs());
	const GMIMEFilter* t;
	const char det[]="Unknown";
	const char* ptr;

	// Go trough each language and create a Item.
	for(CurLang.Start(); !CurLang.End(); CurLang.Next())
		LangItems.InsertPtr(new LangItem(CurLang(),new QListViewItemType(Docs, CurLang()->GetName())));
	LangItems.InsertPtr(new LangItem(0,new QListViewItemType(Docs, "????")));

	// Go through the documents and attach to the item of the corresponding language.
	for(CurDocs.Start();!CurDocs.End();CurDocs.Next())
	{
		t=CurDocs()->GetMIMEType();
		if(t)
			ptr=t->GetName();
		else
			ptr=det;
		QListViewItemType* docitem= new QListViewItemType(CurDocs(),LangItems.GetPtr<const GLang*>(CurDocs()->GetLang())->Item,CurDocs()->GetName(),CurDocs()->GetURL(),ptr);
		docitem->setPixmap(0,QPixmap("/usr/share/icons/hicolor/16x16/mimetypes/document.png"));
		docitem->setPixmap(1,QPixmap("/usr/share/icons/hicolor/16x16/apps/konqueror.png"));
	}
}


//-----------------------------------------------------------------------------
void KViewDocs::update(unsigned int cmd)
{
	if(cmd!=0) return;
	Docs->clear();
	CreateDocsListView();
}


//-----------------------------------------------------------------------------
void KViewDocs::resizeEvent(QResizeEvent *)
{
	//resize of the listview
	Docs->setGeometry(0,0,this->width(),this->height());
	Docs->setColumnWidth(0,(this->width()/2)-2);
	Docs->setColumnWidth(1,(this->width()/2)-2);
}


//-----------------------------------------------------------------------------
KViewDocs::~KViewDocs(void)
{
}
