/*

	GALILEI Project.

	QGALILEIWin.h

	Main windows - Header.

	Copyright 2001-2014 by Pascal Francq (pascal@francq.info).

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
#ifndef QGALILEIWin_H
#define QGALILEIWin_H


//-----------------------------------------------------------------------------
// include files for R/GALILEI projects
#include <galilei.h>


//-----------------------------------------------------------------------------
// include files for Qt
#include <QMainWindow>


//-----------------------------------------------------------------------------
// include files for current project
#include <ui_qgalileiwin.h>


//-----------------------------------------------------------------------------
// forward declaration
class QGALILEI;


//-----------------------------------------------------------------------------
/**
 * The QGALILEIWin provides the main window.
 */
class QGALILEIWin : public QMainWindow, public Ui_QGALILEIWin
{
	Q_OBJECT

	/**
	 * GALILEI Application.
	 */
	QGALILEI* App;

	/**
	* GALILEI session.
	*/
	GALILEI::GSession* Session;

	/**
	 * Name of the current session.
	 */
	R::RString SessionName;

public:

	/**
	 * Constructor.
    * @param app            Application.
    */
	QGALILEIWin(QGALILEI* app);

	/**
	* Returns a pointer to the current session.
	*/
	GALILEI::GSession* getSession(void) const {return(Session);}

private:

	/**
	 * Connect the menu entries with the slots.
    */
	void connectMenus(void);

	/**
	 * Modify the enabled status of the menu entries dependinf if a session is
	 * connected or not.
    */
	void sessionConnected(void);

private slots :

	/**
	* Connect to a session.
	*/
	void sessionConnect(void);

	/**
	 * Disconnect from a session.
	 */
	void sessionDisconnect(void);

	/**
	 * Show the dictionaries.
	 */
	void showDicts(void);

	/**
	 * Set the preferences.
	 */
	void configQGALILEI(void);

	/**
	 * Set the options for the session.
	 */
	void configSession(void);

public:

	/**
	* Destruct the main window.
	*/
	~QGALILEIWin(void);
};


//-----------------------------------------------------------------------------
#endif

