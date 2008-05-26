/*

	GALILEI Research Project

	QGCommunitiesHistory.h

	Widget to show the groups history - Header.

	Copyright 2002-2008 by the Université Libre de Bruxelles.

	Authors:
		David Wartel <dwartel@ulb.ac.be>
		Pascal Francq <pfrancq@ulb.ac.be>

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
#ifndef QGCommunitiesHistoryH
#define QGCommunitiesHistoryH



//-----------------------------------------------------------------------------
// include files for GALILEI
#include <galilei.h>


//-----------------------------------------------------------------------------
// include files for Qt
#include <qlistview.h>
#include <qwidget.h>



//-----------------------------------------------------------------------------
namespace GALILEI{
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/**
* The QGCommunitiesHistory class provides a widget to show a given groupement of the GA.
* @author Pascal Francq
* @short GA Groups Widget.
*/
class QGCommunitiesHistory  :  public QListView
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
	* @param grps           Groups to display.
	* @param name           Name of the widget.
	*/
	QGCommunitiesHistory(QWidget* parent,GCommunitiesHistory* grps, const char* name=0);

	/**
	* The widget has changed and has to be repainted.
	*/
	void setChanged(void) {Changed=true;repaint();}

	/**
	* Set the groups.
	* @param grps           Groups to display.
	*/
	void setGroups(GCommunitiesHistory* grps);

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
	virtual ~QGCommunitiesHistory(void);
};


} //------- End of namespace GALILEI ------------------------------------------


//-----------------------------------------------------------------------------
#endif
