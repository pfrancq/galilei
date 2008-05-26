/*

	GALILEI Research Project

	KViewGroups.cpp

	Window to manipulate the groups - Implementation.

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
#include <guser.h>
#include <gprofile.h>
#include <gcommunity.h>
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
#include "kviewgroups.h"
#include "qsessionprogress.h"
#include "kdoc.h"




//-----------------------------------------------------------------------------
//
// QDragProfile
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
QDragProfile::QDragProfile(QWidget* dragSource,QListViewItem* src,const char* name)
	: QDragObject(dragSource,name), Src(dynamic_cast<QListViewItemType*>(src))
{
}


//-----------------------------------------------------------------------------
const char* QDragProfile::format(int i) const
{
	if(i>1)
		return(0);
	return("galilei");
}


//-----------------------------------------------------------------------------
QByteArray QDragProfile::encodedData(const char*) const
{
	QByteArray Val(0);
	return(Val);
}


//-----------------------------------------------------------------------------
bool QDragProfile::canDecode(const QMimeSource* e )
{
	return(strcmp(e->format(0),"galilei")==0);
}


//-----------------------------------------------------------------------------
//
// QGroups
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
QGroups::QGroups(QWidget* parent,const char* name)
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
void QGroups::contentsDragEnterEvent(QDragEnterEvent *evt )
{
	if(QDragProfile::canDecode(evt))
		evt->accept();
}


//-----------------------------------------------------------------------------
void QGroups::contentsDropEvent( QDropEvent *evt )
{
	QListView* Wid=dynamic_cast<QListView*>(evt->source());
	QListViewItemType* dest=dynamic_cast<QListViewItemType*>(itemAt(evt->pos()));

	// Verify that dest, src and widget are OK, and the object to drag is a profile
	if((!Wid)||(!dest)||(!Cur)||(!Cur->Src->Obj.Profile))
	{
		clearSelection();
		Cur=0;
		return;
	}

	// Verify that the destination is either a group or another profile
	if((dest->Type!=QListViewItemType::tProfile)&&(dest->Type!=QListViewItemType::tGroup))
	{
		clearSelection();
		Cur=0;
		return;
	}

	// Verify that the languages are compatible
	QListViewItemType* group;
	if(dest->Type==QListViewItemType::tProfile)
		group=dynamic_cast<QListViewItemType*>(dest->parent());
	else
		group=dynamic_cast<QListViewItemType*>(dest);

	// Move it (in real and visualy)
	QListViewItemType* SrcGroup=dynamic_cast<QListViewItemType*>(Cur->Src->parent());
	SrcGroup->takeItem(Cur->Src);
	group->insertItem(Cur->Src);
	GSession* session=dynamic_cast<KView*>(parent())->getDocument()->GetSession();
	session->GetCommunity(Cur->Src->Obj.Profile->GetGroupId())->DeleteObj(Cur->Src->Obj.Profile);
	group->Obj.Group->InsertObj(Cur->Src->Obj.Profile);
	group->setText(0,"Group ("+QString::number(group->Obj.Group->GetId())+")");
	group->setText(1,QString::number(group->Obj.Group->GetNbObjs()));
	if(SrcGroup->Obj.Group->GetNbObjs())
		SrcGroup->setText(0,"Group ("+QString::number(SrcGroup->Obj.Group->GetNbObjs())+")");
	else
	{
		session->DeleteCommunity(SrcGroup->Obj.Group);
		delete SrcGroup;
	}

	// Drop finished
	Cur=0;
	clearSelection();
}


//-----------------------------------------------------------------------------
void QGroups::contentsMousePressEvent(QMouseEvent* evt)
{
	QListView::contentsMousePressEvent(evt);
	QListViewItemType* src=dynamic_cast<QListViewItemType*>(itemAt(evt->pos()));
	if(src&&(src->Type==QListViewItemType::tProfile)&&(evt->button()==LeftButton))
		dragging=true;
}


//-----------------------------------------------------------------------------
void QGroups::contentsMouseMoveEvent(QMouseEvent* evt)
{
	if(dragging)
	{
		Cur = new QDragProfile(this,itemAt(evt->pos()));
		Cur->dragCopy();
		dragging = false;
	}
}


//-----------------------------------------------------------------------------
void QGroups::slotRightButton(QListViewItem* item,const QPoint& pos,int)
{
	QListViewItemType* src=dynamic_cast<QListViewItemType*>(item);
	if(!src) return;
	QPopupMenu Menu(this,"Local");
	if(src->Type==QListViewItemType::tGroup)
		Menu.insertItem("Delete Group",this,SLOT(slotDelete(void)));
	if(src->Type==QListViewItemType::tProfile)
		Menu.insertItem("Remove Profile",this,SLOT(slotDelete(void)));
	if(src->Type==QListViewItemType::tLang)
		Menu.insertItem("&Create New Group",this,SLOT(slotNewGroup(void)));
	Menu.insertSeparator();
	Menu.insertItem("Save Groups",this,SLOT(slotSaveGroups(void)));
	Menu.exec(pos);
}


//-----------------------------------------------------------------------------
void QGroups::slotNewGroup(void)
{
	QListViewItemType* src=dynamic_cast<QListViewItemType*>(currentItem());

	RToDo("Create New group");
	// Verify that the current tag is language
	if((!src)||(src->Type!=QListViewItemType::tLang)||(!src->Obj.Lang))
		return;

	// Create a new group in session
	GSession* session=dynamic_cast<KView*>(parent())->getDocument()->GetSession();
	GCommunity* Group=new GCommunity(cNoRef,RDate::GetToday(),RDate::null);
	session->AssignId(Group);
	session->InsertCommunity(Group);

	// Create a new group in the qListView
	QListViewItemType* gritem= new QListViewItemType(Group,src,"Group "+QString::number(Group->GetId()),"0");
	gritem->setPixmap(0,QPixmap(KGlobal::iconLoader()->loadIcon("window_new.png",KIcon::Small)));
}


//-----------------------------------------------------------------------------
void QGroups::slotDelete(void)
{
	QListViewItemType* Src=dynamic_cast<QListViewItemType*>(currentItem());

	// Verify that the destination is either a group or another profile
	if((!Src)||((Src->Type!=QListViewItemType::tProfile)&&(Src->Type!=QListViewItemType::tGroup)))
		return;

	// Get the session
	GSession* session=dynamic_cast<KView*>(parent())->getDocument()->GetSession();

	// If subprofile -> remove it from its group. If the group is empty, delete it.
	if(Src->Type==QListViewItemType::tProfile)
	{
		if(KMessageBox::warningYesNo(this,"Do you want to remove this profile from the group?","Warning")==KMessageBox::No)
			return;
		QListViewItemType* SrcGroup=dynamic_cast<QListViewItemType*>(Src->parent());
		session->GetCommunity(Src->Obj.Profile->GetGroupId())->DeleteObj(Src->Obj.Profile);
		delete Src;
		if(SrcGroup->Obj.Group->GetNbObjs())
		{
			SrcGroup->setText(0,"Group ("+QString::number(SrcGroup->Obj.Group->GetId())+")");
			SrcGroup->setText(1,QString::number(SrcGroup->Obj.Group->GetNbObjs()));
		}
		else
		{
			session->DeleteCommunity(SrcGroup->Obj.Group);
			delete SrcGroup;
		}
	}

	// If this is a group -> delete it.
	if(Src->Type==QListViewItemType::tGroup)
	{
		if(KMessageBox::warningYesNo(this,"Do you want to delete this group?","Warning")==KMessageBox::No)
			return;
		Src->Obj.Group->DeleteObjs();
		session->DeleteCommunity(Src->Obj.Group);
		delete Src;
	}
}


//-----------------------------------------------------------------------------
void QGroups::slotSaveGroups(void)
{
	if(KMessageBox::warningYesNo(this,"Do you want to overwrite the groups in the database?","Warning")==KMessageBox::No)
		return;
	GSession* session=dynamic_cast<KView*>(parent())->getDocument()->GetSession();
	session->GetStorage()->SaveCommunities();
}


//-----------------------------------------------------------------------------
QGroups::~QGroups(void)
{
}



//-----------------------------------------------------------------------------
//
// class KViewGroups
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
KViewGroups::KViewGroups(KDoc* doc,QWidget* parent,const char* name,int wflags)
	: KView(doc,parent,name,wflags)
{
	setCaption("List of Groups");
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

	// Groups
	Groups = new QGroups(this);
	Groups->resize(size());
	Groups->addColumn(QString("Profiles"));
	Groups->addColumn(QString("Users"));
	Groups->addColumn(QString("Attached"));
	Groups->setRootIsDecorated(true);
	MainLayout->addWidget(Groups);
	ConstructGroups();
}


//-----------------------------------------------------------------------------
GCommunity* KViewGroups::GetCurrentGroup(void)
{
	QListViewItemType* t;

	t=(QListViewItemType*)Groups->selectedItem();
	if(!t)
		return(0);
	if(t->Type!=QListViewItemType::tGroup)
		return(0);
	return(t->Obj.Group);
}


//-----------------------------------------------------------------------------
void KViewGroups::ConstructGroups(void)
{
	Groups->clear();
	R::RCursor<GCommunity> Grp=Doc->GetSession()->GetCommunities();
	for(Grp.Start();!Grp.End();Grp.Next())
	{
		QListViewItemType* gritem= new QListViewItemType(Grp(),Groups,"Group ("+QString::number(Grp()->GetId())+")",QString::number(Grp()->GetNbObjs()));
		gritem->setPixmap(0,QPixmap(KGlobal::iconLoader()->loadIcon("window_new.png",KIcon::Small)));
		RCursor<GProfile> Prof(Grp()->GetObjs());
		for(Prof.Start();!Prof.End();Prof.Next())
		{
			QListViewItemType* subitem=new QListViewItemType(Prof(),gritem,ToQString(Prof()->GetAttached()));
			subitem->setPixmap(0,QPixmap(KGlobal::iconLoader()->loadIcon("find.png",KIcon::Small)));
		}
	}	
	Groups->setCurrentItem(Groups->firstChild());
}


//-----------------------------------------------------------------------------
void KViewGroups::update(unsigned int cmd)
{
	if(cmd!=2) return;
	ConstructGroups();
}


//-----------------------------------------------------------------------------
void KViewGroups::FindNext(void)
{
	QListViewItemIterator* it;
	bool Cont=true;
	bool LastTimeFound;
	QString str=What->text();

	if(Groups->currentItem())
	{
		it=new QListViewItemIterator(Groups->currentItem());
		LastTimeFound=true;
		// If starting from current item, begin with the next one
		if(it->current())
			(++(*it));
	}
	else
	{
		it=new QListViewItemIterator(Groups);
		LastTimeFound=false;
	}
	while(Cont)
	{
		while(it->current()&&Cont)
		{
			QListViewItemType* item = dynamic_cast<QListViewItemType*>(it->current());
			if((item)&&(item->Type==QListViewItemType::tProfile))
			{
				if((item->text(0).contains(str,false))||(item->text(1).contains(str,false)))
				{
					Cont=false;
					Groups->setCurrentItem(item);
					Groups->ensureItemVisible(item);
				}
				else
					if(Desc->isChecked())
					{
						GProfile* sub=item->Obj.Profile;
//						GConcept* find=sub->GetLang()->GetDict()->GetConcept(sub->GetLang()->GetStemming(str.latin1()));
						GConcept* find=0;
						RToDo("Find right concept");
						if(find&&(sub->IsIn(find)))
						{
							Cont=false;
							Groups->setCurrentItem(item);
							Groups->ensureItemVisible(item);
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
				it=new QListViewItemIterator(Groups);
				LastTimeFound=false;
			}
			else
				Cont=false;
		}
	}
	delete it;
}


//-----------------------------------------------------------------------------
KViewGroups::~KViewGroups(void)
{
}
