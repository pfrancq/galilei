/*

	GALILEI Research Project

	QGDescription.h

	Description Widget - Header.

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



//------------------------------------------------------------------------------
#ifndef QGDescriptionH
#define QGDescriptionH


//------------------------------------------------------------------------------
// include files for GALILEI
#include <gdescription.h>


//------------------------------------------------------------------------------
// include files for Qt/KDE
#include <QtGui/QWidget>
#include <QtGui/QTreeWidgetItem>


//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
 * The QGDescription class provides a Qt Widget to display the description.
 * It shows the name of the different elements.
 * @short Description Widget
 */
class QGDescription : public QWidget
{
    Q_OBJECT

	/**
	 * Pointer to the widget created by Qt.
	 */
	void* Ui;

	/**
	 * Description.
	 */
	const GDescription* Desc;

public:

	/**
	 * Construct a widget that can display the description.
	 * @param parent         Parent widget.
	 */
	QGDescription(QWidget* parent);

	/**
	 * Set the current description to display. The widget is redraw.
	 * @param session        Session.
	 * @param desc           Description to display.
	 */
	void Set(GSession* session,const GDescription* desc);

public slots:

   /**
    * Change the current vector.
    * @param item           Item selected.
    * @param column         Column clicked.
    */
   void ChangeVector(QTreeWidgetItem* item,int column);

public:

	/**
	 * Destruct the widget.
	 */
	virtual ~QGDescription(void);
};


} //------- End of namespace GALILEI ------------------------------------------


//------------------------------------------------------------------------------
#endif
