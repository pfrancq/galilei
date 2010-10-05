/*

	GALILEI Research Project

	QCreateDatabase.cpp

	Create the database - Header.

	Copyright 2008-2010 by Pascal Francq (pascal@francq.info).

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
#ifndef QCreateDatabaseH
#define QCreateDatabaseH


//------------------------------------------------------------------------------
// include files for current project
#include <ui_qcreatedatabase.h>
#include <kgalileicenter.h>


//------------------------------------------------------------------------------
/**
 * The QCreateDatabase provides a class for create a MySQL database.
 * @author Pascal Francq
 * @short Create Database
 */
class QCreateDatabase : private KDialog, private Ui_QCreateDatabase
{
	/**
	 * Name of the database;
	 */
	RString Name;

	/**
	 * User;
	 */
	RString User;

	/**
	 * Password.
	 */
	RString Pwd;

	/**
	 * Host
	 */
	RString Host;

	/**
	 * URL containing the SQL file which will create the database.
	 */
	RString DbSchema;

	/**
	 * Directory where to store the configuration file.
	 */
	RString ConfigDir;

	/**
	 * Application.
	 */
	KGALILEICenter* App;

public:

	/**
	 * Constructor.
	 * @param parent         Parent widget.
	 */
	QCreateDatabase(KGALILEICenter* parent);

	/**
	 * Show the dialog box. If the user clicks on the 'OK' button, the database
	 * is created.
	 */
	void run(void);

	friend class QCreateDB;
};



//-----------------------------------------------------------------------------
#endif
