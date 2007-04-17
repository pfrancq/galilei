/*

	GALILEI Research Project

	KViewGroups.cpp

	Window to manipulate the groups - Implementation.

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
#include <guser.h>
#include <gprofile.h>
#include <gsubprofile.h>
#include <ggroup.h>
#include <glang.h>
#include <gdict.h>
#include <gconcept.h>
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
// QDragSubProfile
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
QDragSubProfile::QDragSubProfile(QWidget* dragSource,QListViewItem* src,const char* name)
	: QDragObject(dragSource,name), Src(dynamic_cast<QListViewItemType*>(src))
{
}


//-----------------------------------------------------------------------------
const char* QDragSubProfile::format(int i) const
{
	if(i>1)
		return(0);
	return("galilei");
}


//-----------------------------------------------------------------------------
QByteArray QDragSubProfile::encodedData(const char*) const
{
	QByteArray Val(0);
	return(Val);
}


//-----------------------------------------------------------------------------
bool QDragSubProfile::canDecode(const QMimeSource* e )
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
	if(QDragSubProfile::canDecode(evt))
		evt->accept();
}


//-----------------------------------------------------------------------------
void QGroups::contentsDropEvent( QDropEvent *evt )
{
	QListView* Wid=dynamic_cast<QListView*>(evt->source());
	QListViewItemType* dest=dynamic_cast<QListViewItemType*>(itemAt(evt->pos()));

	// Verify that dest, src and widget are OK, and the object to drag is a subprofile
	if((!Wid)||(!dest)||(!Cur)||(!Cur->Src->Obj.SubProfile))
	{
		clearSelection();
		Cur=0;
		return;
	}

	// Verify that the destination is either a group or another subprofile
	if((dest->Type!=QListViewItemType::tSubProfile)&&(dest->Type!=QListViewItemType::tGroup))
	{
		clearSelection();
		Cur=0;
		return;
	}

	// Verify that the languages are compatible
	QListViewItemType* group;
	if(dest->Type==QListViewItemType::tSubProfile)
		group=dynamic_cast<QListViewItemType*>(dest->parent());
	else
		group=dynamic_cast<QListViewItemType*>(dest);
	if(group->Obj.Group->GetLang()!=Cur->Src->Obj.SubProfile->GetLang())
	{
		clearSelection();
		Cur=0;
		return;
	}

	// Move it (in real and visualy)
	QListViewItemType* SrcGroup=dynamic_cast<QListViewItemType*>(Cur->Src->parent());
	SrcGroup->takeItem(Cur->Src);
	group->insertItem(Cur->Src);
	GSession* session=dynamic_cast<KView*>(parent())->getDocument()->GetSession();
	session->GetGroup(Cur->Src->Obj.SubProfile->GetGroupId())->DeleteSubProfile(Cur->Src->Obj.SubProfile);
	group->Obj.Group->InsertSubProfile(Cur->Src->Obj.SubProfile);
	group->setText(0,"Group ("+QString::number(group->Obj.Group->GetNbSubProfiles())+")");
	if(SrcGroup->Obj.Group->GetNbSubProfiles())
		SrcGroup->setText(0,"Group ("+QString::number(SrcGroup->Obj.Group->GetNbSubProfiles())+")");
	else
	{
		session->DeleteGroup(SrcGroup->Obj.Group);
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
	if(src&&(src->Type==QListViewItemType::tSubProfile)&&(evt->button()==LeftButton))
		dragging=true;
}


//-----------------------------------------------------------------------------
void QGroups::contentsMouseMoveEvent(QMouseEvent* evt)
{
	if(dragging)
	{
		Cur = new QDragSubProfile(this,itemAt(evt->pos()));
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
	if(src->Type==QListViewItemType::tSubProfile)
		Menu.insertItem("Remove SubProfile",this,SLOT(slotDelete(void)));
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

	// Verify that the current tag is language
	if((!src)||(src->Type!=QListViewItemType::tLang)||(!src->Obj.Lang))
		return;

	// Create a new group in session
	GSession* session=dynamic_cast<KView*>(parent())->getDocument()->GetSession();
	GGroup* Group=new GGroup(cNoRef,src->Obj.Lang,true,RDate::GetToday(),RDate::null);
	session->AssignId(Group);
	session->InsertGroup(Group);

	// Create a new group in the qListView
	QListViewItemType* gritem= new QListViewItemType(Group,src,"Group (0)");
	gritem->setPixmap(0,QPixmap(KGlobal::iconLoader()->loadIcon("window_new.png",KIcon::Small)));
}


//-----------------------------------------------------------------------------
void QGroups::slotDelete(void)
{
	QListViewItemType* Src=dynamic_cast<QListViewItemType*>(currentItem());

	// Verify that the destination is either a group or another subprofile
	if((!Src)||((Src->Type!=QListViewItemType::tSubProfile)&&(Src->Type!=QListViewItemType::tGroup)))
		return;

	// Get the session
	GSession* session=dynamic_cast<KView*>(parent())->getDocument()->GetSession();

	// If subprofile -> remove it from its group. If the group is empty, delete it.
	if(Src->Type==QListViewItemType::tSubProfile)
	{
		if(KMessageBox::warningYesNo(this,"Do you want to remove this subprofile from the group?","Warning")==KMessageBox::No)
			return;
		QListViewItemType* SrcGroup=dynamic_cast<QListViewItemType*>(Src->parent());
		session->GetGroup(Src->Obj.SubProfile->GetGroupId())->DeleteSubProfile(Src->Obj.SubProfile);
		delete Src;
		if(SrcGroup->Obj.Group->GetNbSubProfiles())
			SrcGroup->setText(0,"Group ("+QString::number(SrcGroup->Obj.Group->GetNbSubProfiles())+")");
		else
		{
			session->DeleteGroup(SrcGroup->Obj.Group);
			delete SrcGroup;
		}
	}

	// If this is a group -> delete it.
	if(Src->Type==QListViewItemType::tGroup)
	{
		if(KMessageBox::warningYesNo(this,"Do you want to delete this group?","Warning")==KMessageBox::No)
			return;
		Src->Obj.Group->DeleteSubProfiles();
		session->DeleteGroup(Src->Obj.Group);
		delete Src;
	}
}


//-----------------------------------------------------------------------------
void QGroups::slotSaveGroups(void)
{
	if(KMessageBox::warningYesNo(this,"Do you want to overwrite the groups in the database?","Warning")==KMessageBox::No)
		return;
	GSession* session=dynamic_cast<KView*>(parent())->getDocument()->GetSession();
	R::RCursor<GLang> CurLang(GALILEIApp->GetManager<GLangManager>("Lang")->GetPlugIns());	
	for(CurLang.Start();!CurLang.End();CurLang.Next())
		session->GetStorage()->SaveGroups(CurLang());
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
GGroup* KViewGroups::GetCurrentGroup(void)
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
	R::RCursor<GFactoryLang> CurLang;
	GLang* lang;
	RDate d;
	RCursor<GSubProfile> Sub;

	Groups->clear();
	CurLang=GALILEIApp->GetManager<GLangManager>("Lang")->GetFactories();
	for(CurLang.Start();!CurLang.End();CurLang.Next())
	{
		lang=CurLang()->GetPlugin();
		if(!lang) continue;
		if(!Doc->GetSession()->GetNbGroups(lang))
			continue;
		R::RCursor<GGroup> grs=Doc->GetSession()->GetGroups(lang);
		QListViewItemType* grsitem = new QListViewItemType(lang,Groups,ToQString(lang->GetName())+" ("+QString::number(grs.GetNb())+")");
		grsitem->setPixmap(0,QPixmap(KGlobal::iconLoader()->loadIcon("locale.png",KIcon::Small)));
		for(grs.Start(); !grs.End(); grs.Next())
		{
			GGroup* gr=grs();
			Sub=grs()->GetSubProfiles();
			QListViewItemType* gritem= new QListViewItemType(gr,grsitem,"Group ("+QString::number(Sub.GetNb())+")");
			gritem->setPixmap(0,QPixmap(KGlobal::iconLoader()->loadIcon("window_new.png",KIcon::Small)));
			for(Sub.Start(); !Sub.End(); Sub.Next())
			{
				GSubProfile* sub=Sub();
				d=sub->GetAttached();
				QListViewItemType* subitem=new QListViewItemType(sub,gritem,ToQString(sub->GetProfile()->GetName()),ToQString(sub->GetProfile()->GetUser()->GetFullName()),ToQString(d));
				subitem->setPixmap(0,QPixmap(KGlobal::iconLoader()->loadIcon("find.png",KIcon::Small)));
			}
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
			if((item)&&(item->Type==QListViewItemType::tSubProfile))
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
						GSubProfile* sub=item->Obj.SubProfile;
						GConcept* find=sub->GetLang()->GetDict()->GetConcept(sub->GetLang()->GetStemming(str.latin1()));
						if(find&&(sub->IsIn(*find)))
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
