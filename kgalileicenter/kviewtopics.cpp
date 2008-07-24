/*

	GALILEI Research Project

	KViewTopics.cpp

	Window to manipulate the topics - Implementation.

	Copyright 2001-2008 by the Université Libre de Bruxelles.

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
#include <gdoc.h>
#include <gtopic.h>
#include <glang.h>
#include <gconcept.h>
#include <gconcepttype.h>
#include <gweightinfo.h>
#include <gsession.h>
#include <gstorage.h>
#include <qlistviewitemtype.h>
#include <rqt.h>
#include <ggalileiapp.h>
using namespace GALILEI;
using namespace R;


//-----------------------------------------------------------------------------
// includes files for Qt
#include <qpixmap.h>
#include <qpushbutton.h>
#include <qcheckbox.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qlayout.h>
#include <qpopupmenu.h>


//-----------------------------------------------------------------------------
// include files for KDE
#include <kiconloader.h>
#include <kmessagebox.h>


//-----------------------------------------------------------------------------
// include files for current application
#include "kviewtopics.h"
#include "qsessionprogress.h"
#include "kdoc.h"




//-----------------------------------------------------------------------------
//
// QDragDoc
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
QDragDoc::QDragDoc(QWidget* dragSource,QListViewItem* src,const char* name)
	: QDragObject(dragSource,name), Src(dynamic_cast<QListViewItemType*>(src))
{
}


//-----------------------------------------------------------------------------
const char* QDragDoc::format(int i) const
{
	if(i>1)
		return(0);
	return("galilei");
}


//-----------------------------------------------------------------------------
QByteArray QDragDoc::encodedData(const char*) const
{
	QByteArray Val(0);
	return(Val);
}


//-----------------------------------------------------------------------------
bool QDragDoc::canDecode(const QMimeSource* e )
{
	return(strcmp(e->format(0),"galilei")==0);
}


//-----------------------------------------------------------------------------
//
// QTopics
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
QTopics::QTopics(QWidget* parent,const char* name)
 : QListView(parent,name), Cur(0)
{
	setRootIsDecorated(TRUE);
	setAcceptDrops(TRUE);
    viewport()->setAcceptDrops(TRUE);
	setSelectionMode(Single);
	dragging=false;
	setSorting(-1);
	connect(this,SIGNAL(rightButtonPressed(QListViewItem*,const QPoint&,int)),SLOT(slotRightButton(QListViewItem*,const QPoint&,int)));
	connect(this,SIGNAL(doubleClicked(QListViewItem*)),parent->parent()->parent()->parent(),SLOT(slotHandleItem(QListViewItem*)));
}


//-----------------------------------------------------------------------------
void QTopics::contentsDragEnterEvent(QDragEnterEvent *evt )
{
	if(QDragDoc::canDecode(evt))
		evt->accept();
}


//-----------------------------------------------------------------------------
void QTopics::contentsDropEvent( QDropEvent *evt )
{
	QListView* Wid=dynamic_cast<QListView*>(evt->source());
	QListViewItemType* dest=dynamic_cast<QListViewItemType*>(itemAt(evt->pos()));

	// Verify that dest, src and widget are OK, and the object to drag is a profile
	if((!Wid)||(!dest)||(!Cur)||(!Cur->Src->Obj.Doc))
	{
		clearSelection();
		Cur=0;
		return;
	}

	// Verify that the destination is either a group or another profile
	if((dest->Type!=otDoc)&&(dest->Type!=otTopic))
	{
		clearSelection();
		Cur=0;
		return;
	}

	// Verify that the languages are compatible
	QListViewItemType* group;
	if(dest->Type==otDoc)
		group=dynamic_cast<QListViewItemType*>(dest->parent());
	else
		group=dynamic_cast<QListViewItemType*>(dest);

	// Move it (in real and visually)
	QListViewItemType* SrcTopic=dynamic_cast<QListViewItemType*>(Cur->Src->parent());
	SrcTopic->takeItem(Cur->Src);
	group->insertItem(Cur->Src);
	GSession* session=dynamic_cast<KView*>(parent())->getDocument()->GetSession();
	session->GetTopic(Cur->Src->Obj.Doc->GetGroupId())->DeleteObj(Cur->Src->Obj.Doc);
	group->Obj.Topic->InsertObj(Cur->Src->Obj.Doc);
	group->setText(0,"Topic ("+QString::number(group->Obj.Topic->GetId())+")");
	group->setText(1,QString::number(group->Obj.Topic->GetNbObjs()));
	if(SrcTopic->Obj.Topic->GetNbObjs())
		SrcTopic->setText(0,"Topic ("+QString::number(SrcTopic->Obj.Topic->GetNbObjs())+")");
	else
	{
		session->DeleteTopic(SrcTopic->Obj.Topic);
		delete SrcTopic;
	}

	// Drop finished
	Cur=0;
	clearSelection();
}


//-----------------------------------------------------------------------------
void QTopics::contentsMousePressEvent(QMouseEvent* evt)
{
	QListView::contentsMousePressEvent(evt);
	QListViewItemType* src=dynamic_cast<QListViewItemType*>(itemAt(evt->pos()));
	if(src&&(src->Type==otDoc)&&(evt->button()==LeftButton))
		dragging=true;
}


//-----------------------------------------------------------------------------
void QTopics::contentsMouseMoveEvent(QMouseEvent* evt)
{
	if(dragging)
	{
		Cur = new QDragDoc(this,itemAt(evt->pos()));
		Cur->dragCopy();
		dragging = false;
	}
}


//-----------------------------------------------------------------------------
void QTopics::slotRightButton(QListViewItem* item,const QPoint& pos,int)
{
	QListViewItemType* src=dynamic_cast<QListViewItemType*>(item);
	if(!src) return;
	QPopupMenu Menu(this,"Local");
	if(src->Type==otTopic)
		Menu.insertItem("Delete Topic",this,SLOT(slotDelete(void)));
	if(src->Type==otDoc)
		Menu.insertItem("Remove Doc",this,SLOT(slotDelete(void)));
	if(src->Type==otLang)
		Menu.insertItem("&Create New Topic",this,SLOT(slotNewTopic(void)));
	Menu.insertSeparator();
	Menu.insertItem("Save Topics",this,SLOT(slotSaveTopics(void)));
	Menu.exec(pos);
}


//-----------------------------------------------------------------------------
void QTopics::slotNewTopic(void)
{
	QListViewItemType* src=dynamic_cast<QListViewItemType*>(currentItem());

	RToDo("Create New group");
	// Verify that the current tag is language
	if((!src)||(src->Type!=otLang)||(!src->Obj.Lang))
		return;

	// Create a new group in session
	GSession* session=dynamic_cast<KView*>(parent())->getDocument()->GetSession();
	GTopic* Topic=new GTopic(cNoRef,RDate::GetToday(),RDate::Null);
	session->AssignId(Topic);
	session->InsertTopic(Topic);

	// Create a new group in the qListView
	QListViewItemType* gritem= new QListViewItemType(Topic,src,"Topic "+QString::number(Topic->GetId()),"0");
	gritem->setPixmap(0,QPixmap(KGlobal::iconLoader()->loadIcon("window_new.png",KIcon::Small)));
}


//-----------------------------------------------------------------------------
void QTopics::slotDelete(void)
{
	QListViewItemType* Src=dynamic_cast<QListViewItemType*>(currentItem());

	// Verify that the destination is either a group or another profile
	if((!Src)||((Src->Type!=otDoc)&&(Src->Type!=otTopic)))
		return;

	// Get the session
	GSession* session=dynamic_cast<KView*>(parent())->getDocument()->GetSession();

	// If subprofile -> remove it from its group. If the group is empty, delete it.
	if(Src->Type==otDoc)
	{
		if(KMessageBox::warningYesNo(this,"Do you want to remove this profile from the group?","Warning")==KMessageBox::No)
			return;
		QListViewItemType* SrcTopic=dynamic_cast<QListViewItemType*>(Src->parent());
		session->GetTopic(Src->Obj.Doc->GetGroupId())->DeleteObj(Src->Obj.Doc);
		delete Src;
		if(SrcTopic->Obj.Topic->GetNbObjs())
		{
			SrcTopic->setText(0,"Topic ("+QString::number(SrcTopic->Obj.Topic->GetId())+")");
			SrcTopic->setText(1,QString::number(SrcTopic->Obj.Topic->GetNbObjs()));
		}
		else
		{
			session->DeleteTopic(SrcTopic->Obj.Topic);
			delete SrcTopic;
		}
	}

	// If this is a group -> delete it.
	if(Src->Type==otTopic)
	{
		if(KMessageBox::warningYesNo(this,"Do you want to delete this group?","Warning")==KMessageBox::No)
			return;
		Src->Obj.Topic->DeleteObjs();
		session->DeleteTopic(Src->Obj.Topic);
		delete Src;
	}
}


//-----------------------------------------------------------------------------
void QTopics::slotSaveTopics(void)
{
	if(KMessageBox::warningYesNo(this,"Do you want to overwrite the groups in the database?","Warning")==KMessageBox::No)
		return;
	GSession* session=dynamic_cast<KView*>(parent())->getDocument()->GetSession();
	session->GetStorage()->SaveCommunities();
}


//-----------------------------------------------------------------------------
QTopics::~QTopics(void)
{
}



//-----------------------------------------------------------------------------
//
// class KViewTopics
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
KViewTopics::KViewTopics(KDoc* doc,QWidget* parent,const char* name,int wflags)
	: KView(doc,parent,name,wflags)
{
	setCaption("List of Topics");
	setIcon(QPixmap(KGlobal::iconLoader()->loadIcon("kmultiple.png",KIcon::Small)));

	// Main Layout
	QVBoxLayout* MainLayout = new QVBoxLayout(this,0,0,"MainLayout");

	// Search Bar
	QHBoxLayout* SearchLayout = new QHBoxLayout(0,0,0,"SearchLayout");
	SearchLayout->setAlignment(Qt::AlignTop);
	QLabel* lbl=new QLabel(this,"lbl");
	lbl->setText("Find: ");
	SearchLayout->addWidget(lbl);
//	SearchLayout->addItem(new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum ));
	What=new QLineEdit(this);
	What->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)1, (QSizePolicy::SizeType)0, 0, 0, What->sizePolicy().hasHeightForWidth() ) );
	What->setAutoMask( FALSE );
	What->setEchoMode( QLineEdit::Normal );
	connect(What,SIGNAL(returnPressed()),this,SLOT(FindNext()));
	SearchLayout->addWidget(What);
	Search=new QPushButton(this);
	Search->setAutoDefault(TRUE);
	Search->setText("Find Next");
	connect(Search,SIGNAL(clicked()),this,SLOT(FindNext()));
	SearchLayout->addWidget(Search);
	Desc=new QCheckBox("Description",this);
	SearchLayout->addWidget(Desc);
	SearchLayout->addItem(new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum ));
    MainLayout->addLayout(SearchLayout);

	// Topics
	Topics = new QTopics(this);
	Topics->resize(size());
	Topics->addColumn(QString("Title"));
	Topics->addColumn(QString("URL"));
	Topics->addColumn(QString("Attached"));
	Topics->setRootIsDecorated(true);
	MainLayout->addWidget(Topics);
	ConstructTopics();
}


//-----------------------------------------------------------------------------
GTopic* KViewTopics::GetCurrentTopic(void)
{
	QListViewItemType* t;

	t=(QListViewItemType*)Topics->selectedItem();
	if(!t)
		return(0);
	if(t->Type!=otTopic)
		return(0);
	return(t->Obj.Topic);
}


//-----------------------------------------------------------------------------
void KViewTopics::ConstructTopics(void)
{
	Topics->clear();
	R::RCursor<GTopic> Grp=Doc->GetSession()->GetTopics();
	for(Grp.Start();!Grp.End();Grp.Next())
	{
		QListViewItemType* gritem= new QListViewItemType(Grp(),Topics,"Topic ("+QString::number(Grp()->GetId())+")",QString::number(Grp()->GetNbObjs()));
		gritem->setPixmap(0,QPixmap(KGlobal::iconLoader()->loadIcon("window_new.png",KIcon::Small)));
		RCursor<GDoc> Prof(Grp()->GetObjs());
		for(Prof.Start();!Prof.End();Prof.Next())
		{
			QListViewItemType* subitem=new QListViewItemType(Prof(),gritem,ToQString(Prof()->GetName()),ToQString(Prof()->GetURL()),ToQString(Prof()->GetAttached()));
			subitem->setPixmap(0,QPixmap(KGlobal::iconLoader()->loadIcon("find.png",KIcon::Small)));
		}
	}
	Topics->setCurrentItem(Topics->firstChild());
}


//-----------------------------------------------------------------------------
void KViewTopics::update(tObjType type)
{
	if(type!=otTopic) return;
	ConstructTopics();
}


//-----------------------------------------------------------------------------
void KViewTopics::FindNext(void)
{
	QListViewItemIterator* it;
	bool Cont=true;
	bool LastTimeFound;
	QString str=What->text();

	if(Topics->currentItem())
	{
		it=new QListViewItemIterator(Topics->currentItem());
		LastTimeFound=true;
		// If starting from current item, begin with the next one
		if(it->current())
			(++(*it));
	}
	else
	{
		it=new QListViewItemIterator(Topics);
		LastTimeFound=false;
	}
	while(Cont)
	{
		while(it->current()&&Cont)
		{
			QListViewItemType* item = dynamic_cast<QListViewItemType*>(it->current());
			if((item)&&(item->Type==otDoc))
			{
				if((item->text(0).contains(str,false))||(item->text(1).contains(str,false)))
				{
					Cont=false;
					Topics->setCurrentItem(item);
					Topics->ensureItemVisible(item);
				}
				else
					if(Desc->isChecked())
					{
						GDoc* sub=item->Obj.Doc;
//						GConcept* find=sub->GetLang()->GetDict()->GetConcept(sub->GetLang()->GetStemming(str.latin1()));
						GConcept* find=0;
						RToDo("Find right concept");
						if(find&&(sub->IsIn(find)))
						{
							Cont=false;
							Topics->setCurrentItem(item);
							Topics->ensureItemVisible(item);
						}
					}
			}
			(++(*it));
		}
		if(Cont)
		{
			if(LastTimeFound)
			{
				delete it;
				it=new QListViewItemIterator(Topics);
				LastTimeFound=false;
			}
			else
				Cont=false;
		}
	}
	delete it;
}


//-----------------------------------------------------------------------------
KViewTopics::~KViewTopics(void)
{
}
