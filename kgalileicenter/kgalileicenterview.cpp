/*

  kgalileicenterview.cpp

  Description - Implementation.

  (c) 2002 by P. Francq.

  Version $Revision$

  Last Modify: $Date$

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

*/

//---------------------------------------------------------------------------

// include files for Qt
#include <qprinter.h>
#include <qpainter.h>
#include <qdir.h>
// include files for KDE

// application specific includes
#include "kgalileicenter.h"
#include "kgalileicenterview.h"
#include "kgalileicenterdoc.h"

KGalileiCenterView::KGalileiCenterView(KGalileiCenterDoc* pDoc, QWidget *parent, const char* name, int wflags)
 : QWidget(parent, name, wflags)
{
    doc=pDoc;
}

KGalileiCenterView::~KGalileiCenterView()
{
}

KGalileiCenterDoc *KGalileiCenterView::getDocument() const
{
	return doc;
}

void KGalileiCenterView::update(KGalileiCenterView* pSender){
	if(pSender != this)
		repaint();
}

void KGalileiCenterView::print(QPrinter *pPrinter)
{
  if (pPrinter->setup(this))
  {
		QPainter p;
		p.begin(pPrinter);
		
		///////////////////////////////
		// TODO: add your printing code here
		///////////////////////////////
		
		p.end();
  }
}

void KGalileiCenterView::closeEvent(QCloseEvent* e){

// DO NOT CALL QWidget::closeEvent(e) here !!
// This will accept the closing by QCloseEvent::accept() by default.
// The installed eventFilter() in KGalileiCenterApp takes care for closing the widget
// or ignoring the close event
		
}
