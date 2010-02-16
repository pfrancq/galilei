/*

	GALILEI Research Project

	KViewTopic.cpp

	Window to manipulate a specific topic - Implementation.

	Copyright 2008-2010 by Pascal Francq (pascal@francq.info).

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
// includes files for Qt/KDE
#include <kfiledialog.h>
#include <kmessagebox.h>


//-----------------------------------------------------------------------------
// include files for R/GALILEI
#include <rqt.h>


//-----------------------------------------------------------------------------
// include files for current application
#include <kviewgroup.h>
#include <kgalileicenter.h>



//-----------------------------------------------------------------------------
//
// class KViewGroup
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
template<class cGroup>
	KViewGroup<cGroup>::KViewGroup(cGroup* obj)
		: QMdiSubWindow(), Ui_KViewGroup(), Obj(obj)
{
	QWidget* ptr=new QWidget();
	setupUi(ptr);
	setWidget(ptr);
	setAttribute(Qt::WA_DeleteOnClose);
}


//-----------------------------------------------------------------------------
template<class cGroup>
	void KViewGroup<cGroup>::update(QGObjectsList::oType type)
{
	Vars->Set(Obj);
	Desc->Set(Obj->GetVector());
	Objects->Set(type,Obj);
}



//-----------------------------------------------------------------------------
//
// class KViewTopic
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
KViewTopic::KViewTopic(GTopic* topic)
	: KViewGroup<GTopic>(topic)
{
	setWindowTitle(ToQString("Topic '"+Obj->GetName()+"'"));
	ObjectsTab->setTabText(1,"Documents");
	connect(Objects,SIGNAL(Show(GDoc*)),dynamic_cast<KGALILEICenter*>(GALILEIApp),SLOT(showDoc(GDoc*)));
	connect(dynamic_cast<KGALILEICenter*>(GALILEIApp),SIGNAL(topicsChanged()),this,SLOT(update()));
	update();
}


//-----------------------------------------------------------------------------
void KViewTopic::update(void)
{
	KViewGroup<GTopic>::update(QGObjectsList::Docs);
}



//-----------------------------------------------------------------------------
//
// class KViewCommunity
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
KViewCommunity::KViewCommunity(GCommunity* community)
	: KViewGroup<GCommunity>(community)
{
	setWindowTitle(ToQString("Community '"+Obj->GetName()+"'"));
	ObjectsTab->setTabText(1,"Profiles");
	connect(Objects,SIGNAL(Show(GProfile*)),dynamic_cast<KGALILEICenter*>(GALILEIApp),SLOT(showProfile(GProfile*)));
	connect(dynamic_cast<KGALILEICenter*>(GALILEIApp),SIGNAL(communitiesChanged()),this,SLOT(update()));
	update();
}


//-----------------------------------------------------------------------------
void KViewCommunity::update(void)
{
	KViewGroup<GCommunity>::update(QGObjectsList::Profiles);
}
