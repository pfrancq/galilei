/*

	GALILEI Research Project

	KViewProfile.cpp

	Window to manipulate a given profile - Implementation.

	Copyright 2001-2012 by Pascal Francq (pascal@francq.info).
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
#include <kapplication.h>


//-----------------------------------------------------------------------------
// include files for current application
#include <kviewprofile.h>
#include <qsessionprogress.h>



//-----------------------------------------------------------------------------
//
// class KViewProfile
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
KViewProfile::KViewProfile(KGALILEICenter* app, GProfile* profile)
	: QMdiSubWindow(), Ui_KViewProfile(), Profile(profile), App(app)
{
	QWidget* ptr=new QWidget();
	setupUi(ptr);
	setWidget(ptr);
	setAttribute(Qt::WA_DeleteOnClose);
	setWindowTitle(ToQString(profile->GetName())+" ("+ToQString(profile->GetUser()->GetFullName())+")");
	Vars->Set(Profile);
	Desc->Set(App->getSession(),Profile);
	Assessments->Set(QGObjectsList::Assessments,Profile);
	Links->Set(QGObjectsList::Links,Profile);
	GCommunity* Group(App->getSession()->GetObj(static_cast<GCommunity*>(0),Profile->GetGroupId(),true,true));
	if(Group)
		Community->Set(QGObjectsList::Profiles,Group);
	connect(dynamic_cast<KGALILEICenter*>(GALILEIApp),SIGNAL(profilesChanged()),this,SLOT(updateProfile()));
	connect(dynamic_cast<KGALILEICenter*>(GALILEIApp),SIGNAL(communitiesChanged()),this,SLOT(updateCommunity()));
	connect(Community,SIGNAL(Show(GProfile*)),dynamic_cast<KGALILEICenter*>(GALILEIApp),SLOT(showProfile(GProfile*)));
}


//-----------------------------------------------------------------------------
void KViewProfile::updateProfile(void)
{
	Vars->Set(Profile);
	Desc->Set(App->getSession(),Profile);
}


//-----------------------------------------------------------------------------
void KViewProfile::updateCommunity(void)
{
	GCommunity* Group(App->getSession()->GetObj(static_cast<GCommunity*>(0),Profile->GetGroupId(),true,true));
	if(Group)
		Community->Set(QGObjectsList::Profiles,Group);
}


//-----------------------------------------------------------------------------
void KViewProfile::ComputeProfile(void)
{
	QSessionProgressDlg Dlg(App,"Compute Profile");
	QComputeProfile* Task(new QComputeProfile(App,Profile));
	connect(Task,SIGNAL(finish()),this,SLOT(updateProfile()));
	Dlg.Run(Task);
}


//-----------------------------------------------------------------------------
KViewProfile::~KViewProfile(void)
{
}
