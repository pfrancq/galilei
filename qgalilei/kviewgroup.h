/*

	GALILEI Research Project

	KViewGroup.h

	Window to manipulate a specific group (topic or community) - Header.

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
#ifndef KViewGroupH
#define KViewGroupH


//-----------------------------------------------------------------------------
// include files for R/GALILEI Projects
#include <gtopic.h>
#include <gcommunity.h>
using namespace GALILEI;
using namespace R;


//-----------------------------------------------------------------------------
// include files for KDE/Qt
#include <QtGui/QMdiSubWindow>


//-----------------------------------------------------------------------------
// include files for current application
#include <ui_kviewgroup.h>
class QGALILEIWin;  // forward declaration


//-----------------------------------------------------------------------------
/**
 * The KViewGroup provides a template widget to represent a community or a
 * topic.
 * @short View Group.
 * @author Pascal Francq.
 */
template<class cGroup>
	class KViewGroup : public QMdiSubWindow, public Ui_KViewGroup
{
protected:

	/**
	 * Main window.
	 */
	QGALILEIWin* Win;

	/**
	 * Object represented.
	 */
	cGroup* Obj;

	/**
	* Construct the widget.
	* @param win             Main window.
	* @param obj             Object to represent.
	*/
	KViewGroup(QGALILEIWin* win,cGroup* obj);

	/**
	 * Update the widget.
	 * @param type           Type of the objects list.
	 */
	void update(QGObjectsList::oType type);
};


//-----------------------------------------------------------------------------
/**
* The KViewTopic class represents a window to manipulate a specific topic.
* @author Pascal Francq.
* @short Topic Window.
*/
class KViewTopic : public KViewGroup<GTopic>
{
	Q_OBJECT

public:

	/**
	* Construct the view.
	* @param win             Main window.
	* @param topic           Corresponding topic.
	*/
	KViewTopic(QGALILEIWin* win,GTopic* topic);

public slots:

	/**
	 * Update the widget.
	 */
	void update(void);
};


//-----------------------------------------------------------------------------
/**
* The KViewCommunity class represents a window to manipulate a specific
* community.
* @author Pascal Francq.
* @short Community Window.
*/
class KViewCommunity : public KViewGroup<GCommunity>
{
	Q_OBJECT

public:

	/**
	* Construct the view.
	* @param win             Main window.
	* @param community       Corresponding community.
	*/
	KViewCommunity(QGALILEIWin* win,GCommunity* community);

public slots:

	/**
	 * Update the widget.
	 */
	void update(void);
};



//-----------------------------------------------------------------------------
#endif
