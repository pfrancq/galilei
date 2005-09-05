/*

	GALILEI Research Project

	GMySQL_KDE.cpp

	A KDE dialog box for MySQL database - Implementation.

	Copyright 2005 by the Université Libre de Bruxelles.

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
// include files for GALILEI
#include <gstorage.h>
using namespace GALILEI;


//-----------------------------------------------------------------------------
// include files for QT
#include <qvariant.h>
#include <qcheckbox.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qdialog.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qgroupbox.h>


//-----------------------------------------------------------------------------
// include files for KDE
#include <kaboutdata.h>
#include <klocale.h>
#include <kaboutapplication.h>
#include <knuminput.h>
#include <qcombobox.h>


//-----------------------------------------------------------------------------
// include files for Current
#include <dlgconfig_qt.h>
#include <dlgconfig_qt.ui.h>


//-----------------------------------------------------------------------------
// Description of the application
static const char *description =
	I18N_NOOP("This is a MySQL database.");


//------------------------------------------------------------------------------
extern "C" {
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
void About(void)
{
	KAboutData aboutData( "text", I18N_NOOP("MySQL"),
		"1.0", description, KAboutData::License_GPL,
		"(c) 12005, Université Libre de Bruxelles\nCAD/CAM Department", 0, "http://cfao.ulb.ac.be", "pfrancq@ulb.ac.be");
	aboutData.addAuthor("Pascal Francq",I18N_NOOP("Maintainer"), "pfrancq@ulb.ac.be");
	KAboutApplication dlg(&aboutData);
	dlg.exec();
}


//------------------------------------------------------------------------------
void Configure(GFactoryStorage* params)
{
	DlgConfig_Qt dlg;

	dlg.txtDb->setText(params->GetString("Database"));
	dlg.txtLogin->setText(params->GetString("User"));
	dlg.txtPwd->setText(params->GetString("Password"));
	dlg.txtHost->setText(params->GetString("Host"));
	dlg.cbEncoding->setCurrentText(params->GetString("Encoding"));
	dlg.Filter->setDate(QDate::currentDate());
	dlg.All->setChecked(true);
	if(dlg.exec())
	{
		params->Set("Database",dlg.txtDb->text());
		params->Set("Host",dlg.txtHost->text());
		params->Set("User",dlg.txtLogin->text());
		params->Set("Password",dlg.txtPwd->text());
		params->Set("Encoding",dlg.cbEncoding->currentText());
		params->Set("All",dlg.All->isChecked());
		params->Set("Filter",dlg.Filter->date().toString(Qt::ISODate));
	}
}

//------------------------------------------------------------------------------
}     // End of extern
//------------------------------------------------------------------------------
