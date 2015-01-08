/*

	GALILEI Research Project

	KPrgConsole.h

	Window to manipulate a program console - Header.

	Copyright 2007-2015 by Pascal Francq (pascal@francq.info).
	Copyright 2007-2008 by the Université Libre de Bruxelles (ULB).

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
#ifndef KPrgConsoleH
#define KPrgConsoleH


//-----------------------------------------------------------------------------
// include files for R/GALILEI
#include <ggalileiprg.h>


//-----------------------------------------------------------------------------
// include files for KDE/Qt
#include <QtGui/QMdiSubWindow>


//---------------------------------------------------------------------------
// include files for current application
#include <ui_kprgconsole.h>


//-----------------------------------------------------------------------------
/**
* The KPrgConsole class provides a window holding a console for the GALILEI
* interpreter.
* @author Pascal Francq.
* @short Program Console.
*/
class KPrgConsole : public QMdiSubWindow, public Ui_KPrgConsole
{
	Q_OBJECT

    /**
     * GALILEI interpreter.
     */
    GALILEI::GGALILEIPrg* Prg;

public:

	/**
	* Construct the window.
	*/
	KPrgConsole(void);

protected slots:

	/**
	 * Receive a command from the console widget.
	 */
    void RunCmd(QString cmd);

public:

	/**
	 * Destruct the window.
	 */
	virtual ~KPrgConsole(void);
};


//-----------------------------------------------------------------------------
#endif
