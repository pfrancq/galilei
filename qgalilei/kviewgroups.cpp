/*

	GALILEI Research Project

	KViewGroups.cpp

	Window to list topics or communities - Implementation.

	Copyright 2008-2015 by Pascal Francq (pascal@francq.info).

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
// includes files for Qt
#include <QResizeEvent>


//------------------------------------------------------------------------------
// include files for current application
#include <kviewgroups.h>
#include <qgalileiwin.h>
#include <kapplication.h>


//-----------------------------------------------------------------------------
// static sizes
static int CommunityWidth;
static int CommunityHeight;
static int TopicWidth;
static int TopicHeight;



//-----------------------------------------------------------------------------
//
// class KViewGroups
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
template<class cGroup>
	KViewGroups<cGroup>::KViewGroups(QGALILEIWin* win)
		: QMdiSubWindow(), Ui_KViewGroups(), Win(win)
{
	QWidget* ptr=new QWidget();
	setupUi(ptr);
	setWidget(ptr);
	setAttribute(Qt::WA_DeleteOnClose);
}



//-----------------------------------------------------------------------------
//
// class KViewTopics
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
KViewTopics::KViewTopics(QGALILEIWin* win)
	: KViewGroups<GTopic>(win)
{
	setWindowTitle("Topics");
	connect(Objects,SIGNAL(Show(GDoc*)),Win,SLOT(showDoc(GDoc*)));
	connect(Objects,SIGNAL(Show(GTopic*)),Win,SLOT(showTopic(GTopic*)));
	connect(Win,SIGNAL(topicsChanged()),this,SLOT(update()));
	connect(What,SIGNAL(returnPressed()),this,SLOT(FindNext()));
	connect(Search,SIGNAL(clicked()),this,SLOT(FindNext()));
	Objects->Set(Win->getSession(),QGObjectsList::Topics);
	resize(TopicWidth,TopicHeight);
}



//------------------------------------------------------------------------------
void KViewTopics::createOptions(RConfig& config)
{
	config.InsertParam(new RParamValue("KViewTopics::Width",300));
	config.InsertParam(new RParamValue("KViewTopics::Height",200));
}


//------------------------------------------------------------------------------
void KViewTopics::readOptions(RConfig& config)
{
	TopicWidth=config.GetInt("KViewTopics::Width");
	TopicHeight=config.GetInt("KViewTopics::Height");
}


//------------------------------------------------------------------------------
void KViewTopics::saveOptions(RConfig& config)
{
	config.SetInt("KViewTopics::Width",TopicWidth);
	config.SetInt("KViewTopics::Height",TopicHeight);
}


//------------------------------------------------------------------------------
void KViewTopics::resizeEvent(QResizeEvent* resizeEvent)
{
	QMdiSubWindow::resizeEvent(resizeEvent);
	TopicWidth=resizeEvent->size().width();
	TopicHeight=resizeEvent->size().height();
}


//-----------------------------------------------------------------------------
void KViewTopics::update(void)
{
	Objects->Set(Win->getSession(),QGObjectsList::Topics);
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
KViewCommunities::KViewCommunities(QGALILEIWin* win)
	: KViewGroups<GCommunity>(win)
{
	setWindowTitle("Communities");
	connect(Objects,SIGNAL(Show(GProfile*)),Win,SLOT(showProfile(GProfile*)));
	connect(Objects,SIGNAL(Show(GCommunity*)),Win,SLOT(showCommunity(GCommunity*)));
	connect(Win,SIGNAL(communitiesChanged()),this,SLOT(update()));
	connect(What,SIGNAL(returnPressed()),this,SLOT(FindNext()));
	connect(Search,SIGNAL(clicked()),this,SLOT(FindNext()));
	Objects->Set(Win->getSession(),QGObjectsList::Communities);
	resize(CommunityWidth,CommunityHeight);
}


//------------------------------------------------------------------------------
void KViewCommunities::createOptions(RConfig& config)
{
	config.InsertParam(new RParamValue("KViewCommunities::Width",300));
	config.InsertParam(new RParamValue("KViewCommunities::Height",200));
}


//------------------------------------------------------------------------------
void KViewCommunities::readOptions(RConfig& config)
{
	CommunityWidth=config.GetInt("KViewCommunities::Width");
	CommunityHeight=config.GetInt("KViewCommunities::Height");
}


//------------------------------------------------------------------------------
void KViewCommunities::saveOptions(RConfig& config)
{
	config.SetInt("KViewCommunities::Width",CommunityWidth);
	config.SetInt("KViewCommunities::Height",CommunityHeight);
}


//------------------------------------------------------------------------------
void KViewCommunities::resizeEvent(QResizeEvent* resizeEvent)
{
	QMdiSubWindow::resizeEvent(resizeEvent);
	CommunityWidth=resizeEvent->size().width();
	CommunityHeight=resizeEvent->size().height();
}


//-----------------------------------------------------------------------------
void KViewCommunities::update(void)
{
	Objects->Set(Win->getSession(),QGObjectsList::Communities);
}


//-----------------------------------------------------------------------------
void KViewCommunities::FindNext(void)
{
	Objects->FindNext(What->text(),Desc->isChecked());
}
