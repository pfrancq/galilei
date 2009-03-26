/*

	KGALILEICenter

	Configure.h

	Configure Dialog Box - Header.

	Copyright 2008-2009 by Pascal Francq (pascal@francq.info).

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


//-----------------------------------------------------------------------------
// include files for R/GALILEI Project
#include <galilei.h>
using namespace GALILEI;


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

	/**
	 * Session (if any).
	 */
	GSession* Session;

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
	 * Add a subject to the list of subjects.
	 * @param subject        Subject.
	 * @param parent         Parent intem (if null, the subject is a top one).
	 */
	void addSubject(GSubject* subject,QTreeWidgetItem* parent);

	/**
	 * Initialize the simulation part.
	 */
	void initSimulation(void);

	/**
	 * Apply the configuration for all sub-subjects of a given one.
	 * @param parent         Parent subject.
	 */
	void applySubSubjects(QTreeWidgetItem* parent);

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
	 */
	void applyPlugIns(void);

public slots:

	/**
	 * Slot called when the button to add a directory was pressed.
	 */
	void slotAddDir(void);

	/**
	 * Slot called when the button to delete a directory was pressed.
	 */
	void slotDelDir(void);

	/**
	 * Slot called when a subject is clicked.
	 * @param item           Item.
	 * @param column         Column.
	 */
	void subjectClicked(QTreeWidgetItem* item, int column);
};



//------------------------------------------------------------------------------
#endif
