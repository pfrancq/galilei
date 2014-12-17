/*

	GALILEI Project.

	QGALILEI.h

	Application - Header.

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
	  * Destructor.
     */
	virtual ~QGALILEI(void);
};


//-----------------------------------------------------------------------------
#endif
