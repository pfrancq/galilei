/*

	GALILEI Research Project

	QGProfile.cpp

	Widget to show a profile - Implementation.

	Copyright 2008 by the Université Libre de Bruxelles.

	Authors:
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
// include files for ANSI C/C++
#include <string.h>


//-----------------------------------------------------------------------------
// include files for Qt
#include <qlistview.h>


//-----------------------------------------------------------------------------
// include files for R Project



//-----------------------------------------------------------------------------
// include files for GALILEI
#include <qgprofile.h>
#include <gprofile.h>
#include <rqt.h>
using namespace GALILEI;
using namespace R;



//-----------------------------------------------------------------------------
//
// class QGProfile
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GALILEI::QGProfile::QGProfile(QWidget* parent,GProfile* profile)
	: QWidget(parent,"QGProfile"), Profile(profile)
{
	General = new QListView(this);
	General->addColumn("Variable");
	General->addColumn("Value");
	slotProfileChanged();
}


//---------------------------------------------------------------------------
void GALILEI::QGProfile::slotProfileChanged(void)
{

	General->clear();
	new QListViewItem(General,"ID",QString::number(Profile->GetId()));
	new QListViewItem(General,"Name",ToQString(Profile->GetName()));
	if(Profile->IsSocial())
		new QListViewItem(General,"Social","Yes");
	else
		new QListViewItem(General,"Social","No");
}


//-----------------------------------------------------------------------------
void GALILEI::QGProfile::resizeEvent(QResizeEvent *)
{
	General->resize(size());
}


//-----------------------------------------------------------------------------
GALILEI::QGProfile::~QGProfile(void)
{
}
