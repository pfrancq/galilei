/*

	GALILEI Research Project

	QGGroupsHistory.cpp

	Widget to show the groups history - Implementation.

	Copyright 2002-2008 by the Université Libre de Bruxelles.

	Authors:
		David Wartel <dwartel@ulb.ac.be>
		Pascal Francq <pfrancq@ulb.ac.be>

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
// include files for R
#include <rcursor.h>
#include <rqt.h>
using namespace R;


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <gprofile.h>
#include <ggrouphistory.h>
#include <ggroupshistory.h>
#include <gsubject.h>
#include <guser.h>
#include <gweightinfoshistory.h>
#include <qlistviewitemtype.h>
#include <qggroupshistory.h>
using namespace GALILEI;


//-----------------------------------------------------------------------------
// include files for Qt
#include <qpixmap.h>
#include <qlistview.h>


//-----------------------------------------------------------------------------
// include files for KDE
#include <kiconloader.h>



//-----------------------------------------------------------------------------
//
// class QGGroupsHistory
//
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
GALILEI::QGGroupsHistory::QGGroupsHistory(QWidget* parent,GGroupsHistory* grps,const char* name)
	: QListView(parent,name) ,pixmap(0),Changed(false)
{
	//init listview component.
	addColumn("Object Id");
	addColumn("Profiles Id");
	addColumn("Profile Name");
	addColumn("User Name");
	setResizeMode(QListView::AllColumns);
	setRootIsDecorated(true);
	setSelectionMode(Multi);
	setGroups(grps);
}


//-----------------------------------------------------------------------------
void GALILEI::QGGroupsHistory::setGroups(GGroupsHistory* grps)
{
	char tmp[100];
	char tmp2[40];
	GProfile* p;

	clear();
	RCursor<GGroupHistory> Cur(*grps);
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		sprintf(tmp,"Group %u  (%u)",Cur()->GetId(),Cur()->GetNb());
		QListViewItemType* gritem= new QListViewItemType(Cur(),this,tmp) ;//, gr->GetSubject()->GetName());
		// manage the color of the item
		if(Cur()->IsModified()==true)
			gritem->Level=1;
		else
			gritem->Level=0;
		gritem->setPixmap(0,QPixmap(KGlobal::iconLoader()->loadIcon("window_new.png",KIcon::Small)));
		RCursor<GWeightInfosHistory> Cur2(*Cur());
		for(Cur2.Start();!Cur2.End();Cur2.Next())
		{
			sprintf(tmp,"%i",Cur2()->GetId());
			p=Cur2()->GetProfile();
			sprintf(tmp2,"%i",p->GetId());
			QListViewItemType* subitem=new QListViewItemType(Cur2(),gritem,tmp,tmp2,ToQString(p->GetName()),ToQString(p->GetUser()->GetFullName()));
			if(Cur2()->IsWellGrouped()==true)
				subitem->Level=0;
			else
				subitem->Level=2;
			subitem->setPixmap(0,QPixmap(KGlobal::iconLoader()->loadIcon("find.png",KIcon::Small)));
		}
	}
}


//-----------------------------------------------------------------------------
void GALILEI::QGGroupsHistory::mousePressEvent(QMouseEvent* e)
{
	QListView::mousePressEvent(e);
}


//-----------------------------------------------------------------------------
GALILEI::QGGroupsHistory::~QGGroupsHistory(void)
{
}
