/*

	QGALILEI

	QModifyDocs.h

	Modify Documents Dialog - Header.

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
#ifndef QModifyDocsDlgH
#define QModifyDocsDlgH


//-----------------------------------------------------------------------------
// include files for R/GALILEI Project
#include <galilei.h>


//------------------------------------------------------------------------------
// include files for Qt
#include <QDialog>


//------------------------------------------------------------------------------
// include files for current project
#include <ui_qmodifydocsdlg.h>
class QGALILEIWin;  // forward declaration


//------------------------------------------------------------------------------
/**
 * The QModifyDocsDlg provides a dialog box to modify the documents.
 * @short QModifyDocsDlg Dialog.
 */
class QModifyDocsDlg : public QDialog, Ui_QModifyDocsDlg
{
	Q_OBJECT

	/**
	 * Main Window.
	 */
	QGALILEIWin* Win;

	/**
	 * Concept type was edited.
	 */
	bool EditType;

public:

	/**
	 * Construct the dialog box.
	 * @param win            Window.
	 */
	QModifyDocsDlg(QGALILEIWin* win);

private:

	/**
	 * Populate the concept types.
    */
	void populateConceptTypes(void);

private slots:

	/**
	 * Populate the concepts.
	 * @param index          Index of the concept type.
    */
	void populateConcepts(int index);

	/**
	 * The concept type was edited.
	 * @param index          Index of the concept type.
    */
	void editConceptType(int index);

	/**
	 * The concept type was edited.
    * @param text           New concept type.
    */
	void editConceptType(const QString& text);

public slots:

	/**
	 * Add an expression to dictionary.
    */
	void addExpression(void);

	/**
	 * Modify the description of a document.
	 */
	void modifyDescription(void);
};


//------------------------------------------------------------------------------
#endif
