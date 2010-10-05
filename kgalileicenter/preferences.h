/*

	KGALILEICenter

	Preferences.h

	Preferences - Header.

	Copyright 2010 by Paul Otlet Institute.

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
#ifndef PreferencesDlgh
#define PreferencesDlgh



//-----------------------------------------------------------------------------
// include files for R/GALILEI Project
#include <galilei.h>
using namespace R;
using namespace GALILEI;
using namespace std;


//------------------------------------------------------------------------------
// include files for current project
#include <ui_preferences.h>
class KGALILEICenter;  // forward declaration


//------------------------------------------------------------------------------
/**
 * The Preferences provides a dialog box to configure KGALILEICenter (and the
 * underlying GGALILEIApp).
 * @short Preferences Dialog.
 * @author Pascal Francq
 */
class Preferences : KDialog, Ui_Preferences
{
	Q_OBJECT

	/**
	 * The main application.
	 */
	KGALILEICenter* App;

public:

	/**
	 * Construct the dialog box.
	 * @param app            Application.
	 */
	Preferences(KGALILEICenter* app);

	/**
	 * Execute the dialog boxes. In practice, when the 'Apply' button is
	 * pressed, the variables are applied.
	 */
	void exec(void);

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
