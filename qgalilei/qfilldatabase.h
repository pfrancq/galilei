/*

	GALILEI Research Project

	QFillDatabase.cpp

	Fill the database with a list of document - Header.

	Copyright 2008-2015 by Pascal Francq (pascal@francq.info).

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
#ifndef QFillDatabaseH
#define QFillDatabaseH


//------------------------------------------------------------------------------
// include files for R/GALILEI
#include <rdb.h>
#include <glang.h>


//------------------------------------------------------------------------------
// include files for Qt
#include <QtGui/QListWidgetItem>
#include <QDialog>


//------------------------------------------------------------------------------
// include files for current project
#include <ui_qfilldatabase.h>
class QGALILEIWin;


//------------------------------------------------------------------------------
/**
 * The QFillDatabase provides a class for importing a set of documents in the
 * current database. The documents may be categorized in subjects on a directory
 * basis.
 * @author Pascal Francq
 * @short Fill Database
 */
class QFillDatabase : private QDialog, private Ui_QFillDatabase
{
	Q_OBJECT

	/**
	 * Current language chosen.
	 */
	GALILEI::GLang* Lang;

	/**
	 * Directory to import;
	 */
	R::RString Dir;

	/**
	 * Are the documents to import categorized.
	 */
	bool Categorized;

	/**
	 * Name of the parent;
	 */
	R::RString Parent;

	/**
	 * Default MIME Type.
	 */
	R::RString DefaultMIME;

	/**
	 * Main window.
	 */
	QGALILEIWin* Win;

public:

	/**
	 * Constructor.
	 * @param parent         Main window.
	 */
	QFillDatabase(QGALILEIWin* parent);

	/**
	 * Show the dialog box. If the user clicks on the 'OK' button, the database
	 * is filled.
	 */
	void run(void);

public slots:

	/**
	 * The parent has changed.
	 * @param s              Index of the parent.
	 */
	void ChangeParent(int s);

	/**
	 * Insert a list of sub-subjects related to the one chosen as parent.
	 * @param db             Database.
	 * @param parentId       Identifier of the parent.
	 * @param item           Item representing the parent.
	 */
	void InsertSubItem(R::RDb* db,int parentId,QListWidgetItem* item);

	/**
	 * Choose the name of the parent.
	 */
	void ChooseParentName(void);

	/**
	 * Slot called when the directory must be edited.
    */
	void editDir(void);

	friend class QImportDocs;
};



//-----------------------------------------------------------------------------
#endif
