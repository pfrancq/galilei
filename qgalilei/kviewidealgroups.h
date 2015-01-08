/*

	GALILEI Research Project

	KViewThTopics.h

	Window to manipulate theoretical topics - Header.

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
#ifndef KViewThGroupsH
#define KViewThGroupsH


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
//include files for current application
#include <ui_kviewidealgroups.h>
class QGALILEIWin;  // forward declaration


//-----------------------------------------------------------------------------
/**
* The KViewIdealGroups class represents a window to manipulate the ideal groups
* of the system.
*
* @author Pascal Francq.
* @short Ideal Topics Window.
*/
template<class cGroup>
	class KViewIdealGroups : public QMdiSubWindow, public Ui_KViewIdealGroups
{
protected:

	/**
	 * Main window.
	 */
	QGALILEIWin* Win;

	/**
	* Construct the window.
	* @param win            Main window.
	*/
	KViewIdealGroups(QGALILEIWin* win);
};


//-----------------------------------------------------------------------------
/**
* The KViewIdealTopics class represents a window to manipulate the ideal topics
* of the system.
* @author Pascal Francq.
* @short Ideal Topics Window.
*/
class KViewIdealTopics : public KViewIdealGroups<GTopic>
{
	Q_OBJECT

public:

	/**
	* Construct the window.
	* @param win            Main window.
	*/
	KViewIdealTopics(QGALILEIWin* win);

public slots:

	/**
	* Gets called to redraw the window.
	*/
	void update(void);

public:

	/**
	* Destruct the window.
	*/
	~KViewIdealTopics(void);
};


//-----------------------------------------------------------------------------
/**
* The KViewIdealCommunities class represents a window to manipulate the ideal
* communities of the system.
* @author Pascal Francq.
* @short Ideal Communities Window.
*/
class KViewIdealCommunities : public KViewIdealGroups<GCommunity>
{
	Q_OBJECT

public:

	/**
	* Construct the window.
	* @param win            Main window.
	*/
	KViewIdealCommunities(QGALILEIWin* win);

public slots:

	/**
	* Gets called to redraw the window.
	*/
	void update(void);

public:

	/**
	* Destruct the window.
	*/
	~KViewIdealCommunities(void);
};


//-----------------------------------------------------------------------------
/**
* The KViewIdealClasses class represents a window to manipulate the ideal
* classes of the system.
* @author Pascal Francq.
* @short Ideal Classes Window.
*/
class KViewIdealClasses : public QMdiSubWindow, public Ui_KViewIdealGroups
{
	Q_OBJECT

	/**
	 * Main window.
	 */
	QGALILEIWin* Win;

public:

	/**
	* Construct the window.
	* @param win            Main window.
	*/
	KViewIdealClasses(QGALILEIWin* win);

public slots:

	/**
	* Gets called to redraw the window.
	*/
	void update(void);
};


//-----------------------------------------------------------------------------
#endif
