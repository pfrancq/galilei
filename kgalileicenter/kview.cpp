/*

	GALILEI Research Project

	KView.cpp

	Basic Window for the application - Implementation.

	(C) 2001 by Pascal Francq

	Version $Revision$

	Last Modify: $Date$

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
void KView::closeEvent(QCloseEvent* e)
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
