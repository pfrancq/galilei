/*

	GALILEI Research Project

	QGGroupsHistory.h

	Widget to show the groups history - Header.

	Copyright 2002 by the Universit� Libre de Bruxelles.

	Authors:
		David Wartel <dwartel@ulb.ac.be>	

	Version $Revision$

	Last Modify: $Date$

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
#ifndef QGGroupsHistoryH
#define QGGroupsHistoryH



//-----------------------------------------------------------------------------
// include files for GALILEI
#include <sessions/galilei.h>


//-----------------------------------------------------------------------------
// include files for Qt
#include <qlistview.h>
#include <qwidget.h>



//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/**
* The QGGroupsHistory class provides a widget to show a given groupement of the GA.
* @author Pascal Francq
* @version $Revision$
* @short GA Groups Widget.
*/
class QGGroupsHistory  :  public QListView
{

	Q_OBJECT

	/**
	* A pixmap to hold the display.
	*/
	QPixmap* pixmap;

	/**
	* Has the display changed.
	*/
	bool Changed;

public:

	/**
	* Constructor of the Widget.
	* @param parent         Parent of the widget.
	* @param gps            groups to display.
	* @param name           Name of the widget.
	*/
	QGGroupsHistory(QWidget* parent,GGroupsHistory* grps, const char* name=0);

	/**
	* The widget has changed and has to be repainted.
	*/
	void setChanged(void) {Changed=true;repaint();}

	/**
	* Set the groups.
	*/
	void setGroups(GGroupsHistory* grps);

protected:

	/**
	* Mouse button press event method.
	* @param e             MouseEvent info.
	*/
	virtual void mousePressEvent(QMouseEvent* e);

public:

	/**
	* Destructor of the widget.
	*/
	virtual ~QGGroupsHistory(void);
};


} //------- End of namespace GALILEI ------------------------------------------


//-----------------------------------------------------------------------------
#endif
