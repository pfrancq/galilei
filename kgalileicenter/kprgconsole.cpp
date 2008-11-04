/*

	GALILEI Research Project

	KViewDebug.cpp

	Window to manipulate a program console - Implementation.

	Copyright 2007-2008 by the Universite Libre de Bruxelles.

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



//-----------------------------------------------------------------------------
// include files for R Project
#include <rstring.h>
using namespace R;


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <gsession.h>
#include <rqt.h>
using namespace GALILEI;


//-----------------------------------------------------------------------------
// includes files for Qt/KDE
#include <qvariant.h>
#include <qpushbutton.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qevent.h>


//-----------------------------------------------------------------------------
// include files for current application
#include "kdoc.h"
#include "kprgconsole.h"
#include "qsessionprogress.h"



//-----------------------------------------------------------------------------
//
// class KPrgConsole
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
KPrgConsole::KPrgConsole(KDoc* doc,QWidget* parent,const char* name,int wflags)
	: KView(doc,parent,name,wflags), Prg(0)
{
	setCaption("Console");
	QVBoxLayout* Layout = new QVBoxLayout( this, 0, 0, "Form1Layout");
    Results=new RQConsole(this,"Results");
    Prg=new GGALILEIPrg(Results);
    Layout->addWidget( Results );
    connect(Results,SIGNAL(EnterCmd(QString)),this,SLOT(RunCmd(QString)));
}


//-----------------------------------------------------------------------------
void KPrgConsole::RunCmd(QString cmd)
{
	Prg->Exec(FromQString(cmd));
}


//-----------------------------------------------------------------------------
KPrgConsole::~KPrgConsole(void)
{
	delete Prg;
}
