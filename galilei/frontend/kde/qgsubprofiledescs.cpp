/*

	GALILEI Research Project

	QGSubProfileDescs.cpp

	Widget to show all the subprofiles for specific description type - Implementation.

	Copyright 2001 by the Université Libre de Bruxelles.

	Authors:
		
		David Wartel (dwartel@ulb.ac.be).

	Version $Revision$

	Last Modify: $Date$

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
// include files for R Project
#include <rstd/rcursor.h>
using namespace R;


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <infos/glang.h>
#include <qgsubprofiledescs.h>
#include <qgsubprofilevector.h>
#include <profiles/gprofile.h>
#include <profiles/gsubprofile.h>
#include <sessions/gsession.h>
#include <profiles/gsubprofile.h>
using namespace GALILEI;



//-----------------------------------------------------------------------------
//
// class QGSubProfileDescs
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GALILEI::QGSubProfileDescs::QGSubProfileDescs(QTabWidget* parent,GSession* session,GProfile* profile,tSubProfileDesc t)
	: QWidget(parent,"QGSubProfileDescs"), Session(session), Profile(profile), SubProfilesDesc(profile->NbPtr,5), DescType(t)
{
	QSize act=size(),tmp, cbsize;
	GSubProfile* s;
	QGSubProfileDesc* w;

	// title of the window
//

	// Construct the combo box
	Lang=new QComboBox(this);
	tmp=Lang->size();
	cbsize=Lang->size();
	Lang->resize(act.width(),tmp.height());
	Lang->setEditable(false);
	connect(Lang,SIGNAL(activated(int)),this,SLOT(slotLangChanged(int)));

	// For each subprofile create a widget
	for(Profile->Start();!Profile->End();Profile->Next())
	{
		s=(*Profile)();
		switch(DescType)
		{
			case sdVector:
				w=new QGSubProfileVector(this,Session,(GSubProfileVector*)s);
				break;

			case sdNothing:
			default:
				w=0;
		}
		if(!w) return;
		SubProfilesDesc.InsertPtr(w);
		Lang->insertItem(s->GetLang()->GetName(),SubProfilesDesc.NbPtr-1);
		w->move(0,cbsize.height());
		w->resize(act.width(),act.height()-cbsize.height());
		if(SubProfilesDesc.NbPtr==1)
		{
			Current=w;
			Lang->setCurrentItem(SubProfilesDesc.NbPtr-1);
			w->show();
		}
		else
			w->hide();
	}
	switch(DescType)
	{
		case sdVector:
			parent->insertTab(this,"Vector Description");
			break;

		case sdNothing:
		default:
			break;
	}
}


//---------------------------------------------------------------------------
void GALILEI::QGSubProfileDescs::slotLangChanged(int index)
{
	Current->hide();
	Current=SubProfilesDesc.Tab[index];
	Current->show();
}


//---------------------------------------------------------------------------
void GALILEI::QGSubProfileDescs::slotProfileChanged(void)
{
	RCursor<QGSubProfileDesc,unsigned int> Cur(SubProfilesDesc);

	for(Cur.Start();!Cur.End();Cur.Next())
		Cur()->Construct();
}


//-----------------------------------------------------------------------------
void GALILEI::QGSubProfileDescs::resizeEvent(QResizeEvent *)
{
	RCursor<QGSubProfileDesc,unsigned int> Cur(SubProfilesDesc);
	QSize act=size(),tmp;
	tmp=Lang->size();

	// Resize Combo box Lang.
	Lang->resize(act.width(),tmp.height());

	// Resize SubProfiles.
	for(Cur.Start();!Cur.End();Cur.Next())
		Cur()->resize(act.width(),act.height()-tmp.height());
}


//-----------------------------------------------------------------------------
GALILEI::QGSubProfileDescs::~QGSubProfileDescs(void)
{
}
