/*

	KGALILEICenter

	QChooseDlg.h

	Choose a tool to run - Header.

	Copyright 2010-2015 by Pascal Francq (pascal@francq.info).

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
#ifndef QChooseDlgH
#define QChooseDlgH


//------------------------------------------------------------------------------
// include files for R
#include <rstring.h>


//------------------------------------------------------------------------------
// include files for Qt
#include <QDialog>


//------------------------------------------------------------------------------
// include files for current project
#include <ui_qchoosedlg.h>
class QGALILEIWin;  // forward declaration
using namespace R;


//------------------------------------------------------------------------------
/**
 * The QChooseDlg provides a dialog box to choose and run a tool.
 * @short Preferences Dialog.
 * @author Pascal Francq
 */
class QChooseDlg : QDialog, Ui_QChooseDlg
{
	Q_OBJECT

	/**
	 * Main Window.
	 */
	QGALILEIWin* Win;

public:

	/**
	 * Construct the dialog box.
	 * @param win            Window.
	 */
	QChooseDlg(QGALILEIWin* win);

	/**
	 * Execute the dialog boxes.
	 */
	bool exec(void) {return(QDialog::exec());}

	/**
	 * Populate the list of tool categories.
    */
	void populateToolCats(const RString& cat);

	/**
	 * Populate the list of tools.
    */
	void populateTools(const RString& cat,const RString& tool);

	/**
    * @return the current choice.
    */
	RString GetChoice(void) const;
};


//------------------------------------------------------------------------------
#endif


