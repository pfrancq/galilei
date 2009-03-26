/*

	GALILEI Research Project

	QGDocStruct.h

	Widget to display the description of an object - Header.

	Copyright 2008-2009 by Pascal Francq (pascal@francq.info).

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



//------------------------------------------------------------------------------
#ifndef QGDocStructH
#define QGDocStructH


//------------------------------------------------------------------------------
// include files for GALILEI
#include <gdocstruct.h>


//------------------------------------------------------------------------------
// include files for Qt
#include <QtGui/QWidget>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
 * The QGDocStruct class provides a Qt Widget to display the description of a
 * GDocStruct object. It shows the name of the different element.
 * @author Pascal Francq
 * @short Description Displayer
 */
class QGDocStruct : public QWidget
{
	void* Ui;

public:

	/**
	 * Construct a widget that can display the description of an object.
	 * @param parent         Parent widget.
	 */
	QGDocStruct(QWidget* parent);

	/**
	 * Set the current object to display. The widget is redraw.
	 * @param obj            Object to display.
	 */
	void Set(GDoc* obj);

	/**
	 * Destruct the widget.
	 */
	virtual ~QGDocStruct(void);
};


} //------- End of namespace GALILEI ------------------------------------------


//------------------------------------------------------------------------------
#endif
