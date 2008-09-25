/*

	Genetic Document Algorithm

	GGDAGrouping_KDE.cpp

	Config and about dialogs - Implementation.

	Copyright 2006-2007 by the Université Libre de Bruxelles.

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
// include files for R Project
#include <rqt.h>


//------------------------------------------------------------------------------
// include files for GALILEI
#include <gpostdoc.h>


//------------------------------------------------------------------------------
// include files for GDA
#include <../../gca.h>


//-----------------------------------------------------------------------------
// include files for QT
#include <qvariant.h>
#include <qcheckbox.h>
#include <qcombobox.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qdialog.h>
#include <qtooltip.h>
#include <qwhatsthis.h>


//-----------------------------------------------------------------------------
// include files for KDE
#include <kaboutdata.h>
#include <klocale.h>
#include <kaboutapplication.h>
#include <knuminput.h>


//-----------------------------------------------------------------------------
// include files for current plug-in
#include <gaconfig_kde.h>


//-----------------------------------------------------------------------------
// Description of the application
static const char *description =
	I18N_NOOP("This is the Genetic Document Algorithm for GALILEI.");


//------------------------------------------------------------------------------
extern "C" {
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void About(void)
{
	KAboutData aboutData( "GDA", I18N_NOOP("Genetic Document Algorithm"),
		"1.0", description, KAboutData::License_GPL,
		"(c) 2005, Université Libre de Bruxelles\nCAD/CAM Department", 0, "http://cfao.ulb.ac.be", "pfrancq@ulb.ac.be");
	aboutData.addAuthor("Pascal Francq",I18N_NOOP("Maintainer"), "pfrancq@ulb.ac.be");
	KAboutApplication dlg(&aboutData);
	dlg.exec();
}


//------------------------------------------------------------------------------
void Configure(GFactoryPostDoc* params)
{
	GAConfigDlg dlg;
	if(dlg.Configure(params))
			params->Apply();
}


//------------------------------------------------------------------------------
}     // end of extern
//------------------------------------------------------------------------------

