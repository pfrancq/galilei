/*

	GALILEI Research Project

	KViewGroups.cpp

	Window to list topics or communities - Implementation.

	Copyright 2008 by the Université libre de Bruxelles.

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



//------------------------------------------------------------------------------
// include files for current application
#include <kviewgroups.h>
#include <kgalileicenter.h>



//-----------------------------------------------------------------------------
//
// class KViewGroups
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
template<class cGroup>
	KViewGroups<cGroup>::KViewGroups(void)
		: QMdiSubWindow(), Ui_KViewGroups()
{
	QWidget* ptr=new QWidget();
	setupUi(ptr);
	setWidget(ptr);
	ptr->setAttribute(Qt::WA_DeleteOnClose);
}



//-----------------------------------------------------------------------------
//
// class KViewTopics
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
KViewTopics::KViewTopics(void)
	: KViewGroups<GTopic>()
{
	setWindowTitle("Topics");
	connect(Objects,SIGNAL(Show(GDoc*)),dynamic_cast<KGALILEICenter*>(GALILEIApp),SLOT(showDoc(GDoc*)));
	connect(dynamic_cast<KGALILEICenter*>(GALILEIApp),SIGNAL(topicsChanged()),this,SLOT(update()));
	connect(What,SIGNAL(returnPressed()),this,SLOT(FindNext()));
	connect(Search,SIGNAL(clicked()),this,SLOT(FindNext()));
	Objects->Set(QGObjectsList::Topics);
}


//-----------------------------------------------------------------------------
void KViewTopics::update(void)
{
	Objects->Set(QGObjectsList::Topics);
}


//-----------------------------------------------------------------------------
void KViewTopics::FindNext(void)
{
	Objects->FindNext(What->text(),Desc->isChecked());
}



//-----------------------------------------------------------------------------
//
// class KViewCommunities
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
KViewCommunities::KViewCommunities(void)
	: KViewGroups<GCommunity>()
{
	setWindowTitle("Communities");
	connect(Objects,SIGNAL(Show(GProfile*)),dynamic_cast<KGALILEICenter*>(GALILEIApp),SLOT(showProfile(GProfile*)));
	connect(dynamic_cast<KGALILEICenter*>(GALILEIApp),SIGNAL(communitiesChanged()),this,SLOT(update()));
	connect(What,SIGNAL(returnPressed()),this,SLOT(FindNext()));
	connect(Search,SIGNAL(clicked()),this,SLOT(FindNext()));
	Objects->Set(QGObjectsList::Communities);
}


//-----------------------------------------------------------------------------
void KViewCommunities::update(void)
{
	Objects->Set(QGObjectsList::Communities);
}


//-----------------------------------------------------------------------------
void KViewCommunities::FindNext(void)
{
	Objects->FindNext(What->text(),Desc->isChecked());
}
