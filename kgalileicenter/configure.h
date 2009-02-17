/*

	KGALILEICenter

	Configure.h

	Configure Dialog Box - Header.

	Copyright 2008 by the Université libre de Bruxelles.

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



//------------------------------------------------------------------------------
#ifndef ConfigureDlgH
#define ConfigureDlgH


//------------------------------------------------------------------------------
// include files for current project
#include <ui_configure.h>


//------------------------------------------------------------------------------
/**
 * The Configure provides a dialog box for all preferences of KGALILEICenter,
 * GGALILEIApp et the different plug-ins.
 * @short Preference Dialog
 * @author Pascal Francq
 */
class Configure : KDialog, Ui_Configure
{
	Q_OBJECT

public:

	/**
	 * Construct the dialog box.
	 * @param parent         Parent widget.
	 */
	Configure(QWidget* parent);

	/**
	 * Execute the dialog boxes. In practice, when the 'Apply' button is
	 * pressed, the variables are applied.
	 */
	void exec(GSession* session);

	/**
	 * Read the options of a configuration file.
	 */
	static void readOptions(void);

	/**
	 * Save the options of a configuration file.
	 */
	static void saveOptions(void);

private:

	/**
	 * Initialize the general part.
	 */
	void initGeneral(void);

	/**
	 * Apply the general part.
	 */
	void applyGeneral(void);

	/**
	 * Initialize the simulation part.
	 */
	void initSimulation(void);

	/**
	 * Apply the simulation part.
	 */
	void applySimulation(void);

	/**
	 * Initialize the plug-ins part.
	 */
	void initPlugIns(void);

	/**
	 * Apply the plug-ins part.
	 * @param session        Current session (if created).
	 */
	void applyPlugIns(GSession* session);

public slots:

	/**
	 * Slot called when the button to add a directory was pressed.
	 */
	void slotAddDir(void);

	/**
	 * Slot called when the button to delete a directory was pressed.
	 */
	void slotDelDir(void);
};



//------------------------------------------------------------------------------
#endif
