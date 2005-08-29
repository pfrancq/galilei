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
#include <gdata.h>
#include <gweightinfo.h>
#include <gsession.h>
#include <qlistviewitemtype.h>
#include <rqt.h>
#include <gpluginmanagers.h>
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


//-----------------------------------------------------------------------------
// include files for KDE
#include <kiconloader.h>


//-----------------------------------------------------------------------------
// include files for current application
#include "kviewgroups.h"
#include "qsessionprogress.h"
#include "kdoc.h"



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
	Groups = new QListView(this);
	Groups->resize(size());
	Groups->addColumn(QString("Profiles"));
	Groups->addColumn(QString("Users"));
	Groups->addColumn(QString("Attached"));
	Groups->setRootIsDecorated(true);
	MainLayout->addWidget(Groups);
	connect(Groups,SIGNAL(doubleClicked(QListViewItem*)),parent->parent()->parent(),SLOT(slotHandleItem(QListViewItem*)));
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
	CurLang=GPluginManagers::GetManager<GLangManager>("Lang")->GetFactories();
	for(CurLang.Start();!CurLang.End();CurLang.Next())
	{
		lang=CurLang()->GetPlugin();
		if(!lang) continue;
		R::RCursor<GGroup> grs=Doc->GetSession()->GetGroups(lang);
		QListViewItemType* grsitem = new QListViewItemType(Groups,ToQString(lang->GetName())+" ("+QString::number(grs.GetNb())+")");
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
				//QListViewItemType* subitem=new QListViewItemType(sub->GetProfile(),gritem,ToQString(sub->GetProfile()->GetName()),ToQString(sub->GetProfile()->GetUser()->GetFullName()),sDate);
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
						GData* find=sub->GetLang()->GetDict()->GetData(sub->GetLang()->GetStemming(str.latin1()));
						if(find&&(sub->IsIn<unsigned int>(find->GetId())))
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
