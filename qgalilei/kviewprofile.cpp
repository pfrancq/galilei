/*

	GALILEI Research Project

	KViewProfile.cpp

	Window to manipulate a given profile - Implementation.

	Copyright 2001-2014 by Pascal Francq (pascal@francq.info).
	Copyright 2001-2004 by David Wartel.
	Copyright 2001-2008 by the Université Libre de Bruxelles (ULB).

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
// include files for R/GALILEI
#include <rqt.h>
#include <guser.h>
#include <gcommunity.h>


//-----------------------------------------------------------------------------
// include files for Qt/KDE


//-----------------------------------------------------------------------------
// include files for current application
#include <kviewprofile.h>
#include <qsessionprogress.h>
#include <qgalileiwin.h>



//-----------------------------------------------------------------------------
//
// class KViewProfile
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
KViewProfile::KViewProfile(QGALILEIWin* win,GProfile* profile)
	: QMdiSubWindow(), Ui_KViewProfile(), Win(win), Profile(profile)
{
	QWidget* ptr=new QWidget();
	setupUi(ptr);
	setWidget(ptr);
	setAttribute(Qt::WA_DeleteOnClose);
	setWindowTitle(ToQString(profile->GetName())+" ("+ToQString(profile->GetUser()->GetFullName())+")");
	Vars->Set(Profile);
	Desc->Set(Win->getSession(),&(*Profile)());
	Assessments->Set(QGObjectsList::Assessments,Profile);
	Links->Set(QGObjectsList::Links,Profile);
	GCommunity* Group(Win->getSession()->GetObj(static_cast<GCommunity*>(0),Profile->GetGroupId(),true,true));
	if(Group)
		Community->Set(QGObjectsList::Profiles,Group);
	connect(Win,SIGNAL(profilesChanged()),this,SLOT(updateProfile()));
	connect(Win,SIGNAL(communitiesChanged()),this,SLOT(updateCommunity()));
	connect(Community,SIGNAL(Show(GProfile*)),Win,SLOT(showProfile(GProfile*)));
}


//-----------------------------------------------------------------------------
void KViewProfile::updateProfile(void)
{
	Vars->Set(Profile);
	Desc->Set(Win->getSession(),&(*Profile)());
}


//-----------------------------------------------------------------------------
void KViewProfile::updateCommunity(void)
{
	GCommunity* Group(Win->getSession()->GetObj(static_cast<GCommunity*>(0),Profile->GetGroupId(),true,true));
	if(Group)
		Community->Set(QGObjectsList::Profiles,Group);
}


//-----------------------------------------------------------------------------
void KViewProfile::ComputeProfile(void)
{
	QComputeProfile(Win,Profile).run();
	updateProfile();
}


//-----------------------------------------------------------------------------
KViewProfile::~KViewProfile(void)
{
}
