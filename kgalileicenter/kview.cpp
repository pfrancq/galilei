/*

	GALILEI Research Project

	KView.cpp

	Basic Window for the application - Implementation.

	Copyright 2001 by the Université Libre de Bruxelles.

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
// include files for Qt
#include <qprinter.h>
#include <qpainter.h>
#include <qdir.h>


//-----------------------------------------------------------------------------
// application specific includes
#include "kview.h"
#include "kdoc.h"



//-----------------------------------------------------------------------------
//
// class KView
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
KView::KView(KDoc* doc, QWidget* parent,const char* name,int wflags)
	: QWidget(parent,name,wflags), Doc(doc)
{
}


//-----------------------------------------------------------------------------
KDoc *KView::getDocument(void) const
{
	return(Doc);
}


//-----------------------------------------------------------------------------
void KView::print(QPrinter* pPrinter)
{
	if (pPrinter->setup(this))
	{
		QPainter p;
		p.begin(pPrinter);

		// TODO: add your printing code here

		p.end();
	}
}


//-----------------------------------------------------------------------------
bool KView::canClose(void)
{
	return(true);
}


//-----------------------------------------------------------------------------
void KView::closeEvent(QCloseEvent* /*e*/)
{
	// DO NOT CALL QWidget::closeEvent(e) here !!
	// This will accept the closing by QCloseEvent::accept() by default.
	// The installed eventFilter() in KGALILEICenterApp takes care for closing the widget
	// or ignoring the close event
}


//-----------------------------------------------------------------------------
KView::~KView(void)
{
}
