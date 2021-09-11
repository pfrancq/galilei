/*

	GALILEI Research Project

	KViewClass.cpp

	Window to manipulate a specific class  - Implementation.

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


//-----------------------------------------------------------------------------
// include files for R/GALILEI
#include <rqt.h>


//-----------------------------------------------------------------------------
// include files for current application
#include <kviewclass.h>
#include <qgalileiwin.h>


//-----------------------------------------------------------------------------
// static sizes
static int Width;
static int Height;



//-----------------------------------------------------------------------------
//
// class KViewClass
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
KViewClass::KViewClass(QGALILEIWin* win,GClass* obj)
		: QMdiSubWindow(), Ui_KViewClass(), Win(win), Obj(obj)
{
	QWidget* ptr=new QWidget();
	setupUi(ptr);
	setWidget(ptr);
	setAttribute(Qt::WA_DeleteOnClose);
	setWindowTitle(ToQString("Class '"+Obj->GetName()+"'"));
	ObjectsTab->setTabText(1,"Documents");
	connect(Win,SIGNAL(classesChanged()),this,SLOT(update()));
	update();
	resize(Width,Height);
}


//------------------------------------------------------------------------------
void KViewClass::createOptions(RConfig& config)
{
	config.InsertParam(new RParamValue("KViewClass::Width",200));
	config.InsertParam(new RParamValue("KViewClass::Height",200));
}


//------------------------------------------------------------------------------
void KViewClass::readOptions(RConfig& config)
{
	Width=config.GetInt("KViewClass::Width");
	Height=config.GetInt("KViewClass::Height");
}


//------------------------------------------------------------------------------
void KViewClass::saveOptions(RConfig& config)
{
	config.SetInt("KViewClass::Width",Width);
	config.SetInt("KViewClass::Height",Height);
}


//------------------------------------------------------------------------------
void KViewClass::resizeEvent(QResizeEvent* resizeEvent)
{
	QMdiSubWindow::resizeEvent(resizeEvent);
	Width=resizeEvent->size().width();
	Height=resizeEvent->size().height();
}


//-----------------------------------------------------------------------------
void KViewClass::update(void)
{
	Vars->Set(Obj);
	Desc->Set(Win->getSession(),&(*Obj)());
}
