/*

	GALILEI Research Project

	KViewUsers.h

	Window for manipulating the users - Header.

	Copyright 2001-2015 by Pascal Francq (pascal@francq.info).
	Copyright 2001-2008 by the Universit√© Libre de Bruxelles (ULB).

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
#ifndef KViewUsersH
#define KViewUsersH

//-----------------------------------------------------------------------------
// include files for R/GALILEI
#include <qgobjectslist.h>
#include <rconfig.h>
using namespace R;
using namespace GALILEI;


//-----------------------------------------------------------------------------
// include files for KDE/Qt
#include <QtGui/QMdiSubWindow>


//-----------------------------------------------------------------------------
//include files for current application
#include <ui_kviewusers.h>
class QGALILEIWin;  // forward declaration


//-----------------------------------------------------------------------------
/**
* The KViewUsers class provides a window to manipulate the users of the system.
* @author Pascal Francq.
* @short Users Window.
*/
class KViewUsers : public QMdiSubWindow, public Ui_KViewUsers
{
	Q_OBJECT

	/**
	 * Main window.
	 */
	QGALILEIWin* Win;

public:

	/**
	* Construct the widget.
	* @param win            Main window.
	*/
	KViewUsers(QGALILEIWin* win);

	/**
	 * Create the options in a configuration structure.
	 * @param config         Configuration structure.
    */
	static void createOptions(R::RConfig& config);

	/**
	 * Read the options of a configuration structure.
	 * @param config         Configuration structure.
	 */
	static void readOptions(R::RConfig& config);

	/**
	 * Save the options of a configuration structure.
	* @param config         Configuration structure.
	 */
	static void saveOptions(R::RConfig& config);

	/**
	 * Resize the window.
    * @param resizeEvent    Event.
    */
	virtual void resizeEvent(QResizeEvent* resizeEvent);

public slots:

	/**
	 * Update the list.
	 */
	void update(void);

protected slots:

	/**
	* Slot that adds a new user.
	*/
	void slotAddUser(void);

	/**
	* Slot that modifies a user.
	*/
	void slotModifyUser(void);

	/**
	 * Slot that add a profile.
	 */
	void slotAddProfile(void);

	/**
	 * Slot that modify a modify.
	 */
	void slotModifyProfile(void);

public:

	/**
	* Destruct the widget.
	*/
	~KViewUsers(void);
};


//-----------------------------------------------------------------------------
#endif
