/*

	GALILEI Research Project

	KPrgConsole.cpp

	Window to manipulate a program console - Implementation.

	Copyright 2007-2015 by Pascal Francq (pascal@francq.info).
	Copyright 2007-2008 by the Université Libre de Bruxelles (ULB).

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
using namespace R;using namespace GALILEI;


//-----------------------------------------------------------------------------
// includes files for Qt
#include <qvariant.h>
#include <qpushbutton.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qevent.h>
#include <QResizeEvent>


//-----------------------------------------------------------------------------
// include files for current application
#include <kprgconsole.h>
#include <qsessionprogress.h>


//-----------------------------------------------------------------------------
// static sizes
static int Width;
static int Height;



//-----------------------------------------------------------------------------
//
// class KPrgConsole
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
KPrgConsole::KPrgConsole(void)
	: QMdiSubWindow(), Ui_KPrgConsole(), Prg(0)
{
	QWidget* ptr=new QWidget();
	setupUi(ptr);
	setWidget(ptr);
	setAttribute(Qt::WA_DeleteOnClose);
	setWindowTitle("Console");
	Prg=new GGALILEIPrg(Results);
	connect(Results,SIGNAL(EnterCmd(QString)),this,SLOT(RunCmd(QString)));
	resize(Width,Height);
}


//------------------------------------------------------------------------------
void KPrgConsole::createOptions(RConfig& config)
{
	config.InsertParam(new RParamValue("KPrgConsole::Width",200));
	config.InsertParam(new RParamValue("KPrgConsole::Height",200));
}


//------------------------------------------------------------------------------
void KPrgConsole::readOptions(RConfig& config)
{
	Width=config.GetInt("KPrgConsole::Width");
	Height=config.GetInt("KPrgConsole::Height");
}


//------------------------------------------------------------------------------
void KPrgConsole::saveOptions(RConfig& config)
{
	config.SetInt("KPrgConsole::Width",Width);
	config.SetInt("KPrgConsole::Height",Height);
}


//------------------------------------------------------------------------------
void KPrgConsole::resizeEvent(QResizeEvent* resizeEvent)
{
	QMdiSubWindow::resizeEvent(resizeEvent);
	Width=resizeEvent->size().width();
	Height=resizeEvent->size().height();
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
