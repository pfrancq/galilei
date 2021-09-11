/*

	GALILEI Research Project

	KViewDocs.cpp

	Window for manipulating the documents - Implementation.

	Copyright 2001-2015 by Pascal Francq (pascal@francq.info).
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
// include files for R
#include <rqt.h>
#include <rcursor.h>


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <gsession.h>
#include <ggalileiapp.h>

using namespace GALILEI;
using namespace R;
using namespace std;


//-----------------------------------------------------------------------------
// include files for Qt
#include <QtGui/QInputDialog>
#include <QResizeEvent>


//-----------------------------------------------------------------------------
// application specific includes
#include <kviewdocs.h>
#include <qgalileiwin.h>


//-----------------------------------------------------------------------------
// static sizes
static int Width;
static int Height;



//-----------------------------------------------------------------------------
//
// class KViewDocs
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
KViewDocs::KViewDocs(QGALILEIWin* win)
	: QMdiSubWindow(), Ui_KViewDocs(), Win(win)
{
	QWidget* ptr=new QWidget();
	setupUi(ptr);
	setWidget(ptr);
	setAttribute(Qt::WA_DeleteOnClose);
	setWindowTitle("Documents");
	List->Set(Win->getSession(),QGObjectsList::Docs);
	resize(Width,Height);
	connect(List,SIGNAL(Show(GDoc*)),win,SLOT(showDoc(GDoc*)));
}


//------------------------------------------------------------------------------
void KViewDocs::createOptions(RConfig& config)
{
	config.InsertParam(new RParamValue("KViewDocs::Width",300));
	config.InsertParam(new RParamValue("KViewDocs::Height",200));
}


//------------------------------------------------------------------------------
void KViewDocs::readOptions(RConfig& config)
{
	Width=config.GetInt("KViewDocs::Width");
	Height=config.GetInt("KViewDocs::Height");
}


//------------------------------------------------------------------------------
void KViewDocs::saveOptions(RConfig& config)
{
	config.SetInt("KViewDocs::Width",Width);
	config.SetInt("KViewDocs::Height",Height);
}


//------------------------------------------------------------------------------
void KViewDocs::resizeEvent(QResizeEvent* resizeEvent)
{
	QMdiSubWindow::resizeEvent(resizeEvent);
	Width=resizeEvent->size().width();
	Height=resizeEvent->size().height();
}


//-----------------------------------------------------------------------------
void KViewDocs::update(void)
{
	List->Set(Win->getSession(),QGObjectsList::Docs);
}


//-----------------------------------------------------------------------------
KViewDocs::~KViewDocs(void)
{
}
