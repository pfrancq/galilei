/*

	GALILEI Research Project

	KViewMetaEngine.h

	Search Window - Header.

	Copyright 2008-2015 by Pascal Francq (pascal@francq.info).
	Copyright 2001 by Valery Vandaele.
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
#ifndef KViewMetaEngineH
#define KViewMetaEngineH


//-----------------------------------------------------------------------------
// includes for Qt/KDE
#include <QtGui/QMdiSubWindow>


//-----------------------------------------------------------------------------
// include files for R/GALILEI Project
#include <gmetaengine.h>
using namespace R;
using namespace GALILEI;


//-----------------------------------------------------------------------------
// include files for current application
#include <ui_kviewmetaengine.h>
class QGALILEIWin;


//-----------------------------------------------------------------------------
/**
* The KViewMetaEngine class represents a window to search for documents.
* @author Pascal Francq.
* @short Search Window.
*/
class KViewMetaEngine : public QMdiSubWindow, public Ui_KViewMetaEngine
{
	Q_OBJECT

	/**
	 * Maion window.
	 */
	QGALILEIWin* Win;

public:

	/**
	* Constructor for the view
	* @param win            Main window.
	*/
	KViewMetaEngine(QGALILEIWin* win);

protected slots:

	/**
	* Process the query and display results.
	*/
	void QueryEngine(void);
};


//-----------------------------------------------------------------------------
#endif
