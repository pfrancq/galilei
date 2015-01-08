/*

	GALILEI Research Project

	KViewClass.h

	Window to manipulate a specific class - Header.

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
#ifndef KViewClassH
#define KViewClassH


//-----------------------------------------------------------------------------
// include files for R/GALILEI Projects
#include <gclass.h>
using namespace GALILEI;
using namespace R;


//-----------------------------------------------------------------------------
// include files for KDE/Qt
#include <QtGui/QMdiSubWindow>


//-----------------------------------------------------------------------------
// include files for current application
#include <ui_kviewclass.h>
class QGALILEIWin;  // forward declaration


//-----------------------------------------------------------------------------
/**
 * The KViewClass provides a template widget to represent a class.
 * @short class Group.
 * @author Pascal Francq.
 */
class KViewClass : public QMdiSubWindow, public Ui_KViewClass
{
	Q_OBJECT

protected:

	/**
	 * Main window.
	 */
	QGALILEIWin* Win;

	/**
	 * Class represented.
	 */
	GClass* Obj;

public:

	/**
	* Construct the widget.
	* @param win             Main window.
	* @param obj             Class to represent.
	*/
	KViewClass(QGALILEIWin* win,GClass* obj);

public slots:

	/**
	 * Update the widget.
	 */
	void update(void);
};



//-----------------------------------------------------------------------------
#endif
