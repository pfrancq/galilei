/*

	GALILEI Project.

	QGALILEI.h

	Application - Header.

	Copyright 2001-2015 by Pascal Francq (pascal@francq.info).

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
#ifndef QGALILEI_H
#define QGALILEI_H


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <ggalileiapp.h>
#include <gsession.h>
#include <gslot.h>


//-----------------------------------------------------------------------------
// include files for Qt
#include <QApplication>


//-----------------------------------------------------------------------------
// forward declatation
class QGALILEIWin;


//-----------------------------------------------------------------------------
/**
 * The QGALILEI class provides the main application.
 */
class QGALILEI : /*public QApplication,*/ public GALILEI::GGALILEIApp
{
	/**
	 * Main Window;
    */
	QGALILEIWin* Main;

	/**
	 * Path for scripts.
	 */
	R::RString ScriptPath;

	/**
	 * The last session connected.
	 */
	R::RString LastSession;

	/**
	 * Last tool category used.
	 */
	R::RString LastToolCat;

	/**
	 * Last tool used.
	 */
	R::RString LastTool;

public:

	/**
	 * Constructor.
    * @param argc           Number of argument.
    * @param argv           Arguments.
    */
	QGALILEI(int argc, char *argv[]);

	/**
	 * Create the configuration parameters.
    */
	virtual void CreateConfig(void);

	/**
	 * Initialize the application.
    */
	virtual void Init(void);

	/**
	  * "Run" the application. In practice, it just show the main widget and
	  * call QApplication::instance()->exec().
     */
	virtual void Run(void);

	/**
	 * Create a valid storage. The method is call when a session is opened for
	 * which a current storage doesn't exist. It proposes to choose one storage,
	 * ahnd call the configuration dialog box for it.
	 * @param session        Session for which the stoarge must be created.
    * @return a valid pointer to a GStorage.
    */
	virtual GALILEI::GStorage* CreateStorage(GALILEI::GSession* session);

	/**
	  * Get a path where the scripts must be initially searched.
     * @return R::RString.
     */
	R::RString GetScriptPath(void) const {return(ScriptPath);}

	/**
	 * Set the path to search for scripts.
    * @param path           Path.
    */
	void SetScriptPath(const QString& path);

	/**
	  * Get the name of the last session to which the user was connected.
     * @return R::RString.
     */
	R::RString GetLastSession(void) const {return(LastSession);}

	/**
	 * Set the last session to which the user was connected.
    * @param last           Last session.
    */
	void SetLastSession(const R::RString& last);

	/**
	 * Get the name of the last tool category chosen.
    * @return a R::RString.
    */
	R::RString GetLastToolCat(void) const {return(LastToolCat);}

	/**
	 * Set the last tool category chosen.
    * @param cat            Category.
    */
	void SetLastToolCat(const R::RString cat);

	/**
	 * Get the name of the last tool chosen.
    * @return a R::RString.
    */
	R::RString GetLastTool(void) const {return(LastTool);}

	/**
	 * Get the last tool chosen.
    * @param tool           Tool.
    */
	void SetLastTool(const R::RString tool);

	 /**
	  * Destructor.
     */
	virtual ~QGALILEI(void);
};


//-----------------------------------------------------------------------------
#endif
