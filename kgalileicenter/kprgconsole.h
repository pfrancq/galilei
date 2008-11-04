/*

	GALILEI Research Project

	KPrgConsole.h

	Window to manipulate a program console - Header.

	Copyright 2007-2008 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).

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
#ifndef KViewDebugH
#define KViewDebugH


//-----------------------------------------------------------------------------
// include files for R Project
#include <rcontainer.h>
#include <rqconsole.h>
using namespace R;


//-----------------------------------------------------------------------------
// include files for GALILEI
#include <qlistviewitemtype.h>
#include <ggalileiprg.h>
using namespace GALILEI;


//-----------------------------------------------------------------------------
// include files for Qt
#include <qvariant.h>
#include <qdialog.h>


//-----------------------------------------------------------------------------
// forward declaration
class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QLabel;
class QLineEdit;
class QPushButton;


//-----------------------------------------------------------------------------
// include files for current application
#include "kview.h"


//-----------------------------------------------------------------------------
/**
* The KPrgConsole class provides a window holding a console for the GALILEI
* interpreter.
* @author Pascal Francq.
* @short Program Console.
*/
class KPrgConsole : public KView
{
	Q_OBJECT

	/**
	 * Console to control the output.
	 */
	RQConsole* Results;

    /**
     * GALILEI interpreter.
     */
    GGALILEIPrg* Prg;

public:

	/**
	* Constructor for the view
	* @param doc            Document instance that the view represents.
	* @param parent         Parent of the window.
	* @param name           Name of the window.
	* @param wflags         Flags.
	*/
	KPrgConsole(KDoc* doc,QWidget* parent,const char* name,int wflags);

protected slots:

	/**
	 * Receive a command from the console widget.
	 */
    void RunCmd(QString cmd);

public:

	/**
	 * Nothing to update.
	 */
	virtual void update(GALILEI::tObjType) {}

	/**
	 * Destruct the window.
	 */
	virtual ~KPrgConsole(void);
};


//-----------------------------------------------------------------------------
#endif
